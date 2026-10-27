# PhysFS 与 PhysFile

通过 Conan 引入 physfs/3.2.0，Base 公开链接 Conan 的 physfs-static 目标。
文件系统操作直接使用 `<physfs.h>` 中的 PHYSFS_* API，不提供自有文件系统层。

## 初始化与路径

宿主调用 PHYSFS_init，再通过 PHYSFS_mount 挂载本机目录或归档。
Launcher 的 --root 指定挂载源，默认当前目录；挂载到虚拟根目录。
多个 Framework 实例共享进程级 PhysFS 状态，不再有各自独立的文件系统实例。

资源名称使用 UTF-8、正斜杠和相对虚拟路径，例如 models/cube.vemodel。
路径规则、符号链接策略、挂载优先级及读写目录直接遵循 PhysFS。
写入前由调用方通过 PHYSFS_setWriteDir 选择写目录；挂载目录不会自动成为写目录。

宿主须先销毁应用、Framework 和全部 PhysFile，再调用 PHYSFS_deinit。
Launcher 在正常退出时检查关闭结果，异常路径使用已有 ScopeGuard 清理。

## PhysFile

`Volcano::PhysFile` 位于 `Volcano/PhysFile.h`，继承 Stream，拥有一个 PHYSFS_File*。
Buffer 与 PhysFile 提供相同的字节流接口，分别用于内存和文件。挂载、枚举及元数据仍直接使用 PhysFS。

```cpp
#include <Volcano/PhysFile.h>

// PhysFS 已初始化并挂载资源。
Volcano::PhysFile file("models/cube.vemodel");
const auto bytes = file.readAll();
file.seek(0);
const auto length = file.size();
// 离开作用域时自动关闭。
```

- 路径构造接受 std::filesystem::path，转换为 UTF-8；OpenMode 默认为 ReadOnly，另支持 WriteOnly（创建或截断）和 Append（创建或初始定位到末尾）。失败抛出包含路径的异常。
  拒绝路径中嵌入 NUL，其他路径校验交给 PhysFS。
- 句柄构造接管所有权，也允许空句柄，必须显式传入与实际打开方式一致的 OpenMode。
  PhysFS 无法从句柄查询模式。这种构造不检查打开结果，由调用方检查 isOpen() 和 PhysFS 错误。
- 不可复制，支持移动；移动后源对象为空。移动赋值先关闭目标原有句柄，关闭失败则抛出异常并保留双方所有权。
- get() 借出句柄供直接调用 PhysFS；不得自行关闭该句柄或交给其他所有者。
- close() 可重复调用；失败时抛异常并保留句柄，调用方可处理后重试。
  析构不抛异常、尽力关闭；需要确认缓冲写入成功时应显式调用 close()。

## 字节流接口

| 方法 | 行为 |
| --- | --- |
| isOpen() | 查询当前是否持有句柄 |
| isReadable() / isWritable() | 按是否打开及打开模式查询能力；关闭或移动后的源对象均返回 false |
| isSequential() | 返回 false，支持定位 |
| read(span<uint8_t>) / read(void*, size_t) | 从当前位置读取，返回实际字节数；正常 EOF 返回零，读错误抛异常 |
| write(span<const uint8_t>) / write(const void*, size_t) | 写入并返回实际字节数，失败抛异常 |
| readExact(span<uint8_t>) / writeAll(span<const uint8_t>) | 复用 Stream 的完整读写循环，提前 EOF 或写入无进展时报错 |
| flush() | 调用 PhysFS 刷出缓冲数据，不保证物理介质持久化 |
| readAll() | 从当前位置读取到 EOF，返回 ByteArray，不自动回到文件开头 |
| seek(position) | 定位到绝对字节位置，失败抛异常 |
| tell() / size() | 查询当前位置和文件长度，无法用 size_t 表示时抛异常 |
| eof() | 查询 PhysFS 的 EOF 状态 |

沿用 Stream 约定，空读写返回零，空 readExact/writeAll 无操作；非空操作按打开状态和读写能力检查。
空句柄上的定位、长度、EOF、flush 和非空读写抛出 std::logic_error。
不保存额外的文件游标或长度缓存，直接调用 PhysFS 查询。
readExact、readAll、writeAll 和指针重载直接继承 Stream，不在 PhysFile 中重复实现。
Schema 的读写接口借用 Stream&，可接受 Buffer 或 PhysFile；不负责打开、关闭或挂载。

## 验证

```sh
cmake --build build/Debug --target BasePhysFileCheck BaseStreamCheck BaseBufferCheck -j2
ctest --test-dir build/Debug --output-on-failure -R 'BasePhysFile|BaseStream|BaseBuffer'
```

测试覆盖读取与定位、短读和 EOF、文件不存在、UTF-8 路径、空句柄、异常退出自动关闭、
移动所有权、显式关闭、缓冲写入、追加，以及通过 Stream& 对文件和内存执行相同读写。无显示环境时不执行窗口和像素测试。
