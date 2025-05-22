using System.Xml.Linq;

namespace FixBuildArgs;

internal class Program
{
    static void Main(string[] args)
    {
        // Set directory path (modify as needed)
        var directoryPath = @"C:\Projects\3_generalsr\CnC_Generals_Zero_Hour";

        // Process all .vcxproj files in the directory
        foreach (string file in Directory.GetFiles(directoryPath, "*.vcxproj", SearchOption.AllDirectories))
        {
            try
            {
                Console.WriteLine($"Processing: {file}");

                // Load XML file
                XDocument doc = XDocument.Load(file);
                XElement project = doc.Root;

                // Get XML namespace if present
                XNamespace ns = project.GetDefaultNamespace();

                // Find all <ItemDefinitionGroup> elements
                foreach (XElement itemGroup in project.Elements(ns + "ItemDefinitionGroup"))
                {
                    XElement clCompile = itemGroup.Element(ns + "ClCompile");
                    if (clCompile != null)
                    {
                        XElement debugInfo = clCompile.Element(ns + "DebugInformationFormat");
                        if (debugInfo != null && debugInfo.Value.Trim() == "EditAndContinue")
                        {
                            // Check if <FunctionLevelLinking> already exists, if not add it
                            XElement functionLevelLinking = clCompile.Element(ns + "FunctionLevelLinking");
                            if (functionLevelLinking == null)
                            {
                                clCompile.Add(new XElement(ns + "FunctionLevelLinking", "true"));
                                Console.WriteLine($"Added: {file}");
                            }
                            else
                            {
                                functionLevelLinking.SetValue("true");
                                Console.WriteLine($"Updated: {file}");
                            }
                        }
                    }
                }

                // Save the modified XML back to file
                doc.Save(file);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error processing {file}: {ex.Message}");
            }
        }

        Console.WriteLine("Processing complete.");
    }
}