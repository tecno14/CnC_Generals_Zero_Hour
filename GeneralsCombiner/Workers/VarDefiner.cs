using GeneralsCombiner;
using System.Xml.Linq;

namespace Workers;

/// <summary>
/// Find .vcxproj files, if it's name end with versionB, define versionB, else define versionA.
/// AND delete old dsp files if updated to dsp
/// </summary>
/// <param name="versionA"></param>
/// <param name="versionB"></param>
/// <param name="destinationRoot"></param>
public class VarDefiner(
    string versionA,
    string versionB,
    string destinationRoot)
{
    private readonly string VersionA = versionA;
    private readonly string VersionB = versionB;
    private readonly string DestinationRoot = destinationRoot;

    /// <summary>
    ///  - Get Identical files from both folders into result folder with : #ifdef{content}#endif
    ///  - For shared files, generate one that have both changes merged.
    /// </summary>
    /// <returns></returns>
    public void Start()
    {
        try
        {
            var projFiles = Directory.GetFiles(DestinationRoot, $"*.vcxproj", SearchOption.AllDirectories);

            foreach (var proj in projFiles)
            {
                ProcessAndCopy(proj);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }

    void ProcessAndCopy(string projFile)
    {
        try
        {
            // Check if it's name end with versionA or versionB
            var isVersionB = Path.GetFileNameWithoutExtension(projFile).EndsWith($"_{VersionB}");

            //// if not vB and there is no vB in same folder, then do nothing
            //if (!isVersionB)
            //{
            //    var projPath = Path.GetDirectoryName(projFile);
            //    var projName = Path.GetFileNameWithoutExtension(projFile);
            //    var projExte = Path.GetExtension(projFile);

            //    if (!File.Exists(Path.Combine(projPath!, projName + "_" + VersionB + projExte)))
            //    {
            //        Console.WriteLine($"[Skipped] not version base project ({projFile})");
            //        return;
            //    }
            //}

            // If dsp exist for this vcxproj, delete it
            var projPath = Path.GetDirectoryName(projFile);
            var projName = Path.GetFileNameWithoutExtension(projFile);
            var dspProj = Path.Combine(projPath!, projName + ".dsp");

            if (File.Exists(dspProj))
            {
                File.Delete(dspProj);
                Console.WriteLine($"[DSP Deleted] old project file detected and deleted ({dspProj})");
            }

            var variableToAdd = isVersionB ? VersionB : VersionA;

            // Load XML file
            var doc = XDocument.Load(projFile);
            var project = doc.Root ?? 
                throw new Exception("couldn't convert the project file into xml object");

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
                if (!existingValue.ContainsWord(variableToAdd))
                    preprocessorDefinitions.Value = variableToAdd + ";" + existingValue;
            }

            // Save modified XML
            doc.Save(projFile);
            Console.WriteLine($"[Updated] {projFile}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[Error] processing {projFile}: {ex.Message}");
        }
    }
}
