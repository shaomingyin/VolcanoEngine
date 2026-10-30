# Physics

Physics 提供 `Physics::System`，读取 World 的标准组件，并在内部转换为 Jolt 对象。
依赖方向为 Physics → World → Base/EnTT，以及 Physics → Jolt；World 不依赖 Physics 或 Jolt。
公共头文件为 `<Volcano/Physics/System.h>`，CMake 目标为 `Physics`。

## 服务与数据

System 借用 `entt::registry&`，禁止复制和移动。registry 必须比 System 活得更久；
同一 registry 同时只能绑定一个 System。

- gravity()/setGravity()：读取、配置重力，默认 (0, -9.81, 0)。
- contains(entity)：查询实体是否已经有对应的物理刚体，不暴露后端句柄。
- synchronize()：处理新增和修改的组件描述，错误通过异常报告。
- update(elapsed)：先同步描述，正时间模拟并写回 Transform；零时间只同步，负时间违反调用前置条件，使用 VOLCANO_ASSERT 检查。

成员包括运行时守卫、registry 引用、重力、碰撞层接口、临时分配器、线程池、Jolt PhysicsSystem、
借用的 BodyInterface，以及实体到 BodyID 的内部映射、待同步实体集合。
Jolt 的 BodyCreationSettings、Shape、BodyID 均不作为 Scene 组件使用。

## 组件转换和同步

实体同时具有 Transform 和 [World::RigidBody](../../World/docs/RigidBody.md) 时创建刚体。
RigidBody 合并形状、形状局部 offset、运动类型、质量、摩擦、弹性、阻尼、重力倍率。
World 构造和 setter 校验组件字段；Physics 校验变换兼容性后构造 Jolt Shape 和 BodyCreationSettings；惯性由形状计算并按总质量缩放。
Static 使用非移动碰撞层，Kinematic/Dynamic 使用移动层。

实体 Transform 定义刚体的世界原点和旋转；RigidBody::offset 定义碰撞形状相对实体的局部平移和旋转。
Physics 校验两者均具有有限位置、单位四元数和单位缩放，使用 Jolt RotatedTranslatedShape 包装原始形状。
形状世界变换为 `实体 Transform × offset`，实体变换不预先乘入 offset。
Jolt 自行计算形状质心，创建刚体和运动学目标使用刚体原点，回写使用 GetPosition/GetRotation，
不使用 GetCenterOfMassPosition，避免把质心误当作实体原点。修改 offset 与修改形状一样触发重建。

构造时同步已有实体。之后添加或 patch/replace 组件只记录待同步实体，避免在 EnTT 回调中创建刚体。
描述变化在 synchronize() 或下一次 update() 时生效，零时间也可同步。
移除组件或销毁实体时立即删除对应刚体，并清理待同步标记。

替换单个刚体时先校验并创建新对象，成功后移除旧对象，保留非静态刚体的线速度、角速度。
变换校验失败或容量不足会抛出异常，旧刚体保持有效；修正描述后可以重试。
同步多个实体时可能部分成功，不保证整个批次的原子性。
构造失败会清理已创建的刚体、回调连接和运行时引用。

## 更新与生命周期

Kinematic 使用 Transform 的目标位置、旋转调用 Jolt 运动学移动；Dynamic 由物理模拟驱动。
模拟后位置和旋转写回 Transform，不触发组件更新回调。
修改已存在的描述或传送静态/动态物体须使用 patch/replace；形状要求单位 scale。

Framework::Base 持有 Scene 及 System，加载和重置先准备候选场景、物理系统、资源再提交。
Playing 顺序为：前置接口 → Scene 游戏逻辑 → Physics 更新 → 后置接口。
Ready/Paused 不调用物理更新；如需同步组件，可显式调用 base.physics().synchronize()。
卸载、重置和析构先释放 System 再销毁 Scene，运行时句柄不会遗留在场景中。

Jolt 运行时完全由 System 的 RAII 守卫管理，并按实例引用计数。
Scene 只保存 VE 数据，构造和销毁 Scene 均不要求 Jolt 已初始化；宿主无需调用 init/shutdown。
多个 System 可独立模拟不同 registry。

## 验证与当前限制

PhysicsCheck 覆盖组件绑定、三种形状碰撞、局部平移/旋转偏移、偏移刚体的原点回写与运动学目标、
非法偏移与修改重试、运动类型、步进、校验、速度保留、
移除和析构、构造失败清理。FrameworkCheck 覆盖状态、暂停、重置和多个实例的生命周期。

保留两类碰撞层、每系统 1024 个刚体上限和宿主提供的可变步长。
替换时需要一个空闲刚体槽位，容量满时替换失败并保留旧对象。
当前不增加固定步长、物理资源格式、网格或组合形状、约束和碰撞事件。
