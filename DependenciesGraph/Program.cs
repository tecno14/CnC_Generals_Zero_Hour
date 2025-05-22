using System.Xml.Linq;

namespace DependenciesGraph;
class Project
{
    public string Name { get; set; }
    public string Path { get; set; }
    public List<string> References { get; set; } = new List<string>();
}

class Program
{
    static void Main(string[] args)
    {
        string folderPath = args.Length > 0 ? args[0] : Directory.GetCurrentDirectory();
        var projects = LoadProjects(folderPath);
        string html = GenerateHtml(projects);
        var outputFile = Path.Combine(folderPath, "dependencies.html");
        File.WriteAllText(outputFile, html);
        Console.WriteLine($"Generated: {outputFile}");
    }

    static Dictionary<string, Project> LoadProjects(string folderPath)
    {
        var projects = new Dictionary<string, Project>(StringComparer.OrdinalIgnoreCase);
        foreach (var file in Directory.GetFiles(folderPath, "*.vcxproj", SearchOption.AllDirectories))
        {
            try
            {
                var doc = XDocument.Load(file);
                var ns = doc.Root.Name.Namespace;

                var project = new Project
                {
                    Path = file,
                    Name = Path.GetFileNameWithoutExtension(file)
                };

                foreach (var reference in doc.Descendants(ns + "ProjectReference"))
                {
                    var include = reference.Attribute("Include")?.Value;
                    if (!string.IsNullOrWhiteSpace(include))
                    {
                        string refName = Path.GetFileNameWithoutExtension(include);
                        project.References.Add(refName);
                    }
                }

                projects[project.Name] = project;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error parsing {file}: {ex.Message}");
            }
        }

        return projects;
    }

    static string GenerateHtml(Dictionary<string, Project> projects)
    {
        var nodes = new HashSet<string>();
        var elements = new List<string>();

        foreach (var proj in projects.Values)
        {
            nodes.Add(proj.Name);
            foreach (var dep in proj.References)
                nodes.Add(dep);
        }

        foreach (var name in nodes)
            elements.Add($"{{ data: {{ id: '{Escape(name)}' }} }}");

        foreach (var proj in projects.Values)
        {
            foreach (var dep in proj.References)
            {
                elements.Add($"{{ data: {{ source: '{Escape(proj.Name)}', target: '{Escape(dep)}' }} }}");
            }
        }

        var elementsJson = string.Join(",\n      ", elements);

        return $@"
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8' />
  <title>VC++ Project Dependencies</title>
  <script src='https://unpkg.com/cytoscape@3.26.0/dist/cytoscape.min.js'></script>
  <script src='https://unpkg.com/dagre@0.8.5/dist/dagre.min.js'></script>
  <script src='https://unpkg.com/cytoscape-dagre@2.3.0/cytoscape-dagre.js'></script>
  <style>
    html, body, #cy {{
      height: 100%;
      margin: 0;
      padding: 0;
      font-family: sans-serif;
    }}
  </style>
</head>
<body>
  <div id='cy'></div>
  <script>
    const elements = [
      {elementsJson}
    ];

    const cy = cytoscape({{
      container: document.getElementById('cy'),
      elements,
      layout: {{name: 'dagre',
          rankDir: 'TB',       // Top to Bottom
          nodeSep: 50,
          edgeSep: 10,
          rankSep: 100,
          ranker: 'longest-path' // Ensures root (leaf) nodes at top
        }},
      style: [
        {{
          selector: 'node',
          style: {{
            'shape': 'roundrectangle',
            'background-color': '#007acc',
            'label': 'data(id)',
            'color': 'white',
            'text-valign': 'center',
            'text-halign': 'center',
            'font-size': '12px',
            'padding': '8px',
            'text-wrap': 'wrap',
            'text-max-width': '120px', // control max width before wrapping
            'width': 'label',
            'height': 'label'
          }}
        }}
        ,
        {{
          selector: 'edge',
          style: {{
            'width': 2,
            'line-color': '#ccc',
            'target-arrow-color': '#ccc',
            'target-arrow-shape': 'triangle'
          }}
        }}
      ]
    }});
  </script>
</body>
</html>";
    }

    static string Escape(string input)
    {
        return input.Replace("\\", "\\\\").Replace("'", "\\'");
    }
}
