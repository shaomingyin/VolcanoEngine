# 数学类型

数学模块的运算使用标准库实现，依赖 Base 的公共定义，不复制或依赖 Eigen、GLM。
实现位于 `runtime/Math`，命名空间为 `Volcano::Math`；Base 不依赖 Math。
CMake 目标为 `Math`，使用方通过 `target_link_libraries(... Math)` 引入。
统一入口是 `Volcano/Math/Math.h`，也可以单独包含需要的头文件。

| 头文件 | 内容 |
| --- | --- |
| `Volcano/Math/Vector.h` | `Vector<T, N>`，二维、三维和四维向量 |
| `Volcano/Math/Matrix.h` | `Matrix<T, N>`，2×2、3×3 和 4×4 矩阵，变换与投影 |
| `Volcano/Math/Quaternion.h` | `Quaternion<T>`，旋转、插值及矩阵转换 |
| `Volcano/Math/Rect.h` | `BasicRect<T>`，二维矩形，提供整数别名 `Rect` 和浮点别名 `Rectf` |
| `Volcano/Math/BoundingBox.h` | `BoundingBox<T, N>`，二维和三维轴对齐包围盒 |
| `Volcano/Math/Color.h` | `BasicColor<T>`，RGBA 颜色、HSV 转换与颜色运算 |
| `Volcano/Math/Affine.h` | `Affine<T, N>`，二维和三维仿射变换 |

类型别名在文件底部定义，例如 `Vector2f`、`Vector3d`、`Matrix4f`、`Quaternionf`、
`Affine2f`、`Affine3d`。向量还提供 int/unsigned 别名；矩阵、四元数和变换使用浮点类型。
向量分量使用 `.x/.y/.z/.w` 或下标访问；四元数构造顺序为 `(w, x, y, z)`。

## 成员接口

- 向量：`length()`、`normalized()`、`normalize()`、`dot(other)`、`cross(other)`、
  `distance(other)`、`mix(other, t)`。叉积仅适用于三维；长度、归一化、距离及插值仅供浮点向量使用。
- 矩阵：`translate(offset)`、`rotate(angle)`（二维）、`rotate(angle, axis)`（三维）、
  `rotate(quaternion)`、`scale(factors)`、`transposed()`、`determinant()`、`inverse()`、
  `transform(point)`、`transformVector(vector)`、`normalMatrix()`。
  二维和三维变换分别使用 3×3、4×4 矩阵，法线矩阵由 4×4 矩阵返回 3×3 矩阵。
- 矩阵静态工厂：`Matrix4f::lookAt(eye, target, up)`、
  `Matrix4f::perspective(fov, aspect, nearPlane, farPlane)`、
  `Matrix4f::ortho(left, right, bottom, top, nearPlane, farPlane)`。
- 四元数：`normalized()`、`normalize()`、`conjugate()`、`inverse()`、`toMatrix()`、
  `dot(other)`、`slerp(other, t)`；静态工厂 `Quaternionf::from(angle, axis)`、
  `Quaternionf::from(matrix3)`，后者输入应为正交旋转矩阵。
- 仿射变换：`translate()`、`rotate()`、`scale()`、`matrix()`、`data()`、`inverse()`、
  `transform(point)`、`transformVector(vector)`，二维通过 `transform(rect)` 重载处理矩形，
  二维和三维均通过 `transform(box)` 返回变换后各角点的轴对齐包围盒。
- 包围盒：`minimum()`、`maximum()`、`center()`、`extent()`、`empty()`、`reset()`、
  `extend(point/box)`、`contains(point/box)`、`intersects(box)`、`distance(point)`。
  别名为 `BoundingBox2f/2d/3f/3d`；`extent()` 表示半尺寸。
- 矩形：保留 `left/top/width/height` 公共成员和聚合初始化；提供 `right()`、`bottom()`、
  四个角点查询、`position()`、`size()`、`center()`、`empty()`、`null()`。
  `setLeft/Top/Right/Bottom()` 调整指定边并保持对边的位置，`setSize()` 改变尺寸；
  `moveTo()`、`moveCenter()` 和 `translate()` 保持尺寸，`translated()` 返回平移后的副本。
  `adjust()`/`adjusted()` 分别偏移左、上、右、下四条边；`transposed()` 交换宽高。
  `normalize()`/`normalized()` 将负宽高转换为同一区域的非负宽高表示；
  `contains()` 接受坐标、点或矩形，`intersects()` 判断相交，`intersected()`/`united()` 返回交集/外包矩形。
  上述操作支持 `constexpr`，矩形之间可以用 `==` 和 `!=` 比较成员值。
  `contains()` 使用花括号参数时，显式写出 `Vector2<T>` 或 `BasicRect<T>`，避免点和矩形重载歧义。

同一操作通过参数类型重载，例如 `Quaternionf::from(angle, axis)` 和 `Quaternionf::from(matrix3)`、
`transform.transform(Vector2f{1, 2})` 和 `transform.transform(BasicRect<float>{0, 0, 2, 1})`。
二维 `transform()` 的花括号参数可能同时匹配向量和矩形，调用时可显式写出参数类型。
点和方向都使用向量类型，变换语义不同，因此方向变换保留 `transformVector()`。

`normalize()` 原地修改并返回自身引用，`normalized()` 返回副本；失败时不修改原对象。
矩阵和变换的 `translate/rotate/scale`、复合赋值操作原地修改并支持链式调用。
`transposed/inverse/conjugate/toMatrix`、查询和插值均返回值，不修改自身。
自由函数只保留普通算术运算符以及标量角度换算 `radians()`、`degrees()`。

## 约定与错误处理

- 矩阵列主序，使用 `m[column][row]`，`data()` 返回连续元素，可上传至 OpenGL。
- 使用列向量，`projection * view * model * position` 中右侧变换先执行。
- 平移、旋转和缩放均在右侧乘入新矩阵。例如 `.translate(t).rotate(q).scale(s)` 产生 `T * R * S`。
- 角度使用弧度；右手坐标系，相机朝向局部 -Z，OpenGL NDC 深度为 [-1, 1]。
- 默认向量为零，默认矩阵和变换为单位矩阵，默认四元数为单位旋转。
- `Rectf` 和 `Rect` 均使用左上包含、右下不包含的边界，`right() = left + width`，
  `bottom() = top + height`，不采用 Qt `QRect` 的整数右下边界减一规则。
  几何判断支持负宽高；`empty()` 表示宽或高为零，`null()` 表示宽和高都为零。
  仅接触边界的矩形不相交；空矩形不参与包含和相交，合并时忽略空矩形，
  空交集或两个空矩形的合并返回默认矩形。`intersected()` 和 `united()` 返回非负宽高。
  整数矩形的中心向较小坐标取整。角点查询保留原始边的方向，比较不自动归一化。
  坐标应为有限值，坐标、尺寸及运算中间值和结果应在类型的可表示范围内。
- 点变换包含平移和齐次除法；方向变换忽略平移。非均匀缩放下法线使用 `normalMatrix()` 后归一化。
- `Affine` 支持平移、旋转、缩放和剪切；从矩阵构造时要求最后一行精确为 `[0, …, 0, 1]`，
  否则抛出 `std::invalid_argument`。允许不可逆的仿射变换，求逆时仍检查奇异矩阵。
  透视投影和包含透视投影的组合使用 `Matrix`，不构造 `Affine`。
- 零向量归一化、奇异或非有限矩阵求逆、无效四元数和无效齐次除法抛出 `std::domain_error`。
  非法投影参数或非有限旋转角度抛出 `std::invalid_argument`。
- `lookAt` 的眼点与目标点不可重合，up 方向不可为零或与视线平行。
- 包围盒默认空；某一轴的 minimum 大于 maximum 时也是空盒。点盒和扁平盒非空，
  包含和相交测试包含边界，接触也算相交。空盒不参与包含和相交测试，合并时忽略空盒。
  空盒的中心和半尺寸为零，点到空盒的距离为正无穷；非空盒内的点距离为零。
  构造、扩展和距离查询拒绝非有限坐标并抛出 `std::invalid_argument`，`contains()` 对 NaN 返回 false。
  `World::BoundingBox` 是 `BoundingBox3f` 的别名；`Mesh::recalculateBounds()` 对非法顶点抛出异常，保留之前的包围盒。

World 中的 `Volcano::World::Transform` 是保留可编辑平移、旋转和缩放数据的物理组件。
其 `affine()` 返回 `Affine3f`，`matrix()` 返回该仿射变换的矩阵，保留原组件的 `S * R * T` 顺序。
Math 的仿射变换按调用顺序右乘。数学层不再定义 `Transform` 或兼容别名。
World 相机继续使用角度制 FOV，对接数学层时转换成弧度；非法相机参数可从 `toMatrix()` 抛出异常。

```cpp
#include <Volcano/Math/Math.h>

Volcano::Math::Affine3f model;
model.translate({1, 2, 3})
     .rotate(Volcano::Math::radians(45.0f), {0, 1, 0})
     .scale({2, 2, 2});

const auto view = Volcano::Math::Matrix4f::lookAt({0, 0, 5}, {0, 0, 0}, {0, 1, 0});
const auto projection = Volcano::Math::Matrix4f::perspective(Volcano::Math::radians(60.0f),
                                                     16.0f / 9.0f, 0.1f, 100.0f);
const auto mvp = projection * view * model.matrix();
const auto ndc = mvp.transform({0, 0, 0});
const auto normal = (model.matrix().normalMatrix() * Volcano::Math::Vector3f{0, 1, 0}).normalized();
```

## 验证

```sh
cmake --build build/Debug --target MathCheck
build/Debug/bin/MathCheck
cmake --build build/Debug --target WorldMathCheck
build/Debug/bin/WorldMathCheck
cmake --build build/Debug --target MathGeometryCheck
build/Debug/bin/MathGeometryCheck
```

数学测试使用 Conan 引入的 GoogleTest，覆盖 float/double、二维/三维变换、
2/3/4 阶矩阵、四元数、投影边界、原地修改和异常输入。

`WorldMathCheck` 验证物理分量转换、原组件矩阵顺序、相机投影和包围盒，需配置引擎依赖。

当前实现没有显式 SSE/AVX/NEON 路径；编译器可根据优化选项自动向量化，尚未进行 SIMD 性能测量。
