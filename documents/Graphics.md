# Graphics

## 模块边界

Graphics 依赖 Base、Math 和内置 gl3w，提供 Target、Texture、Mesh、Material 和 Renderer。
它不依赖 GLFW、X11、GLX 或 World，不创建窗口、不处理系统事件，也不管理 OpenGL Context。

Launcher 通过 Conan 的 `glfw/3.4` 创建窗口、处理关闭事件、管理 Context 和交换缓冲区。
只有 Launcher 及其集成测试链接 GLFW，Graphics 保持独立。

## Target

Texture 是 Target，具有可采样的 RGBA8 二维纹理、离屏帧缓冲和深度/模板缓冲。
Target 提供尺寸和 framebuffer 标识，由 Renderer 绑定；它不负责 Context 生命周期。
`Launcher::Window` 派生自 Target，返回 GLFW 查询的实际 framebuffer 像素尺寸，以及默认帧缓冲标识 0。
Texture 同时可以作为输入和输出，但不允许在绘制到它时采样它本身。

## 基本使用

以下代码要求集成方已创建并激活 OpenGL 3.3 Core 或更新的 Context，且提供函数地址解析器：

```cpp
#include <Volcano/Graphics/Texture.h>
#include <Volcano/Graphics/Renderer.h>

using namespace Volcano;

Graphics::Renderer renderer(resolveOpenGLFunction);
Graphics::Texture picture(256, 256);
renderer.begin(picture);
renderer.clear(Math::Colorf::Red);
// 在对应 Context 仍有效且处于激活状态时析构 GPU 对象。
```

`resolveOpenGLFunction` 的签名为 `GL3WglProc(const char*)`。Launcher 在
`glfwMakeContextCurrent` 之后构造 `Graphics::Renderer renderer(glfwGetProcAddress)`。
应用目标自行查找并链接 Conan 提供的 GLFW；Graphics 不需要修改。

Launcher 的 run 负责 GLFW 初始化和清理；先创建 Window，再构造 Renderer，由其加载 gl3w、检查版本并创建 GPU 资源。
退出或异常时，先销毁 Renderer，再销毁 Window，最后终止 GLFW。
主循环处理关闭请求、更新场景、通过 Renderer 清屏并交换缓冲区；framebuffer 尺寸为零时等待事件。
默认开启垂直同步，软件帧率上限为 60；场景遍历和实际游戏内容绘制尚未接入。

## 多阶段渲染框架

Renderer 内部组合 GeometryPass、LightingPass、TransparentPass，不引入单独的 Pipeline 类或通用 Pass 基类。
各 Pass 将负责自己的 Shader、绘制状态和阶段资源；预期执行顺序为几何、光照、透明物体。

- `DrawItem` 描述 Mesh、Material、模型变换和透明分类。纹理通过 Material 指定。
- `RenderContext` 描述输出 Target、绘制项列表、投影矩阵和视图矩阵。
- `Renderer::render(context)` 是多阶段渲染入口。
- 三个 Pass 各自提供 `render(context)`，由 Renderer 组织使用。

**新增 render 方法目前全部为空函数，不绘制、不清屏、不执行 Pass。**
暂未设计 GBuffer、光源输入和阶段间资源传递；实现延迟渲染时再补充这些接口。
现有 begin/clear/draw 仍执行立即绘制，Launcher 继续使用这些接口。

```cpp
Graphics::DrawItem items[] = {
    {mesh, material, transform, false}
};
Graphics::RenderContext context{target, items, projection, view};
renderer.render(context); // 框架占位，目前无渲染效果。
```

RenderContext 不持有资源：Target、绘制项数组、Mesh 和 Material 引用的 Texture，均须在 render 调用期间保持有效。
后续如改为跨调用排队执行，必须同时调整这一生命周期约定。

## 渲染接口

- `begin(target, projection, view)` 选择输出与矩阵；默认投影和视图为单位矩阵，默认开启深度测试，不自动清屏。
- `clear(color)` 清除颜色和深度。
- `draw(mesh, material, transform)` 绘制索引三角形网格。
- `draw(texture, transform, color)` 绘制局部范围为 `[0, 1] × [0, 1]` 的四边形；原点和 UV 原点在左下角，显示尺寸由变换决定。
- `setDepthTest(false)` 用于 HUD 等不参与场景深度遮挡的内容，需要在 begin 之后设置。

二维和三维内容使用同一 Renderer。模型变换支持三维平移、缩放和旋转；矩阵按列存储，变换顺序为 `projection * view * model`。相机及场景遍历由调用者负责。绘制时根据 Target 尺寸设置 viewport；窗口尺寸、framebuffer 像素尺寸和投影宽高比由集成方维护。

Material 包含乘法色和可选的纹理指针。目前着色不受光照影响；Mesh 中的法线保留为网格数据。绘制按提交顺序执行，不自动排序；半透明对象通常需要从远到近绘制。

颜色参数及上传 RGBA 字节使用 straight alpha，内部统一以预乘 alpha 存储与混合，支持离屏结果再次合成。模块不自动执行 sRGB 转换。

Texture 上传数据每行紧密排列，第一行为底行，必须提供 `width * height * 4` 个字节。新纹理与改变尺寸后的纹理内容未定义，使用前必须清屏或上传。同尺寸 resize 保留内容，非法尺寸不会修改已有纹理。

## Context 使用约定

Graphics 不保存窗口引用，也不会自动激活 Context。Renderer、Mesh、Texture 的构造、使用和析构，
都要求调用者保证对应的 Context 已激活。GPU 资源必须在其 Context 销毁前析构。
跨 Context 误用 GPU 对象不由 Graphics 检测，资源共享由集成方决定。

`Renderer(resolver)` 加载并检查当前 Context，将 GL3WProcs 作为 Renderer 的成员持有，不再提供独立初始化接口。
加载、版本检查或内部 GPU 资源创建失败会抛出异常，并保留此前的分派指针。
Renderer 的绘制操作会选择自己的函数表，但不会切换 Context；切换 Context 时，由调用者选择对应的 Renderer。
创建、使用和销毁 Mesh、Texture 前，必须已有对应 Context 的有效 Renderer 和当前分派表；必要时先调用该 Renderer 的 begin。
应用持有的 GPU 资源须在 Renderer 之前析构，Renderer 须在 Context 之前析构。
Renderer 析构会先用自己的函数表释放内部 GPU 资源；如果当前分派指针指向自己，随后将其清空。
底层 gl3w 仍使用全局分派指针，当前不支持并发渲染。
Renderer 会设置所需的 GL 状态，不保存或恢复外部 GL 状态。

最后一次 begin 指定的 Target 必须在后续绘制期间存活，Material 的非拥有纹理指针必须在对应 draw 调用期间有效。

## 验证

```sh
cmake --build build/Debug --target GraphicsLoaderCheck LauncherWindowCheck Launcher Editor -j2
ctest --test-dir build/Debug --output-on-failure
```

GraphicsLoaderCheck 不依赖窗口或显示服务，验证 Renderer 构造时的函数加载失败和版本检查。
LauncherWindowCheck 使用实际 GLFW Context，验证 Target 尺寸、默认 framebuffer、纹理绘制像素、交换缓冲和关闭标记，以及 Renderer 函数表的独立所有权、构造失败恢复和析构清理。
测试带有 display 标签，需要环境中可用的显示服务及 OpenGL 3.3；不显式指定 DISPLAY。
无显示服务时运行 `ctest --test-dir build/Debug -LE display`。

旧 Frame/DeferredRenderer 接口已由上述接口替代；新增 Pass 框架仅保留空实现。目前不包含光照、场景遍历、字体或路径绘制。
