# Microsoft Developer Studio Project File - Name="RTS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

#ifdef OG
CFG=RTS - Win32 Internal
#endif
#ifdef ZH
CFG=RTS - Win32 DebugW3D
#endif
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RTS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
#ifdef OG
!MESSAGE NMAKE /f "RTS.mak" CFG="RTS - Win32 Internal"
#endif
#ifdef ZH
!MESSAGE NMAKE /f "RTS.mak" CFG="RTS - Win32 DebugW3D"
#endif
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RTS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RTS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "RTS - Win32 Internal" (based on "Win32 (x86) Application")
#ifdef ZH
!MESSAGE "RTS - Win32 DebugW3D" (based on "Win32 (x86) Application")
!MESSAGE "RTS - Win32 Profile" (based on "Win32 (x86) Application")
#endif
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RTS/Code", CAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RTS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Main\Release"
# PROP Intermediate_Dir "Main\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
#ifdef OG
# ADD CPP /nologo /G6 /MD /W3 /WX /GX /O2 /Ob2 /I "Libraries\Include" /I "GameEngine\Include" /I "gameenginedevice\Include" /I "Libraries\Source\WWVegas" /I "Libraries\Source\WWVegas\WWLib" /I "Libraries\Source\WWVegas\WWMath" /I "Libraries\Source\WWVegas\WWDebug" /I "Libraries\Source\WWVegas\WWSaveLoad" /I "Libraries\Source\WWVegas\WW3D2" /I "Libraries\Include\Granny" /D "IG_DEBUG_STACKTRACE" /D "NDEBUG" /D "_RELEASE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fr /YX /FD /c

#endif
#ifdef ZH
# ADD CPP /nologo /G6 /MD /W3 /WX /GX /Zi /O2 /Ob2 /I "Libraries\Include" /I "GameEngine\Include" /I "gameenginedevice\Include" /I "Libraries\Source\WWVegas" /I "Libraries\Source\WWVegas\WWLib" /I "Libraries\Source\WWVegas\WWMath" /I "Libraries\Source\WWVegas\WWDebug" /I "Libraries\Source\WWVegas\WWSaveLoad" /I "Libraries\Source\WWVegas\WW3D2" /I "Libraries\Include\Granny" /D "IG_DEBUG_STACKTRACE" /D "NDEBUG" /D "_RELEASE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Fr
#endif
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
#ifdef OG
# ADD LINK32 Benchmark.lib WW3D2.lib WWDebug.lib WWLib.lib WWUtil.lib WWMath.lib GameEngine.lib GameEngineDevice.lib binkw32.lib dxguid.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx8.lib mss32.lib d3d8.lib winmm.lib vfw32.lib dsound.lib wsock32.lib imm32.lib wininet.lib /nologo /subsystem:windows /pdb:"..\Run\RTS.pdb" /map:"..\Run\RTS.map" /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"..\Run\RTS.exe" /libpath:"..\..\GameEngine" /libpath:"GameEngine\Lib" /libpath:"GameEngineDevice\Lib" /libpath:"Libraries\Lib" /libpath:"GameEngine"
#endif
#ifdef ZH
# ADD LINK32 Benchmark.lib WW3D2.lib WWDebug.lib WWLib.lib WWUtil.lib WWMath.lib GameEngine.lib GameEngineDevice.lib binkw32.lib dxguid.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx8.lib mss32.lib d3d8.lib winmm.lib vfw32.lib dsound.lib wsock32.lib imm32.lib wininet.lib /nologo /subsystem:windows /pdb:"..\Run\RTS.pdb" /map:"..\Run\RTS.map" /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"debug.lib" /out:"..\Run\RTS.exe" /libpath:"..\..\GameEngine" /libpath:"GameEngine\Lib" /libpath:"GameEngineDevice\Lib" /libpath:"Libraries\Lib" /libpath:"GameEngine" /OPT:REF /OPT:ICF
#endif
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RTS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Main\Debug"
# PROP Intermediate_Dir "Main\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
#ifdef OG
# ADD CPP /nologo /G6 /MDd /W3 /WX /Gm /GX /ZI /Od /I "Libraries\Include" /I "GameEngine\Include" /I "gameenginedevice\Include" /I "Libraries\Source\WWVegas" /I "Libraries\Source\WWVegas\WWLib" /I "Libraries\Source\WWVegas\WWMath" /I "Libraries\Source\WWVegas\WWDebug" /I "Libraries\Source\WWVegas\WWSaveLoad" /I "Libraries\Source\WWVegas\WW3D2" /I "Libraries\Include\Granny" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "BROWSER_DEBUG" /Fr /YX /FD /GZ /c

#endif
#ifdef ZH
# ADD CPP /nologo /G6 /MDd /W3 /WX /Gm /GX /ZI /Od /I "Libraries\Include" /I "GameEngine\Include" /I "gameenginedevice\Include" /I "Libraries\Source\WWVegas" /I "Libraries\Source\WWVegas\WWLib" /I "Libraries\Source\WWVegas\WWMath" /I "Libraries\Source\WWVegas\WWDebug" /I "Libraries\Source\WWVegas\WWSaveLoad" /I "Libraries\Source\WWVegas\WW3D2" /I "Libraries\Include\Granny" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "BROWSER_DEBUG" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
#endif
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
LINK32=link.exe
#ifdef OG
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:IX86 /machine:I38
#endif
#ifdef ZH
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
#endif
# ADD LINK32 BenchmarkD.lib WW3D2Debug.lib WWDebugDebug.lib WWUtilDebug.lib WWLibDebug.lib WWMathDebug.lib GameEngineDebug.lib GameEngineDeviceDebug.lib binkw32.lib dxguid.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx8.lib mss32.lib d3d8.lib winmm.lib vfw32.lib dsound.lib wsock32.lib imm32.lib wininet.lib /nologo /subsystem:windows /pdb:"..\Run\RTSD.pdb" /map:"..\Run\RTSD.map" /debug /machine:I386 /nodefaultlib:"libcd.lib" /out:"..\Run\RTSD.exe" /libpath:"GameEngine" /libpath:"GameEngine\Lib" /libpath:"GameEngineDevice\Lib" /libpath:"Libraries\Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RTS - Win32 Internal"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Internal"
# PROP BASE Intermediate_Dir "Internal"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Main\Internal"
# PROP Intermediate_Dir "Main\Internal"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
#ifdef OG
# ADD CPP /nologo /G6 /MD /W3 /WX /GX /Zi /O2 /I "Libraries\Include" /I "GameEngine\Include" /I "gameenginedevice\Include" /I "Libraries\Source\WWVegas" /I "Libraries\Source\WWVegas\WWLib" /I "Libraries\Source\WWVegas\WWMath" /I "Libraries\Source\WWVegas\WWDebug" /I "Libraries\Source\WWVegas\WWSaveLoad" /I "Libraries\Source\WWVegas\WW3D2" /I "Libraries\Include\Granny" /D "NDEBUG" /D "_INTERNAL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fr /YX /FD /c

#endif
#ifdef ZH
# ADD CPP /nologo /G6 /MD /W3 /WX /GX /Zi /O2 /I "Libraries\Include" /I "GameEngine\Include" /I "gameenginedevice\Include" /I "Libraries\Source\WWVegas" /I "Libraries\Source\WWVegas\WWLib" /I "Libraries\Source\WWVegas\WWMath" /I "Libraries\Source\WWVegas\WWDebug" /I "Libraries\Source\WWVegas\WWSaveLoad" /I "Libraries\Source\WWVegas\WW3D2" /I "Libraries\Include\Granny" /D "NDEBUG" /D "_INTERNAL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Fr
#endif
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 Benchmark.lib WW3D2Internal.lib WWDebugInternal.lib WWLibInternal.lib WWUtilInternal.lib WWMathInternal.lib GameEngineInternal.lib GameEngineDeviceInternal.lib binkw32.lib dxguid.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx8.lib mss32.lib d3d8.lib winmm.lib vfw32.lib dsound.lib wsock32.lib imm32.lib wininet.lib /nologo /subsystem:windows /pdb:"..\Run\RTSI.pdb" /map:"..\Run\RTSI.map" /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"..\Run\RTSI.exe" /libpath:"GameEngine\Lib" /libpath:"GameEngineDevice\Lib" /libpath:"Libraries\Lib" /libpath:"GameEngine" /fixed:no
# SUBTRACT LINK32 /pdb:none
#ifdef ZH

!ELSEIF  "$(CFG)" == "RTS - Win32 DebugW3D"
#endif

#ifdef ZH
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugW3D"
# PROP BASE Intermediate_Dir "DebugW3D"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Main\DebugW3D"
# PROP Intermediate_Dir "Main\DebugW3D"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /WX /Gm /GX /ZI /Od /I "Libraries\Include" /I "GameEngine\Include" /I "gameenginedevice\Include" /I "Libraries\Source\WWVegas" /I "Libraries\Source\WWVegas\WWLib" /I "Libraries\Source\WWVegas\WWMath" /I "Libraries\Source\WWVegas\WWDebug" /I "Libraries\Source\WWVegas\WWSaveLoad" /I "Libraries\Source\WWVegas\WW3D2" /I "Libraries\Include\Granny" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 BenchmarkD.lib WWDownloadDebug.lib GameSpyHTTPDebug.lib GameSpyPresenceDebug.lib GameSpyStatsDebug.lib GameSpyPeerDebug.lib GameSpyPatchingDebug.lib WW3D2DebugW3D.lib WWDebugDebugW3D.lib WWUtilDebugW3D.lib WWLibDebugW3D.lib WWMathDebugW3D.lib GameEngineDebug.lib GameEngineDeviceDebug.lib binkw32.lib dxguid.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx8.lib mss32.lib d3d8.lib winmm.lib vfw32.lib dsound.lib wsock32.lib imm32.lib wininet.lib /nologo /subsystem:windows /pdb:"..\Run\RTSDW3D.pdb" /map:"..\Run\RTSDW3D.map" /debug /machine:I386 /nodefaultlib:"libcd.lib" /out:"..\Run\RTSDW3D.exe" /libpath:"GameEngine" /libpath:"GameEngine\Lib" /libpath:"GameEngineDevice\Lib" /libpath:"Libraries\Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RTS - Win32 Profile"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Profile"
# PROP BASE Intermediate_Dir "Profile"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Main\Profile"
# PROP Intermediate_Dir "Main\Profile"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /WX /GX /O2 /Ob2 /I "Libraries\Include" /I "GameEngine\Include" /I "gameenginedevice\Include" /I "Libraries\Source\WWVegas" /I "Libraries\Source\WWVegas\WWLib" /I "Libraries\Source\WWVegas\WWMath" /I "Libraries\Source\WWVegas\WWDebug" /I "Libraries\Source\WWVegas\WWSaveLoad" /I "Libraries\Source\WWVegas\WW3D2" /I "Libraries\Include\Granny" /D "IG_DEBUG_STACKTRACE" /D "NDEBUG" /D "_RELEASE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /MD /W3 /WX /GX /Zi /O2 /Ob2 /I "Libraries\Include" /I "GameEngine\Include" /I "gameenginedevice\Include" /I "Libraries\Source\WWVegas" /I "Libraries\Source\WWVegas\WWLib" /I "Libraries\Source\WWVegas\WWMath" /I "Libraries\Source\WWVegas\WWDebug" /I "Libraries\Source\WWVegas\WWSaveLoad" /I "Libraries\Source\WWVegas\WW3D2" /I "Libraries\Include\Granny" /D "IG_DEBUG_STACKTRACE" /D "NDEBUG" /D "_RELEASE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_PROFILE" /YX /FD /Gh /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Benchmark.lib WW3D2.lib WWDebug.lib WWLib.lib WWUtil.lib WWMath.lib GameEngine.lib GameEngineDevice.lib binkw32.lib dxguid.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx8.lib mss32.lib d3d8.lib winmm.lib vfw32.lib dsound.lib wsock32.lib imm32.lib wininet.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /out:"..\Run\RTS.exe" /libpath:"..\..\GameEngine" /libpath:"GameEngine\Lib" /libpath:"GameEngineDevice\Lib" /libpath:"Libraries\Lib" /libpath:"GameEngine"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 Benchmark.lib WW3D2Profile.lib WWDebugProfile.lib WWLibProfile.lib WWUtilProfile.lib WWMathProfile.lib GameEngineProfile.lib GameEngineDeviceProfile.lib binkw32.lib dxguid.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx8.lib mss32.lib d3d8.lib winmm.lib vfw32.lib dsound.lib wsock32.lib imm32.lib wininet.lib gamespyhttp.lib /nologo /subsystem:windows /pdb:"..\Run\RTSP.pdb" /map:"..\Run\RTSP.map" /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"gameengine.lib" /out:"..\Run\RTSP.exe" /libpath:"GameEngine\Lib" /libpath:"GameEngineDevice\Lib" /libpath:"Libraries\Lib" /libpath:"GameEngine"
# SUBTRACT LINK32 /pdb:none

#endif
!ENDIF 

# Begin Target

# Name "RTS - Win32 Release"
# Name "RTS - Win32 Debug"
# Name "RTS - Win32 Internal"
#ifdef ZH
# Name "RTS - Win32 DebugW3D"
# Name "RTS - Win32 Profile"
#endif
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Main\buildVersion.h
# End Source File
# Begin Source File

SOURCE=.\Main\generatedVersion.h

!IF  "$(CFG)" == "RTS - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__GENER="$(ProjDir)\main\winmain.cpp"	"$(ProjDir)\main\winmain.h"	"$(ProjDir)\Libraries\Lib\WW3D2.lib"	"$(ProjDir)\Libraries\Lib\WWDebug.lib"	"$(ProjDir)\Libraries\Lib\WWUtil.lib"	"$(ProjDir)\Libraries\Lib\WWLib.lib"	"$(ProjDir)\Libraries\Lib\WWMath.lib"	"$(ProjDir)\GameEngine\Lib\GameEngine.lib"	"$(ProjDir)\GameEngineDevice\Lib\GameEngineDevice.lib"	
# Begin Custom Build - Incrementing version numbers held in $(InputPath) .\Main\buildVersion.h
ProjDir=.
#ifdef OG
TargetDir=\projects\generals\production\Run
#endif
#ifdef ZH
TargetDir=\projects\generalsmd\production\Run
#endif
InputPath=.\Main\generatedVersion.h

"$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(TargetDir)\versionUpdate.exe $(InputPath) 
	$(TargetDir)\buildVersionUpdate.exe .\Main\buildVersion.h 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "RTS - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__GENER="$(ProjDir)\main\winmain.cpp"	"$(ProjDir)\main\winmain.h"	"$(ProjDir)\Libraries\Lib\WW3D2Debug.lib"	"$(ProjDir)\Libraries\Lib\WWDebugDebug.lib"	"$(ProjDir)\Libraries\Lib\WWUtilDebug.lib"	"$(ProjDir)\Libraries\Lib\WWLibDebug.lib"	"$(ProjDir)\Libraries\Lib\WWMathDebug.lib"	"$(ProjDir)\GameEngine\Lib\GameEngineDebug.lib"	"$(ProjDir)\GameEngineDevice\Lib\GameEngineDeviceDebug.lib"	
# Begin Custom Build - Incrementing version numbers held in $(InputPath) .\Main\buildVersion.h
ProjDir=.
#ifdef OG
TargetDir=\projects\generals\production\Run
#endif
#ifdef ZH
TargetDir=\projects\generalsmd\production\Run
#endif
InputPath=.\Main\generatedVersion.h

"$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(TargetDir)\versionUpdate.exe $(InputPath) 
	$(TargetDir)\buildVersionUpdate.exe .\Main\buildVersion.h 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "RTS - Win32 Internal"

# PROP Ignore_Default_Tool 1
USERDEP__GENER="$(ProjDir)\main\winmain.cpp"	"$(ProjDir)\main\winmain.h"	"$(ProjDir)\Libraries\Lib\WW3D2Internal.lib"	"$(ProjDir)\Libraries\Lib\WWDebugInternal.lib"	"$(ProjDir)\Libraries\Lib\WWUtilInternal.lib"	"$(ProjDir)\Libraries\Lib\WWLibInternal.lib"	"$(ProjDir)\Libraries\Lib\WWMathInternal.lib"	"$(ProjDir)\GameEngine\Lib\GameEngineInternal.lib"	"$(ProjDir)\GameEngineDevice\Lib\GameEngineDeviceInternal.lib"	
#ifdef ZH
# Begin Custom Build - Incrementing version numbers held in $(InputPath) .\Main\buildVersion.h
ProjDir=.
TargetDir=\projects\generalsmd\production\Run
InputPath=.\Main\generatedVersion.h

"$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(TargetDir)\versionUpdate.exe $(InputPath) 
	$(TargetDir)\buildVersionUpdate.exe .\Main\buildVersion.h 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "RTS - Win32 DebugW3D"

# PROP Ignore_Default_Tool 1
USERDEP__GENER="$(ProjDir)\main\winmain.cpp"	"$(ProjDir)\main\winmain.h"	"$(ProjDir)\Libraries\Lib\WW3D2Debug.lib"	"$(ProjDir)\Libraries\Lib\WWDebugDebug.lib"	"$(ProjDir)\Libraries\Lib\WWUtilDebug.lib"	"$(ProjDir)\Libraries\Lib\WWLibDebug.lib"	"$(ProjDir)\Libraries\Lib\WWMathDebug.lib"	"$(ProjDir)\GameEngine\Lib\GameEngineDebug.lib"	"$(ProjDir)\GameEngineDevice\Lib\GameEngineDeviceDebug.lib"	
# Begin Custom Build - Incrementing version numbers held in $(InputPath) .\Main\buildVersion.h
ProjDir=.
TargetDir=\projects\generalsmd\production\Run
InputPath=.\Main\generatedVersion.h

"$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(TargetDir)\versionUpdate.exe $(InputPath) 
	$(TargetDir)\buildVersionUpdate.exe .\Main\buildVersion.h 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "RTS - Win32 Profile"

# PROP BASE Ignore_Default_Tool 1
# PROP Ignore_Default_Tool 1
USERDEP__GENER="$(ProjDir)\main\winmain.cpp"	"$(ProjDir)\main\winmain.h"	"$(ProjDir)\Libraries\Lib\WW3D2.lib"	"$(ProjDir)\Libraries\Lib\WWDebug.lib"	"$(ProjDir)\Libraries\Lib\WWUtil.lib"	"$(ProjDir)\Libraries\Lib\WWLib.lib"	"$(ProjDir)\Libraries\Lib\WWMath.lib"	"$(ProjDir)\GameEngine\Lib\GameEngine.lib"	"$(ProjDir)\GameEngineDevice\Lib\GameEngineDevice.lib"	
#endif
# Begin Custom Build - Incrementing version numbers held in $(InputPath) .\Main\buildVersion.h
ProjDir=.
#ifdef OG
TargetDir=\projects\generals\production\Run
#endif
#ifdef ZH
TargetDir=\projects\generalsmd\production\Run
#endif
InputPath=.\Main\generatedVersion.h

"$(InputPath)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	$(TargetDir)\versionUpdate.exe $(InputPath) 
	$(TargetDir)\buildVersionUpdate.exe .\Main\buildVersion.h 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Main\WinMain.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\WinMain.h
# End Source File
# End Group
# Begin Group "INI Files"

# PROP Default_Filter "*.ini"
# Begin Group "Default"

# PROP Default_Filter "*.ini"
# Begin Source File

SOURCE=..\Run\Data\INI\Default\AIData.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\ButtonSets.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\CommandButton.ini
#ifdef OG
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\CommandMap.ini
#endif
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\ControlBarScheme.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\Crate.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\FXList.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\GameData.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\multiplayer.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\Music.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\Object.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\ObjectCreationList.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\PlayerTemplate.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\Roads.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\Science.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\ShellMenuScheme.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\SoundEffects.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\SpecialPower.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\Speech.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\Terrain.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\Upgrade.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\Video.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\Voice.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Default\Water.ini
# End Source File
# Begin Source File

#ifdef ZH
SOURCE=..\Run\Data\INI\Default\Weather.ini
# End Source File
# Begin Source File

#endif
SOURCE=..\Run\Data\INI\Default\Webpages.ini
# End Source File
# End Group
# Begin Group "MappedImages"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\HandCreated\HandCreatedMappedImages.INI
#ifdef ZH
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\HandCreatedMappedImages.INI
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SAControlBar512.INI
#endif
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SAUserInterface512.INI
#ifdef ZH
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SCGameUserInterface512.INI
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SCGeneralsChallenge512.INI
#endif
# End Source File
# Begin Source File

#ifdef ZH
SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SCLogosUserInterface512.INI
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SCShellUserInterface512.INI
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SCSmShellUserInterface512.INI
# End Source File
# Begin Source File

#endif
SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SCUserInterface512.INI
#ifdef ZH
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SNControlBar512.INI
#endif
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SNUserInterface512.INI
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SSUserInterface512.INI
#ifdef ZH
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SUControlBar512.INI
#endif
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MappedImages\TextureSize_512\SUUserInterface512.INI
# End Source File
# End Group
# Begin Group "Object"
#ifdef ZH

# PROP Default_Filter ""
# Begin Group "America"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Run\Data\INI\Object\AmericaAir.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\AmericaCINEUnit.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\AmericaInfantry.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\AmericaMiscUnit.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\AmericaVehicle.ini
# End Source File
# End Group
# Begin Group "China"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Run\Data\INI\Object\ChinaAir.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\ChinaCINEUnit.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\ChinaInfantry.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\ChinaMiscUnit.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\ChinaVehicle.ini
# End Source File
# End Group
# Begin Group "GLA"

# PROP Default_Filter ""
# Begin Source File
#endif

#ifdef ZH
SOURCE=..\Run\Data\INI\Object\GLAAir.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\GLACINEUnit.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\GLAInfantry.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\GLAMiscUnit.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\GLAVehicle.ini
# End Source File
# End Group
# Begin Group "Challenge Mode"

#endif
# PROP Default_Filter ""
#ifdef ZH
# Begin Source File

SOURCE=..\Run\Data\INI\Object\GC_Chem_GLABuildings.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\GC_Chem_GLASystem.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\GC_Chem_GLAUnits.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\GC_Slth_GLABuildings.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\GC_Slth_GLASystem.ini
# End Source File
#endif
# Begin Source File

#ifdef ZH
SOURCE=..\Run\Data\INI\Object\GC_Slth_GLAUnits.ini
# End Source File
# End Group
# Begin Group "Generals"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Run\Data\INI\Object\AirforceGeneral.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\BossGeneral.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\ChemicalGeneral.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\DemoGeneral.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\InfantryGeneral.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\LaserGeneral.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\NukeGeneral.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\StealthGeneral.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\SuperWeaponGeneral.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\TankGeneral.ini
# End Source File
# End Group
# Begin Source File

#endif
SOURCE=..\Run\Data\INI\Object\CivilianBuilding.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\CivilianProp.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\CivilianUnit.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\FactionBuilding.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\FactionProp.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\FactionUnit.ini
#ifdef ZH
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\Hulk.ini
#endif
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\NatureProp.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\NatureUnit.ini
#ifdef ZH
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\SpecialPowerObjects.ini
#endif
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\System.ini
# End Source File
#ifdef ZH
# Begin Source File

SOURCE=..\Run\Data\INI\Object\TechBuildings.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Object\WeaponObjects.ini
# End Source File
#endif
# End Group
#ifdef ZH
# Begin Group "Scripts"

# PROP Default_Filter "*.ini"
# Begin Source File

SOURCE=..\Run\Data\Scripts\Scripts.ini
# End Source File
# End Group
#endif
# Begin Source File

SOURCE=..\Run\Data\INI\AIData.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Animation2D.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Armor.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\AudioSettings.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Campaign.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\CommandButton.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\CommandMap.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\CommandMapDebug.ini
#ifdef ZH
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\CommandMapDemo.ini
#endif
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\CommandSet.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\ControlBarResizer.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\ControlBarScheme.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Crate.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Credits.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\DamageFX.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\DrawGroupInfo.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Eva.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\FXList.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\GameData.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\GameDataDebug.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\GameLOD.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\GameLODPresets.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\English\HeaderTemplate.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\InGameUI.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\English\Language.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Locomotor.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\MiscAudio.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Mouse.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Multiplayer.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Music.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\ObjectCreationList.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\ParticleSystem.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\PlayerTemplate.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Rank.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Roads.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Science.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\ShellMenuScheme.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\SoundEffects.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\SpecialPower.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Speech.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Terrain.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Upgrade.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Video.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Voice.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Water.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\Weapon.ini
# End Source File
# Begin Source File

#ifdef ZH
SOURCE=..\Run\Data\INI\Weather.ini
# End Source File
# Begin Source File

#endif
SOURCE=..\Run\Data\INI\Webpages.ini
# End Source File
# Begin Source File

SOURCE=..\Run\Data\INI\WindowTransitions.ini
# End Source File
# End Group
# Begin Group "Window"

# PROP Default_Filter ".wnd"
# Begin Group "Menu"

# PROP Default_Filter ".wnd"
# Begin Source File

SOURCE=..\Run\Window\Menus\BlankWindow.wnd
#ifdef ZH
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\ChallengeMenu.wnd
#endif
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\CRCMismatch.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\CreditsMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\Defeat.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\DifficultySelect.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\DisconnectScreen.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\DownloadMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\EstablishConnectionsScreen.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\GameInfoWindow.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\GameSpyGameOptionsMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\GameSpyLoadScreen.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\GameSpyLoginProfile.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\GameSpyLoginQuick.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\LanGameOptionsMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\LanLobbyMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\LanMapSelectMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\LocalDefeat.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\MainMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\MapSelectMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\MapTransferScreen.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\MessageBox.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\MultiplayerLoadScreen.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\NetworkDirectConnect.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\ObserverQuit.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\OptionsMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\PopupCommunicator.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\PopupGameSpyCreateGame.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\PopupHostGame.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\PopupJoinGame.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\PopupLadderDetails.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\PopupLadderSelect.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\PopupLocaleSelect.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\PopupPlayerInfo.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\PopupReplay.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\PopupSaveLoad.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\QuitMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\QuitMessageBox.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\QuitNoSave.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\RCBuddiesMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\RCBuddyRequestMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\RCGameDetailsMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\RCLocalPlayerMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\RCNonBuddiesMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\RCNoProfileMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\ReplayMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\SaveLoad.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\ScoreScreen.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\ShellGameLoadScreen.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\SinglePlayerLoadScreen.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\SinglePlayerMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\SkirmishGameOptionsMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\SkirmishMapSelectMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\Victorious.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\WOLBuddyOverlay.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\WOLCustomLobby.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\WOLLadderScreen.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\WOLLoginMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\WOLMapSelectMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\WOLMessageWindow.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\WOLQuickMatchMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\WOLStatusMenu.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Menus\WOLWelcomeMenu.wnd
# End Source File
# End Group
# Begin Source File

SOURCE=..\Run\Window\ControlBar.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\ControlBarPopupDescription.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\Diplomacy.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\GeneralsExpPoints.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\GenPowersShortcutBarChina.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\GenPowersShortcutBarGLA.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\GenPowersShortcutBarUS.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\IMECandidateWindow.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\IMEStatusWindow.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\InGamePopupMessage.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\MOTD.wnd
# End Source File
# Begin Source File

SOURCE=..\Run\Window\ReplayControl.wnd
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Main\Generals.ico
# End Source File
# Begin Source File

SOURCE=.\Main\resource.h
# End Source File
# Begin Source File

SOURCE=.\Main\RTS.RC
# End Source File
# End Group
# Begin Source File

SOURCE=..\Run\Data\Generals.str
# End Source File
# Begin Source File

SOURCE=.\Main\Install_Final.bmp
# End Source File
# End Target
# End Project
