# Graphics

## 模块与对象

Graphics 提供统一二维/三维渲染，公开依赖 Base、World 和 Eigen，使用内置 gl3w，不依赖 Asset、Gui、GLFW 或窗口系统。
Framework::Local 使用 GLFW 创建窗口及原生 OpenGL context；Framework::Window 实现 Graphics::Target。
Launcher 处理事件及交换缓冲。

完整的对象关系、成员结构、shader 接口归属及架构调整见 [Graphics 设计](Design.md)。

| 对象 | 用途 |
| --- | --- |
| Renderer | 加载函数、管理 GBuffer 和具体 Pass、清屏及调度 |
| View | 通过 `begin/add/end` 直接提交或通过 `extract` 从 `World::Scene` 提取渲染输入并计算矩阵、光源和透明排序 |
| Model | 保存按 Part 预解析的非拥有 `Mesh*`、默认 `Material` 与局部变换的运行时组件 |
| Mesh | 定义交错 position/normal/texcoord 顶点格式，管理 VAO/VBO/索引缓冲 |
| Material | 乘法色、可选借用 Texture 指针、lit 开关 |
| Texture | 仅管理可采样 RGBA8 纹理、上传和尺寸 |
| FrameBuffer | 实现离屏 Target，持有颜色 Texture 和深度/模板附件 |
| Target | 输出绑定、像素尺寸和纹理反馈查询；由 Window、FrameBuffer 实现 |
| GeometryPass / LightingPass / TransparentPass | 几何写入、全屏延迟光照、透明前向光照 |

## 基本使用

调用方先创建并激活 OpenGL 3.3 Core 或更新的 context，提供函数地址解析器：

```cpp
#include <Volcano/Graphics/FrameBuffer.h>
#include <Volcano/Graphics/Renderer.h>

namespace Graphics = Volcano::Graphics;

// 宿主须先激活 OpenGL context，并初始化和绑定 GL3W。
Graphics::Renderer renderer;
Graphics::FrameBuffer output(256, 256);
Graphics::View view;
view.begin();
view.setClearColor(Volcano::Colorf::Red);
view.end();
renderer.render(view, output);

// 离屏结果可用于另一次绘制的材质。
Graphics::Material material;
material.texture = &output.texture();
```

Framework::Window 创建窗口后使用 glfwGetProcAddress 初始化 GL3W；其它宿主自行完成初始化。
View 不绑定 Target，同一 View 可输出到窗口或多个 FrameBuffer。不能在输出到某 FrameBuffer 时采样其颜色 Texture，这是调用前置条件，Renderer 在 Debug 下通过断言检查这种反馈，Release 不执行检查。

纯采样资源仍直接使用 `Texture(width, height, rgba)`，无需 framebuffer 或深度附件。RGBA 字节须紧密排列、底行在前，大小为 width × height × 4，上传时从 straight alpha 转为预乘 alpha；不自动执行 sRGB 转换。

Texture 和 FrameBuffer 的同尺寸 resize 保留内容，不重新分配；非法尺寸及 FrameBuffer 不完整时保留原资源。改变尺寸后内容未定义。FrameBuffer::texture() 返回只读 Texture 引用，resize 保持该对象地址，底层 GL 标识可能变化。

## 绘制输入与光照

```cpp
Graphics::View view;
view.begin();
view.setProjection(projection);
view.setViewMatrix(viewMatrix);
view.setAmbientLight(Eigen::Vector3f::Constant(0.1f));

Graphics::Material material;
material.lit = true;
Volcano::Transform transform;
transform.setTranslation({1, 2, 3});
view.add(mesh, material, transform, false);

Graphics::Light sun;
sun.direction = {0.0f, -1.0f, -1.0f};
sun.strength = 0.8f;
view.add(sun);
view.end();
renderer.render(view, output);
```

View 的 items()/lights() 只返回 const vector&。输入通过 add 按值提交，Mesh 和 Texture 资源仍借用；资源必须在绘制期间有效。复制 View 独立复制 CPU 数据。相机矩阵默认为单位矩阵，环境光默认为零，清屏色默认为黑。

Material::lit 默认 false；启用时计算世界空间 Lambert 漫反射，支持环境光、方向光、点光及聚光。光照只影响 RGB，不影响 alpha。方向沿光传播方向，上传前归一化；局部光源使用位置及正数 range，聚光半角以度表示，要求 0 ≤ inner < outer ≤ 90。

模型变换保存为 Eigen::Affine3f，add 也接受 Volcano::Transform 并转换，投影/视图使用 Eigen::Matrix4f；变换顺序为 projection × view × model。受光项需要可逆模型变换，法线使用逆转置处理非均匀缩放。

`renderer.setMaxLights(24)` 修改容量，默认 16，maxLights() 查询容量。容量须为可表示为 GLSL int 的正数；超限输入抛异常。修改容量会重新构建两个受光程序，编译失败保留原容量和程序，设置相同值直接返回。

## 渲染阶段和 shader 扩展

调用方通过 begin() 开始新一轮构建，调用 add() 提交输入，最后 end() 统一校验并计算模型/法线矩阵、光源参数和透明排序。Renderer 直接读取结果；ready() 和输出反馈通过 Debug 断言检查，自己的光源容量仍执行运行时校验，再调整 GBuffer 并清屏。

begin 清空绘制项、灯光和计算缓存，保留相机、环境光及清屏色。add/end 只允许在构建期间调用；默认 View 和构建中的 View 不能渲染。end 失败时不能使用部分结果，可 begin 后重新提交；构建中再次 begin 也会重置未完成输入。

相机和环境设置使用 setProjection/setViewMatrix/setAmbientLight/setClearColor，不返回可变引用。setter 可以在 begin 前或构建期间调用；end 后修改会取消可渲染状态，须重新 begin/end。非 const 接口供调用方构建，const 接口供 Renderer/Pass 读取，没有独立的 prepare 接口。

完成后的 View 可以跨帧或输出到多个目标，无需重复构建。内部复用缓存容量，不将材质、相机或 Mesh 引用复制到第二份输入，计算结果通过索引关联原绘制项。

- GeometryPass 按纹理、lit、Mesh 的稳定分组顺序，将不透明项写入 GBuffer：RGBA32F 颜色、法线/受光标记、世界位置，另有 DEPTH_COMPONENT24 深度纹理。
- LightingPass 采样 GBuffer，计算不透明光照并输出深度，背景保留清屏色。
- TransparentPass 按模型原点相机空间 Z 排序，使用前向光照、预乘 alpha 混合和深度测试，不写深度。

不透明排序在 end() 中完成，只调整绘制索引，items() 保持原提交顺序；相同分组键保留相对顺序，颜色不参与排序。不同状态的共面物体不保证原先的覆盖顺序。当前尚未跳过重复材质上传、纹理或 Mesh 绑定，排序本身不减少 GL 调用次数。

GBuffer 格式保持不变，没有 MSAA。透明标记由 DrawItem::transparent 显式给出，不按 alpha 推断；原点排序不能解决网格穿插和内部透明三角形排序。目前不支持阴影、PBR、镜面反射、法线贴图、HDR 色调映射或自动场景遍历。

各阶段显式接收准备好的 const View&，LightingPass 另接收 const GBuffer&；不再有 Graphics::Context。独立调用时，调用方负责绑定输出、viewport 和清屏。LightingPass 要求 GBuffer 已分配、输出与输入不同、viewport 从原点开始且尺寸匹配。

Pass 基类仅持有 gl::Program，提供 protected attachShader(stage, version, source, definitions)。具体 Pass 选择版本和阶段、调用 link 并校验接口。基类没有虚函数，不通过基类指针执行或销毁具体阶段。

Mesh::shaderDefinitions() 提供顶点属性宏，GBuffer::shaderDefinitions() 提供附件宏，Light::shaderSource(maxLights) 提供光照扩展；具体 Pass 显式选择组合，颜色输出由对应 Pass 定义。自定义 shader 不再自动接收这些宏。

attachShader 不限制 vertex/fragment，可添加 geometry、tessellation 或 compute，前提是当前 context 支持对应版本及阶段组合。内置 Pass 继续使用 GLSL 330 core 的 vertex + fragment。

内置程序链接后一次性校验并缓存参数位置，每阶段每次 render 绑定程序一次。gl::Program 封装 fragmentLocation，以及 uniform、uniform2f/3f/4f、uniformMatrix3fv/4fv 的名字和 GLint 位置重载。按位置上传不查询名字、不自动绑定程序；sampler 单元使用 GLint。

## 生命周期及错误处理

Graphics 不切换原生 OpenGL context。构造、使用和析构 GPU 对象前须激活正确 context，并有对应有效函数表。Renderer 和其它 GPU 资源都须先于所属 context 和函数表销毁。

Graphics 不初始化、持有或切换 GL3W 函数表；Renderer 使用宿主当前已激活的 context 和函数表，创建和析构也遵循相同前提。gl3w 仍使用全局分派指针，不支持并发渲染。

Target 实现提供颜色输出及深度存储。自定义纹理 Target 必须覆盖 contains(const Texture&)，报告使用的纹理，以防读写反馈；默认实现返回 false。Renderer 不检测底层 GL 资源别名或跨 context 误用。

View 提交时序、已准备状态、Pass 输入分配状态和输出 viewport 匹配属于调用前置条件，使用 VOLCANO_ASSERT；Release 不执行这些检查。
调用方 end 失败或 GBuffer 分配失败均发生在清屏前；绘制开始后不回滚已写像素。异常继续向调用方传播，Renderer 恢复本次输出及约定 viewport、深度写入和混合状态，不恢复调用方任意外部 GL 状态。目标尺寸非正时，完成输入校验后跳过绘制。

## 验证

```sh
cmake --build build/eigen --target GraphicsLoaderCheck GraphicsLightingCheck FrameworkWindowCheck Launcher Editor -j2
ctest --test-dir build/eigen -R 'GraphicsLoader|GraphicsLighting|GraphicsResources' --output-on-failure
```

GraphicsLoaderCheck 包含无显示的函数加载及资源测试，GraphicsLightingCheck 覆盖输入所有权、容量和非法参数。FrameworkWindowCheck 的 display 测试使用真实 GLFW/OpenGL，覆盖像素、阶段接口、资源失败回滚、纹理反馈和状态恢复。无显示环境可运行 `ctest --test-dir build/eigen -LE display --output-on-failure`。

Schema 的预乘 RGBA8 可使用 `Texture::uploadPremultiplied` 直接上传；`upload` 仍接受直通 alpha 并转换。
