# Volcano Runtime

VolcanoEngine 的运行时核心模块集合，包含引擎运转所需的所有基础服务与管线。

## 模块列表

- [Base](Base/README.md)：基础设施、数学运算、流/文件 I/O、内存缓存与 stdexec 异步库。
- [Schema](Schema/README.md)：原生资源格式定义（`.vemodel`, `.vemesh`, `.vemat`, `.vetex`）与流序列化。
- [World](World/README.md)：基于 EnTT 的 ECS 场景与组件系统（实体、相机、灯光、刚体等）。
- [Physics](Physics/README.md)：基于 Jolt Physics 的 3D 刚体动力学与碰撞检测模拟。
- [Acoustics](Acoustics/README.md)：音频加载、播放与空间化音效系统（规划中）。
- [Graphics](Graphics/README.md)：OpenGL 3.3 2D/3D 渲染管线、着色器管理、Pass 组织与 FreeType 文本排版。
- [Gui](Gui/README.md)：2D 用户界面系统、Flexbox 自动布局与交互控件。
- [Framework](Framework/README.md)：上层生命周期框架（Base/Local/Server/Client）与资源管理器。
