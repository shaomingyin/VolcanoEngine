# 在 QML 中设置刚体形状

应用启动时，在加载 QML 前调用一次 `Volcano::World::Scene::registerQmlTypes()`。

```qml
import Volcano.World 1.0

Scene {
    Entity {
        RigidBody {
            shape: CapsuleCollisionShape {
                radius: 0.35
                cylinderHeight: 1.2
            }
        }
    }
}
```

`shape` 也可以设为 `BoxCollisionShape` 或 `SphereCollisionShape`。

QML 通过 `Entity.components` 声明物理组件。宿主在 C++ 中持有并集成 `Volcano::Physics::System`。

宿主加载场景后，用 `Physics::System physics(scene)` 传入场景引用，再调用 `physics.enable()`。每个物理系统实例固定绑定一个场景，场景的生命周期必须覆盖该实例。场景切换时，由宿主销毁旧实例并为新场景创建实例。Physics 用实体的 `Transform` 初始化后端刚体，并处理场景组件的增删。禁用或销毁物理系统会销毁场景刚体并清除组件上的运行时关联。

每个 `RigidBody` 实例最多对应一个后端刚体。Physics 自动处理刚体的创建、加入模拟和销毁。配置无效时记录警告。整合方在场景线程调用 `System::update(deltaTime)` 后，Physics 会把每个实体的第一个已加入模拟的动态刚体的位置和旋转写回该实体的 `Transform`。Graphics 随后可直接读取场景变换。

`World::Scene::gravity` 默认是 `(0, -9.81, 0)`，可在 QML 或 C++ 中配置，运行中的物理系统会跟随场景更新。
