# VG 与世界动态纹理设计

World 描述资源来源与矢量绘制指令集 `World::Canvas`（不继承 `World::Texture`），`Framework` 管理加载、生命周期、Scene 到 View 转换和依赖顺序，`Graphics` 执行渲染；`Gui` 定义控件树、事件与绘图接口，`Gui::Document` 直接派生自 `World::Canvas`，不依赖 `Graphics`。

纹理来源包括文件、GUI、自定义绘图提供者和相机 RTT 输出。Scene 持有共享描述，Model 按 Part 下标绑定动态实体覆盖，带有 `World::Overlay` 的实体作为屏幕叠加层绘制。Framework 将逻辑引用解析为运行时资源。拒绝循环依赖和输出反馈。

World 的 Canvas 拥有路径、文本和绘图状态，引用字体及图片；Graphics::View 记录输出任务和覆盖绘制。Renderer 先执行纹理生成，再执行现有 3D Pass，最后绘制覆盖内容。输出仍为普通 FrameBuffer/Texture。Canvas 是命令记录对象，不增加 Screen 或画布派生类，也不引入 NanoVG。

路径支持曲线细分、填充、描边、渐变、图片和矩形裁剪。填充使用 stencil-and-cover，支持非零及奇偶规则。绘图采用左上原点，预乘 Alpha；纹理存储保持现有底行优先约定。

Graphics 管理字体运行时对象、FreeType/HarfBuzz、文字测量、字形栅格化和有界分页图集。Framework 通过 PhysFS 加载字体字节、共享字体资源并决定卸载。Gui 只保存字体引用和样式，通过接口测量和绘制，不维护字体缓存。字形按需生成，支持回退、预热和页级淘汰。命令不保存图集 UV。

首版 Gui 提供容器、文本、图片、按钮、横向/纵向布局，以及指针、焦点和捕获。Framework 将输入转换到画布坐标；模型拾取由应用提供 UV。文件加载后复用，GUI 按变更更新，自定义及相机输出每次相关提交更新。

实施顺序：纹理与绘图资源、VG、字体、World/Framework、Gui/Demo。验证命令与资源生命周期、图形像素、中文缓存、依赖和加载回滚、事件及原有 3D 回归。不包含 SVG 解析、任意路径裁剪、IME、完整双向排版、多后端和新持久化格式。

## 使用

```cpp
Graphics::FrameBuffer output(512,256);
World::Canvas canvas;
canvas.begin();
canvas.setColor(Colorf::Green);
canvas.beginPath();
canvas.roundedRect(10,10,200,80,8);
canvas.fill();
canvas.end();
view.begin();
view.add(output,canvas);
// 提交采样 output.texture() 的模型，或通过覆盖 Canvas 绘制该纹理。
view.end();
renderer.render(view,target);
```

Canvas 必须 begin/end 后才能提交；记录时保留绘图顺序，执行时细分路径。路径采用 fill/stroke 时的变换。image 使用当前颜色/渐变进行调制，原样显示前应 setColor(White)。矩形裁剪在设定时变换为输出空间轴对齐包围盒。

Font 的 fallbacks 为借用引用，须比使用它的主字体活得更久；回退列表是按顺序直接查找，不递归。字号为逻辑像素，measure 返回逻辑行框与最大行前进宽度，不是墨迹边界。通过 Renderer::setFontAtlasBudget 设置图集预算，通过 prewarm 预热，通过 fontCacheStats 查询驻留字形、命中、栅格化及上传量。图集页为 1024×1024 单通道，默认预算 4 MiB；单字形加边框超过页大小时报告错误。批次在覆盖旧页之前提交，淘汰复用存储；命令重放重新取得字形位置。

动态纹理依赖由 Framework 排列；直接使用 View::add 时由调用方保证生产者先于消费者。renderVG 可执行纯 2D 输出，GUI、窗口和模型概念均不进入 Graphics。Canvas、目标、字体及图片必须存活至执行结束，绘制提供者不得在执行期间卸载场景或递归渲染。
