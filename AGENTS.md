# 开发规范

## 代码

- 使用 C++23、C11，遵循现有代码风格。
- 命名空间统一使用 `Common.h` 中的 `VOLCANO_BEGIN` / `VOLCANO_END`；子模块使用对应的模块宏。
- 公共头文件须能独立包含，避免循环依赖和不必要的包含。
- 覆盖虚函数使用 `override`，不重复添加 `virtual`。
- 优先复用现有实现，保持改动最小，不添加无实际需求的抽象、配置或依赖。
- 保留必要的边界校验和错误处理，明确资源所有权及生命周期。
- 修改接口时同步更新调用方、派生类、测试和文档。
- 不修改与任务无关的代码，不覆盖已有的未提交改动。

## 工程与依赖

- 使用 CMake 构建，新增文件和目标须更新对应的 `CMakeLists.txt`。
- Math 依赖 Base；Base 不得包含或链接 Math，也不得使用 Math 类型。
- 第三方包通过 Conan 引入，在 `conanfile.txt` 中管理。
- CMake 使用 Conan 生成的依赖配置和导入目标，不硬编码本机依赖路径。
- `runtime` 存放运行时模块，`tools` 存放工具，`games` 存放游戏；公共头文件位于模块的 `include/Volcano/` 下。

## 测试与验证

- 测试统一使用 GoogleTest，通过 Conan 的 `[test_requires]` 引入。
- 测试目标由 `BUILD_TESTING` 控制，通过 `gtest_discover_tests` 接入 CTest。
- 非平凡的行为修改须补充相关测试，覆盖正常路径、边界和错误处理。
- 完成修改后编译受影响目标并运行相关测试，如实说明未验证项。
- 模块使用说明维护在 `documents/`，构建和测试命令见 `README.md`。
