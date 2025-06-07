using System.Text.RegularExpressions;

namespace Workers;

/// <summary>
/// Optimize code by keep pragma once instead of using #ifdef
/// </summary>
/// <param name="versionA"></param>
/// <param name="versionB"></param>
/// <param name="destinationRoot"></param>
public partial class PragmaOnceOptimizer(
    string destinationRoot,
    string[] validFilesExtensions)
{
    private readonly string DestinationRoot = destinationRoot;
    private readonly string[] ValidFilesExtensions = validFilesExtensions;

    [GeneratedRegex(@"^\s*#pragma\s+once", RegexOptions.Multiline)]
    private static partial Regex PragmaOnceRegex();

    [GeneratedRegex(@"^\s*#ifndef\s+(\S+)", RegexOptions.Multiline)]
    private static partial Regex IfndefRegex();

    [GeneratedRegex(@"^\s*#define\s+(\S+)", RegexOptions.Multiline)]
    private static partial Regex DefineRegex();

    [GeneratedRegex(@"^\s*#endif(?:\s+.*)?$", RegexOptions.Multiline)]
    private static partial Regex EndifRegex();

    /// <summary>
    ///  - Get Identical files from both folders into result folder with : #ifdef{content}#endif
    ///  - For shared files, generate one that have both changes merged.
    /// </summary>
    /// <returns></returns>
    public void Start()
    {
        try
        {
            var files = ValidFilesExtensions
               .SelectMany(ext => Directory.GetFiles(DestinationRoot, $"*{ext}", SearchOption.AllDirectories))
               .ToArray();

            foreach (var file in files)
                ProcessFiles(file);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }

    static void ProcessFiles(string filePath)
    {
        try
        {
            var fileContent = File.ReadAllText(filePath);

            // Check if #pragma once exists
            if (!PragmaOnceRegex().IsMatch(fileContent))
                return;

            // Find first #ifndef and #define
            var ifndefMatch = IfndefRegex().Match(fileContent);
            if (!ifndefMatch.Success)
                return;

            var defineMatch = DefineRegex().Match(fileContent, ifndefMatch.Index + ifndefMatch.Length);
            if (!defineMatch.Success || ifndefMatch.Groups[1].Value != defineMatch.Groups[1].Value)
                return; // throw new Exception("Couldn't find #define for same value");

            // Remove #ifndef and #define
            fileContent = fileContent.Remove(ifndefMatch.Index, defineMatch.Index + defineMatch.Length - ifndefMatch.Index);

            // Remove last #endif
            var endifMatches = EndifRegex().Matches(fileContent);
            if (endifMatches.Count == 0)
                throw new Exception("Couldn't find last #endif");

            // Get the last match
            var lastEndifMatch = endifMatches[^1]; // Using `^1` to get the last item in C# 8+

            // Remove only the last occurrence
            fileContent = fileContent.Remove(lastEndifMatch.Index, lastEndifMatch.Length);

            // Write back the modified content
            File.WriteAllText(filePath, fileContent);
            Console.WriteLine($"Processed: {filePath}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[Error] processing {filePath}: {ex.Message}");
            Console.WriteLine("Press ENTER to continue ...");
            Console.ReadLine();
        }
    }
}
