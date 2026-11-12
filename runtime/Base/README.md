# Volcano Base

VolcanoEngine 的基础支持库（静态库目标 `Base` / `libVolcanoBase.a`），提供引擎通用的基础设施、基础数据结构、数学运算、文件 I/O、流抽象与内存缓存。

## 主要特性

- **流与 I/O 抽象**：提供统一的抽象流接口 `Stream`，以及基于 PhysFS 的虚拟文件流 [`PhysFile`](docs/PhysFile.md) 和内存缓冲流 [`Buffer`](docs/Buffer.md)。
- **数学运算**：基于 Eigen，提供基础向量、矩阵与变换 [`Transform`](docs/Math.md)，以及专门的 [`Color`](docs/Color.md)、`Size`、`Rect` 类型。
- **缓存管理**：支持引用计数、LRU、FIFO 等不同淘汰策略的通用资源内存缓存 [`Cache`](docs/Cache.md)。
- **异步与并发**：内置集成三方异步执行框架 `stdexec` 与 `Async++`，提供线程池调度与异步管道能力。
- **工具与辅助**：提供 RAII 范围守卫 `ScopeGuard`、统一错误报告机制 `Error` 及引擎版本号信息 `Version`。

## 外部依赖

- `Eigen3`
- `PhysFS`
- `spdlog`
- `Async++`
- `Threads`
- 内置集成三方源码：`stdexec`（位于 `include/` 及 `src/stdexec/`）

## 详细文档

- [Buffer 内存流设计](docs/Buffer.md)
- [Cache 缓存策略设计](docs/Cache.md)
- [Color 颜色定义](docs/Color.md)
- [Math 数学库与变换说明](docs/Math.md)
- [PhysFile 虚拟文件流说明](docs/PhysFile.md)
