# Volcano Physics

VolcanoEngine 的物理模拟系统（静态库目标 `Physics` / `libVolcanoPhysics.a`），基于 **Jolt Physics** 实现 3D 刚体动力学模拟与碰撞检测。

## 主要特性

- **Jolt 物理引擎集成**：封装 Jolt 的 `PhysicsSystem`、JobSystem、BroadPhase 及内存分配器，提供高性能多线程物理模拟。
- **ECS 自动绑定**：`Physics::System` 直接绑定 `World::Scene` 的实体注册表，自动发现带有 `World::RigidBody` 的实体并创建对应的物理刚体（Body）。
- **双向同步**：
  - 模拟前同步实体变换与状态变更。
  - 步进模拟后将物理位置与旋转写回实体的 `Inherent` 变换组件。
- **解耦设计**：`World` 模块本身不直接链接或依赖 Jolt，所有具体物理交互与求解均在 `Physics` 模块内完成。

## 外部依赖

- `World`
- `Jolt` (Jolt Physics 5.2.0)

## 详细文档

- [Physics 物理系统设计与模拟流程](docs/Physics.md)
