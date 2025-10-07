
target_sources(ortha-gfx PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
        Font.cppm
        FontDescriptor.cppm
        FontObject.cppm
)

target_sources(ortha-gfx PRIVATE
)

add_subdirectory(services)
add_subdirectory(systems)
