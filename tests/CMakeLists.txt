
cmake_minimum_required(VERSION 3.31)

find_package(Catch2 3 REQUIRED)

add_executable(ortha-gfx-tests)

target_sources(ortha-gfx-tests PUBLIC FILE_SET modules TYPE CXX_MODULES BASE_DIRS .)
target_compile_features(ortha-gfx-tests PUBLIC cxx_std_23)
target_link_libraries(ortha-gfx-tests PRIVATE ortha-gfx Catch2::Catch2WithMain)

#add_custom_target(ortha-gfx-tests-assets
#        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
#        "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
#        "$<TARGET_FILE_DIR:ortha-gfx-tests>/assets/"
#        COMMENT "Copying assets"
#)

#add_dependencies(ortha-gfx-tests ortha-gfx-tests-assets)

add_subdirectory(bgfx)

include(CTest)
include(Catch)
catch_discover_tests(ortha-gfx-tests)
