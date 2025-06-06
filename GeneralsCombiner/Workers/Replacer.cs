using GeneralsCombiner;

namespace Workers;

/// <summary>
/// Overwride new version files with destination files (version files stay)
/// Used to see changes before commit
/// </summary>
/// <param name="versionRoot"></param>
/// <param name="destinationRoot"></param>
/// <param name="validFilesExtensions"></param>
public class Replacer(
    string versionRoot,
    string destinationRoot,
    string[] validFilesExtensions,
    bool replaceVersion = true)
{
    private readonly string DestinationRoot = destinationRoot;
    private readonly string[] ValidFilesExtensions = validFilesExtensions;
    private readonly bool ReplaceVersion = replaceVersion;

    public void Start()
    {
        try
        {
            var files = ValidFilesExtensions
                .SelectMany(ext => Directory.GetFiles(versionRoot, $"*{ext}", SearchOption.AllDirectories))
                .ToArray();

            var relative = files.ToDictionary(f => Path.GetRelativePath(versionRoot, f), f => f);

            foreach (var (rel, fullFile) in relative)
            {
                ProcessAndCopy(fullFile, Path.Combine(DestinationRoot, rel));
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }

    void ProcessAndCopy(string source, string dest)
    {
        try
        {
            var fileExist = File.Exists(dest);
            // if not ReplaceVersion, then the file is not text based, then can't use File.ReadAllText and WriteAllText
            if (!fileExist || !ReplaceVersion)
            {
                var desName = Path.GetDirectoryName(dest);
                Directory.CreateDirectory(desName!);
                File.Copy(source, dest, true);
            }
            else
            {
                var content = File.ReadAllText(source).ReplaceVersionComments();
                File.WriteAllText(dest, content);
            }

            Console.WriteLine($"[{(fileExist ? "Overwrite" : "Copy")}] {dest}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error processing {source}: {ex.Message}");
        }
    }
}
