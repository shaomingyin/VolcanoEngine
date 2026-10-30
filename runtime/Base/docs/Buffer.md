# Buffer

`Volcano::Buffer` 位于 `Volcano/Buffer.h`，直接继承 Stream，以自己持有的 ByteArray
实现可读写、可定位的内存流。数据访问和打开模式参考 Qt QBuffer，不依赖 Qt。
默认构造创建空数据；传入 ByteArray 时复制或移动数据，不绑定外部存储。

```cpp
#include <Volcano/Buffer.h>

Volcano::Buffer buffer({1, 0, 2});
buffer.open(Volcano::Buffer::ReadWrite);
buffer.seek(1);
buffer.writeAll(Volcano::ByteArray{3, 4});
buffer.seek(0);
const auto bytes = buffer.readAll(); // {1, 3, 4}
buffer.close();                     // 保留数据，位置归零
```

## 打开与关闭

Buffer 默认关闭，先调用 `open(mode)`，用 `isOpen()` 查询状态。
`isReadable()` / `isWritable()` 反映当前模式；关闭时都为 false。
模式可以通过 `|` 组合：

| 模式 | 行为 |
| --- | --- |
| `ReadOnly` | 从头读取，禁止通过 Stream 写入 |
| `WriteOnly` | 从头覆盖写入，保留未覆盖的原数据，不隐式截断 |
| `ReadWrite` | 从头读写，共用一个游标 |
| `Append` | 自动启用写入，打开时定位到末尾，每次写入都使用当前数据末尾 |
| `Truncate` | 自动启用写入，打开时清空原数据 |

例如 `ReadWrite | Append` 可读写并追加，`WriteOnly | Truncate` 清空后写入。
`NotOpen` 和未知模式值不能用于打开，仍通过异常报告。重复打开或在打开期间调用 `setData` 属于调用时序错误，
通过 `VOLCANO_ASSERT` 检查；Release 不执行这些检查。`close()` 可以重复调用，不清空数据。

## 数据与定位

- `data()` 返回内部 ByteArray 的只读引用；`buffer()` 提供可变及只读引用。
  `setData(ByteArray)` 只允许在关闭时替换数据。
- `isSequential()` 为 false。`seek` 使用绝对位置；只读时不能越过 EOF，
  可写时立即扩展数据并将新增字节填零，分配失败抛异常。
- `tell()` 返回当前位置，`size()` 返回当前数据长度；这两个查询关闭时仍可用。
- 读操作在 EOF 返回零；写操作覆盖当前位置并按需增长，保留其后的原数据。
  Append 模式在 seek 后仍追加写入。读写均支持与内部存储重叠的有效 span，
  包括扩容时追加 Buffer 自身的数据。
- 继承 Stream 的指针重载、`readExact`、`writeAll` 和 `readAll`。
  `readAll` 从当前位置开始；`flush` 不做操作。空读写始终返回零，
  非空读写要求已打开并具有对应读写能力，通过 `VOLCANO_ASSERT` 检查。

通过可变 `buffer()` 可以直接修改存储，游标不会自动改变。若数据被缩短到游标之前，
随后读取返回 EOF，普通写入补零到原游标再写入；Append 仍从实际末尾写入。
内部数据的 span、指针和元素引用可能在扩容或替换数据后失效；
调用方须保证传入 span 有效，并为同一 Buffer 的并发访问提供同步。
Buffer 不可复制或移动。

## 验证

```sh
cmake --build build/Debug --target BaseBufferCheck BaseStreamCheck -j2
ctest --test-dir build/Debug --output-on-failure -R 'BaseBuffer|BaseStream'
```

测试覆盖模式与权限、覆盖和追加、截断、短读与 EOF、定位补零、游标共享、
关闭与重新打开、Debug 状态断言、参数失败保留状态、直接修改存储，以及重叠读写和自身追加扩容。
