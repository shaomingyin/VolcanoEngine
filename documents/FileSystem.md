# FileSystem

`Volcano::FileSystem` 是抽象基类，统一路径校验、读写接口和目录遍历。
`NativeFileSystem` 负责本机存储；`VirtualFileSystem` 负责组合多个文件系统，
只有它提供 `mount` / `unmount`。未来的 `ZipFileSystem` 同样直接继承 `FileSystem`。
这些类型只依赖 C++ 标准库，没有 Backend 层、全局实例或 `init` / `shutdown`。
公共头文件均位于 `Volcano/` 下。

## 使用

直接访问本机目录：

```cpp
#include <Volcano/NativeFileSystem.h>

Volcano::NativeFileSystem disk("./data"); // 根目录必须已存在
const auto bytes = disk.readFile("/config.json");
auto file = disk.openFileForRead("/config.json");
file->seek(0);
auto copy = file->read(); // 从文件开头完整读取
```

组合多个文件系统：

```cpp
#include <Volcano/VirtualFileSystem.h>
#include <Volcano/NativeFileSystem.h>

Volcano::VirtualFileSystem filesystem;
auto assets = std::make_shared<Volcano::NativeFileSystem>("./assets");
filesystem.mount("/assets", assets);
filesystem.mount("/save", std::filesystem::path("./save"));

for (const Volcano::DirEntry &entry : filesystem.listDirectory("/assets", true)) {
    if (entry.isFile()) {
        const auto &path = entry.path(); // 例如 /assets/config.json
        auto length = entry.size();
    }
}

filesystem.createDirectory("/save/slots");
filesystem.writeFile("/save/slots/one.bin", Volcano::ByteArray{1, 2, 3});
```

具体文件系统构造后即可使用。实例不可复制或移动；只有 VirtualFileSystem 持有挂载表和挂载同步锁。
文件句柄自行持有所需存储，卸载或销毁文件系统不影响已打开文件；Native 文件持有自己的本机文件流。

## 文件与条目

- `File` 继承 `Stream`，是可定位的抽象文件接口，由 `unique_ptr<File>` 管理。提供 `read`、`write`、
  `seek`、`tell`、`size`、`flush`。字节读写使用 `span`，也提供指针和长度重载。
- `read(span)` 返回实际读取量，允许短读，零表示 EOF；错误抛异常。
  `read()` 从开头完整读取并处理短读，提前 EOF 抛异常。
- Native 文件分别以只读、覆盖写入、追加写入模式打开。`seek` 是绝对位置，允许 EOF，
  禁止超过 EOF；追加模式即使定位也始终在末尾写入。
- `write` 返回实际写入量，允许短写；需要完整写入时使用 `writeAll`。
  直接使用文件句柄写入后，应调用 `flush` 检查缓冲写入错误；
  `writeFile` 和 `appendFile` 已使用 `writeAll` 并执行 `flush`。覆盖写入会截断原文件，不保证原子替换。
- `DirEntry` 是元数据快照，提供 `path`、`type`、`exists`、`isFile`、`isDirectory`、
  `size`。只有普通文件可以查询大小。状态变化后需要重新查询。
- `stat` 对不存在的路径返回 `FileType::Missing`；枚举返回 `vector<DirEntry>`，
  回调形式接收 `const DirEntry &`。条目路径是相对于当前实例根目录的完整虚拟路径，以 `/` 开头。

## Stream

`Volcano::Stream` 是同步字节流基类，位于 `Volcano/Stream.h`，不依赖文件路径或文件系统。
未来的内存流、解压流等可直接继承它。序列化格式和字节序转换由上层负责。

- `isReadable` / `isWritable` 查询当前句柄的读写能力，派生类必须实现。
- `isSequential` 默认为 `true`；不支持的 `seek`、`tell`、`size` 默认抛异常。
  `File` 返回 `false` 并要求派生类实现这三个操作，`seek` 使用绝对位置。
- `read(span)` 和 `write(span)` 返回实际传输量，允许短读和短写。
  非空读缓冲区返回零表示 EOF；非阻塞的“暂时无数据”不属于此接口的约定。
  空缓冲区返回零，不表示 EOF；指针重载拒绝非零长度的空指针。
- `readExact` 循环读取直到缓冲区填满，提前 EOF 抛异常。
  `writeAll` 循环写完，写入无进展时抛异常。辅助方法也拒绝超过缓冲区长度的返回值。
- `readAll` 从当前位置读到 EOF，不回到开头，也不依赖 `size`，可用于顺序流。
  `File::read()` 保留从开头读取整个文件、按文件大小校验提前 EOF 的行为。
- `flush` 默认不做操作；有缓冲的实现应覆盖它。刷新不保证数据已持久化到磁盘。
- 不支持的非空读写操作默认抛异常。I/O 异常可能发生在部分数据已经传输之后，不保证回滚。
  同一个流的并发访问需要调用方同步；资源由派生实现通过 RAII 管理。

实现派生类时，若覆盖 `read` 或 `write`，可用 `using Stream::read` / `using Stream::write`
保留指针重载的可见性；文件派生类用 `using File::read` 同时保留整文件读取重载。

## 挂载与写入

`VirtualFileSystem::mount(虚拟路径, shared_ptr<FileSystem>)` 挂载一个实例，
本机路径重载会创建 `NativeFileSystem`。Native 和 ZIP 实现本身不提供挂载操作。
也可以挂载其它 VirtualFileSystem，直接或间接的循环挂载会被拒绝。

路由先选择路径分量匹配的最深挂载点，不会将 `/assets2` 当作 `/assets` 的后代。
同一挂载点的多个层按优先级查找：默认新挂载排在前面，第三个参数 `true` 将其放在最后。
该点各层都不存在的条目返回 Missing，不回退到更浅挂载点的存储。
目录子项合并、去重并排序；同名文件遮挡低优先级目录及其后代。
更深挂载点及其父目录会出现在虚拟树中，并优先于较浅挂载提供的同名条目。
`unmount(虚拟路径)` 删除该点的所有层，不存在时抛异常。

读取和修改使用同一挂载树，没有独立写入目录。已有条目交给其可见来源修改；
新条目交给所选挂载点的最高优先级层。目标只读时直接抛异常，
不跳过它寻找其它可写层，也不复制条目到其它层。
删除上层条目后，下层同名条目会重新可见。

`isWritable(虚拟路径)` 查询该路径的目标存储是否支持修改。
默认派生实现只读，Native 实现支持写入，Virtual 实现根据路由结果查询目标。
VFS 的根目录、挂载点及其祖先没有可修改目标；本机文件系统权限仍可能使实际操作失败。
公开修改方法在调用修改钩子前统一执行可写性检查，目标文件系统还会再次检查。

`createDirectory` 创建缺失的父目录；文件写入不自动创建父目录。
`remove` 只删除文件或空目录，不递归删除。所有文件系统的根目录均不能删除或覆盖。
递归枚举采用深度优先遍历，回调返回 `false` 停止整个遍历。
Virtual 实现的每次底层查询使用挂载表快照，递归遍历整体不冻结挂载表和磁盘状态。
不跨 I/O 或用户回调持有挂载锁；同一文件句柄的并发访问需要调用方同步。

## 路径规则

VFS 没有当前工作目录（CWD），公开方法只接受以 `/` 开头的虚拟绝对路径；空路径和相对路径均拒绝。
路径先按虚拟目录结构进行词法规范化，再选择挂载目标：忽略 `.` 和重复分隔符，
允许根目录范围内的 `..`，例如 `/assets/../save/file` 等价于 `/save/file`，`/assets/..` 表示 `/`。
若任一步试图越过 VFS 根目录（例如 `/../file` 或 `/assets/../../file`），则抛异常。
被 `..` 抵消的分量无需实际存在，枚举和元数据中的路径均为规范化后的完整虚拟路径。
仍拒绝驱动器名、反斜杠、冒号和嵌入的空字节。Native 根目录在构造时规范化；
根目录内的符号链接不允许访问，也不会被枚举。
路径检查与打开文件存在竞争窗口，不能防御其它进程同时恶意替换挂载目录，也不提供 OS 级隔离。
本机路径使用 `std::filesystem::path`，`FileSystem::toNativeString` 返回 UTF-8。
Native 构造参数和 `mount` 的本机路径参数属于宿主机配置，仍按宿主机规则解释，不属于 VFS 路径。

## 派生文件系统

未来的 `ZipFileSystem` 可以直接继承 `FileSystem`，实现以下受保护虚函数：

- `statImpl`：返回相对路径的元数据，不存在时返回 `Missing`，根目录必须存在且为目录。
- `listDirectoryImpl`：返回直接子项，每个条目的路径是单独的子项名称，不能重复或包含父目录。
- `openFileForReadImpl`：返回自己的 `File` 派生实现。

这些接口收到的都是已经校验、规范化的相对路径，空路径表示根目录。
基类公开方法统一处理路径校验、元数据路径和递归遍历；挂载路由和目录合并由 Virtual 实现负责。
只读派生类无需实现修改钩子。支持写入时，覆盖 `isWritableImpl(相对路径)` 返回 `true`，
并实现 `openFileForWriteImpl`、`removeImpl`、`createDirectoryImpl`。

文件实现应持有其需要的存储，例如 ZIP 条目文件持有共享的归档数据，保证直接打开的文件也能独立存活。
文件句柄不依赖所属文件系统实例存活，也没有额外的 MountedFile 包装。
不同文件句柄应可独立使用，派生类自行同步共享可变存储。
ZIP 读取和解压尚未实现。

## 自检

```sh
cmake --build build/Debug --target BaseFileSystemCheck BaseStreamCheck
build/Debug/bin/BaseFileSystemCheck
build/Debug/bin/BaseStreamCheck
```

自检使用独立临时目录，覆盖 Native 直接读写、实例隔离、最深挂载点优先、挂载边界、组合及循环挂载、二进制和 Unicode 路径、
定位与 EOF、覆盖和追加、目录合并、递归及停止、非法路径、符号链接、挂载写入和文件生命周期。
内存派生文件系统验证只读检查先于修改钩子执行、只读层不回退、按路径查询可写性、短读和存储生命周期，
不需要 ZIP。测试使用 Conan 引入的 GoogleTest；Stream 用例覆盖顺序读取、短读短写、EOF、
空缓冲区、非法返回值和 I/O 异常，文件系统用例还验证短写后的完整写入和刷新错误传播。
