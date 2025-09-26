
cmake_minimum_required(VERSION 3.31)

find_package(Catch2 3 REQUIRED)

set(gfx-lib-tests-src
        bgfx/BGFXStateTests.cpp
)

set(gfx-lib-tests-modules
)

add_executable(gfx-lib-tests ${gfx-lib-tests-src})

target_sources(gfx-lib-tests PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${gfx-lib-tests-modules})
target_compile_features(gfx-lib-tests PUBLIC cxx_std_23)
target_link_libraries(gfx-lib-tests PRIVATE gfx-lib Catch2::Catch2WithMain)

#add_custom_target(gfx-lib-tests-assets
#        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
#        "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
#        "$<TARGET_FILE_DIR:gfx-lib-tests>/assets/"
#        COMMENT "Copying assets"
#)

#add_dependencies(gfx-lib-tests gfx-lib-tests-assets)

include(CTest)
include(Catch)
catch_discover_tests(gfx-lib-tests)
