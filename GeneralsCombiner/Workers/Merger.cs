using DiffPlex;
using DiffPlex.DiffBuilder;
using DiffPlex.DiffBuilder.Model;
using GeneralsCombiner;
using System.Text;
using System.Text.RegularExpressions;

namespace Workers;

/// <summary>
/// Used to merge two versions of code files, by adding #ifdef at changes
/// </summary>
/// <param name="versionA"></param>
/// <param name="versionB"></param>
/// <param name="versionARoot"></param>
/// <param name="versionBRoot"></param>
/// <param name="destinationRoot"></param>
/// <param name="validFilesExtensions"></param>
public class Merger(
    string versionA,
    string versionB,
    string versionARoot,
    string versionBRoot,
    string destinationRoot,
    string[] validFilesExtensions)
{
    private readonly string VersionA = versionA;
    private readonly string VersionB = versionB;
    private readonly string VersionARoot = versionARoot;
    private readonly string VersionBRoot = versionBRoot;
    private readonly string DestinationRoot = destinationRoot;
    private readonly string[] ValidFilesExtensions = validFilesExtensions;

    /// <summary>
    ///  - Get Identical files from both folders into result folder with : #ifdef{content}#endif
    ///  - For shared files, generate one that have both changes merged.
    /// </summary>
    /// <returns></returns>
    public List<DiffResult> Start()
    {
        List<DiffResult> diffs = [];

        try
        {
            var ogFiles = ValidFilesExtensions
                .SelectMany(ext => Directory.GetFiles(VersionARoot, $"*{ext}", SearchOption.AllDirectories))
                .ToArray();

            var zhFiles = ValidFilesExtensions
                .SelectMany(ext => Directory.GetFiles(VersionBRoot, $"*{ext}", SearchOption.AllDirectories))
                .ToArray();

            var ogRelative = ogFiles.ToDictionary(f => Path.GetRelativePath(VersionARoot, f), f => f);
            var zhRelative = zhFiles.ToDictionary(f => Path.GetRelativePath(VersionBRoot, f), f => f);

            foreach (var (rel, fullOg) in ogRelative)
            {
                if (zhRelative.TryGetValue(rel, out var fullZh))
                {
                    var result = HandleCommonFile(fullOg, fullZh, rel);
                    zhRelative.Remove(rel);
                    diffs.Add(result);
                }
                else
                {
                    ProcessAndCopy(fullOg, Path.Combine(DestinationRoot, rel), VersionA);
                    diffs.Add(DiffResult.VersionA);
                }
            }

            foreach (var (rel, fullZh) in zhRelative)
            {
                ProcessAndCopy(fullZh, Path.Combine(DestinationRoot, rel), VersionB);
                diffs.Add(DiffResult.VersionB);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }

        return diffs;
    }

    static void ProcessAndCopy(string source, string dest, string symbol)
    {
        try
        {
            var content = File.ReadAllText(source);
            Directory.CreateDirectory(Path.GetDirectoryName(dest)!);
            File.WriteAllText(dest, $"#ifdef {symbol}\r\n{content}\r\n#endif\r\n");
            Console.WriteLine($"[{symbol}] {dest}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error processing {source}: {ex.Message}");
        }
    }

    DiffResult HandleCommonFile(string ogFile, string zhFile, string relPath)
    {
        try
        {
            var destFile = Path.Combine(DestinationRoot, relPath);
            var ext = Path.GetExtension(ogFile).ToLower();

            var ogContent = File.ReadAllText(ogFile);
            var zhContent = File.ReadAllText(zhFile);

            zhContent = zhContent.ReplaceVersionComments();

            string trimmed = GetTrimmedEnd(ogContent);
            string lineEnding = GetLineEnding(ogContent);

            var differ = new Differ();
            var builder = new SideBySideDiffBuilder(differ);
            var diff = builder.BuildDiffModel(ogContent, zhContent);

            var merged = new StringBuilder();
            int oldIndex = 0, newIndex = 0;

            while (oldIndex < diff.OldText.Lines.Count || newIndex < diff.NewText.Lines.Count)
            {
                var oldLine = oldIndex < diff.OldText.Lines.Count ? diff.OldText.Lines[oldIndex] : null;
                var newLine = newIndex < diff.NewText.Lines.Count ? diff.NewText.Lines[newIndex] : null;

                if (oldLine?.Type == ChangeType.Unchanged && newLine?.Type == ChangeType.Unchanged)
                {
                    merged.AppendLine(oldLine.Text);
                    oldIndex++;
                    newIndex++;
                }
                else
                {
                    ProcessDiffBlock(diff, ref oldIndex, ref newIndex, merged);
                }
            }

            string mergedText = merged.ToString().TrimEnd() + trimmed;
            Directory.CreateDirectory(Path.GetDirectoryName(destFile)!);
            File.WriteAllText(destFile, mergedText);

            if (mergedText.Trim().Equals(ogContent.Trim()))
            {
                Console.WriteLine($"[SAME] {relPath}");
                return DiffResult.Same;
            }

            Console.WriteLine($"[MERGED] {relPath}");
            return DiffResult.Custom;
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error merging {relPath}: {ex.Message}");
            Console.WriteLine("Press Enter to continue");
            Console.ReadLine();
            return DiffResult.Error;
        }
    }

    // NEW: Calculate trailing empty lines from OG content
    //static int GetTrailingEmptyLines(string content)
    //{
    //    var lines = content.Replace("\r\n", "\n").Split('\n');
    //    int count = 0;
    //    for (int i = lines.Length - 1; i >= 0; i--)
    //    {
    //        if (string.IsNullOrEmpty(lines[i])) count++;
    //        else break;
    //    }
    //    return count;
    //}

    public static string GetTrimmedEnd(string input)
    {
        if (string.IsNullOrEmpty(input)) return string.Empty; // Handle null or empty strings

        int originalLength = input.Length;
        int trimmedLength = input.TrimEnd().Length;

        return input.Substring(trimmedLength, originalLength - trimmedLength); // Extract removed portion
    }

    void ProcessDiffBlock(SideBySideDiffModel diff, ref int oldIndex, ref int newIndex,
                               StringBuilder merged)
    {
        var ogBuffer = new StringBuilder();
        var zhBuffer = new StringBuilder();

        // Capture OG changes
        while (oldIndex < diff.OldText.Lines.Count &&
               diff.OldText.Lines[oldIndex].Type != ChangeType.Unchanged)
        {
            ogBuffer.AppendLine(diff.OldText.Lines[oldIndex].Text);
            oldIndex++;
        }

        // Capture ZH changes
        while (newIndex < diff.NewText.Lines.Count &&
               diff.NewText.Lines[newIndex].Type != ChangeType.Unchanged)
        {
            zhBuffer.AppendLine(diff.NewText.Lines[newIndex].Text);
            newIndex++;
        }

        // Add conditional blocks
        AddConditionalBlock(ogBuffer, VersionA, merged);
        AddConditionalBlock(zhBuffer, VersionB, merged);
    }

    static void AddConditionalBlock(StringBuilder content, string symbol, StringBuilder merged)
    {
        if (content.Length == 0) return;

        var processed = CollapseBlankLines(content.ToString());
        if (!IsAllBlank(processed))
        {
            merged.AppendLine($"#ifdef {symbol}");
            merged.Append(processed);
            merged.AppendLine("#endif");
        }
    }

    static bool IsAllBlank(string text)
    {
        return text
            .Replace("\r\n", "\n")
            .Split('\n')
            .All(line => string.IsNullOrWhiteSpace(line));
    }

    static string CollapseBlankLines(string text)
    {
        return Regex.Replace(text, @"(\r?\n){3,}", "\n\n");
    }

    static string GetLineEnding(string content)
    {
        return content.EndsWith("\r\n") ? "\r\n" : "\n";
    }

    public static void GenerateReport(List<DiffResult> results)
    {
        Console.WriteLine($"Result:{Environment.NewLine}" +
           $"VersionA: {results.Count(r => r == DiffResult.VersionA)}{Environment.NewLine}" +
           $"VersionB: {results.Count(r => r == DiffResult.VersionB)}{Environment.NewLine}" +
           $"Same: {results.Count(r => r == DiffResult.Same)}{Environment.NewLine}" +
           $"Custom: {results.Count(r => r == DiffResult.Custom)}{Environment.NewLine}"
        );
    }
}
