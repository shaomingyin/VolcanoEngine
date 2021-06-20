
file(GLOB_RECURSE RUNTIME_SOURCES LIST_DIRECTORIES false
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*
    )

add_library(runtime STATIC ${RUNTIME_SOURCES})
set_target_properties(runtime PROPERTIES OUTPUT_NAME "VolcanoRuntime")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RUNTIME_SOURCES})

target_include_directories(runtime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    )

configure_file(
    include/Volcano/Config.hpp.in
    include/Volcano/Config.hpp
    )

find_package(Qt6 COMPONENTS Concurrent Gui OpenGL Qml Quick REQUIRED)
target_link_libraries(runtime PUBLIC Qt6::Concurrent Qt6::Gui Qt6::OpenGL Qt6::Qml Qt6::Quick)

set_target_properties(runtime PROPERTIES
    QT_QML_MODULE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    QT_QML_MODULE_URI "volcano"
    )

qt6_qml_type_registration(runtime)

#include(FindAssimp)
#if(ASSIMP_FOUND)
#    target_include_directories(runtime PUBLIC ${ASSIMP_INCLUDE_DIR})
#    target_link_libraries(runtime PUBLIC ${ASSIMP_LIBRARIES})
#else()
#    message(FATAL_ERROR "Assimp not found.")
#endif()

include(FindBullet)
if(BULLET_FOUND)
    target_include_directories(runtime PUBLIC ${BULLET_INCLUDE_DIRS})
    target_link_libraries(runtime PUBLIC ${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Failed to find BulletPhysics.")
endif()
