# Volcano World

VolcanoEngine 的实体组件系统（ECS）与场景管理模块（静态库目标 `World` / `libVolcanoWorld.a`），负责游戏世界中的实体组织、场景树层级、视口相机、灯光及各子系统组件数据的定义。

## 主要特性

- **EnTT 驱动的 ECS 架构**：`World::Scene` 按值持有 `entt::registry` 作为成员（而非继承），以值语义封装场景实体与组件生命周期及访问接口。
- **视图构建（ViewBuilder）**：`World::Scene` 提供 `buildView` 方法，内部调用 `ViewBuilder` 纯虚接口；支持派生并结合空间加速等附加扩展资源实现高效可见集算法。
- **层级变换（Inherent）**：支持父子级实体挂载，自动维护局部与世界变换矩阵更新。
- **核心组件体系**：
  - **视觉与模型**：`Model`（关联 Schema 模型与材质重载）、`Texture`（动态纹理映射虚基类）。
  - **相机系统**：抽象基类 `Camera`，派生 `PerspectiveCamera` 与 `OrthographicCamera`。
  - **灯光系统**：抽象基类 `Light`，派生 `DirectionalLight`、`PointLight`、`SpotLight`。
  - **物理与交互**：[`RigidBody`](docs/RigidBody.md)（描述碰撞形状、质量与动力学属性，解耦物理引擎后端）。
  - **音频**：`AudioSource` 与 `AudioListener`。

## 外部依赖

- `Base`
- `EnTT`

## 详细文档

- [World 组件设计与生命周期规范](docs/Design.md)
- [RigidBody 物理组件说明](docs/RigidBody.md)
