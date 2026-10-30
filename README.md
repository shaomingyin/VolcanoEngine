# VolcanoEngine
A simple game engine.

## Tests

GoogleTest is provided by Conan. From the repository root:

```sh
conan install . --build=missing -s build_type=Debug
cmake -S . -B build/Debug -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build/Debug -j2
ctest --test-dir build/Debug --output-on-failure
```

Set `-DBUILD_TESTING=OFF` to omit the test targets.

## Streams

Base provides [Buffer](runtime/Base/docs/Buffer.md), a seekable Stream backed by an owned ByteArray.
It supports read, write, append and explicit truncation modes without a Qt dependency.

## Math

Base publicly depends on Conan Eigen. Use Eigen types directly; see [Math](runtime/Base/docs/Math.md) for migration and the retained extensions.
Math extensions use the `Volcano::Eigen` namespace; third-party types use `Eigen::`.
Color, Size and Rect use the `Volcano` namespace and have separate headers: `Volcano/Color.h`, `Volcano/Size.h` and `Volcano/Rect.h`.
`Volcano/Transform.h` provides `Volcano::Transform`, which stores translation, scale and quaternion rotation and generates an `Eigen::Affine3f` on request. World components and Graphics drawing items use this type.

## Graphics

See [Graphics](runtime/Graphics/docs/Graphics.md) for the unified 2D/3D renderer.
Graphics uses the caller's current OpenGL context and does not depend on a window library.
View owns drawing items, lights and rendering data. Build it with `view.begin()`, `add(...)` and `view.end()`,
then pass the output separately with `renderer.render(view, target)`. A completed View can be rendered repeatedly.
Renderer uses deferred Lambert lighting for opaque items and forward lighting for sorted transparent items,
with ambient, directional, point and spot lights. Texture manages sampled images; FrameBuffer provides offscreen output.
See [Graphics design](runtime/Graphics/docs/Design.md) for object relationships and shader ownership.
`renderer.setMaxLights(24)` changes light capacity (default 16); `renderer.maxLights()` returns it.
Passes validate their linked shader interfaces once and cache uniform locations for rendering.
Framework::Local uses Conan GLFW to own its window and OpenGL context. Framework::Window implements Graphics::Target; Graphics does not link GLFW.
Construct Local/Client with window dimensions and a title; use `local.window()` to access the window. Launcher supplies the event loop and buffer swaps.
FrameworkWindowCheck requires a display and OpenGL 3.3; use `ctest --test-dir build/Debug -LE display` to skip display tests.

## World

See [World design](runtime/World/docs/Design.md) for component ownership, encapsulated value semantics,
Camera/Light inheritance, validation and update notification rules.
World also defines AudioSource/AudioListener descriptions; audio loading and playback are not implemented yet.

## Physics

[Physics](runtime/Physics/docs/Physics.md) converts [World::RigidBody](runtime/World/docs/RigidBody.md) components into an EnTT registry-bound Jolt simulation.
Framework::Base owns the scene and its Physics::System, runs physics after game logic while Playing,
and releases the simulation before the scene. Use `framework.physics()` to configure gravity.
World does not include or link Jolt.

## Framework and resource schema

See [Framework](runtime/Framework/docs/Framework.md) for Base/Local/Client/Server ownership and lifecycle,
and [Schema](runtime/Schema/docs/Schema.md) for model, mesh, material and texture resource formats.
Schema implements validation and Stream serialization. Framework implements scene lifecycle, resource loading and local rendering.
A [game](games/Demo/docs/Game.md) implements a World::Scene subclass. Framework::Base owns and drives that instance;
the host supplies the selected game factory through that game's own `Volcano/Game.h`. Games depend on World, and Framework does not depend on a concrete game.
Launcher uses Framework::Local/Client directly; Server uses Framework::Server without graphics dependencies.
Run `VolcanoServer --root . --ticks 2` for a bounded headless startup check. FrameworkLocalCheck requires a display and uses the `display` CTest label.

## Files

File access uses Conan PhysFS directly. Base provides [PhysFile](runtime/Base/docs/PhysFile.md)
for automatic handle closure, path-based opening, byte reads/writes, seeking and file queries.
PhysFile and Buffer implement Stream; Schema reads and writes through Stream references for both file and memory data.
The host owns PhysFS initialization, mounts and shutdown.

Schema validation and serialization tests:

```sh
cmake --build build/Debug --target SchemaCheck -j2
ctest --test-dir build/Debug --output-on-failure -R '^Schema'
```

## Editor

The [Qt Creator editor plugin](tools/Editor/docs/Editor.md) was imported from main (`86663c4`).
Enable it with `-DVOLCANO_EDITOR=ON` and provide the matching Qt Creator SDK and Qt 6 CMake packages.
It is disabled by default; the imported editor remains a prototype, and scene document loading/saving is not implemented.

## Documentation

Module documentation lives in each module's `docs/` directory.
`documents/` is reserved for the Doxygen API documentation build configuration.
