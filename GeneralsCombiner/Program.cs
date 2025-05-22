/*
 - Get Identical files from both folders into result folder with : #ifdef{content}#endif
 - For shared files, generate one that have both changes merged.
 */

using DiffPlex;
using DiffPlex.DiffBuilder;
using DiffPlex.DiffBuilder.Model;
using System.Text;
using System.Text.RegularExpressions;

namespace GeneralsCombiner
{
    internal class Program
    {
        const string IgnoreCommentVersion = "**	Command & Conquer Generals Zero Hour(tm)";
        const string CommentVersion = "**	Command & Conquer Generals(tm)";
        static readonly HashSet<string> ValidFiles = new HashSet<string>(
            new[] { ".gitignore", ".dsp", ".dsw", ".txt", ".h", ".rc", ".cpp",
                    ".odl", ".hm", ".pl", ".idl", ".rc2", ".c", ".vsh", ".psh",
                    ".nvp", ".nvv" },
            StringComparer.OrdinalIgnoreCase
        );

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

            string ogPath = @"C:\Projects\3_generalsr\Original Code Unchanged plus combine\CnC_Generals_Zero_Hour\Generals";
            string zhPath = @"C:\Projects\3_generalsr\Original Code Unchanged plus combine\CnC_Generals_Zero_Hour\GeneralsMD";
            string resultPath = @"C:\Projects\3_generalsr\Original Code Unchanged plus combine\CnC_Generals_Zero_Hour\Combined";


            try
            {
                int og = 0, zh = 0, comm = 0;
                CleanResultDirectory(resultPath);
                
                var ogFiles = Directory.GetFiles(ogPath, "*.*", SearchOption.AllDirectories);
                var zhFiles = Directory.GetFiles(zhPath, "*.*", SearchOption.AllDirectories);

                var ogRelative = ogFiles.ToDictionary(f => Path.GetRelativePath(ogPath, f), f => f);
                var zhRelative = zhFiles.ToDictionary(f => Path.GetRelativePath(zhPath, f), f => f);

                foreach (var (rel, fullOg) in ogRelative)
                {
                    if (zhRelative.TryGetValue(rel, out var fullZh))
                    {
                        comm++;
                        HandleCommonFile(fullOg, fullZh, Path.Combine(resultPath, rel), rel);
                        zhRelative.Remove(rel);
                    }
                    else
                    {
                        og++;
                        ProcessAndCopy(fullOg, Path.Combine(resultPath, rel), "OG");
                    }
                }

                foreach (var (rel, fullZh) in zhRelative)
                {
                    zh++;
                    ProcessAndCopy(fullZh, Path.Combine(resultPath, rel), "ZH");
                }

                Console.WriteLine($"Done, OG: {og}, ZH: {zh}, Common: {comm}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
        }

        static void CleanResultDirectory(string resultPath)
        {
            if (Directory.Exists(resultPath))
            {
                Directory.Delete(resultPath, true);
                Directory.CreateDirectory(resultPath);
            }
        }

        static void ProcessAndCopy(string source, string dest, string symbol)
        {
            try
            {
                var ext = Path.GetExtension(source);
                if (!ValidFiles.Contains(ext))
                {
                    Directory.CreateDirectory(Path.GetDirectoryName(dest)!);
                    File.Copy(source, dest, true);
                    return;
                }

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

        static void HandleCommonFile(string ogFile, string zhFile, string destFile, string relPath)
        {
            try
            {
                var ext = Path.GetExtension(ogFile);
                if (!ValidFiles.Contains(ext))
                {
                    Directory.CreateDirectory(Path.GetDirectoryName(destFile)!);
                    File.Copy(ogFile, destFile, true);
                    return;
                }

                var ogContent = File.ReadAllText(ogFile);
                var zhContent = File.ReadAllText(zhFile);

                // Targeted comment replacement using regex
                //zhContent = zhContent.Replace(zhContent, $"^{Regex.Escape(IgnoreCommentVersion)}$",
                //    CommentVersion, RegexOptions.Multiline);
                zhContent = zhContent.Replace(IgnoreCommentVersion, CommentVersion);

                // NEW: Get OG's trailing empty line count and line ending
                //int trailingEmptyLines = GetTrailingEmptyLines(ogContent);
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
                //mergedText = TrimEndNewLines(mergedText);  // Remove all existing newlines at end
                //mergedText += string.Join("", Enumerable.Repeat(lineEnding, trailingEmptyLines));

                Directory.CreateDirectory(Path.GetDirectoryName(destFile)!);
                //File.WriteAllText(destFile, merged.ToString().TrimEnd() + GetLineEnding(ogContent));
                File.WriteAllText(destFile, mergedText);
                Console.WriteLine($"[MERGED] {relPath}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error merging {relPath}: {ex.Message}");
                Console.WriteLine("Press Enter to continue");
                Console.ReadLine();
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


        // NEW: Aggressively trim all newline characters from end
        static string TrimEndNewLines(string input)
        {
            int trimLength = 0;
            for (int i = input.Length - 1; i >= 0; i--)
            {
                if (input[i] is '\n' or '\r') trimLength++;
                else break;
            }
            return trimLength > 0 ? input[..^trimLength] : input;
        }


        static void ProcessDiffBlock(SideBySideDiffModel diff, ref int oldIndex, ref int newIndex, 
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
            AddConditionalBlock(ogBuffer, "OG", merged);
            AddConditionalBlock(zhBuffer, "ZH", merged);
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
            return text.Replace("\r\n", "\n")
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
    }
}