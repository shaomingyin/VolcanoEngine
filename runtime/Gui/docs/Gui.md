# Gui

Gui 公开依赖 Base，不依赖 Graphics、窗口系统或 GPU。Document 持有控件树及焦点、指针捕获，并借用使用方提供的 Platform；Widget 管理父子关系、子树绘制与命中，Layout 提供布局规则，Text、Image、Button 提供具体内容。

## 控件文件

Widget、Layout、Text、Image、Button、CheckBox、Slider、ProgressBar 各自定义在同名的 `.h` / `.cpp` 中。使用具体控件时直接包含对应的 `<Volcano/Gui/控件名.h>`；`Widget.h` 只定义基类，不汇总其它控件。

## Platform

GUI 需要的宿主接口集中在 `Gui::Platform`：路径、填充、描边、图片、文字、裁剪、文字测量，以及光标、UTF-8 剪贴板、文本输入会话、输入法区域和宿主指针捕获。输入事件通过 Document 投递，不通过 Platform 注册事件回调。Gui 不维护字体或字形缓存。

```cpp
// platform 由宿主实现，生命周期必须长于 document。
auto root = std::make_unique<Volcano::Gui::Layout>();
Volcano::Gui::Document document(platform, std::move(root));
document.layout({800, 600});
document.draw();
```

Document 构造时接收 `Platform&`，不获取所有权、不复制平台，也不负责销毁平台。多个 Document 可以借用同一 Platform；使用方须确保平台在所有借用者销毁前有效，并在界面所属线程依次调用它们。`platform()` 返回构造时借用的平台，`draw()` 自动使用该平台，不能为单次绘制替换平台。

宿主负责在绘制前准备当前目标，多个 Document 共享平台不表示共享控件树、焦点或捕获状态。输入法定位区域使用 Document 局部逻辑坐标，宿主负责转换到屏幕坐标；宿主捕获丢失时须向对应 Document 投递指针取消事件。字体使用 PhysFS 虚拟资源路径，图片使用宿主约定的资源标识。

## Style

`Gui::Style` 定义基础外观配置，位于 `<Volcano/Gui/Style.h>`。所有配置通过同名 getter 和 setXxx setter 访问。

| 配置 | 默认值 | 用途 |
| --- | --- | --- |
| `textColor` | 白色 | Text 默认文字颜色，Button 继承使用 |
| `disabledTextColor` | RGB 均为 0.5 的灰色 | 控件或祖先禁用时的默认文字颜色 |
| `fontSize` | 16 | 默认字号，范围为 1～4096 |
| `lineHeight` | 1.2 | 行高相对字号的倍数，同时用于测量和绘制 |
| `backgroundColor` | 透明 | 所有 Widget 的默认背景 |
| `borderColor` | 白色 | 所有 Widget 的边框颜色 |
| `borderWidth` | 0 | 非负有限宽度，零表示不绘制边框 |
| `imageColor` | 白色 | Image 的图片着色，包含透明度 |
| `buttonPressedColor` | 透明度 0.2 的黑色 | Button 按下时叠加在内容上的颜色 |

lineHeight 须为正数，且与最大字号相乘后仍为有限值；非法字号、行高和边框宽度抛出 invalid_argument，不改变原配置。边框在控件内容及子树之后绘制，向内缩进半个线宽；实际线宽限制在控件最短边以内，不占据布局空间。默认背景和边框会作用于每个节点，不只作用于 Document 根节点。

宿主实现 `Platform::style() const noexcept`，返回当前 Style 的 const 引用，并保证配置在 GUI 调用期间有效。Style 由宿主持有，可以复制或整体替换；多个 Document 借用同一 Platform 时读取同一份当前配置，不依赖全局变量。

```cpp
Gui::Style style;
style.setTextColor(Colorf{0.9f, 0.9f, 0.9f});
style.setFontSize(18);
style.setLineHeight(1.4f);
style.setBackgroundColor(Colorf{0.1f, 0.1f, 0.1f});
style.setBorderWidth(1);
style.setButtonPressedColor(Colorf{0, 0, 0, 0.35f});
// 宿主的 Platform::style() 返回所持有的 style。
```

Text 的字号和文字颜色、Widget 的背景均以显式 setter 设置为优先，未设置时读取 Platform 当前样式。显式透明背景同样会覆盖默认背景；显式文字颜色在禁用状态下仍然优先。`Text::fontSize(platform)` 和 `Widget::background(platform)` 返回解析样式后的有效值，替代原来不带参数的 getter。

Style 不负责绘图、不包含 GPU 资源，也不替代控件的布局属性或字体路径接口。当前边框、行高、图片着色和按钮按下覆盖颜色由共享样式统一设置，不额外添加逐控件覆盖接口。

颜色和边框变更在下一次 draw 时生效；字号或行高变化后，应先重新调用 Document::layout，再绘制。样式变化不会修改控件树 revision。宿主须主动请求重新绘制；只根据 Document::revision 判断是否重绘的调用方还需要跟踪宿主样式变化。当前不实现选择器、层叠规则、样式继承、自动失效通知或主题文件加载。

## 基础交互控件

| 控件 | 当前能力 |
| --- | --- |
| `CheckBox` | 二态复选框；主按钮在控件内释放或 Space 按下时切换；取消、失焦和控件外释放不切换 |
| `Slider` | 水平连续数值选择；点击轨道和拖动手柄；方向键步进、Home/End 跳转端点，支持按键重复 |
| `ProgressBar` | 只读水平进度显示；setValue 接收 0～1 的比例，不接收交互事件、不获取焦点 |

三个控件都支持构造时传 parent、setParent、Auto/Fixed/Fill 尺寸和现有树生命周期。默认首选高度为 Style::fontSize；CheckBox 默认宽高相同，Slider 和 ProgressBar 默认宽度是高度的八倍。需要随窗口伸缩时显式设置 Fill。Slider 宽度必须大于高度才有拖动空间，尺寸不足时仍可通过键盘调值。

```cpp
auto* checkbox = new Gui::CheckBox(root.get());
checkbox->setOnToggled([](bool checked) {
    // 更新游戏设置。
});
auto* slider = new Gui::Slider(root.get());
slider->setRange(0, 100);
slider->setStep(5); // 仅用于键盘调节；拖动保持连续。
slider->setOnValueChanged([](float value) {
    // 更新音量等数值。
});
auto* progress = new Gui::ProgressBar(root.get());
progress->setValue(0.5f);
```

Slider 默认为范围 [0, 1]、值 0、键盘步长 0.01；setRange 要求有限且 minimum < maximum，setStep 要求有限正数。Slider 和 ProgressBar 的有限输入值会限制到有效区间，NaN、无穷大和非法范围抛出 invalid_argument。

CheckBox::setChecked 和 Slider::setValue 在实际值变化时更新 revision，并同步调用对应回调；程序设置与用户输入遵循相同规则。Slider 缩小范围导致值变化时同样通知。重复设置相同值不通知，回调异常直接传播，已设置的有效状态保留。回调可替换自身；结构修改仍须遵守控件树的遍历约束。

这版使用 Style 的 textColor 作为前景、disabledTextColor 作为禁用/轨道颜色，并沿用背景和边框配置，不新增控件专用样式。CheckBox 只绘制方框和勾选标记，不内置标签；配套文字可用 Layout 排列 Text，点击文字不会自动切换复选框。当前不提供三态、垂直滑块、拖动吸附、进度文字或不定进度动画。

## 控件与事件

控件通过 setter 修改属性，revision 表示内容变化；修改沿 parent 链更新祖先版本，查询根版本无需遍历子树。布局由已有 layout.h 驱动，支持绝对、横向和纵向排列、Auto/Fixed/Fill 尺寸、边距、内边距、间距、对齐、换行及嵌套测量，详见 [Layout](Layout.md)。Layout 裁剪子节点，Text 的测量和绘图共用 UTF-8 分行逻辑。

Document::layout 设置当前画布布局，draw 输出绘图操作。输入定义在 `Event.h`，通过 Document 的重载 `event(const XxxEvent&)` 同步投递，返回是否消费。旧的 pointer/key 入口已删除。事件不依赖窗口系统，不使用 RTTI 或统一事件基类。

| 类型 | 内容与投递目标 |
| --- | --- |
| `PointerEvent` | 移动、按下、释放、取消、进入/离开；Document 逻辑坐标、设备、标识、按钮、修饰键。投递给命中控件或捕获控件 |
| `ScrollEvent` | 位置、横纵滚动量、行/像素单位、修饰键。投递给命中控件 |
| `KeyEvent` | 按下、释放、重复、逻辑键、宿主扫描码、修饰键。投递给焦点控件 |
| `TextInputEvent` | 已确认的 UTF-8 文本，投递给焦点控件，不能由按键事件推导 |
| `CompositionEvent` | 输入法组合开始、更新、提交、取消、UTF-8 文本及选区，投递给焦点控件 |
| `FocusEvent` | 宿主激活或失活；Document 切换控件焦点时也向对应控件发送通知 |

```cpp
using namespace Volcano::Gui;
document.event(PointerEvent{PointerEvent::Action::Down, 10, 20});
document.event(PointerEvent{PointerEvent::Action::Up, 10, 20});
document.event(KeyEvent{KeyEvent::Action::Down, KeyEvent::Key::Enter});
document.event(TextInputEvent{"中文"});
```

Widget 对各类事件提供虚函数 `event(const XxxEvent&, Platform&)`，默认返回 false；派生控件按需覆盖，并使用 `using Widget::event`（或对应直接基类）保留其它重载。事件及其中的数据只在同步处理期间借用，控件需要留存时自行复制。输入文本由宿主保证是有效 UTF-8；CompositionEvent 的选区使用字节偏移，Document 校验范围且不允许截断 UTF-8 字符。

主按钮按下时 Document 设置或清除焦点。焦点与捕获目标须仍属于可见且启用的控件树，Widget 的 acceptsInput 查询沿父链检查这一条件，不依赖遮挡关系。消费按下事件的控件获得内部指针捕获，后续移动和释放投递给同一控件；这一版只保留一个捕获，期间忽略其它设备/指针标识，以及不匹配的按钮释放。Document 设置 PointerEvent::inside 表示捕获控件是否仍被命中。鼠标和笔的命中控件变化会生成 Enter/Leave，触摸不生成悬停事件。

Button 只接受主按钮点击，在原控件上释放才触发回调，取消或失去焦点会清除按下状态。Enter/Space 按下可激活焦点按钮，释放与重复不重复激活；Control/Alt/Super 组合不作为按钮激活。界面失活时 Document 取消捕获、清除悬停与焦点，并忽略后续输入，重新激活不自动恢复焦点。

Platform 的宿主指针捕获、剪贴板和输入法接口仍由宿主及具体交互控件调用，Document 不自动操作这些全局服务。共享平台时由宿主决定向哪个 Document 投递输入，并协调系统捕获及文本输入会话。CompositionEvent::Commit 与 TextInputEvent 都能表达确认文本，同一次提交须选择一种投递，避免重复插入。

当前只实现同步目标分发，没有事件冒泡、Tab 焦点导航、多指针同时捕获、拖放或文本编辑控件；滚动和输入法事件已有可覆盖的处理入口，尚未提供对应控件和系统后端。Framework 的平台实现及场景构造调用方仍需适配此前的 Platform/Document 接口；现有 Local 指针及键盘转发改为使用类型化事件，但本轮不扩展原生输入来源。

GUI 树可作为 World::GuiTexture 的内容。子控件由父 Widget 独占，Widget 保存非拥有的父指针；通过构造参数和 setParent 建立、解除或转移父子关系，详见 [Tree](Tree.md)。事件回调属于游戏逻辑，不作为文件序列化内容。结构修改在事件、布局和绘制调用返回后执行，不在回调中销毁正在处理事件的 Document。
