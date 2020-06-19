
find_package(Qt5Core REQUIRED)

add_library(BulletPhysics SHARED)

set_target_properties(BulletPhysics
    PROPERTIES
        OUTPUT_NAME "VolcanoBulletPhysics"
    )

target_compile_definitions(BulletPhysics
    PRIVATE
        VOLCANO_PHYSICS
    )

add_dependencies(BulletPhysics
    Base
    )

target_link_libraries(BulletPhysics
    Base
    Qt5::Core
    )

include(FindBullet)
if(BULLET_FOUND)
    target_include_directories(BulletPhysics PUBLIC ${BULLET_INCLUDE_DIRS})
    target_link_directories(BulletPhysics PUBLIC ${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "BulletPhysics not found.")
endif()

add_subdirectory(include include)
add_subdirectory(src src)
