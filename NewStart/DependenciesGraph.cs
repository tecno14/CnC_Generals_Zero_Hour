using Models;
using System.Text.Json;
using System.Xml.Linq;

namespace NewStart;

public partial class DependencyGraph
{
    public Dictionary<string, Project> ProjectsByGuid { get; } = new Dictionary<string, Project>(StringComparer.OrdinalIgnoreCase);

    public Dictionary<string, Project> ProjectsByPath { get; } = new Dictionary<string, Project>(StringComparer.OrdinalIgnoreCase);

    public DependencyGraph(string folderPath)
    {
        LoadProjects(folderPath);
        BuildReferences();
    }

    private void LoadProjects(string folderPath)
    {
        foreach (var file in Directory.GetFiles(folderPath, "*.vcxproj", SearchOption.AllDirectories))
        {
            try
            {
                var doc = XDocument.Load(file);
                var ns = doc.Root?.Name.Namespace ?? XNamespace.None;

                var project = new Project
                {
                    Path = file,
                    Name = Path.GetFileNameWithoutExtension(file)
                };

                // Extract ProjectGuid
                var guidElement = doc.Descendants(ns + "ProjectGuid").FirstOrDefault();
                if (guidElement != null)
                {
                    project.Guid = guidElement.Value.Trim('{', '}');
                }
                else
                {
                    // Fallback if Guid is missing
                    project.Guid = Guid.NewGuid().ToString();
                }

                // Process references
                foreach (var reference in doc.Descendants(ns + "ProjectReference"))
                {
                    var include = reference.Attribute("Include")?.Value;
                    if (!string.IsNullOrWhiteSpace(include))
                    {
                        // Resolve absolute path for reference
                        var refPath = System.IO.Path.GetFullPath(
                            System.IO.Path.Combine(
                                System.IO.Path.GetDirectoryName(file),
                                include
                            )
                        );
                        project.References.Add(refPath);
                    }
                }

                ProjectsByGuid[project.Guid] = project;
                ProjectsByPath[file] = project;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error parsing {file}: {ex.Message}");
            }
        }
    }

    private void BuildReferences()
    {
        // Build reverse references
        foreach (var project in ProjectsByGuid.Values)
        {
            // Convert reference paths to Guids
            var validReferences = new List<string>();
            foreach (var referencePath in project.References)
            {
                if (ProjectsByPath.TryGetValue(referencePath, out var referencedProject))
                {
                    validReferences.Add(referencedProject.Guid);

                    // Build reverse reference
                    if (!referencedProject.ReferencedBy.Contains(project.Guid))
                    {
                        referencedProject.ReferencedBy.Add(project.Guid);
                    }
                }
            }
            project.References = validReferences;
        }
    }

    public bool SaveHtml(string outputFile)
    {
        try
        {
            var html = GenerateHtml();
            File.WriteAllText(outputFile, html);
            return true;
        }
        catch { return false; }
    }

    public string GenerateHtml()
    {
        // Prepare graph data
        var graphData = new
        {
            nodes = ProjectsByGuid.Values.Select(p => new
            {
                id = p.Guid,
                name = p.Name,
                path = p.Path,
                references = p.References,
                referencedBy = p.ReferencedBy,
                exists = true
            }),
            edges = ProjectsByGuid.Values
                .SelectMany(p => p.References
                    .Select(r => new { source = p.Guid, target = r }))
        };

        string graphDataJson = JsonSerializer.Serialize(graphData, new JsonSerializerOptions
        {
            PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
            WriteIndented = false
        });

        return this.HtmlContent(graphDataJson);
    }
}