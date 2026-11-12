# Volcano Framework

VolcanoEngine 的上层运行时应用框架，统领游戏生命周期、资源加载与缓存、场景装配及物理/图形/界面的调度集成。

## 构建目标

| 目标名称 | 产物 | 依赖项 | 说明 |
|---|---|---|---|
| **Framework** | `libVolcanoFramework.a` | `World`, `Schema`, `Physics`, `Graphics` | 无窗口图形依赖的核心框架，包含生命周期基类 `Framework::Base`、服务端实例 `Framework::Server` 与资源加载器 `Framework::Resources` |
| **FrameworkLocal** | `libVolcanoFrameworkLocal.a` | `Framework`, `Graphics`, `Gui`, `glfw` | 本地客户端与单机运行框架，包含 GLFW 窗口管理 `Window`、交互实例 `Framework::Local` 与客户端 `Framework::Client` |

## 主要特性

- **应用生命周期抽象**：
  - `Framework::Base`：拥有并调度 `World::Scene` 与 `Physics::System`，支持非阻塞异步加载场景、进度通知与协作式取消。
  - `Framework::Local`：拥有独立的 GLFW 渲染窗口与 OpenGL 上下文，集成 `World::DrawList2D` 与 `Graphics::Renderer` 驱动场景动态贴图与画面协同渲染。
  - `Framework::Server`：面向无窗口、高吞吐运行的服务端仿真抽象。
- **集中化资源管理（Resources）**：
  - 基于路径的统一部署与解析，透明调度 `Base::Cache` 缓存已载入的 Model、Mesh、Material 与 Texture。
  - 自动管理 GPU 显存资源的上传与安全回收。

## 外部依赖

- 内部模块：`Base`, `Schema`, `World`, `Physics`, `Graphics`, `Gui`
- 第三方库：`glfw3`

## 详细文档

- [Framework 实例所有权与生命周期说明](docs/Framework.md)
- [Resources 资源加载器与缓存设计](docs/Resources.md)
