using System.Linq;

namespace GelFolderFilesExts;

internal class Program
{
    static void Main()
    {
        string folderPath = @"C:\Projects\3_generalsr\temp\CnC_Generals_Zero_Hour\"; // Change this to your target folder

        if (!Directory.Exists(folderPath))
        {
            Console.WriteLine("Folder does not exist.");
            return;
        }

        var distinctExtensions = Directory.GetFiles(folderPath, "*", SearchOption.AllDirectories)
                .Select(file => Path.GetExtension(file).ToLower()) // Get file extensions in lowercase
                .Where(ext => !string.IsNullOrEmpty(ext)) // Exclude empty extensions
                .Distinct(); // Ensure uniqueness

        Console.WriteLine($"Distinct File Extensions: \"{string.Join("\", \"", distinctExtensions)}\"");
        foreach (var ext in distinctExtensions)
        {
            Console.WriteLine(ext);
        }
    }
}
