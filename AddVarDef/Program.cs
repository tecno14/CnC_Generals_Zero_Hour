using System.Xml.Linq;

namespace AddVarDef;

internal class Program
{
    static void Main()
    {
        // Set directory path (modify as needed)
        var directoryPath = @"C:\Projects\3_generalsr\CnC_Generals_Zero_Hour";

        // Variables to be prepended
        var OG = "OG";
        var ZH = "ZH";

        // Process all .vcxproj files in the directory
        foreach (string file in Directory.GetFiles(directoryPath, "*.vcxproj", SearchOption.AllDirectories))
        {
            var isZH = file.EndsWith("_ZH.vcxproj");

            // Load XML file
            var doc = XDocument.Load(file);
            var project = doc.Root;

            if (project == null)
                continue;

            // Get XML namespace
            var pns = project.GetDefaultNamespace();

            foreach (var itemGroup in project.Elements(pns + "ItemDefinitionGroup"))
            {
                var ins = project.GetDefaultNamespace();
                var clCompile = itemGroup.Element(ins + "ClCompile");
                if (clCompile == null)
                    continue;

                var cns = project.GetDefaultNamespace();
                var preprocessorDefinitions = clCompile.Element(cns + "PreprocessorDefinitions");
                if (preprocessorDefinitions == null)
                    continue;

                // Get existing value and prepend with the correct variable
                var existingValue = preprocessorDefinitions.Value;
                var newValue = (isZH ? ZH : OG) + ";" + existingValue;
                preprocessorDefinitions.Value = newValue;
            }

            // Save modified XML
            doc.Save(file);
            Console.WriteLine($"Updated: {file}");
        }

        Console.WriteLine("Processing complete.");
    }
}
