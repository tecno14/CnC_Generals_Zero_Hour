namespace Workers;

/// <summary>
/// For "file_.bin", copy a new file called "file_versionX.bin"
/// </summary>
/// <param name="versionB"></param>
/// <param name="versionBRoot"></param>
/// <param name="destinationRoot"></param>
/// <param name="validFilesExtensions"></param>
public class Duplicator(
    string versionB,
    string versionBRoot,
    string destinationRoot,
    string[] validFilesExtensions)
{
    private readonly string VersionB = versionB;
    private readonly string VersionBRoot = versionBRoot;
    private readonly string DestinationRoot = destinationRoot;
    private readonly string[] ValidFilesExtensions = validFilesExtensions;

    public void Start()
    {
        try
        {
            var zhFiles = ValidFilesExtensions
                .SelectMany(ext => Directory.GetFiles(VersionBRoot, $"*{ext}", SearchOption.AllDirectories))
                .ToArray();

            var zhRelative = zhFiles.ToDictionary(f => Path.GetRelativePath(VersionBRoot, f), f => f);

            foreach (var (rel, fullZh) in zhRelative)
            {
                ProcessAndCopy(fullZh, Path.Combine(DestinationRoot, rel), VersionB);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }

    static void ProcessAndCopy(string source, string dest, string symbol)
    {
        try
        {
            var desDir = Path.GetDirectoryName(dest);
            var desNam = Path.GetFileNameWithoutExtension(dest);
            var desExt = Path.GetExtension(dest);
            var newDes = Path.Combine(desDir!, desNam + symbol + desExt);

            Directory.CreateDirectory(desNam!);
            File.Copy(source, newDes, true);
            Console.WriteLine($"[Duplicated] {newDes}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error processing {source}: {ex.Message}");
        }
    }
}
