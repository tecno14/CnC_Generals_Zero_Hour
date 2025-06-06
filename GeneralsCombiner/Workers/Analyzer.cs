using System.Security.Cryptography;
using System.Text;

namespace GeneralsCombiner.Workers;

public static class Analyzer
{
    /// <summary>
    /// Check only:
    /// get both versions paths
    /// ignore ignored extensions
    /// for each file check if there are change to it in the next version
    /// order by after last point
    /// </summary>
    /// <param name="d1"></param>
    /// <param name="d2"></param>
    /// <returns></returns>
    public static Dictionary<string, List<string>> Copmare(string d1, string d2)
    {
        var result = new Dictionary<string, List<string>>();
        var newVersionFiles = Directory.GetFiles(d2, "*", SearchOption.AllDirectories);

        foreach (var file in newVersionFiles)
        {
            var oldFile = file.Replace(d2, d1);
            if (
                    !File.Exists(oldFile) ||
                    Constants.CheckedFilesExtensions.Any(e => file.EndsWith(e, StringComparison.OrdinalIgnoreCase)) ||
                    AreFilesIdentical(file, oldFile)
                )
                continue;

            var fileName = Path.GetFileName(file);
            int firstDotIndex = fileName.IndexOf('.');
            var extension = (firstDotIndex != -1 ? fileName[firstDotIndex..] : fileName).ToLower();

            if (result.TryGetValue(extension, out List<string>? value))
                value.Add(file);
            else
                result.Add(extension, [file]);
        }

        return result;
    }

    static bool AreFilesIdentical(string path1, string path2)
    {
        //// Step 1: Compare file sizes (quick elimination)
        //if (AreFilesSameSize(path1, path2))
        //    return true;

        // Step 2: Compare content using hash
        if (CompareFilesByHash(path1, path2))
            return true;

        // Step 2: Compare content using hash (after replace version)
        return CompareFilesByHashWithReplacement(path1, path2);
    }

    [Obsolete("False result", true)]
    static bool AreFilesSameSize(string path1, string path2)
    {
        var file1 = new FileInfo(path1);
        var file2 = new FileInfo(path2);
        return file1.Length != file2.Length;
    }

    static bool CompareFilesByHash(string path1, string path2)
    {
        using var sha256 = SHA256.Create();
        using var stream1 = File.OpenRead(path1);
        using var stream2 = File.OpenRead(path2);

        var hash1 = sha256.ComputeHash(stream1);
        var hash2 = sha256.ComputeHash(stream2);

        return hash1.SequenceEqual(hash2);
    }

    static bool CompareFilesByHashWithReplacement(string path1, string path2)
    {
        // Read and process first file
        var hash1 = SHA256.HashData(Encoding.UTF8.GetBytes(
            File.ReadAllText(path1).ReplaceVersionComments()));

        // Read and process second file
        var hash2 = SHA256.HashData(Encoding.UTF8.GetBytes(
            File.ReadAllText(path1).ReplaceVersionComments()));

        return hash1.SequenceEqual(hash2);
    }

    static Dictionary<string, List<string>> GetFilesGroupedByExtension(string directoryPath)
    {
        if (!Directory.Exists(directoryPath))
        {
            Console.WriteLine("Directory does not exist.");
            return [];
        }

        var files = Directory.GetFiles(directoryPath, "*", SearchOption.AllDirectories);

        var filesByExtension = files
            .Select(file =>
            {
                var fileName = Path.GetFileName(file);
                int firstDotIndex = fileName.IndexOf('.');
                return firstDotIndex != -1 ? new { Extension = fileName[(firstDotIndex + 1)..].ToLower(), Path = file } : null;
            })
            .Where(fileData => fileData != null)
            .GroupBy(fileData => fileData!.Extension)
            .OrderBy(group => group.Key)
            .ToDictionary(group => group.Key, group => group.Select(fileData => fileData.Path).ToList());

        return filesByExtension;
    }

    public static void GenerateReport(Dictionary<string, List<string>> data)
    {
        Console.WriteLine($"Total extensions count: {data.Count}, total files count: {data.Values.Sum(v => v.Count)}");
        foreach (var ext in data)
        {
            Console.WriteLine($"{ext.Key}: {ext.Value.Count}, example: {ext.Value.First()}");
        }
    }
}
