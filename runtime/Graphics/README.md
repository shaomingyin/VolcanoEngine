# Volcano Graphics

VolcanoEngine 的图形渲染核心库（静态库目标 `Graphics` / `libVolcanoGraphics.a`），提供统一的 2D / 3D 渲染管线、着色器管理、矢量绘制及字体排版支持。

## 主要特性

- **现代 OpenGL 渲染后端**：基于 OpenGL 3.3 Core Profile（通过内置 `gl3w` 加载），不绑定特定窗口系统，面向当前活跃上下文执行。
- **3D 混合渲染管线**：
  - **延迟渲染（Deferred Shading）**：通过 `GBuffer` 与 `GeometryPass` 记录位置、法线、反照率及高光属性，在 `LightingPass` 中统一计算方向光、点光源与聚光灯光照。
  - **正向透明渲染（Forward Transparent）**：通过 `TransparentPass` 对有序透明对象进行深度排序与光照混合。
- **2D 矢量与排版系统（VG）**：
  - [`Canvas`](docs/VG.md)：命令式 2D 绘制接口，支持矩形、圆角矩形、边框、投影、渐变及动态纹理绘制。
  - [`Font`](docs/VG.md)：基于 FreeType 2 栅格化与 HarfBuzz 塑形引擎的高质量文本排版与字形图集管理。
  - `PaintPass`：执行 2D 命令队列的批处理着色与混合。
- **渲染目标抽象**：抽象基类 `Target`，支持离线帧缓冲渲染（`FrameBuffer`）与屏幕直接渲染。

## 外部依赖

- `Base`
- `freetype`
- `harfbuzz`
- `opengl` (系统依赖)

## 详细文档

- [Graphics 渲染器架构设计与使用指南](docs/Graphics.md)
- [渲染管线与 Pass 设计规范](docs/Design.md)
- [VG 2D 画布与字体排版设计](docs/VG.md)
