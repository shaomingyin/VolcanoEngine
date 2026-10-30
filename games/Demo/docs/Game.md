# Demo

游戏由 `World::Scene` 的派生类实现。以 Demo 为例，`Volcano::Demo::Game` 负责场景构建和游戏逻辑，
`Framework::Base` 以 `std::unique_ptr<World::Scene>` 保存这个实例，并通过 Scene 接口驱动它。
同一游戏可交给 Local、Client 或 Server 运行，具体游戏无需继承 Framework 的类。

## 游戏入口

Demo 自己的 `games/Demo/include/Volcano/Game.h` 声明游戏入口，宿主通过 `<Volcano/Game.h>` 包含它：

```cpp
std::unique_ptr<Volcano::World::Scene> volcanoCreateGame();
```

每次调用必须返回新的非空 Scene。Demo 的实现是：

```cpp
#include <Volcano/Game.h>
#include <Volcano/Demo/Game.h>

std::unique_ptr<Volcano::World::Scene> volcanoCreateGame() {
    return std::make_unique<Volcano::Demo::Game>();
}
```

当前场景构建、实体配置和游戏规则均直接写在 `games/Demo` 的 Scene 派生类中，暂不引入 JSON Manifest。
游戏通过 Scene 派生类的构造过程创建实体和资源路径组件，通过 onUpdate 实现规则。
运行控制和资源加载由 Framework 负责，Local/Client 创建窗口及 GL 上下文；
PhysFS 挂载和宿主循环由 Launcher/Server 入口负责。
World 不声明游戏入口，Framework 也不包含具体游戏的头文件。

## 工程接入

每个游戏在 `games/<name>` 下提供 CMakeLists.txt 和自己的公共头文件，调用 `volcano_game_setup(target)`。
该宏链接 World，并创建指向当前游戏目标的 Gameplay 别名。
各游戏自己导出 include 目录，其中的 `Volcano/Game.h` 提供同名创建入口；文档放在各游戏自己的 docs 下。
`games` 根目录仅负责选择具体游戏，不提供公共 include 或 docs 目录。
通过 CMake 的 `VOLCANO_GAME` 选择游戏；宿主链接 Gameplay，并将 volcanoCreateGame 交给 Framework::load。

```cpp
framework.load(volcanoCreateGame);
framework.play();
```

Base 保存工厂以支持 stop 时重建 Scene，暂停则保留当前实例。
工厂承担创建职责，Scene 派生类承担游戏逻辑；生命周期约定见 [Framework](../../../runtime/Framework/docs/Framework.md)。

## GUI 动态纹理演示

使用 `Launcher --root games/Demo/assets` 运行。相同的 GUI 输出纹理同时用于 HUD 和场景模型；点击 HUD 绿色区域会改变两处颜色。场景中的点击拾取不在本演示内。示例不绑定系统字体；使用文字控件时应把字体随游戏资源分发，并通过控件的 setFont 指定虚拟路径。
