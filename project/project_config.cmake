# Rename this file to project_config.cmake for it to take effect

set(project_config_name "minefield") # Replace cpp_prj_model with your project's name
set(project_config_type EXE) # STATIC, SHARED, EXE
set(project_config_cpp_std 20)
set(project_config_use_clang_tidy true)
set(project_config_use_unit_tests true)
set(project_config_use_benchmark true)
set(project_config_recursive_file_gathering true) # When set to false, it'll create sub-projects for nested folders in include/${project_config_name} folders
set(project_config_vs_header_filters_erase_tokens "include;src") # When creating VS filters, it'll remove those from the paths the files belong to
set(project_config_vs_source_filters_erase_tokens "src") # Same as above, now for sources instead of header files
set(project_config_unit_tests_file_tag ".tests") # "Tag" files containing unit tests related stuff are identified with (i.e. MyModule.tests.cpp)
set(project_config_benchmark_file_tag ".bench") # Same as above, but for benchmark files

include(FetchContent)
FetchContent_Declare(
    jngl
    GIT_REPOSITORY https://github.com/jhasse/jngl.git
    GIT_TAG v1.7.0
)
FetchContent_MakeAvailable(jngl)
if(NOT TARGET jngl)
    add_subdirectory(${jngl_SOURCE_DIR} ${jngl_BINARY_DIR})
endif()
include_directories((${jngl_SOURCE_DIR}/src/) (${jngl_SOURCE_DIR}/include/boost-qvm/))

set_target_properties(jngl PROPERTIES FOLDER "_external/jngl/")
set(jngl_dependencies freetype ogg sdl_headers_copy SDL2_test SDL2main SDL2-static sharpyuv uninstall vorbis vorbisenc vorbisfile webp webpdecode webpdecoder webpdemux webpdsp webpdspdecode webpencode webputils webputilsdecode)
foreach(_jngl_dependency IN ITEMS ${jngl_dependencies})
    set_target_properties(${_jngl_dependency} PROPERTIES FOLDER "_external/jngl/deps")
endforeach()

gather_files(data_files true "../data/*.*" "")
foreach(_data_file IN ITEMS ${data_files})
    file(COPY ${_data_file} DESTINATION ${CMAKE_BINARY_DIR}/data)
endforeach()

# set(project_config_<subproject>_type SHARED) # Change a specific subproject to STATIC[default], SHARED, EXE
# set(project_config_<subproject>_link_libraries "example") # Set libraries to be linked for a specific subproject
# set(project_config_<subproject>_dependencies "example") # Set other targets as dependencies for a specific subproject

set(link_libraries jngl)

# set(project_config_extra_sources "someFile.cpp") # Extra sources that need to be compiled as part of the main project

# set(project_config_unit_tests_extra_sources "../src/*.cpp") # Extra sources that need to be compiled as part of a tests project
# set(project_config_unit_tests_extra_libraries "dbghelp") # Extra libraries that need to be linked as part of a tests project

# set(project_config_benchmark_extra_sources "../src/*.cpp") # Extra sources that need to be compiled as part of a benchmark project
# set(project_config_benchmark_extra_libraries "dbghelp") # Extra libraries that need to be linked as part of a benchmark project
