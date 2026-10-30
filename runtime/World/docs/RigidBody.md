# RigidBody

`<Volcano/World/RigidBody.h>` 定义 VE 自己的物理描述组件，不包含 Jolt 类型或运行时句柄。
实体同时具有 Base `Transform` 和 `World::RigidBody` 时，Physics 才创建刚体。
形状和运动属性放在同一个组件内，不需要额外 Collider 组件。

本组件遵循 [World 设计原则](Design.md)：私有数据、const 查询、显式 setter、按值持有。
Box/Sphere/Capsule 也使用封装的值类；通过 shape() 查询，通过 setShape() 替换。

## 数据

| 字段 | 含义 | 默认值 |
| --- | --- | --- |
| shape | Box / Sphere / Capsule 的 std::variant | Box，半尺寸各 0.5 |
| offset | 碰撞形状相对实体原点的局部变换 | 单位变换 |
| motion | Static / Kinematic / Dynamic | Dynamic |
| mass | 总质量，Dynamic 使用，惯性由形状计算 | 1 |
| friction | 摩擦系数 | 0.2 |
| restitution | 弹性系数，范围 [0, 1] | 0 |
| linearDamping / angularDamping | 线性 / 角阻尼，范围 [0, 1] | 0.05 |
| gravityFactor | 重力倍率，可为负数 | 1 |

Box 的 halfExtents 是沿三个局部轴的半尺寸；Sphere 的 radius 是半径。
Capsule 沿局部 Y 轴，halfHeight 是中间直线段的一半，整体高度为 `2 * (halfHeight + radius)`。
形状尺寸必须有限且大于零。`offset()` / `setOffset()` 描述形状相对于实体的局部平移和旋转，
按值保存；默认单位变换使形状位于实体原点。形状世界变换为 `实体 Transform × offset`。
实体 Transform 始终对应刚体原点和旋转，不是形状中心或质心。
模拟结果直接写回实体 Transform，不修改 offset，也不把 offset 重复乘入实体变换。
mass 必须有限且大于零；friction 必须有限且非负；其余标量必须有限并满足上述范围。

实体 Transform 和 offset 均要求位置有限、旋转为单位四元数、scale 为 (1, 1, 1)。
通过修改 shape 的尺寸设置物理大小，当前不隐式使用 Transform 的缩放。

## 使用

```cpp
const auto entity = scene.create();
scene.emplace<Transform>(entity).setTranslation(Eigen::Vector3f(0, 2, 0));
auto& body = scene.emplace<World::RigidBody>(entity);
body.setShape(World::RigidBody::Sphere{0.5f});
body.setMass(2.0f);
Transform offset;
offset.setTranslation(Eigen::Vector3f(0, 1, 0));
body.setOffset(offset); // 球心位于实体局部坐标 (0, 1, 0)
```

首次添加后通过方法配置；Physics 在下一次 synchronize/update 时读取完整描述。
之后使用 registry.patch/replace 修改描述，以通知 Physics；直接取得引用修改已有描述不会触发同步。
描述变更会重建对应刚体；非静态刚体之间替换保留线速度和角速度。
构造和 setter 校验组件自身的约束，非法值抛出 std::invalid_argument 并保留原值。
实体 Transform 和 offset 与后端的兼容性在物理同步时校验；同步失败保留已有刚体，修正后可以重试。

Static 固定不动；通过 patch/replace Transform 可更新其位置。
Dynamic 的位置、旋转由物理模拟写回；通过 patch Transform 可显式传送并保留速度。
Kinematic 直接读取 Transform 作为每次正时间更新的目标，产生运动速度并参与碰撞。
移除任一组件或销毁实体时，对应刚体立即移除。

运行时策略和生命周期见 [Physics](../../Physics/docs/Physics.md) 与
[Framework](../../Framework/docs/Framework.md)。当前不提供组合形状、网格形状、触发器、约束和碰撞事件。
