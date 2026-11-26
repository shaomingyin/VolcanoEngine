# 在 QML 中设置刚体形状

应用启动时，在创建 QML 引擎前调用一次 `Volcano::Physics::RigidBody::registerQmlTypes()`。

```qml
import Volcano.Physics 1.0

RigidBody {
    shape: CapsuleCollisionShape {
        radius: 0.35
        cylinderHeight: 1.2
    }
}
```

`shape` 也可以设为 `BoxCollisionShape` 或 `SphereCollisionShape`。

QML 属性读写组件配置。宿主为场景持有独立的 `Volcano::Physics::System`，场景内部通过 `Entity.components` 挂载物理组件。

`setShape()` 可以在启用物理系统之前设置形状配置。启用 System 后，调用 `createBody(rigidBody, position, rotation)` 创建刚体并取得 `System::BodyId`。位姿使用 `QVector3D` 和 `QQuaternion`，默认是原点和单位旋转。随后用 `addBody()` 将刚体加入模拟。宿主每帧调用 `System::update()`，可以通过 `bodyPose()` 取得模拟后的位姿。

`removeBody()` 将刚体移出模拟，之后可以再次加入。`destroyBody()` 会先移出仍在模拟中的刚体，再销毁它。刚体 ID 只在创建它的 System 和本次启用周期内有效。无效配置、无效 ID 或物理后端失败通过 C++ 异常报告。
