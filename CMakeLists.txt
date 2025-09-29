 
cmake_minimum_required(VERSION 3.31)

find_package(bgfx CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_library(gfx-lib STATIC)

target_sources(gfx-lib PUBLIC FILE_SET modules TYPE CXX_MODULES BASE_DIRS src)
target_compile_features(gfx-lib PUBLIC cxx_std_23)
target_link_libraries(gfx-lib PUBLIC core-lib bgfx::bx bgfx::bimg_decode)
target_include_directories(gfx-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src PRIVATE ${Stb_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(tests)
