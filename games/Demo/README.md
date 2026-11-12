# Volcano Demo Game

VolcanoEngine 的示例游戏实现（静态库目标 `Demo` / `Gameplay`，输出 `libVolcanoDemo.a`）。

## 说明

- **场景实现**：实现 `Demo::Game`（派生自 `World::Scene`），构建包含物理碰撞刚体、摄像机轨迹、动态光源与 3D 模型的演示场景。
- **工厂接入**：通过包含 `<Volcano/Game.h>` 提供全局场景构造工厂函数 `volcanoCreateGame()`，供外部启动器（如 `Launcher`、`Server`）统一接入并交由 `Framework` 驱动。
- **解耦关系**：游戏实现只依赖引擎的 `World` 模块，引擎核心运行时并不反向依赖具体的演示游戏。

## 详细文档

- [Demo 场景架构与游戏逻辑说明](docs/Game.md)
