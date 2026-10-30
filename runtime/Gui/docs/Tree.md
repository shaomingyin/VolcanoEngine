# 控件树

Widget 采用类似 Qt 的父子生命周期：构造时可传入 `Widget* parent`，父控件负责删除仍挂接的子控件；子控件提前销毁时自动解除父链接。Document 继续用 unique_ptr 独占根 Layout，根节点不能设置 parent。Widget 和 Document 禁止复制或移动对象本身。

## 查询和修改

- `Widget(parent = nullptr)`：指定初始父控件；Layout、Text、Image、Button 继承此构造接口。
- `setParent(parent)`：转移到新父控件，追加到其子控件列表末尾；传入 nullptr 脱离当前父控件。同一个 parent 不产生变化。
- `parent()`：返回直接父控件，提供 const 与非 const 重载。
- `root()`：沿父链返回树根，独立控件返回自身。
- `contains(widget)`：判断是否包含目标，包含自身，不受可见性或启用状态影响。
- `children()`：只读访问有序的子控件指针列表，不提供 add/remove/clear。

```cpp
Gui::Layout left;
Gui::Layout right;
auto* button = new Gui::Button(&left); // 由 left 管理生命周期。
auto* label = new Gui::Text(button);
button->setParent(&right);            // right 接管整个子树。
button->setParent(nullptr);           // 脱离后由调用方管理。
std::unique_ptr<Gui::Button> owner(button);
```

转移不改变控件地址、不复制控件。有父节点的控件不应再由 unique_ptr 等独立所有者管理。栈上的子控件必须先于父控件销毁，通常先构造父控件，再构造子控件；不要将栈控件挂接到会更早销毁的父控件。

禁止以自身或自己的后代作为 parent，也禁止移动 Document 的根节点，Debug 构建使用 VOLCANO_ASSERT 检测。设置新 parent 所需的内存分配在解除旧链接前完成，分配失败保留原树。父控件按子控件列表的反向顺序删除子控件。

结构或属性变化沿父链更新 revision，移除后子树的修改不再影响原树。`acceptsInput(target)` 沿目标到当前控件的父链检查成员关系、可见性和启用状态，复杂度与深度有关；它不依赖几何命中或遮挡。

## 子树布局与绘制

任何 Widget 都可以拥有子控件，包括 Text、Image 和 Button。普通 Widget 按子控件 frame 和 margin 进行基本相对定位，支持固定尺寸、内容尺寸和 Fill；不根据子控件自动计算自身内容尺寸。需要行列、换行、内边距或对齐规则时使用 Layout。

`draw()` 统一先绘制自身，再按添加顺序绘制子控件，并将子树裁剪到父控件 bounds。自定义外观覆盖受保护的 `paint(Platform&) const`；需要保留基类外观时调用基类 paint。命中按相反顺序优先检查子控件，资源收集也遍历子树；覆盖 collectResources 时须调用基类版本。

事件仍直接发给命中控件，不向父控件冒泡。因此 Button 中添加的 Text 子控件命中后不会自动触发 Button 点击；这一版不扩展该交互规则。

## Document 交互状态

根节点上的 Widget::Observer 供 Document 处理移除通知及遍历状态。Document 构造时注册，析构时撤销；普通使用方无需自行注册观察者。

setParent 改变已挂接子树的父节点前，Document 检查捕获、悬停和焦点是否位于该子树内，先清空内部引用，然后分别发送 Cancel、Leave 和失焦通知。控件在通知期间仍然存活，父链接仍指向原父控件；setParent 完成后父链接才改为新值。移除其它分支不会干扰当前交互状态。转移到另一个 Document 不会继承旧 Document 的焦点或捕获。

通知异常直接传播，原父控件继续持有子树且父链接保持有效，已经清空的交互状态和通知产生的副作用不回滚。调用方可处理异常后重试。

直接删除控件时，Document 只清理指向该子树的交互引用，不在析构路径分发 Cancel、Leave 或失焦事件。需要这些通知时，先调用 setParent(nullptr)，再销毁控件。

Document 析构先撤销观察者，再销毁控件树，不从析构中调用用户事件处理器。

## 调用约束

控件树在界面所属线程使用。结构修改必须在事件分发、布局和绘制调用结束后执行；回调可记录修改请求，由调用方在返回后执行。Document 遍历及移除通知期间修改结构、同一 Document 的回调重入和销毁正在分发事件的 Document，属于编程错误，Debug 构建使用断言检测。

结构变化后重新执行 Document::layout，再使用新的 bounds 绘制或命中。当前事件仍直接投递给目标控件，父链尚不用于事件冒泡。
