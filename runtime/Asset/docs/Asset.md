# Asset

Asset 实现 Texture、Mesh、Material、Model、Scene 的资源只读反序列化与数据流式传输。
公共接口依赖 Base 与 Protobuf。
纯元信息资产（Material、Model、Scene）直接采用 `Schema.proto` 生成的 Protobuf 消息类；
带大块二进制载荷的资产（Texture、Mesh）派生自 `google::protobuf::io::ZeroCopyInputStream`，在构造时提取 Protobuf 头部元信息，并根据内部元信息对外提供底层二进制载荷的零拷贝流式访问。运行时纯粹用于消费资产，不包含写资产接口。

## 流与错误处理

- 底层输入流由外部注入（借用 `ZeroCopyInputStream&` 引用），Asset 不管理底层流的生命周期。
- 构造时先从底层流中提取 Protobuf 元信息。对于 `Texture` 与 `Mesh`，提取完成后流指针精确对齐在二进制载荷的首字节。
- `Texture` 与 `Mesh` 实现 `Next`、`BackUp`、`Skip`、`ByteCount`，将流读取范围严格限定在元信息声明的载荷边界内，避免过度消耗或读取溢出。
- 格式错误、版本不匹配或截断通过 C++ 异常（如 `std::invalid_argument` / `std::runtime_error`）报告。

## Schema.proto

位于 `runtime/Asset/Schema.proto`：

- `TextureHeader`：宽度 `width`、高度 `height`。
- `MeshHeader`：顶点数 `vertex_count`、索引数 `index_count`。
- `Material`：反照率 `albedo`、高光 `specular`、金属度 `metallic`、粗糙度 `roughness`、自发光 `emissive`、贴图路径及混合模式 `alpha_mode`。
- `Model`：包含若干 `Part`（`mesh` 路径、`material` 路径与 16 元素仿射变换矩阵 `transform`）。
- `Scene`：场景主清单（`.vescene`），描述 `name`、`description`、`gravity` 及实体列表 `entities`（`name`、`enabled`、`visible`、`Transform`、`ModelRef`、相机、灯光与 `RigidBody` 描述），所有间接资源均使用路径引用。

## 二进制布局（Texture & Mesh）

- 0..3：ASCII Magic（Texture 为 `VETX`，Mesh 为 `VEMH`）。
- 4..7：uint32 小端，Header 序列化长度 `header_size`。
- 8..(8+header_size-1)：Protobuf 序列化 Header 字节。
- (8+header_size)..：大块数据载荷（Texture 为 RGBA8 像素数组；Mesh 为连续顶点数组，紧随 uint32 索引数组）。

## 构建与测试

```sh
cmake --build build/Debug --target AssetCheck -j2
ctest --test-dir build/Debug --output-on-failure -R '^Asset'
```
