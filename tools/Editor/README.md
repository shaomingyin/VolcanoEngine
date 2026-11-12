# Volcano Editor

VolcanoEngine 的场景与项目编辑工具（基于 **Qt Creator 插件** 架构构建）。

## 说明

- **插件集成**：以 Qt Creator 插件形式扩展 IDE 功能，提供工程向导模板、场景树（SceneTree）、属性检查面板（PropertyView）及基于 OpenGL 的场景视口预览（SceneEditorViewport）。
- **构建说明**：默认不开启构建。需在 CMake 配置时传入 `-DVOLCANO_EDITOR=ON` 并提供匹配的 Qt Creator SDK 与 Qt 6 开发包。

## 外部依赖

- `QtCreator` Core
- `Qt6` (OpenGLWidgets)

## 详细文档

- [Editor 插件导入说明与设计状态](docs/Editor.md)
