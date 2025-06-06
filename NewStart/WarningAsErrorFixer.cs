using System.Xml.Linq;

namespace NewStart;

/// <summary>
/// Utility class for fixing "Treat warnings as errors" setting in Visual Studio .vcxproj files.
/// </summary>
public static class WarningAsErrorFixer
{
    /// <summary>
    /// Processes all .vcxproj files in the specified directory and subdirectories.
    /// </summary>
    /// <param name="directory">Root directory containing project files.</param>
    public static void ProcessProjectFiles(string directory)
    {
        foreach (var file in Directory.GetFiles(directory, "*.vcxproj", SearchOption.AllDirectories))
        {
            try
            {
                Console.WriteLine($"Processing: {file}");
                FixWarningAsError(file);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error processing {file}: {ex.Message}");
            }
        }
    }

    /// <summary>
    /// Fixes the "Treat warnings as errors" issue in a given .vcxproj file by setting <TreatWarningAsError> to false.
    /// </summary>
    /// <param name="filePath">Path to the project file.</param>
    public static void FixWarningAsError(string filePath)
    {
        try
        {
            // Load XML file
            var doc = XDocument.Load(filePath);
            var project = doc.Root ?? throw new Exception("Couldn't load project file.");

            // Get XML namespace if present
            var ns = project.GetDefaultNamespace();

            // Iterate through all <ItemDefinitionGroup> elements
            foreach (var itemGroup in project.Elements(ns + "ItemDefinitionGroup"))
            {
                var clCompile = itemGroup.Element(ns + "ClCompile");
                if (clCompile == null)
                    continue;

                EnsureWarningAsErrorDisabled(clCompile, ns);
            }

            // Save the modified XML back to file
            doc.Save(filePath);
            Console.WriteLine($"Fixed: {filePath}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error processing {filePath}: {ex.Message}");
        }
    }

    /// <summary>
    /// Ensures the <TreatWarningAsError> element is set to "false".
    /// </summary>
    /// <param name="clCompile">The ClCompile XML element.</param>
    /// <param name="ns">The XML namespace.</param>
    private static void EnsureWarningAsErrorDisabled(XElement clCompile, XNamespace ns)
    {
        var treatWarningsAsErrors = clCompile.Element(ns + "TreatWarningAsError");

        if (treatWarningsAsErrors == null)
        {
            clCompile.Add(new XElement(ns + "TreatWarningAsError", "false"));
            Console.WriteLine("Disabled TreatWarningAsError.");
        }
        else
        {
            treatWarningsAsErrors.SetValue("false");
            Console.WriteLine("Updated TreatWarningAsError to false.");
        }
    }
}
