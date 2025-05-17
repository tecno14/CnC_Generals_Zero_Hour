using DiffPlex;
using DiffPlex.DiffBuilder;
using DiffPlex.DiffBuilder.Model;
using System.Text;
using static System.Net.Mime.MediaTypeNames;

namespace GeneralsCombiner;

internal class Program
{
    const string IgnoreCommentVersion = @"#ifdef OG
**	Command & Conquer Generals(tm)
#endif
#ifdef ZH
**	Command & Conquer Generals Zero Hour(tm)
#endif";

    const string CommentVersion = @"**	Command & Conquer Generals(tm)";

    static void Main(string[] args)
    {
        //if (args.Length != 3)
        //{
        //    Console.WriteLine("Usage: CodeBaseMerger <OGPath> <ZHPath> <ResultPath>");
        //    return;
        //}

        //string ogPath = args[0];
        //string zhPath = args[1];
        //string resultPath = args[2];

        string ogPath = @"C:\Projects\3_generalsr\temp\CnC_Generals_Zero_Hour\Generals";
        string zhPath = @"C:\Projects\3_generalsr\temp\CnC_Generals_Zero_Hour\GeneralsMD";
        string resultPath = @"C:\Projects\3_generalsr\temp\CnC_Generals_Zero_Hour\Combined";

        int og = 0, zh = 0, comm = 0;

        if (Directory.GetDirectories(resultPath).Length != 0 || Directory.GetFiles(resultPath).Length != 0)
        {
            Console.WriteLine("Result folder is not empty, would you like to clear it or exit");
            Console.WriteLine("Y clear, n exit");
            var input = Console.ReadKey();
            if (input.KeyChar != 'Y' && input.KeyChar != 'y')
                return;

            Console.WriteLine();
            Directory.Delete(resultPath, true);
            Directory.CreateDirectory(resultPath);
        }

        // Recursively gather all .cpp/.h files (or adjust patterns as needed)
        var ogFiles = Directory.GetFiles(ogPath, "*.*", SearchOption.AllDirectories);
        var zhFiles = Directory.GetFiles(zhPath, "*.*", SearchOption.AllDirectories);

        // Build lookup of relative paths for quick existence checks
        var ogSet = new HashSet<string>(ogFiles, StringComparer.OrdinalIgnoreCase);
        var zhSet = new HashSet<string>(zhFiles, StringComparer.OrdinalIgnoreCase);

        foreach (var fullOg in ogFiles)
        {
            // Relative path under OG root
            var rel = Path.GetRelativePath(ogPath, fullOg);
            var fullZh = Path.Combine(zhPath, rel);

            if (!File.Exists(fullZh))
            {
                // Only in OG
                og++;
                ProcessAndCopy(fullOg, Path.Combine(resultPath, rel), "OG");
            }
            else
            {
                // Exists in both
                comm++;
                HandleCommonFile(fullOg, fullZh, Path.Combine(resultPath, rel), rel);
            }

            // Remove from zhSet so later we know what's left only in ZH
            zhSet.Remove(fullZh);
        }

        // Files remaining in zhSet are only in ZH
        foreach (var fullZh in zhSet)
        {
            zh++;
            var rel = Path.GetRelativePath(zhPath, fullZh);
            ProcessAndCopy(fullZh, Path.Combine(resultPath, rel), "ZH");
        }

        Console.WriteLine($"Done, OG {og}, ZH {zh}, Common files {comm}");
    }

    /// <summary>
    /// Wraps the contents of <paramref name="sourceFile"/> in #ifdef/#endif
    /// for the given symbol, then writes to <paramref name="destFile"/>,
    /// creating directories as needed.
    /// </summary>
    static void ProcessAndCopy(string sourceFile, string destFile, string symbol)
    {
        // Read original content
        var content = File.ReadAllText(sourceFile);

        // Prepare wrapped content
        var wrapped =
$@"#ifdef {symbol}
{content}
#endif
";

        // Ensure target directory exists
        Directory.CreateDirectory(Path.GetDirectoryName(destFile)!);

        // Write out
        File.WriteAllText(destFile, wrapped);
        Console.WriteLine($"[{symbol}] {sourceFile} -> {destFile}");
    }

    /// <summary>
    /// Stub for handling files present in both OG and ZH.
    /// You can compare contents, merge diffs, etc.
    /// Currently just copies OG version unchanged.
    /// </summary>
    //static void HandleCommonFile(string ogFile, string zhFile, string destFile)
    //{
    //    // TODO: implement merging logic here
    //    // For now, simply copy the ZH version without wrapping

    //    // Ensure the target directory exists
    //    Directory.CreateDirectory(Path.GetDirectoryName(destFile)!);

    //    // Always copy the ZH version
    //    //File.Copy(zhFile, destFile, overwrite: true);
    //    //Console.WriteLine($"[BOTH] Copied ZH version of {zhFile} → {destFile}");
    //}

    static void HandleCommonFile(string ogFile, string zhFile, string destFile, string reletivePath)
    {
        // Read both versions
        string ogText = File.ReadAllText(ogFile);
        string zhText = File.ReadAllText(zhFile);

        // Build a side-by-side diff model
        var diffBuilder = new SideBySideDiffBuilder(new Differ());
        var model = diffBuilder.BuildDiffModel(ogText, zhText);

        var oldLines = model.OldText.Lines;
        var newLines = model.NewText.Lines;
        var sb = new StringBuilder();

        // Walk through aligned lines (padding handles unequal counts)
        for (int i = 0; i < oldLines.Count; i++)
        {
            var o = oldLines[i];
            var n = newLines[i];

            // Unchanged: emit once
            if (o.Type == ChangeType.Unchanged && n.Type == ChangeType.Unchanged)
            {
                sb.AppendLine(o.Text);
                continue;
            }

            // If any OG-only content at this line, group a run
            if (o.Type != ChangeType.Unchanged)
            {
                sb.AppendLine("#ifdef OG");
                // Collect contiguous OG removals/changes
                int j = i;
                while (j < oldLines.Count && oldLines[j].Type != ChangeType.Unchanged)
                {
                    sb.AppendLine(oldLines[j].Text);
                    j++;
                }
                sb.AppendLine("#endif");
            }

            // If any ZH-only content at this line, group a run
            if (n.Type != ChangeType.Unchanged)
            {
                sb.AppendLine("#ifdef ZH");
                // Collect contiguous ZH inserts/changes
                int j = i;
                while (j < newLines.Count && newLines[j].Type != ChangeType.Unchanged)
                {
                    sb.AppendLine(newLines[j].Text);
                    j++;
                }
                sb.AppendLine("#endif");
            }

            // Advance i past the longest run we just consumed
            // (to avoid re-emitting the same block)
            int maxAdvance = 1;
            if (o.Type != ChangeType.Unchanged || n.Type != ChangeType.Unchanged)
            {
                int oldRun = 0, newRun = 0;
                // compute run lengths
                while (i + oldRun < oldLines.Count && oldLines[i + oldRun].Type != ChangeType.Unchanged) oldRun++;
                while (i + newRun < newLines.Count && newLines[i + newRun].Type != ChangeType.Unchanged) newRun++;
                maxAdvance = Math.Max(oldRun, newRun);
            }
            i += (maxAdvance - 1);
        }

        // after your loop, before writing:
        var mergedText = sb.ToString()
            .TrimEnd('\r', '\n')   // remove all trailing CR/LF
            .Replace(IgnoreCommentVersion, CommentVersion);

        // Ensure result directory exists and write out
        Directory.CreateDirectory(Path.GetDirectoryName(destFile)!);
        File.WriteAllText(destFile, mergedText);
        Console.WriteLine($"[BOTH] Diff-merged {reletivePath}");
    }
}
