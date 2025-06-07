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

        // Add versions as projects variables for project files
#if true
        var varDefiner = new VarDefiner("OG", "ZH", destinationPath);
        varDefiner.Start();
#endif
        /*
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\RTS.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\RTS.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\RTS_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\RTS_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\GameEngine\GameEngine.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\GameEngine\GameEngine.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\GameEngine\GameEngine_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\GameEngine\GameEngine_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\GameEngineDevice\GameEngineDevice.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\GameEngineDevice\GameEngineDevice.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\GameEngineDevice\GameEngineDevice_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\GameEngineDevice\GameEngineDevice_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\assetcull\assetcull_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\assetcull\assetcull_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\buildVersionUpdate\buildVersionUpdate.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\buildVersionUpdate\buildVersionUpdate.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\buildVersionUpdate\buildVersionUpdate_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\buildVersionUpdate\buildVersionUpdate_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\Compress\Compress.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\Compress\Compress.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\CRCDiff\CRCDiff_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\CRCDiff\CRCDiff_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\DebugWindow\DebugWindow.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\DebugWindow\DebugWindow.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\DebugWindow\DebugWindow_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\DebugWindow\DebugWindow_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\GUIEdit\GUIEdit.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\GUIEdit\GUIEdit.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\GUIEdit\GUIEdit_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\GUIEdit\GUIEdit_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\ImagePacker\ImagePacker.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\ImagePacker\ImagePacker.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\ImagePacker\ImagePacker_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\ImagePacker\ImagePacker_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\Launcher\launcher.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\Launcher\launcher.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\Launcher\launcher_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\Launcher\launcher_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\MapCacheBuilder\MapCacheBuilder.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\MapCacheBuilder\MapCacheBuilder.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\MapCacheBuilder\MapCacheBuilder_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\MapCacheBuilder\MapCacheBuilder_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\ParticleEditor\ParticleEditor.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\ParticleEditor\ParticleEditor.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\ParticleEditor\ParticleEditor_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\ParticleEditor\ParticleEditor_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\PATCHGET\patchgrabber_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\PATCHGET\patchgrabber_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\textureCompress\textureCompress.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\textureCompress\textureCompress.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\versionUpdate\versionUpdate.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\versionUpdate\versionUpdate.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\versionUpdate\versionUpdate_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\versionUpdate\versionUpdate_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\WorldBuilder\WorldBuilder.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\WorldBuilder\WorldBuilder.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\WorldBuilder\WorldBuilder_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\WorldBuilder\WorldBuilder_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\Benchmark\Benchmark_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\Benchmark\Benchmark_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\Compression\Compression.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\Compression\Compression.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\Compression\Compression_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\Compression\Compression_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\debug\debug_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\debug\debug_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\EABrowserDispatch\EABrowserDispatch.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\EABrowserDispatch\EABrowserDispatch.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\EABrowserDispatch\EABrowserDispatch_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\EABrowserDispatch\EABrowserDispatch_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\profile\profile_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\profile\profile_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\Launcher\DatGen\DatGen.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\Launcher\DatGen\DatGen.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\Launcher\DatGen\DatGen_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\Launcher\DatGen\DatGen_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\WW3D\max2w3d\max2w3d.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\WW3D\max2w3d\max2w3d.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\WW3D\pluglib\pluglib.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Tools\WW3D\pluglib\pluglib.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WW3D2\ww3d2.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WW3D2\ww3d2.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WW3D2\ww3d2_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WW3D2\ww3d2_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWDebug\wwdebug.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWDebug\wwdebug.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWDebug\wwdebug_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWDebug\wwdebug_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWDownload\WWDownload.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWDownload\WWDownload.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWDownload\WWDownload_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWDownload\WWDownload_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWLib\wwlib.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWLib\wwlib.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWLib\wwlib_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWLib\wwlib_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWMath\wwmath.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWMath\wwmath.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWMath\wwmath_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWMath\wwmath_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWSaveLoad\wwsaveload.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWSaveLoad\wwsaveload.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWSaveLoad\wwsaveload_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\WWSaveLoad\wwsaveload_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\wwshade\wwshade_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\wwshade\wwshade_ZH.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\Wwutil\wwutil.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\Wwutil\wwutil.vcxproj
[DSP Deleted] old project file detected and deleted (C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\Wwutil\wwutil_ZH.dsp)
[Updated] C:\Projects\3_generals\src\Game\CnC_Generals_Zero_Hour\Generals\Code\Libraries\Source\WWVegas\Wwutil\wwutil_ZH.vcxproj
         */

        // - solve ifdef confligts:
        //      merge code using #ifdef wasn't perfect which made mismatch errors,
        //      because of many commit happened after that, I have to make new merge only for code files,
        //      then check using the old way to check if result is same as current file (not changed for other stuff),
        //      then check if there is #ifdef or #endif in each block, if so ...fix it and save it
        //      add comment at #endif 
        // - check files that still in zh and not in og
        // - fix c++ upgrade from98 to new version errors
    }
}