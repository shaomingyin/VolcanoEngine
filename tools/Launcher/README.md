# Volcano Launcher

VolcanoEngine 的图形客户端启动器（可执行程序目标 `Launcher` / `bin/VolcanoLauncher`）。

## 说明

- **程序入口**：负责解析命令行参数（基于 `argh`），挂载 PhysFS 文件系统根路径。
- **窗口与驱动**：基于 `Framework::Local` 创建渲染窗口与 OpenGL 上下文，加载并驱动 `Gameplay`（如 `Demo`）提供的游戏场景。
- **主循环**：维持客户端主事件循环，处理窗口输入轮询、场景逻辑刷新与垂直同步双缓冲交换。

## 外部依赖

- `FrameworkLocal`
- `Gameplay`（游戏模块实现）
- `argh`
