# Schema

Schema 实现 Model、Mesh、Material、Texture 的校验与资源读写。
公共接口仅依赖 Base；实现私有使用 Conan nlohmann_json，不引用 World、Graphics 或 Framework。
每种资源提供 validate()、静态 read(Stream&) 和 write(Stream&) const。

## 流与错误处理

读写均从流当前位置开始，读取消耗当前位置至 EOF 的全部资源数据；不调用 seek/tell/size，支持顺序流。
Schema 借用 Stream&，可以使用 Buffer 或 PhysFile，不负责打开、关闭、flush 或挂载。
write 在写出首字节前校验对象；底层写入失败可能留下部分输出，由调用方处理。
错误格式、版本、字段、长度及资源数据通过 std::invalid_argument 报告；底层流异常直接传播。
不截断、重置或恢复流，也不解析外部模型格式。

## JSON v1

Model 使用 .vemodel，Material 使用 .vematerial，编码为 UTF-8 JSON。
所有以下字段均必须存在，额外字段忽略；version 必须是整数 1，type 必须匹配。
允许尾部空白，拒绝尾部非 JSON 数据。

Model 示例：

```json
{
  "type": "model",
  "version": 1,
  "parts": [
    {
      "mesh": "meshes/cube.vemesh",
      "material": "materials/cube.vematerial",
      "transform": [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]
    }
  ]
}
```

transform 为按行排列的 16 个 float32 可表示的有限数，末行为 [0, 0, 0, 1]。
保存完整仿射矩阵，包含剪切；不拆解为 TRS。parts 可以为空，不包含 ECS、相机、灯光或游戏逻辑。
每个 Part 的 mesh/material 路径均须非空。

Material 示例：

```json
{
  "type": "material",
  "version": 1,
  "albedo": [1, 1, 1, 1],
  "specular": 0.5,
  "metallic": 0,
  "roughness": 1,
  "emissive": [0, 0, 0],
  "albedoTexture": "",
  "normalTexture": "",
  "metallicRoughnessTexture": "",
  "lit": true,
  "alphaMode": "opaque"
}
```

albedo 为 RGBA，各分量及 specular/metallic/roughness 均须有限且位于 [0, 1]。
emissive 为有限非负 RGB，允许大于 1 的 HDR 值。纹理路径可以为空，表示未指定。
alphaMode 仅支持 opaque/blend，lit 必须为 JSON 布尔值；保留完整材质描述，不执行渲染能力映射。

## 二进制 v1

所有整数为 uint32 小端，浮点为 IEEE 754 binary32 小端；不使用 C++ 或 Eigen 结构体的内存布局。
以下头部均为 16 字节。读取时先校验计数乘加溢出及实际载荷长度，再按计数分配资源容器。
拒绝截断、额外尾字节、错误 magic 或版本；资源没有压缩、校验和及可选区段。

| 偏移 | Mesh (.vemesh) | Texture (.vetexture) |
| --- | --- | --- |
| 0..3 | ASCII VEMH | ASCII VETX |
| 4..7 | 版本 1 | 版本 1 |
| 8..11 | 顶点数量 | 宽度 |
| 12..15 | 索引数量 | 高度 |
| 16.. | 顶点数组，随后是索引数组 | RGBA8 像素数组 |

Mesh 每个顶点为连续的 8 个 float32：位置 xyz、法线 xyz、UV uv，共 32 字节。
索引使用 uint32，数量必须为 3 的倍数，每个索引小于顶点数。
顶点属性必须有限；UV 可以超出 [0, 1]，不强制法线单位化或删除退化三角形。
允许空 Mesh，也允许未被索引引用的顶点。

Texture 宽高须为正，载荷恰好 width * height * 4 字节。
格式固定为单层 RGBA8、底行优先、预乘透明度，所有 RGB 字节须不大于对应 alpha。
Schema 不翻转图像、不解码图片、不再次预乘，读写保留像素字节。

当前二进制读取暂存剩余输入后解码，以校验真实长度，内存随实际资源大小增长。

## 路径与集成

资源路径使用 UTF-8 PhysFS 相对路径，例如 models/cube.vemodel；不允许绝对路径、NUL、反斜杠、冒号、点或父目录分量。
Schema 只校验引用的形式，不检查文件是否存在，也不解析依赖或查询挂载表。
Framework 将依赖路径按引用文件目录解析后交给 PhysFS；文件与内存复用同一套序列化实现。
默认材质由资源生产方生成，不在读取时隐式补齐必需字段。

转换器、资源到 GPU 的接入、Graphics 材质映射及 Framework 资源加载不属于本轮实现范围。

## 构建与测试

```sh
cmake --build build/Debug --target SchemaCheck -j2
ctest --test-dir build/Debug --output-on-failure -R '^Schema'
```

测试覆盖四种资源往返、标准字节序和矩阵排列、顺序流短读写、异常传播及格式边界。
