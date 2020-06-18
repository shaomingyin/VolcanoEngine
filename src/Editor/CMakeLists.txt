
set(QtCreatorSource_DIR "" CACHE PATH "qtcreator source directory.")
set(QtCreatorBuild_DIR "" CACHE PATH "qtcreator build directory.")

if(NOT IS_DIRECTORY ${QtCreatorSource_DIR})
    message(FATAL_ERROR "Invalid qtcreator source directory: QtCreatorSource_DIR")
endif()

if(NOT IS_DIRECTORY ${QtCreatorBuild_DIR})
    message(FATAL_ERROR "Invalid qtcreator source directory: QtCreatorBuild_DIR")
endif()

target_sources(Editor
    PRIVATE
        Common.cpp
        Plugin.cpp
        SceneEditor.cpp
        SceneEditorFactory.cpp
        Wizard.json.in
    )

configure_file(
    Wizard.json.in
    Wizard.json
    )

target_compile_definitions(Editor
    PRIVATE
        IDE_LIBRARY_BASENAME=\"lib\"
        QT_CREATOR
        QT_NO_CAST_TO_ASCII
        QT_RESTRICTED_CAST_FROM_ASCII
        QT_DISABLE_DEPRECATED_BEFORE=0x050900
        QT_USE_FAST_OPERATOR_PLUS
        QT_USE_FAST_CONCATENATION
        QT_QML_DEBUG
        QT_PLUGIN
        QT_WIDGETS_LIB
        QT_GUI_LIB
        QT_TESTLIB_LIB
        QT_CONCURRENT_LIB
        QT_CORE_LIB
    )

target_include_directories(Editor
    PRIVATE
        ${QtCreatorBuild_DIR}/src
        ${QtCreatorSource_DIR}/src
        ${QtCreatorSource_DIR}/src/libs
        ${QtCreatorSource_DIR}/tools
        ${QtCreatorSource_DIR}/src/plugins
        ${QtCreatorSource_DIR}/libs
        ${QtCreatorSource_DIR}/src/libs/3rdparty
    )

target_link_directories(Editor
    PRIVATE
        ${QtCreatorBuild_DIR}/lib/qtcreator
        ${QtCreatorBuild_DIR}/lib/qtcreator/plugins
    )

target_link_libraries(Editor
    Core
    Aggregation
    ExtensionSystem
    Utils
    Qt5::Widgets
    Qt5::Gui
    Qt5::Test
    Qt5::Concurrent
    Qt5::Core
    )
