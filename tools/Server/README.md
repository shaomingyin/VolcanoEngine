# Volcano Server

VolcanoEngine 的无头独立服务端程序（可执行程序目标 `Server` / `bin/VolcanoServer`）。

## 说明

- **独立仿真**：面向无显示设备环境的纯逻辑与物理仿真服务，不依赖 OpenGL、窗口系统或图形显卡。
- **命令行参数**：支持通过命令行指定运行根路径及固定模拟步数（例如 `--root . --ticks 100`）。
- **生命周期**：基于 `Framework::Server` 实例化并步进执行 `Gameplay` 游戏场景与物理世界。

## 外部依赖

- `Framework`
- `Gameplay`（游戏模块实现）
- `argh`
