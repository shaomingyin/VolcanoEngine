# World 设计原则与组件

## 定位

World 定义 VE 能描述的世界，是以 Entity 为身份、Component 为记录的实时状态数据库。
游戏逻辑通过组件表达对象、属性和期望状态；各子系统提取相关描述，处理后写回它们负责的状态。
Framework 组织资源加载、子系统绑定、运行状态、更新顺序和生命周期。

World 不创建 GPU、Jolt、音频后端对象，不调用文件加载、渲染、物理或声音设备接口。
依赖方向为子系统读取 World；World 不依赖具体后端。
组件描述既包含可持久化内容，也包含游戏过程中的状态，不能把整个 registry 的内存直接作为文件格式。

## 类与值语义

`World::Scene` 直接公有继承 `entt::registry`，在保留 ECS 注册表全部能力的同时补充场景元信息、`Context` 引用与 `update()`/`player()` 生命周期接口。由于 `entt::registry` 不带虚析构函数，`Scene` 的所有权和销毁须始终通过 `World::Scene`（而非 `entt::registry*`）管理。

World 的工程自有组件使用 class，成员数据私有，提供构造、const 查询和显式修改方法。
不返回可修改的成员引用、指针或容器迭代器；修改嵌套数据时先复制、修改，再通过方法写回。
默认构造产生可使用的默认值；资源路径为空表示尚未指定资源，加载时由 Framework 报告缺失。

内部尽量保持简单值语义：优先基本类型、Eigen 值、资源路径和必要的标准容器，按值持有；
不为字段访问增加 PImpl、动态分配、通用属性系统或一套平行的组件包装对象。
复制组件得到独立状态，组件内不保存所属 Scene 指针，不暗中发起跨系统操作。
所有具体组件及刚体形状支持复制构造、复制赋值、移动构造和移动赋值；优先使用编译器隐式生成的操作，
不为逐字段复制手写特殊成员函数。
移动后的源对象仍可销毁或重新赋值，不依赖其名称、路径等字段保持原值；不承诺所有移动都是常数时间或 noexcept。
Camera/Light 基类的复制和移动保持 protected，由具体派生组件使用，防止通过基类赋值绕过派生类型约束。
此处要求针对组件，不要求 Scene、Framework 宿主或持有独占后端资源的子系统可复制。

这里的“尽量按 POD 实现”指实现简单、数据职责明确，不强求 C++ 的 trivial 或 standard-layout 条件。
Camera/Light 使用继承与虚析构，Model/AudioSource 保存路径，Eigen 值也不保证可按字节复制。
禁止用 memcpy、sizeof 或对象内存布局定义持久化格式。
Eigen、Color、Rect、Size 等数学值保持原生接口，不为统一命名额外包装。
包围盒直接使用 Eigen::AlignedBox3f，不提供 World 别名。
Base::Transform 是实际挂载的变换组件，提供 const 查询和 setTranslation/setRotation/setScale。

## 方法、校验与异常

方法负责自身数据的访问、修改、校验和派生计算，例如投影矩阵、形状尺寸和光线方向归一化。
不在组件中实现资源加载、物理步进或绘制。

- 独立属性使用 `property() const` 与 `setProperty(value)`；布尔查询使用 `isXxx()`，动作使用 enable/play 等明确动词。
- getter 和直接赋值的简单 setter 在类内定义；涉及复杂校验、归一化、矩阵计算或后端操作的方法在头文件声明、在 `.cpp` 实现。
- `constexpr` 方法的定义保留在头文件，包括带标量校验的 setter；不为移动实现而取消已有的编译期能力。
- 查询如需执行状态检查或资源查找，同样在 `.cpp` 实现；不以 getter/setter 的名字判断实现是否简单。
- 保留各类型的所有权和访问语义，不为统一风格给组件添加可修改引用，也不删除流或宿主已有的可修改对象访问接口。
- 不机械地添加无需求的重载和接口。
- 相互约束的字段成组修改，例如 setZRange、setRect、setAngles、setDimensions。
- World 描述的域约束在构造或 setter 中校验，失败抛出 std::invalid_argument，保留修改前的值。
- 已由构造/setter 保证的内部状态，在派生计算前使用 VOLCANO_ASSERT 检查，不重复增加异常分支。
- 派生计算仍检查无法表示的结果，例如浮点运算导致投影矩阵非有限。
- 后端特有约束由子系统校验；Physics 对 Transform 要求单位缩放和单位四元数。
- Base::Transform 是通用数学值，保留零/负缩放；有限矩阵等要求在 affine/normalMatrix 和消费方检查。
- 字符串、路径赋值及可能报告错误的方法不声明 noexcept；析构不得向外抛出异常。

Scene::update 调用游戏的 onUpdate，允许异常传播到宿主边界。游戏只有在确实不会抛出时才将 override 标为 noexcept。

## constexpr

不改变现有存储和继承方式，为可参与常量求值的接口添加 constexpr：

- 自身字段的只读查询，包括返回 const 引用的接口。
- 不依赖 Eigen 运算的标量校验与修改，以及声音期望状态、循环/空间化开关。
- Sphere/Capsule 的默认构造、参数构造、尺寸查询与修改。
- Inherent 的构造、名称和标记操作，利用标准库 C++20 起提供的 constexpr string 支持。
  名称参数使用 string_view，组件仍复制并拥有内部 string，不借用外部字符存储。

constexpr 定义放在头文件；简单校验直接写在所属构造或修改方法中，不额外封装通用校验函数。
Sphere/Capsule 和 Inherent 的有效操作通过 static_assert 验证确实可以在编译期执行。
非法值在运行时仍抛出原有异常，在常量求值时则不能形成有效常量表达式。

constexpr 修饰方法并不保证整个组件都能构造为常量对象。
Eigen 目前限制相机、灯光、Transform、Box 等对象的常量构造和计算；
filesystem::path 仍是运行时类型；C++23 不保证 std::cos 可用于常量求值，聚光灯锥角校验保留运行时实现。
string 的动态分配也必须遵守常量求值生命周期规则，不承诺任意长度名称都能保存为 constexpr 对象。
本次不改写数学存储、路径或虚函数，也不以 constexpr 代替可平凡复制的类型约束。

## 继承与组件查询

相机和灯光保留语义明确的派生关系：

```text
Camera
├── PerspectiveCamera
└── OrthographicCamera

Light
├── DirectionalLight
├── PointLight
└── SpotLight
```

Camera 共用局部变换、裁剪范围和投影接口；PerspectiveCamera 保存视场角、宽高比，
OrthographicCamera 保存投影范围。透视裁剪范围满足 0 < near < far；正交范围允许轴反向。
通过 Camera 引用修改裁剪范围时，仍调用具体相机的校验。

Light 共用局部变换、颜色和强度。三个派生类各自保存方向、范围或锥角。
SpotLight 直接继承 Light，自行保存方向，不通过 DirectionalLight 复用字段。
颜色允许 HDR 非负值，强度及最终辐射值须有限；方向归一化，零向量和非有限方向报错。
聚光灯角度以度为单位，使用半角，满足 0 <= inner < outer <= 90，且余弦值可区分。

Camera 提供实际的多态投影计算，Camera/Light 的虚析构保证通过基类销毁安全。
不为所有组件增加通用基类、虚函数和虚拟属性接口。
EnTT 按具体类型存储，view<Camera>() 不会查询到派生组件。Framework 分别查询具体相机和灯光。
渲染使用的相机实体必须恰好具有一种支持的相机组件；Framework 已执行此项检查。

## 组件清单与职责

| 组件 | 内容 | 处理方 |
| --- | --- | --- |
| Inherent | 名称、enabled/visible 标记 | 游戏逻辑、Framework |
| Base::Transform | 平移、缩放、旋转，按需生成仿射矩阵 | 游戏逻辑、Physics 写回、Framework 提取 |
| Model | 模型资源路径及其按 Part 下标覆盖的材质定制（Material 映射） | Framework 加载 Asset 资源并准备 GPU 对象，关联材质与网格 |
| PerspectiveCamera / OrthographicCamera | 相机参数、局部变换及独占拥有的可选 HUD 纹理（`std::unique_ptr<Texture>`） | Framework 提取设置视图/投影矩阵，渲染末尾叠加相机 HUD（由调用方控制实际输出 Target） |
| DirectionalLight / PointLight / SpotLight | 灯光参数及局部变换 | Framework 提取并加入 View |
| RigidBody | 形状、形状局部 offset、运动类型、质量、表面属性、阻尼、重力倍率 | Physics 转换为后端对象并模拟 |
| AudioSource | 声音路径、局部变换、增益、音高、循环、空间化、期望播放状态 | 声音子系统；目前尚无播放后端 |
| AudioListener | 监听局部变换和增益 | 声音子系统；目前尚无播放后端 |

Inherent 在当前渲染提取中控制 enabled/visible；它不会自动停用物理或所有游戏逻辑。
跨系统的启用策略应由 Framework 明确定义，不在组件 setter 中暗中执行。

AudioSource 的 play/pause/stop 只修改期望状态；暂停停止状态无操作，play 表示期望进入 Playing。
当前没有音频资源加载、播放时间、播放结束状态反馈或设备管理实现。
空间音源和监听器的位置通过实体 Transform 与组件 offset 组合表达。
是否选择单个或多个监听器由后续 Framework 声音策略决定。
本轮不增加尚无对应处理机制的层级、动画、脚本、网络组件。

## 修改通知与状态写入

组件的方法不隐式通知 EnTT，已挂载组件的修改通过 patch/replace 显式通知消费方：

```cpp
scene.patch<World::RigidBody>(entity, [](auto& body) {
    body.setMass(2.0f);
});

scene.patch<World::PerspectiveCamera>(cameraEntity, [](auto& camera) {
    camera.setAspect(16.0f / 9.0f);
});
```

首次 emplace 后可完成初始配置，随后交给 Framework 加载/同步。
patch 回调不保证多次 setter 的整体事务性；需要多个字段一起提交时，先编辑组件副本，再 replace。
修改资源路径后仍需调用 Framework 的资源同步接口，setter 不执行 I/O。

各子系统可在 Entity 上附加自己定义的运行时组件，也可使用内部缓存。
运行时组件不是 World 的内容描述，不参与场景持久化；创建它的子系统负责解除绑定时的清理。
明确每份状态的写入方：例如动态刚体位置由 Physics 写回，Graphics 随后读取，避免多个系统无序覆盖。

## 当前调整与验证

本轮修复可修改引用绕过封装、Model/RigidBody 公开字段、SpotLight 继承关系、基类销毁安全，
以及 Scene 字符串赋值和更新接口不合理的 noexcept。补充 AudioSource/AudioListener 描述。

WorldMathCheck 覆盖投影，以及组件构造/修改、失败保留状态、复制独立性、声音描述和异常传播。
BaseTransformCheck、PhysicsCheck、FrameworkCheck 覆盖调用方和生命周期。
World 的公共头文件应能够独立包含。窗口和真实渲染验证需要 display 环境。

物理组件细节见 [RigidBody](RigidBody.md)；运行策略见 [Framework](../../Framework/docs/Framework.md)。

## 视图提取与渲染解耦

- **纯粹的声明数据层**：`World::Scene` 保持为纯净的 ECS 状态容器，不内建任何图形后端对象或 Draw List 生成逻辑，服务端与客户端共享相同的声明式场景与组件。
- **运行时解析与视角提取**：`Framework`（如 `Local`）在提交资源时将声明式 `World::Model` 解析为轻量的运行时组件（`Graphics::Model`），并通过 `World::Canvas` 的虚接口（`prepare()`、`revision()`、`collectResources()`）统一刷新 `Camera::hud()` 与 `World::Material` 拥有的动态画布（含 `Gui::Document`）而无需感知上层 GUI 细节；`Graphics::View::extract` 直接基于 EnTT 从 `Scene` 读取运行时渲染组件、变换、灯光、相机矩阵及相机 HUD，通过 `TextureResolver` 回调解析纹理并填充 `Graphics::View`。
- **Graphics::View 即绘制列表**：消除中间的 `DrawList3D` 或 `World::View` 冗余抽象，`Graphics::View` 本身即为包含 3D 渲染项、光源、相机矩阵、2D Canvas 与相机 HUD 的完整 Draw List，直接送入 `Renderer::render(view, target)` 执行渲染。
- **材质与纹理归位**：`World::Canvas` 继承自 `World::Texture`（通过 `Texture::Kind` 区分 `Path`、`Canvas`、`Document`、`Graphics`），使 `World` 在不依赖 `Graphics`、不使用 RTTI 或 `std::shared_ptr` 的前提下统一表达纹理所有权。`World::Model` 按 Part 下标管理运行时材质覆盖（`Material`），`World::Material` 与 `World::Camera::hud()` 直接独占拥有 `std::unique_ptr<World::Texture>`，各自独立持有静态贴图、`World::Canvas`、`Gui::Document` 或运行时 GPU 纹理。
