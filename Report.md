
# **Notes about Code**

## **I - Missing Projects**  

Below is a list of projects and their availability across different games:  

☑ Missing
☐ Exist

| #  | Game                | Generals | GeneralsMD |
|----|---------------------|---------|------------|
| 1  | Benchmark          | ☑       | ☐          |
| 2  | GameSpyHTTP        | ☑       | ☑          |
| 3  | GameSpyPatching    | ☑       | ☑          |
| 4  | GameSpyPeer        | ☑       | ☑          |
| 5  | GameSpyPresence    | ☑       | ☑          |
| 6  | GameSpyStats       | ☑       | ☑          |

---

## **II - Dependencies**  

We need to find or write replacements (or remove code using them entirely) for the following dependencies:  

| #  | Dependency Name                      | Expected Path | Added  |
|----|--------------------------------------|-----------------------------------------------|-------|
| 1  | DirectX SDK (Version 9.0 or higher) | `\Code\Libraries\DirectX\`                   | ☑
| 2  | STLport (4.5.3)                      | `\Code\Libraries\STLport - 4.5.3`            | ☑
| 3  | 3DSMax 4 SDK                         | `\Code\Libraries\Max4SDK\`                   | ☐
| 4  | NVASM                                | `\Code\Tools\NVASM\`                         | ☐
| 5  | BYTEmark                             | `\Code\Libraries\Source\Benchmark`           | ☐
| 6  | RAD Miles Sound System SDK           | `\Code\Libraries\Source\WWVegas\Miles6\`     | ☑
| 7  | RAD Bink SDK                         | `\Code\GameEngineDevice\Include\VideoDevice\Bink` | ☐
| 8  | SafeDisk API                         | `\Code\GameEngine\Include\Common\SafeDisk` <br> `\Code\Tools\Launcher\SafeDisk\` | ☑
| 9  | Miles Sound System "Asimp3"          | `\Code\Libraries\WPAudio\Asimp3`             | ☐
| 10 | GameSpy SDK                          | `\Code\Libraries\Source\GameSpy\`            | ☐
| 11 | ZLib (1.1.4)                         | `\Code\Libraries\Source\Compression\ZLib\`   | ☑
| 12 | LZH - Light (1.0)                    | `\Code\Libraries\Source\Compression\LZHCompress\CompLibSource` <br> `\CompLibHeader` | ☑

---

## **III - Progress**  

| #  | Details                                      | Progress       |
|----|---------------------------------------------|------------|
| 1  | Update project files and `.sln` solutions  | ✅ Done |
| 2  | Adding dependencies and try build  | ⏳ In Progress |
| 3  | Move dependencies into shared folder  | ⏳ In Progress |
| 4  | Merge Generals and Generals ZH codebase    | ✅ Done |

-----
-----
-----
-----
# Notes for me: (nothing important)

## **Symbols for Reference**  

☑ ✅ ☐ ❌ ∨ √  
✔ ✖ ⏳ ⏰ 🔄 🔥 🚀 📌 💡 ❗ ℹ 🏆 🎯 🔍 📑 📝 ✍ 💬 🛠 ⚠ 🔗

⏳ In Progress
❌ Not Started
✅ Done

-------------------

A C++ project can have various types of dependencies, 
including SDKs, open-source libraries, system libraries, and custom dependencies

project-root/
│── src/                  # Source code
│   ├── main.cpp          # Main application entry point
│   ├── game_logic.cpp    # Game logic file (example)
│   ├── renderer.cpp      # Graphics rendering (example)
│── include/              # Header files
│   ├── game_logic.h      
│   ├── renderer.h        
│── external/             # External dependencies
│   ├── SDL/              # Third-party library (example)
│   │   ├── include/      # Headers for SDL
│   │   ├── lib/          # Library binaries
│   ├── Boost/            # Another third-party library
│── libs/                 # Custom static/shared libraries
│   ├── MyLibrary/        # Custom library
│   │   ├── include/      # Headers
│   │   ├── src/          # Source files
│── build/                # Compiled binaries and object files
│── cmake/                # CMake-related configuration
│── vendor/               # Vendor-specific SDKs (optional)
│── README.md             # Documentation
│── CMakeLists.txt        # CMake build script
│── .gitignore            # Git ignore file


# What Generals should be:
project-root/
├───Code
│   ├───GameEngine
│   │   ├───Include
│   │   │   ├───Common
│   │   │   │   └───SafeDisk
│   │   │   ├───GameLogic
│   │   │   │   ├───AI
│   │   │   │   ├───Map
│   │   │   │   └───Object
│   │   │   │       ├───Behavior
│   │   │   │       ├───Collide
│   │   │   │       ├───SpecialPower
│   │   │   │       └───Update
│   │   └───Source
│   │       ├───Common
│   │       │   ├───Audio
│   │       │   ├───INI
│   │       │   ├───RTS
│   │       │   ├───System
│   │       │   └───Thing
│   │       ├───GameLogic
│   │       │   ├───AI
│   │       │   ├───Object
│   │       │   ├───ScriptEngine
│   │       │   ├───System
│   │       │   ├───Map
│   │       │   ├───Update
│   │       │   └───Upgrade
│   │       ├───GameNetwork
│   │       │   ├───GameSpy
│   │       │   └───WOLBrowser
│   ├───GameEngineDevice
│   │   ├───Include
│   │   │   ├───MilesAudioDevice
│   │   │   ├───VideoDevice
│   │   │   │   └───Bink
│   │   │   ├───W3DDevice
│   │   │   │   ├───Common
│   │   │   │   ├───GameClient
│   │   │   │   └───GameLogic
│   │   ├───Source
│   │       ├───MilesAudioDevice
│   │       ├───VideoDevice
│   │       │   └───Bink
│   │       ├───W3DDevice
│   │       │   ├───Common
│   │       │   ├───GameClient
│   │       │   ├───Shaders
│   │       │   ├───Shadow
│   │       │   └───Water
│   ├───Libraries
│   │   ├───DX90SDK
│   │   ├───Include
│   │   │   ├───Granny
│   │   │   ├───Lib
│   │   │   ├───MSS
│   │   ├───max4sdk
│   │   ├───Source
│   │   │   ├───Benchmark
│   │   │   ├───Compression
│   │   │   │   ├───EAC
│   │   │   │   ├───LZHCompress
│   │   │   │   │   ├───CompLibHeader
│   │   │   │   │   └───CompLibSource
│   │   │   │   └───ZLib
│   │   │   ├───GameSpy
│   │   │   ├───WPAudio
│   │   │   ├───WWVegas
│   │   │   │   ├───Miles6
│   │   │   │   ├───WW3D2
│   │   │   │   ├───WWAudio
│   │   │   │   ├───WWDebug
│   │   │   │   ├───WWDownload
│   │   │   │   ├───WWLib
│   │   │   │   ├───WWMath
│   │   │   │   ├───WWSaveLoad
│   │   │   │   └───Wwutil
│   │   └───STLport-4.5.3
│   ├───Tools
│   │   ├───Compress
│   │   ├───CRCDiff
│   │   ├───DebugWindow
│   │   ├───GUIEdit
│   │   │   ├───Include
│   │   │   ├───Resource
│   │   │   ├───Source
│   │   │   │   ├───Dialog Procedures
│   │   ├───ImagePacker
│   │   │   ├───Include
│   │   │   ├───Resource
│   │   │   ├───Source
│   │   │   │   ├───Window Procedures
│   │   ├───Launcher
│   │   │   ├───SafeDisk
│   │   ├───NVASM
│   │   ├───ParticleEditor
│   │   ├───PATCHGET
│   │   ├───textureCompress
│   │   ├───timingTest
│   │   ├───WorldBuilder
│   │   │   ├───include
│   │   │   ├───res
│   │   │   ├───src
│   └───Run


or

For a **C++ project** that relies on multiple SDKs (like **DirectX, 3ds Max SDK, and open-source libraries**), a well-structured `Dependencies/` folder helps maintain clarity and organization. Here’s an ideal **directory tree**:

```
ProjectRoot/
├───Dependencies/         # All external dependencies (SDKs, third-party libraries)
│   ├───DirectX9SDK/      # DirectX SDK files
│   │   ├───Include/      # DirectX headers (*.h)
│   │   ├───Lib/          # DirectX libraries (*.lib, *.a)
│   │   └───Bin/          # DirectX runtime DLLs (*.dll)
│   ├───3dsMaxSDK/        # Autodesk 3ds Max SDK
│   │   ├───Include/      # 3ds Max headers
│   │   ├───Lib/          # 3ds Max libraries
│   │   └───Bin/          # Any required executables/DLLs
│   ├───OpenSource/       # Open-source dependencies
│   │   ├───STLport/      # STLport headers & libraries
│   │   ├───ZLib/         # Compression library
│   │   ├───GameSpy/      # Multiplayer SDK
│   │   ├───Granny/       # Animation library
│   │   ├───Miles6/       # Audio SDK
│   │   ├───OtherLib/     # Any other dependencies (e.g., Boost, OpenGL, etc.)
│   ├───CMakeModules/     # CMake dependency find scripts
│   └───README.md         # Documentation explaining dependencies usage
├───Source/               # Main source code
│   ├───Engine/
│   ├───Rendering/
│   ├───Audio/
│   ├───AI/
│   ├───Networking/
│   ├───Game/
│   └───Tools/
├───Include/              # Project-specific headers
├───Lib/                  # Compiled project libraries
├───Resources/            # Game assets (textures, models, sounds)
├───Build/                # Compiled binaries
├───Scripts/              # Build scripts and automation tools
├───Tests/                # Unit and integration tests
└───CMakeLists.txt        # CMake configuration file
```

### **Advantages of This Structure**
✅ **Each SDK has its own folder** (`DirectX9SDK`, `3dsMaxSDK`, `OpenSource`), making it easy to manage.  
✅ **Includes, Libs, and Binaries are separated** within each dependency for **clarity**.  
✅ **Prepares for modular builds** using CMake (`CMakeModules/`).  
✅ **Scales well** for adding/removing third-party SDKs without clutter.

Would you like help integrating this structure into your project? 🚀


------------------------------------------------

A **full Visual Studio C++ project structure** typically includes several key directories and files to keep 
the project organized and maintainable. Here's a breakdown:

### **1. Root Directory (`ProjectName/`)**
   - Contains the main solution file (`ProjectName.sln`) and project files (`ProjectName.vcxproj`).
   - May include a `.gitignore` file for version control.

### **2. `src/` (Source Files)**
   - Holds `.cpp` files where the actual implementation resides.
   - Example: `main.cpp`, `utils.cpp`, `game_logic.cpp`.

### **3. `include/` (Header Files)**
   - Stores `.h` or `.hpp` files defining class declarations and function prototypes.
   - Example: `utils.h`, `game_logic.h`.

### **4. `lib/` (Libraries)**
   - Holds external or internal libraries used in the project.
   - Example: Precompiled `.lib`, `.a`, `.so`, or `.dll` files.

### **5. `bin/` (Executables)**
   - Contains final compiled executables for distribution or testing.

### **6. `build/` (Compiled Output)**
   - Stores compiled binaries, object files (`.obj`), and executables.
   - Keeps the workspace clean by separating source from compiled files.

### **7. `Debug/` and `Release/`**
   - Contains compiled executables and object files.
   - **Debug** builds include debugging symbols, while **Release** is optimized for performance.

### **8. `.vs/` (Visual Studio Metadata)**
   - Contains Visual Studio settings, debugging configuration, and temporary data.
   - Usually excluded from version control.

### **9. `x64/` or `x86/` (Architecture-Specific Build Directories)**
   - Stores intermediate files and binaries depending on the selected build architecture.

### **10. `tests/` (Unit Tests)**
   - Houses test cases and frameworks like Google Test or Catch2.
   - Example: `test_main.cpp`, `test_utils.cpp`.

### **11. `docs/` (Documentation)**
   - Stores project documentation, API references, or README files.

### **12. `cmake/` or `Makefile/` (Build System)**
   - Contains build scripts for CMake or Makefile to automate compilation.

### **13. `assets/` (Resources)**
   - Holds images, configuration files, or other non-code assets.


# Final Structure should look like this:

```
Dependencies/
├───SDKs/                  # Dedicated directory for proprietary SDKs
│   ├───DirectX9SDK/       # DirectX SDK files
│   │   ├───Include/
│   │   ├───Lib/
│   │   ├───Bin/
│   ├───3dsMaxSDK/         # Autodesk 3ds Max SDK files
│   ├───OtherSDK/          # Any additional proprietary SDKs
│
├───ThirdParty/            # Open-source third-party dependencies
│   ├───ZLib/
│   ├───GameSpy/
│   ├───STLport/
│   ├───OtherLib/
│
├───Include/			   # Header files (.h, .hpp)
|
├───Lib/                   # Compiled static/shared libraries
│
├───Source/                # Source code files for third-party dependencies
│
|───Docs/                  # Documentation related to dependencies
|
└── README.md              # Instructions for using shared libraries
```
