using GeneralsCombiner.Workers;

namespace GeneralsCombiner;

internal class Program
{
    static void Main()
    {
        var v1DirectoryPath = @"C:\Projects\3_generals\src\temp\test new updater\Generals";
        var v2DirectoryPath = @"C:\Projects\3_generals\src\temp\test new updater\GeneralsMD";
        var destinationPath = @"C:\Projects\3_generals\src\temp\test new updater\newGenerals2\Generals";

        // Compare two versions files
#if false
        var analyzerResult = Analyzer.Copmare(v1DirectoryPath, v2DirectoryPath);
        Analyzer.GenerateReport(analyzerResult);
#endif
        /*
            Done.
         */

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
        var duplicator = new Duplicator("_ZH", v2DirectoryPath, destinationPath,
            [.. Constants.OldProjectFiles, .. Constants.ProjectFiles]);
        //duplicator.Start();
    }
}