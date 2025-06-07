namespace NewStart;

internal class Program
{
    static void Main()
    {
        var rootDirectoryPath = @"C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour";

#if false
        // Fix this error: '/ZI' and '/Gy-' command-line options are incompatible
        Console.WriteLine($"Processing files to fix this error: " +
            $"\"'/ZI' and '/Gy-' command-line options are incompatible\", {nameof(ZiGyFixer)}");
        ZiGyFixer.ProcessProjectFiles(rootDirectoryPath);
        Console.WriteLine("------------------------------------------------");
        Console.WriteLine();
#endif

#if false
        // Fix this error: treat warnings as errors
        Console.WriteLine($"Processing files to fix this error: " +
            $"\"treat warnings as errors\", {nameof(WarningAsErrorFixer)}");
        WarningAsErrorFixer.ProcessProjectFiles(rootDirectoryPath);
        Console.WriteLine("------------------------------------------------");
        Console.WriteLine();
#endif

#if false
        // Draw dependency graph
        Console.WriteLine("Processing files to draw dependency graph");
        var graph = new DependencyGraph(rootDirectoryPath);
        var outputFile = Path.Combine(rootDirectoryPath, "dependencies.html");
        var result = graph.SaveHtml(outputFile);
        Console.WriteLine($"Dependency graph generated: {outputFile}");
        Console.WriteLine("Open this file in your browser to view the dependency graph");
        Console.WriteLine("------------------------------------------------");
        Console.WriteLine();
#endif

        Console.WriteLine("Processing complete.");
        Console.WriteLine("Press any key to close...");
        Console.ReadKey();
    }
}
