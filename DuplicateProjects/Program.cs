/*
 - This make a copy of 'ValidFiles' files, add a world at end of it's name, so making 
 */
namespace DuplicateProjects;

internal class Program
{
    static readonly HashSet<string> ValidFiles = new(
            [".sln", ".vcxproj", ".vcxproj.filters"],
            StringComparer.OrdinalIgnoreCase
        );

    static void Main(string[] args)
    {
        string projectPath = @"C:\Projects\3_generalsr\Original Code Unchanged plus combine\CnC_Generals_Zero_Hour\GeneralsMD";
        string resultPath = @"C:\Projects\3_generalsr\Original Code Unchanged plus combine\CnC_Generals_Zero_Hour\Duplicated";
        string addon = @"_ZH";

        try
        {
            var i = 0;
            CleanResultDirectory(resultPath);

            var projFiles = Directory.GetFiles(projectPath, "*.*", SearchOption.AllDirectories);
            foreach (var file in projFiles)
            {
                if (!ValidFiles.Contains(Path.GetExtension(file)))
                    continue;

                var folder = Path.GetDirectoryName(file);
                var rel = Path.GetRelativePath(projectPath, folder!);
                var newFolder = Path.Combine(resultPath, rel);
                var newFileName = $"{Path.GetFileNameWithoutExtension(file)}{addon}{Path.GetExtension(file)}";
                var newFile = Path.Combine(newFolder, newFileName);

                var content = File.ReadAllText(file);
                content = content.Replace(".vcxproj", $"{addon}.vcxproj");

                Directory.CreateDirectory(newFolder);
                File.WriteAllText(newFile, content);

                Console.WriteLine($"Copy: {newFile}");
                i++;
            }
            Console.WriteLine($"Done, Copied: {i}.");
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

}