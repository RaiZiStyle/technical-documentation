# Cmake CLI : 

## Generation build config & Build

> Generate the build folder
```bash
cmake -S . -B build -G "Unix Makefiles" # -S CMakeLists.txt, -B <build_folder> -G <generator> can be Ninja makefile etc..
cmake .     # Equivalent 
```

> Build the project
```bash
cmake --build <build_folder>
cmake --build . 
```

> Clear the project & config generated
```bash
rm -rf <build_folder>/
```

# Cmake Commande

`cmake_minimum_required(VERSION 3.23)` : Minimum cmake version to compile/generate

`project(MyProjectName)` : Define the name of the project

`add_executable(Tutorial)` : Name of the executable

`add_librairy(MyLibrairy)` : Like add_executable but for librairiy

`target_sources(Tutorial  PRIVATE   Tutorial/Tutorial.cxx)` : List the sources to compiles ? 
```cmake
target_sources(MathFunctions
  PRIVATE
    MathFunctions/MathFunctions.cxx

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      MathFunctions
    FILES
      MathFunctions/MathFunctions.h
)
```

`target_link_libraries(MyProgram PRIVATE MyLibrary)` : It describes relationships between targets generally.


## Cmake Variable

`CMAKE_CURRENT_SOURCE_DIR` : Cmake dir