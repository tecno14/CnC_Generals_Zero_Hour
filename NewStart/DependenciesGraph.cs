using NewStart.Models;
using System.Text.Json;
using System.Xml.Linq;

namespace NewStart;

public class DependencyGraph
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

        return $@"
<!DOCTYPE html>
<html>
<head>
  <meta charset='utf-8' />
  <title>Project Dependency Graph</title>
  <script src='https://unpkg.com/cytoscape@3.26.0/dist/cytoscape.min.js'></script>
  <script src='https://unpkg.com/dagre@0.8.5/dist/dagre.min.js'></script>
  <script src='https://unpkg.com/cytoscape-dagre@2.3.0/cytoscape-dagre.js'></script>
  <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css'>
  <style>
    * {{ box-sizing: border-box; }}
    body {{ 
        margin: 0; 
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        background: #f5f7fa;
        color: #333;
        height: 100vh;
        display: flex;
        flex-direction: column;
    }}
    .header {{
        background: linear-gradient(135deg, #6a11cb 0%, #2575fc 100%);
        color: white;
        padding: 15px 25px;
        box-shadow: 0 4px 6px rgba(0,0,0,0.1);
        display: flex;
        justify-content: space-between;
        align-items: center;
    }}
    .controls {{
        display: flex;
        gap: 15px;
        padding: 15px;
        background: white;
        border-bottom: 1px solid #e0e0e0;
        flex-wrap: wrap;
    }}
    .control-group {{
        display: flex;
        align-items: center;
        gap: 8px;
    }}
    input, select, button {{
        padding: 8px 12px;
        border: 1px solid #ddd;
        border-radius: 4px;
        font-size: 14px;
    }}
    button {{
        background: #4a6cf7;
        color: white;
        border: none;
        cursor: pointer;
        transition: background 0.3s;
    }}
    button:hover {{ background: #3a5ce0; }}
    #search {{ width: 250px; }}
    #cy {{
        flex: 1;
        background: white;
        border: 1px solid #e0e0e0;
        margin: 0 15px 15px;
        border-radius: 4px;
        box-shadow: 0 2px 4px rgba(0,0,0,0.05);
    }}
    .panel {{
        display: flex;
        height: 100%;
    }}
    .details-panel {{
        width: 350px;
        background: white;
        border-left: 1px solid #e0e0e0;
        padding: 20px;
        overflow-y: auto;
        box-shadow: -2px 0 5px rgba(0,0,0,0.05);
        display: none;
    }}
    .node-highlight {{ 
        border: 3px solid gold !important;
        z-index: 9999 !important;
    }}
    .node-dim {{ opacity: 0.2; }}
    .details-header {{
        display: flex;
        justify-content: space-between;
        align-items: center;
        border-bottom: 1px solid #eee;
        padding-bottom: 15px;
        margin-bottom: 15px;
    }}
    .detail-section {{
        margin-bottom: 20px;
    }}
    .detail-list {{
        max-height: 150px;
        overflow-y: auto;
        border: 1px solid #eee;
        border-radius: 4px;
        padding: 10px;
    }}
    .detail-item {{
        padding: 5px 0;
        border-bottom: 1px solid #f5f5f5;
        cursor: pointer;
    }}
    .detail-item:hover {{ background-color: #f0f5ff; }}
    .fa-close {{ cursor: pointer; color: #999; }}
    .fa-close:hover {{ color: #333; }}
    .filter-option {{ display: flex; align-items: center; gap: 5px; }}
    .legend {{
        display: flex;
        gap: 15px;
        margin-top: 10px;
    }}
    .legend-item {{
        display: flex;
        align-items: center;
        gap: 5px;
        font-size: 13px;
    }}
    .legend-color {{
        width: 15px;
        height: 15px;
        border-radius: 3px;
    }}
    #tooltip {{
        position: absolute;
        background: rgba(0,0,0,0.8);
        color: white;
        padding: 5px 10px;
        border-radius: 4px;
        font-size: 12px;
        pointer-events: none;
        z-index: 10000;
        max-width: 400px;
        display: none;
    }}
  </style>
</head>
<body>
  <div class='header'>
    <h1>Project Dependency Graph</h1>
    <div class='legend'>
      <div class='legend-item'>
        <div class='legend-color' style='background: #6a11cb;'></div>
        <span>Project</span>
      </div>
      <div class='legend-item'>
        <div class='legend-color' style='background: #ff6b6b;'></div>
        <span>Test Project</span>
      </div>
      <div class='legend-item'>
        <div class='legend-color' style='background: #4cd97b;'></div>
        <span>Library</span>
      </div>
    </div>
  </div>
  
  <div class='controls'>
    <div class='control-group'>
      <i class='fas fa-search'></i>
      <input type='text' id='search' placeholder='Search projects...'>
    </div>
    
    <div class='control-group'>
      <label for='layout'>Layout:</label>
      <select id='layout'>
        <option value='dagre'>Hierarchical</option>
        <option value='breadthfirst'>Tree</option>
        <option value='cose'>Force-Directed</option>
      </select>
    </div>
    
    <div class='control-group'>
      <span>Show:</span>
      <div class='filter-option'>
        <input type='checkbox' id='filter-tests' checked>
        <label for='filter-tests'>Test Projects</label>
      </div>
      <div class='filter-option'>
        <input type='checkbox' id='filter-libs' checked>
        <label for='filter-libs'>Libraries</label>
      </div>
    </div>
    
    <button id='reset'><i class='fas fa-sync-alt'></i> Reset View</button>
  </div>
  
  <div class='panel'>
    <div id='cy'></div>
    <div class='details-panel' id='details'>
      <div class='details-header'>
        <h2 id='detail-title'>Project Details</h2>
        <i class='fas fa-close' id='close-details'></i>
      </div>
      <div class='detail-section'>
        <h3>Project Path</h3>
        <div id='detail-path'></div>
      </div>
      <div class='detail-section'>
        <h3>References ({{'<span id=""ref-count"">0</span>'}})</h3>
        <div class='detail-list' id='detail-references'></div>
      </div>
      <div class='detail-section'>
        <h3>Referenced By ({{'<span id=""refby-count"">0</span>'}})</h3>
        <div class='detail-list' id='detail-referencedby'></div>
      </div>
    </div>
  </div>
  
  <div id='tooltip'></div>

  <script>
    // Graph data
    const graphData = {graphDataJson};
    
    // Create GUID to name mapping
    const guidToName = {{}};
    graphData.nodes.forEach(node => {{
        guidToName[node.id] = node.name;
    }});
    
    // Initialize Cytoscape
    const cy = cytoscape({{
      container: document.getElementById('cy'),
      elements: {{
        nodes: graphData.nodes.map(n => ({{
          data: {{
            id: n.id,
            name: n.name,
            path: n.path,
            references: n.references,
            referencedBy: n.referencedBy,
            type: getProjectType(n.name)
          }},
          classes: getProjectType(n.name)
        }})),
        edges: graphData.edges.map(e => ({{
          data: {{ source: e.source, target: e.target }}
        }}))
      }},
      layout: {{ name: 'dagre', rankDir: 'TB' }},
      style: [
        // Node styles
        {{
          selector: 'node',
          style: {{
            'label': 'data(name)',
            'text-valign': 'center',
            'text-halign': 'center',
            'font-size': '12px',
            'font-weight': '600',
            'color': '#fff',
            'text-outline-width': '1px',
            'text-outline-color': '#555',
            'text-wrap': 'wrap',
            'text-max-width': '120px',
            'padding': '8px',
            'shape': 'roundrectangle'
          }}
        }},
        {{
          selector: '.project',
          style: {{ 
            'background': 'linear-gradient(135deg, #6a11cb 0%, #2575fc 100%)',
            'width': 'label',
            'height': 'label'
          }}
        }},
        {{
          selector: '.test',
          style: {{ 
            'background': 'linear-gradient(135deg, #ff6b6b 0%, #ff8e8e 100%)',
            'width': 'label',
            'height': 'label'
          }}
        }},
        {{
          selector: '.library',
          style: {{ 
            'background': 'linear-gradient(135deg, #4cd97b 0%, #76e89a 100%)',
            'width': 'label',
            'height': 'label'
          }}
        }},
        
        // Edge styles
        {{
          selector: 'edge',
          style: {{
            'width': 2,
            'line-color': '#aaa',
            'target-arrow-color': '#aaa',
            'target-arrow-shape': 'triangle',
            'curve-style': 'bezier',
            'arrow-scale': 1.2
          }}
        }},
        
        // Highlighting
        {{
          selector: '.highlight',
          style: {{
            'border-width': 3,
            'border-color': 'gold',
            'border-style': 'solid',
            'z-index': 9999
          }}
        }},
        {{
          selector: '.dim',
          style: {{ 'opacity': 0.2 }}
        }}
      ]
    }});
    
    // Tooltip element
    const tooltip = document.getElementById('tooltip');
    
    // Setup hover events
    cy.on('mouseover', 'node', function(evt) {{
      const node = evt.target;
      tooltip.innerHTML = node.data('path');
      tooltip.style.display = 'block';
      tooltip.style.left = (evt.originalEvent.pageX + 10) + 'px';
      tooltip.style.top = (evt.originalEvent.pageY + 10) + 'px';
    }});
    
    cy.on('mousemove', 'node', function(evt) {{
      tooltip.style.left = (evt.originalEvent.pageX + 10) + 'px';
      tooltip.style.top = (evt.originalEvent.pageY + 10) + 'px';
    }});
    
    cy.on('mouseout', 'node', function() {{
      tooltip.style.display = 'none';
    }});
    
    // Helper function to determine project type
    function getProjectType(name) {{
      if (name.toLowerCase().includes('test')) return 'test';
      if (name.toLowerCase().includes('lib')) return 'library';
      return 'project';
    }}
    
    // UI Controls Functionality
    document.getElementById('search').addEventListener('input', function(e) {{
      const term = e.target.value.toLowerCase();
      
      cy.nodes().forEach(node => {{
        if (term === '' || node.data('name').toLowerCase().includes(term)) {{
          node.removeClass('dim');
          node.addClass('highlight');
        }} else {{
          node.removeClass('highlight');
          node.addClass('dim');
        }}
      }});
    }});
    
    document.getElementById('layout').addEventListener('change', function(e) {{
      const layout = e.target.value;
      const options = {{ 
        name: layout,
        animate: true,
        animationDuration: 500
      }};
      
      if (layout === 'dagre') {{
        options.rankDir = 'TB';
      }} else if (layout === 'breadthfirst') {{
        options.directed = true;
        options.circle = false;
      }}
      
      cy.layout(options).run();
    }});
    
    document.getElementById('reset').addEventListener('click', function() {{
      cy.reset();
      cy.fit();
      document.getElementById('search').value = '';
      cy.nodes().removeClass('highlight dim');
    }});
    
    // Filter controls
    function applyFilters() {{
      const showTests = document.getElementById('filter-tests').checked;
      const showLibs = document.getElementById('filter-libs').checked;
      
      cy.nodes().forEach(node => {{
        const type = node.data('type');
        
        if ((type === 'test' && !showTests) || 
            (type === 'library' && !showLibs)) {{
          node.style('display', 'none');
        }} else {{
          node.style('display', 'element');
        }}
      }});
      
      cy.layout({{ name: 'dagre' }}).run();
    }}
    
    document.getElementById('filter-tests').addEventListener('change', applyFilters);
    document.getElementById('filter-libs').addEventListener('change', applyFilters);
    
    // Node click handler
    cy.on('tap', 'node', function(evt) {{
      const node = evt.target;
      const detailsPanel = document.getElementById('details');
      const title = document.getElementById('detail-title');
      const path = document.getElementById('detail-path');
      const references = document.getElementById('detail-references');
      const referencedBy = document.getElementById('detail-referencedby');
      const refCount = document.getElementById('ref-count');
      const refByCount = document.getElementById('refby-count');
      
      // Update details panel
      title.textContent = node.data('name');
      path.textContent = node.data('path');
      
      // Populate references
      references.innerHTML = '';
      const refs = node.data('references') || [];
      refs.forEach(guid => {{
        const item = document.createElement('div');
        item.className = 'detail-item';
        item.textContent = guidToName[guid] || guid;
        item.onclick = () => {{
          const targetNode = cy.getElementById(guid);
          if (targetNode) {{
            cy.animate({{
              center: {{ eles: targetNode }},
              zoom: cy.zoom() * 1.2
            }});
            targetNode.trigger('tap');
          }}
        }};
        references.appendChild(item);
      }});
      refCount.textContent = refs.length;
      
      // Populate referenced by
      referencedBy.innerHTML = '';
      const refBy = node.data('referencedBy') || [];
      refBy.forEach(guid => {{
        const item = document.createElement('div');
        item.className = 'detail-item';
        item.textContent = guidToName[guid] || guid;
        item.onclick = () => {{
          const targetNode = cy.getElementById(guid);
          if (targetNode) {{
            cy.animate({{
              center: {{ eles: targetNode }},
              zoom: cy.zoom() * 1.2
            }});
            targetNode.trigger('tap');
          }}
        }};
        referencedBy.appendChild(item);
      }});
      refByCount.textContent = refBy.length;
      
      // Show panel
      detailsPanel.style.display = 'block';
    }});
    
    // Close details panel
    document.getElementById('close-details').addEventListener('click', function() {{
      document.getElementById('details').style.display = 'none';
    }});
    
    // Initial fit to viewport
    cy.fit();
  </script>
</body>
</html>";
    }
}