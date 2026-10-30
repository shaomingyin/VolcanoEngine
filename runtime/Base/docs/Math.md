# 数学类型与扩展

独立的 `runtime/Math` 模块及 CMake `Math` 目标已删除。Base 通过 Conan 的 `eigen/5.0.1`
公开依赖 `Eigen3::Eigen`；使用方只需链接 Base，World 和 Graphics 已通过 Base 获得 Eigen。
向量、矩阵、四元数和仿射变换直接使用 Eigen；Base 的 Transform 只保存变换分量。

| 用途 | 头文件 | 类型或接口 |
| --- | --- | --- |
| 向量、矩阵 | `Eigen/Core` | `Eigen::Vector3f`、`Eigen::Matrix4f` 等 |
| 四元数、仿射变换、包围盒 | `Eigen/Geometry` | `Eigen::Quaternionf`、`Eigen::Affine3f`、`Eigen::AlignedBox3f` 等 |
| 角度转换 | `Volcano/Math.h` | `Volcano::Eigen::radians()`、`degrees()` |
| 颜色 | `Volcano/Color.h` | `Volcano::Color`、`Colorf`，详见 [Color](Color.md) |
| 尺寸 | `Volcano/Size.h` | `Volcano::BasicSize<T>`、`Size`、`Sizef` |
| 矩形 | `Volcano/Rect.h` | `Volcano::BasicRect<T>`、`Rect`、`Rectf` |
| 变换分量 | `Volcano/Transform.h` | `Volcano::Transform` |

Eigen 5.0.1 不提供度与弧度的转换函数，`radians()` 和 `degrees()` 因此保留在 `Volcano/Math.h`。

数学扩展定义在 `Volcano::Eigen` 命名空间中，并引入第三方 Eigen 的名字，使 Volcano 内部可以直接使用 `Eigen::` 访问类型和扩展。
Color、Size、Rect、Transform 直接定义在 `Volcano` 下，分别保存在 Base 的独立头文件中。
Eigen 类型统一通过 `Eigen::` 使用，不加全局限定符；位置等向量接口直接使用 Eigen。
`BasicRect` 保存 left、top、width、height，支持负尺寸，并以左闭右开的边界进行包含与相交判断。
`Rect` 为整数矩形，`Rectf` 为浮点矩形。涉及 Eigen 向量的接口不再支持常量求值。

## 尺寸与矩形

`BasicSize<T>` 按值保存公开的 width、height，默认都为零；`Size` 使用 int，`Sizef` 使用 float。
提供 `empty()`（任一维为零）、`null()`（两维都为零）、`transposed()`（交换宽高）和相等比较，支持 constexpr。
与 Rect 一样保留负尺寸，不自动截断或取绝对值。

`Rect::size()` 返回对应的 `BasicSize<T>` 值，`setSize()` 更新宽高并保持 left、top 不变，两个接口都支持 constexpr。
修改 size() 返回的副本不会影响矩形，须通过 setSize() 写回。原来的 Eigen 向量尺寸接口改为 Size；位置接口仍使用 Eigen。

```cpp
#include <Volcano/Rect.h> // 同时提供 Size

Volcano::Size size{640, 480};
Volcano::Rect rect{10, 20, 0, 0};
rect.setSize(size);
auto dimensions = rect.size();
dimensions.width = 800;
rect.setSize(dimensions); // left、top 仍为 10、20
```

## 迁移要点

- 将旧的 `Volcano/Math/*.h` 替换为上表中的对应头文件。
- Eigen 分量通过 `x()`、`y()`、`z()`、`w()` 访问。
- Eigen 默认构造的向量、矩阵、四元数和仿射变换不初始化数值；按需要显式使用 `Zero()`、`Ones()` 或 `Identity()`。
- Eigen 矩阵以 `(row, column)` 访问元素，默认按列存储；向 OpenGL 上传时使用 `data()`。
- 轴角旋转使用 `Eigen::AngleAxisf`，旋转角度以弧度表示。
- 投影矩阵变换点使用齐次坐标，并通过 `hnormalized()` 完成透视除法。
- 包围盒直接使用 `Eigen::AlignedBox3f`：使用 `min()`、`max()`、`sizes()`、`isEmpty()` 和 `setEmpty()`。
  `sizes()` 返回完整边长；空盒的中心和尺寸没有有效几何含义，不应查询。Eigen 不校验坐标有限性，调用方须校验输入。

```cpp
#include <Eigen/Geometry>
#include <Volcano/Math.h>

Eigen::Affine3f model = Eigen::Affine3f::Identity();
model.translate(Eigen::Vector3f(1, 2, 3))
     .rotate(Eigen::AngleAxisf(Volcano::Eigen::radians(45.0f), Eigen::Vector3f::UnitY()))
     .scale(Eigen::Vector3f(2, 1, 1));
const Eigen::Vector3f position = model * Eigen::Vector3f::Zero();
```

## Transform

`Volcano::Transform` 位于 Base，只按值保存 `Eigen::Vector3f` 平移、`Eigen::Vector3f` 缩放和
`Eigen::Quaternionf` 旋转，不保存或缓存矩阵。默认值分别为零、全一和单位四元数；
构造函数参数顺序为 translation、scale、rotation。
`translation()`、`scale()` 和 `rotation()` 提供只读引用；
通过 `setTranslation()`、`setScale()` 和 `setRotation()` 修改值，`reset()` 恢复单位变换。

`affine()` 按需生成独立的 `Eigen::Affine3f`，修改分量不会影响此前生成的结果。
保留从 World 迁移过来的 `S * R * T` 顺序：对于列向量，点先平移、再旋转、最后缩放，
因此平移分量也受旋转和缩放影响。允许负缩放和零缩放，零缩放可能产生不可逆变换。
`affine()` 在生成结果包含 NaN/Inf 时抛出 `std::invalid_argument`，包括有限输入计算溢出的情况。
`Volcano::Eigen::normalMatrix(const Eigen::Affine3f&)` 提供矩阵形式的法线变换计算，`Transform::normalMatrix()` 复用该函数。
`normalMatrix()` 返回线性部分的逆转置 `Eigen::Matrix3f`，可用于非均匀缩放下的法线变换；
遇到奇异变换或非有限结果时抛出 `std::invalid_argument`。零缩放仍允许生成 affine，但不能生成法线矩阵。
调用方须提供单位四元数，类不自动归一化旋转。

```cpp
#include <Volcano/Transform.h>

Volcano::Transform transform;
transform.setTranslation({1, 2, 3});
transform.setScale({2, 2, 2});
const Eigen::Affine3f affine = transform.affine();
```

World 的相机、光源、模型和物理组件统一使用 Base 的 Transform，头文件改为 `Volcano/Transform.h`，
类型名改为 `Volcano::Transform`；在 Volcano 子命名空间内直接使用 `Transform`。
需要矩阵时通过 `affine().matrix()` 获取。Graphics 的默认模型、投影和视图仍为单位变换。
相机继续提供 OpenGL 投影矩阵，透视相机的 fov 输入为度，保留非法参数检查。

## 验证

```sh
cmake --build build/Debug --target BaseMathCheck BaseColorCheck BaseTransformCheck WorldMathCheck -j2
ctest --test-dir build/Debug --output-on-failure -R 'BaseMath|MathColor|BaseTransform|WorldMath'
```

BaseMathCheck 验证尺寸值、零及负尺寸、矩形尺寸读写、常量求值、矩形操作与角度转换；BaseColorCheck 验证颜色转换、算术及错误处理。
BaseTransformCheck 验证默认值、分量读写、变换顺序、独立生成的仿射结果、复制与重置以及零和负缩放、法线矩阵与无效结果处理。
PhysicsCheck 验证物理模拟中的 Eigen 位置、旋转转换；WorldMathCheck 验证 Base Transform 的使用、网格包围盒与相机投影。
