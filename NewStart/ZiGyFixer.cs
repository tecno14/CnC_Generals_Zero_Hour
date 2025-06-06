using System.Xml.Linq;

namespace NewStart;

/// <summary>
/// Provides functionality to fix the incompatibility between the `/ZI` (Edit and Continue) 
/// and `/Gy-` (Disable Function-Level Linking) compiler options in upgraded C++ projects.
/// </summary>
public static class ZiGyFixer
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
                FixFunctionLevelLinking(file);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error processing {file}: {ex.Message}");
            }
        }
    }

    /// <summary>
    /// Fixes the function-level linking issue in a given .vcxproj file.
    /// </summary>
    /// <param name="filePath">Path to the project file.</param>
    public static void FixFunctionLevelLinking(string filePath)
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

                var debugInfo = clCompile.Element(ns + "DebugInformationFormat");
                if (debugInfo == null || debugInfo.Value.Trim() != "EditAndContinue")
                    continue;

                EnsureFunctionLevelLinking(clCompile, ns);
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
    /// Ensures the <FunctionLevelLinking> element is set to "true".
    /// </summary>
    /// <param name="clCompile">The ClCompile XML element.</param>
    /// <param name="ns">The XML namespace.</param>
    private static void EnsureFunctionLevelLinking(XElement clCompile, XNamespace ns)
    {
        var functionLevelLinking = clCompile.Element(ns + "FunctionLevelLinking");

        if (functionLevelLinking == null)
        {
            clCompile.Add(new XElement(ns + "FunctionLevelLinking", "true"));
            Console.WriteLine("Added FunctionLevelLinking.");
        }
        else
        {
            functionLevelLinking.SetValue("true");
            Console.WriteLine("Updated FunctionLevelLinking.");
        }
    }
}