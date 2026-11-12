# Resources

`Framework::Resources` 提供明确的单项资源加载和缓存接口，不使用 RTTI、类型注册或公开模板接口。

```cpp
#include <Volcano/Framework/Resources.h>

using namespace Volcano;

auto mesh = Framework::Resources::loadMesh("/models/mesh.vemesh");
auto material = Framework::Resources::loadMaterial("/models/material.vematerial");
auto texture = Framework::Resources::loadTexture("/textures/albedo.vetexture");
auto same = Framework::Resources::loadMesh("/models/mesh.vemesh");
Framework::Resources::clear();
```

每种资源只提供 `loadXxx(path)`。调用时先查缓存，存在仍存活的对象就直接返回，否则读取文件并创建资源；路径、文件读取或资源内容有问题就抛出异常，成功返回非空 shared_ptr。资源随最后一个 shared_ptr 释放而自动销毁，不提供单项 unload 接口。`clear()` 删除所有索引，已有句柄继续有效。

| 加载接口 | 返回对象 | 内容 |
| --- | --- | --- |
| `loadModel` | `Asset::Model` | 模型描述（Protobuf），解析 Mesh、Material 相对路径 |
| `loadMaterial` | `Asset::Material` | 材质描述（Protobuf），解析纹理相对路径 |
| `loadMesh` | `Graphics::Mesh` | 通过 `Asset::Mesh` 零拷贝流式读取顶点/索引并直接上传 GPU 可绘制对象 |
| `loadTexture` | `Graphics::Texture` | 通过 `Asset::Texture` 零拷贝流式读取像素并直接上传 GPU 纹理对象 |
| `loadFont` | `Graphics::Font` | 读取字体字节并创建字体对象 |

资源统一以 PhysFS 虚拟根目录中的绝对路径标识，例如 `/textures/albedo.vetexture`，与宿主文件系统路径无关。PhysFS 没有工作目录，不带开头 `/` 的路径也从虚拟根定位；所有输入在缓存中规范化为带 `/` 的绝对虚拟路径。重复分隔符和 `.` 会被规范化，禁止 `..`、NUL、反斜杠、冒号以及未指向文件的路径。所有加载接口使用相同的路径规则。模型和材质文件中的相对引用先按其文件目录解析，再交给 PhysFS 定位；描述对象中的引用使用从虚拟根定位的文件名。

Resources 不提供自定义标识或 factory 重载。动态生成的纹理由 Local 直接持有，不放入文件资源缓存。文件加载失败报告虚拟路径，失败结果不进入缓存。模型和材质只加载自身描述，其引用的其它资源由调用方按需加载。

文件读取、解析、对象创建和各类型弱引用缓存统一放在 Resources.cpp，接口声明在 include/Volcano/Framework/Resources.h。Resources 不依赖 World，不接收 Scene，不保存场景强引用或暂存状态，不提供 prepare、commit、discard、unload 等流程接口。Base 持有模型描述句柄，Local 持有 Mesh、Material、Texture、Font 句柄；加载成功后发布本轮句柄，失败或取消时丢弃暂存句柄，卸载时释放所属句柄。各类型分别保存缓存，不使用 void 指针或运行时类型标识。Mesh 和 Texture 的缓存按当前 GL3W 函数表分区，各分区内仍只以虚拟绝对路径索引；调用前须激活所属 GL context 和 GL3W，Resources 不负责初始化。其它类型按路径共享。全部加载同步发生在调用线程；Framework 负责线程安排、场景事务、进度和取消。

缓存保存 weak_ptr，由调用方持有的 shared_ptr 共同拥有资源。最后一个句柄释放时销毁对象，缓存不延长寿命。GPU 对象的最后一个句柄须在有效的所属 GL context 中释放。

缓存索引可并发访问，文件加载时不持有缓存锁。同一路径并发未命中可能重复构建，最终复用先发布且仍存活的对象。资源对象本身的并发访问由使用方保证。clear 不等待或取消正在执行的加载。

动态纹理的离屏目标、Canvas、内容版本、重绘标记、绘图回调及依赖排序由 Local 持有和管理；Resources 不提供 Output 或动态纹理生成接口。Local 在提交、取消及卸载时分别提交、丢弃和释放这些状态，保证 GL 对象在所属窗口销毁前释放。

缓存查找、加载结果发布及并发保护使用 Base 的 `Cache<T, std::filesystem::path>`，各类型明确配置独立的 `WeakCachePolicy`。默认策略与自定义策略约定见 [Base Cache](../../Base/docs/Cache.md)。Resources 只保留文件加载和 GL context 分区，不重复实现缓存算法。
