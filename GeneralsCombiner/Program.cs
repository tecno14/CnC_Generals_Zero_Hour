using Workers;

namespace GeneralsCombiner;

internal class Program
{
    static void Main()
    {
        var v1DirectoryPath = @"C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals";
        var v2DirectoryPath = @"C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\GeneralsMD";
        var destinationPath = @"C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals";

        // Compare two versions files
#if false
        var analyzerResult = Analyzer.Copmare(v1DirectoryPath, v2DirectoryPath);
        Analyzer.GenerateReport(analyzerResult);
#endif

// Merge VersionDependent files
#if false
        var merger = new Merger("OG", "ZH",
            v1DirectoryPath, v2DirectoryPath, destinationPath, Constants.VersionDependent);
        var mergerResult = merger.Start();
        Merger.GenerateReport(mergerResult);
#endif

// Duplicate OldProjectFiles and ProjectFiles
#if false
        var duplicator = new Duplicator("_ZH", v2DirectoryPath, destinationPath,
            [.. Constants.OldProjectFiles, .. Constants.ProjectFiles]);
        duplicator.Start();
#endif

// Replace unchanged files to make sure everything is OK
#if false
        var replacer = new Replacer(v2DirectoryPath, destinationPath,
            [.. Constants.OriginalUnchanged]);
        replacer.Start();
#endif

// Replace manual changed files for compair files
#if false
        foreach (var ext in Constants.MergedManually)
        {
            var replacer = new Replacer(v2DirectoryPath, destinationPath, [ext], false);
            replacer.Start();
        }
#endif

// Add versions as projects variables for project files
#if false
        var varDefiner = new VarDefiner("OG", "ZH", destinationPath);
        varDefiner.Start();
#endif

// Apply PragmaOnceOptimizer and remove additional check (to fix mismatch)
#if true
        var pragmaOptimizer = new PragmaOnceOptimizer(destinationPath, Constants.VersionDependent);
        pragmaOptimizer.Start();
#endif

        // - solve ifdef confligts:
        //      merge code using #ifdef wasn't perfect which made mismatch errors,
        //      because of many commit happened after that, I have to make new merge only for code files,
        //      then check using the old way to check if result is same as current file (not changed for other stuff),
        //      then check if there is #ifdef or #endif in each block, if so ...fix it and save it
        //      add comment at #endif 

        // temp

        //var tmp_gv1 = @"C:\Projects\3_generals\src\temp\new merger\CnC_Generals_Zero_Hour\Generals";
        //var tmp_gv2 = @"C:\Projects\3_generals\src\temp\new merger\CnC_Generals_Zero_Hour\GeneralsMD";
        //var tmp_gv3 = @"C:\Projects\3_generals\src\temp\new merger\CnC_Generals_Zero_Hour\Generals3";

        //1- delete all files but VersionDependent
        //var otherFiles = Directory.GetFiles(tmp_gv1, "*", SearchOption.AllDirectories)
        //    .Where(file => !Constants.VersionDependent.Any(ex => Path.GetExtension(file).Equals(ex, StringComparison.OrdinalIgnoreCase)))
        //    .ToList();

        //otherFiles.ForEach(File.Delete);
        //Console.WriteLine($"({otherFiles.Count}) files removed");

        //var oldmerger = new Merger("OG", "ZH",
        //    tmp_gv1, tmp_gv2, destinationPath,
        //    Constants.VersionDependent);
        //var oldmergerResult = oldmerger.Start();
        //Merger.GenerateReport(oldmergerResult);

        // - check files that still in zh and not in og
        // - fix c++ upgrade from98 to new version errors
    }
}