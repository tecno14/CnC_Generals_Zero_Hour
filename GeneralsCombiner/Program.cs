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
        /*
            VersionA: 16
            VersionB: 288
            Same: 1775
            Custom: 895
         */

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

        // add OG or ZH to projects
        // check files that still in zh and not in og
    }
}