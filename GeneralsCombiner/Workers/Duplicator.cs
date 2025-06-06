namespace GeneralsCombiner.Workers;

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

    void ProcessAndCopy(string source, string dest, string symbol)
    {
        try
        {
            var desDir = Path.GetDirectoryName(dest);
            var desNam = Path.GetDirectoryName(dest);
            var desExt = Path.GetExtension(dest);
            var newDes = desDir + desNam + symbol + desExt;

            Directory.CreateDirectory(desNam!);
            File.Copy(source, newDes);
            Console.WriteLine($"[{symbol}] {newDes}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error processing {source}: {ex.Message}");
        }
    }
}
