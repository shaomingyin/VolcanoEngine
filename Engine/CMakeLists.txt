
macro(link_qt_modules)
    foreach(QT_MODULE ${ARGN})
        find_package(Qt6 COMPONENTS ${QT_MODULE} REQUIRED)
        link_libraries(Qt6::${QT_MODULE})
    endforeach()
endmacro()

macro(target_link_qt_modules TARGET SCOPE)
    foreach(QT_MODULE ${ARGN})
        find_package(Qt6 COMPONENTS ${QT_MODULE} REQUIRED)
        target_link_libraries(${TARGET} ${SCOPE} Qt6::${QT_MODULE})
    endforeach()
endmacro()

find_package(Qt6 COMPONENTS Core REQUIRED)
link_libraries(Qt6::Core)

add_subdirectory(Runtime Runtime)
add_subdirectory(Launcher Launcher)
add_subdirectory(Editor Editor)
