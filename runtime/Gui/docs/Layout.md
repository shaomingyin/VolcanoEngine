# Layout

Gui 使用已有的 `include/layout.h` 进行布局计算，不依赖 Graphics 或窗口系统。Widget 保存自身布局属性，Layout 保存子控件排列规则，Document 借用 Platform 进行文字测量。

## 使用方式

```cpp
#include <memory>
#include <Volcano/Gui/Layout.h>
#include <Volcano/Gui/Document.h>
#include <Volcano/Gui/Text.h>

using namespace Volcano;
using Sizing = Gui::Widget::Sizing;

auto root = std::make_unique<Gui::Layout>();
root->setLayout(Gui::Layout::Mode::Vertical);
root->setPadding({12, 8, 12, 8});
root->setGap(8);

auto* title = new Gui::Text(root.get());
title->setFont("fonts/ui.ttf");
title->setFontSize(24);
title->setText("设置");
title->setWidth(Sizing::Fill); // 高度由文字内容决定。

auto* content = new Gui::Layout(root.get());
content->setWidth(Sizing::Fill);
content->setHeight(Sizing::Fill); // 占据标题和间距之外的剩余高度。

// platform 提供字体测量和绘图服务，生命周期长于 document。
Gui::Document document(platform, std::move(root));
document.layout({800, 600});
document.draw();
```

## Widget 属性

| 接口 | 含义 |
| --- | --- |
| `setFrame(Rectf)` | 设置相对位置和固定宽高；同时将两个轴切换到 Fixed |
| `setWidth(float)` / `setHeight(float)` | 设置非负固定尺寸，允许零 |
| `setWidth(Sizing)` / `setHeight(Sizing)` | 分别选择 Fixed、Auto 或 Fill，默认 Auto；切回 Fixed 时使用最后保存的固定值 |
| `setMargin(float)` / `setMargin(Insets)` | 设置统一或左、上、右、下外边距，允许有限的负值以表达重叠 |
| `setHorizontalAlignment(Alignment)` / `setVerticalAlignment(Alignment)` | 选择 Auto、Start、Center、End；默认 Auto，继承父容器的交叉轴对齐规则 |
| `setBreakBefore(bool)` | 在父容器开启换行时，从该控件开始新行或新列 |

`Insets` 定义在 Widget 内，Layout 可以直接使用继承的类型。布局属性通过方法访问，不暴露控件成员数据。

`frame()` 保留调用方设置的位置和固定尺寸，`bounds()` 是布局后 Document 坐标系中的实际矩形。frame 的 left/top 与外边距起始边相加；流式布局一般让 left/top 为零，使用 margin 表达间距。布局不会改写 frame，也不增加 revision。

Auto 对普通 Widget/Image 没有隐含资源尺寸，默认为零；这些控件应指定尺寸或使用 Fill。Text 根据 Platform 的文字测量计算内容尺寸；Layout 根据子控件及 padding 计算内容尺寸。

Fill 在流式布局主轴上与同一行或列的其它 Fill 控件等分剩余空间，在交叉轴或绝对布局中填满可用区域，均扣除外边距。Fill 优先于同一轴上的对齐属性。不支持填充权重。可用区域不足时，layout.h 会压缩非固定项；固定主轴尺寸允许溢出，由容器绘图裁剪限制可见部分。

普通 Widget 也能包含子控件，按相对 frame 和 margin 排列，Fill 使用父控件剩余区域；不处理对齐、换行和 gap，也不根据子控件扩展自身 Auto 尺寸。这些布局规则由 Layout 提供。所有 Widget 都裁剪超出自身 bounds 的子树。

## Layout 属性

| 接口 | 含义 |
| --- | --- |
| `setLayout(Mode)` | Absolute、Horizontal、Vertical；默认 Absolute |
| `setPadding(float)` / `setPadding(Insets)` | 设置非负内边距，四个边独立 |
| `setGap(float)` / `setGap(horizontal, vertical)` | 设置非负水平、垂直间距；行列边缘不额外添加尾部间距 |
| `setJustify(Justify)` | 主轴 Start、Center、End、SpaceBetween；默认 Start，Absolute 下无效 |
| `setAlignment(Alignment)` | 设置子控件默认交叉轴 Start、Center、End 对齐；Absolute 下作用于两个轴 |
| `setWrap(bool)` | Horizontal 按宽度换行，Vertical 按高度换列；Absolute 下无效 |

子控件可以用自身对齐属性覆盖默认规则。Center 对齐外边距盒，非对称外边距不会被重复计入位置偏移。SpaceBetween 遵循 layout.h 的换行规则：自动换行的末行或显式断行之前不额外拉开项目间距；单个项目保持起始对齐。

不可见控件不占布局空间、没有 gap，并得到空 bounds；禁用控件继续占用空间。容器内容区域为零时，子控件得到空 bounds。Document 的根容器总是采用调用方传入的尺寸，其自身尺寸和外边距不改变 Document 区域。

## 测量与排列

`measure(Platform&, Sizef available) const` 返回首选尺寸，不改变 bounds。available 必须是有限、非负的尺寸；不使用无穷大表示无限空间。Auto 容器在宽度不足时会约束到可用宽度；开启换行的 Auto 容器使用可用主轴尺寸决定换行位置。

`layout(const Rectf&, Platform&)` 接收已经分配的 Document 坐标矩形。Layout 先测量子控件，再把尺寸、边距、填充与对齐映射为 layout.h 项目，计算后递归排列。分配宽度改变且高度不是固定值时，会再次测量高度并重排，用于嵌套布局和文字换行。

Text 的测量和绘图共用分行逻辑，支持显式换行及按 UTF-8 字符边界自动换行，行高由 Platform 当前 Style::lineHeight 决定，默认是有效字号的 1.2 倍；未显式设置的字号也来自当前 Style。它不实现完整 Unicode 行分隔、字素簇或按单词换行规则。

调用方应在内容或目标尺寸变化后重新执行 Document::layout。当前每次布局都会重新测量并建立临时布局上下文，不依赖隐含的全局布局状态，也不缓存可能变化的字体测量结果。上下文用 ScopeGuard 释放；分配失败抛出 bad_alloc，非有限计算结果和非法测量结果通过异常报告。

## 后端与范围

`src/Volcano/Gui/Layout.cpp` 是唯一启用 LAY_IMPLEMENTATION 的翻译单元；不修改第三方 layout.h。Gui 目标公开定义 `LAY_FLOAT=1`，保证使用该库的依赖目标采用一致的浮点 ABI，支持小数坐标和超过 int16 范围的尺寸。

本版不提供 CSS Grid、百分比尺寸、min/max 尺寸约束、加权 flex、滚动容器或布局动画。换行支持行内/列内对齐，没有跨行内容分布接口。事件命中继续使用排布后的 bounds 和容器裁剪范围。
