# 颜色

`Volcano/Color.h` 提供 `BasicColor<T>`，支持 `std::uint8_t` 和 `float`，独立于具体窗口和 GUI 库。

```cpp
using Color = BasicColor<std::uint8_t>;
using Colorf = BasicColor<float>;
```

两个类型均保存四个 RGBA 通道，默认是不透明黑色，alpha 为 0 表示完全透明。
`Color` 的通道范围为 `[0, 255]`，默认 alpha 为 255；`Colorf` 的通道通常在 `[0, 1]`，
默认 alpha 为 1，但允许有限的负数和大于 1 的数值，用于颜色运算和 HDR。

```cpp
#include <Volcano/Color.h>

Volcano::Color accent(32, 128, 240, 200);
accent.setAlpha(128);
const Volcano::Colorf floating = accent.toFloat();
const Volcano::Colorf hdr(2.5f, 0.5f, -0.25f, 1.0f);
const Volcano::Color display = hdr.toByte(); // (255, 128, 0, 255)
const auto packed = accent.toRgba();        // 0x2080F080
const auto restored = Volcano::Color::fromRgba(packed);
```

`red/green/blue/alpha()` 返回通道值：`Color` 返回 int，`Colorf` 返回 float。
通过对应的 `setRed/Green/Blue/Alpha()` 修改通道。整数输入必须在 `[0, 255]`；
浮点输入必须有限。非法输入抛出 `std::out_of_range`，失败的单通道修改保留原值。

`redF/greenF/blueF/alphaF()` 和 `toVector()` 返回浮点通道，8 位通道除以 255，
浮点通道原样返回。`fromRgbF()` 和 `fromVector()` 在 8 位版本中要求输入位于 `[0, 1]`，
按最近整数转换；浮点版本原样保存有限输入。

`toFloat()` 和 `toByte()` 显式转换类型，不提供隐式的字节/浮点颜色转换。
`toByte()` 将浮点通道钳制到 `[0, 1]` 后量化；NaN 无法量化并抛出 `std::out_of_range`。
浮点算术遵循通常的浮点规则，不做饱和处理，极端运算可能产生无穷或 NaN。

`toRgba()`/`fromRgba()` 的整数布局固定为 `0xRRGGBBAA`，不受主机字节序影响。
浮点颜色打包时先调用 `toByte()`；解包得到的浮点颜色位于 `[0, 1]`。
Qt `QColor::rgba()` 使用的 `0xAARRGGBB` 需要先转换。

`fromHsvF(h, s, v, a)` 的 hue 和 saturation 在 `[0, 1]`，hue 的 1 等同于 0。
8 位版本的 value 和 alpha 也在 `[0, 1]`；浮点版本的 value 可以是任意有限非负值，
alpha 可以是任意有限值。`toHsvF()` 返回 `(h, s, v)`，alpha 单独通过 `alphaF()` 获取；
灰色和黑色的 hue 定义为 0。浮点 RGB 转 HSV 时拒绝负 RGB 和非有限 RGB。
HSV 转换基于存储的 RGB 通道，8 位版本返回的 RGB 会量化。

常量为 `Black/White/Red/Green/Blue/Yellow/Magenta/Cyan/Transparent`，
分别通过 `Color::Red` 或 `Colorf::Red` 等名称访问。
`==` 和 `!=` 比较同一类型颜色的四个通道。

| 运算 | Color | Colorf |
| --- | --- | --- |
| `+`、`-` | 逐通道相加减，饱和到 `[0, 255]` | 逐通道相加减，保留浮点结果 |
| `*` | `left * right / 255`，向下取整 | `left * right` |

三种运算都包含 alpha，并提供 `+=`、`-=`、`*=`；这些运算不执行透明度合成。
不涉及 Eigen 向量的接口支持 `constexpr`。浮点转换只缩放数值，不执行 sRGB/线性空间转换。

```sh
cmake --build build/Debug --target BaseColorCheck
build/Debug/bin/BaseColorCheck
```
