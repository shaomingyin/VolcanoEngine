# VolcanoEngine
A simple game engine.

## Tests

GoogleTest is provided by Conan. From the repository root:

```sh
conan install . --build=missing -s build_type=Debug
cmake -S . -B build/Debug -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build/Debug --target MathCheck BaseFileSystemCheck BaseStreamCheck BaseVersionCheck BaseUtilityCheck MathGeometryCheck MathColorCheck WorldMathCheck -j2
ctest --test-dir build/Debug --output-on-failure
```

Set `-DBUILD_TESTING=OFF` to omit the test targets.

## Graphics

See [Graphics](documents/Graphics.md) for the unified 2D/3D renderer.
Graphics uses the caller's current OpenGL context and does not depend on a window library.
Launcher uses Conan GLFW to create its window and OpenGL context. Its Window implements Graphics::Target; Graphics does not link GLFW.
LauncherWindowCheck requires a display and OpenGL 3.3; use `ctest --test-dir build/Debug -LE display` to skip display tests.
