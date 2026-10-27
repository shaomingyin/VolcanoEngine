# Qt Creator Editor 插件

插件源码、元数据及向导模板移植自本仓库 main 的 `86663c4`，位于 tools/Editor。
此次只移入编辑器及必要的构建接线，保留当前 World、Graphics、Framework 和游戏工程结构。
插件直接使用 World/Graphics，不使用 Framework::Local 或 GLFW；视口由 Qt 提供窗口及 OpenGL 上下文。

## 构建

Editor 是通过 add_qtc_plugin 创建的 Qt Creator 插件，不是独立可执行程序。
默认关闭 VOLCANO_EDITOR，因此常规运行时构建不要求安装 Qt Creator SDK。
构建插件需要目标 Qt Creator 的开发 SDK，以及与宿主匹配的 Qt 6（含 OpenGLWidgets）。
Qt Creator 和 Qt 通过 SDK 提供的 CMake 配置及导入目标接入，Conan 继续提供引擎依赖。
不要将面向另一 Qt/Qt Creator 版本构建的插件直接安装到宿主中。

在按 README 准备 Conan 依赖后，配置时追加：

```sh
cmake -S . -B build/Editor \
    -DCMAKE_TOOLCHAIN_FILE=<Conan生成的conan_toolchain.cmake> \
    -DVOLCANO_EDITOR=ON \
    -DQtCreator_DIR=<QtCreator-SDK中QtCreatorConfig.cmake所在目录> \
    -DCMAKE_PREFIX_PATH=<与该QtCreator匹配的Qt安装前缀>
cmake --build build/Editor --target Editor -j2
```

QtCreator SDK 缺失时，启用插件会在配置阶段明确报错，不生成替代的空 Editor 程序。
运行及安装位置遵循所用 Qt Creator SDK 的插件部署规则。

## 当前内容及限制

保留 main 中的插件入口、设置页、Kit Aspect、SDK 管理原型、场景编辑器及向导模板。
这些是原型骨架，尚未完成场景编辑、属性绑定和真实 SDK 发现；已有 SDK 示例数据仍需后续替换。

移入时完成以下适配：

- 构建使用当前工程的目标和辅助规则，静态运行时库在构建插件时启用 PIC。
- 视口通过当前 Qt OpenGL 上下文为 Graphics::Renderer 提供函数解析器，不在 resize 时重建 Renderer。
- Qt 上下文销毁及视口析构前，在有效上下文中释放 GPU 对象。
- 删除插件加载时向所有 Kit 注入测试 CMake 变量的代码。
- 补齐场景向导模板资源路径；场景文档未实现的操作明确返回错误，避免空实现报告保存成功。

场景模板沿用 main 中的占位文件，不代表已经定义或实现新的 JSON Manifest。
当前 Game 的场景构建及逻辑仍直接放在 games/Demo。

当前阶段只要求源码移植和 VE Runtime 的基本适配，不要求编译通过。
本次环境也缺少 Qt Creator 开发 SDK，完整插件编译、宿主加载和交互均未验证。
构建所用 SDK 版本的 API 兼容性留待后续实际验证。
