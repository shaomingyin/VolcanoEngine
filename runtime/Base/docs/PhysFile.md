# PhysFS 与 PhysInputStream / PhysOutputStream

通过 Conan 引入 physfs/3.2.0，Base 公开链接 Conan 的 physfs-static 目标。
文件系统操作直接使用 `<physfs.h>` 中的 PHYSFS_* API，不提供冗余文件系统层。

## 初始化与路径

宿主调用 PHYSFS_init，再通过 PHYSFS_mount 挂载本机目录或归档。
Launcher 的 --root 指定挂载源，默认当前目录；挂载到虚拟根目录。
多个 Framework 实例共享进程级 PhysFS 状态，不再有各自独立的文件系统实例。

资源名称使用 UTF-8、正斜杠和相对虚拟路径，例如 `models/cube.vemodel`。
路径规则、符号链接策略、挂载优先级及读写目录直接遵循 PhysFS。
写入前由调用方通过 PHYSFS_setWriteDir 选择写目录；挂载目录不会自动成为写目录。

宿主须先销毁应用、Framework 和全部流对象，再调用 PHYSFS_deinit。

## 零拷贝流实现

`Volcano::PhysInputStream` 和 `Volcano::PhysOutputStream` 位于 `Volcano/PhysFile.h`。

- `PhysInputStream` 派生自 `google::protobuf::io::ZeroCopyInputStream`，提供 `Next`、`BackUp`、`Skip`、`ByteCount`，并支持 `fileLength()` 和 `readAll()`。
- `PhysOutputStream` 派生自 `google::protobuf::io::ZeroCopyOutputStream`，提供 `Next`、`BackUp`、`ByteCount` 和 `Flush()`。
- `PhysFile` 作为 `PhysInputStream` 的类型别名提供便利：`using PhysFile = PhysInputStream;`。

```cpp
#include <Volcano/PhysFile.h>

// 读取
Volcano::PhysInputStream input("models/cube.vemodel");
// 或配合 Protobuf:
// message.ParseFromZeroCopyStream(&input);

// 写入
Volcano::PhysOutputStream output("models/output.vemodel");
// message.SerializeToZeroCopyStream(&output);
```

- 路径构造接受 `std::filesystem::path`，打开失败抛出包含详细路径与 PhysFS 错误信息的异常。
- 不可复制，支持移动语义（`noexcept`）。析构时自动刷出缓冲并关闭句柄。
- 支持缓冲流式读写，默认缓冲区大小为 64KB。

## 验证

```sh
cmake --build build/Debug --target BasePhysFileCheck -j2
ctest --test-dir build/Debug --output-on-failure -R 'BasePhysFile'
```
