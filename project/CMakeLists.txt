cmake_minimum_required(VERSION 3.18)

### cmake includes
include("cmake_utils/include_required_utils.cmake")
include("project_config.cmake")
include("cmake_utils/include_optional_utils.cmake")

### Basic project configuration
project(${project_config_name})
set(ARTIFACT_TYPE ${project_config_type})
set(CMAKE_CXX_STANDARD ${project_config_cpp_std})

### Current project's include paths
get_filename_component(abs_include_dir "../include/" REALPATH)
set(include_dirs ${abs_include_dir})
include_directories(${include_dirs})
###

set(project_folder "")
if (project_config_${PROJECT_NAME}_make_external)
    set(project_folder "_external/${PROJECT_NAME}/")
endif()
set(internals_project_folder ${project_folder}${PROJECT_NAME}.internals)

if (NOT link_targets)
    set(link_targets "")
endif()

if (NOT link_libraries)
    set(link_libraries "")
endif()

if (NOT ${project_config_recursive_file_gathering})
    configure_subprojects(${PROJECT_NAME} link_targets ${project_config_unit_tests_file_tag})
endif()

### Gather files and put them in folders
set(file_excludes ".*\\${project_config_unit_tests_file_tag}.*" ".*\\${project_config_benchmark_file_tag}.*")
fill_platform_excludes(platform_excludes)
list(APPEND file_excludes ${platform_excludes})
gather_files(header_files ${project_config_recursive_file_gathering} "../include/${project_config_name}/*.hpp;../include/${project_config_name}/*.h;../include/${project_config_name}/*.inl;../src/*.hpp;../src/*.h;../src/*.inl" "${file_excludes}")
gather_files(src_files ${project_config_recursive_file_gathering} "../src/${project_config_name}/*.cpp" "${file_excludes}")
list(APPEND src_files "../src/main.cpp")

if (project_config_extra_sources)
    list(APPEND src_files "${project_config_extra_sources}")
endif()

if (${project_config_use_unit_tests})
    set(file_patterns "../src/${project_config_name}/*.tests.cpp")
    set(project_config_unit_tests_extra_sources "../src/${project_config_name}/*.cpp")
    if (project_config_unit_tests_extra_sources)
        list(APPEND file_patterns "${project_config_unit_tests_extra_sources}")
    endif()
    gather_files(test_files true "${file_patterns}" "${platform_excludes}")
    gather_files(test_headers true "../src/*.tests.h" "${platform_excludes}")
endif()

if (${project_config_use_benchmark})
    set(file_patterns "../src/*${project_config_benchmark_file_tag}.cpp")
    if (project_config_benchmark_extra_sources)
        list(APPEND file_patterns "${project_config_benchmark_extra_sources}")
    endif()
    gather_files(benchmark_files true "${file_patterns}" "${platform_excludes}")
    gather_files(benchmark_headers true "../src/*.bench.h" "${platform_excludes}")
endif()
###

if (benchmark_files)
    setup_benchmark(${PROJECT_NAME} "${benchmark_headers}" "${benchmark_files}" "${link_targets}" "${project_config_benchmark_extra_libraries}")
    set_target_properties(${PROJECT_NAME}.benchmark PROPERTIES LINKER_LANGUAGE CXX FOLDER ${internals_project_folder})
endif()

if (test_files)
    setup_gtest(${PROJECT_NAME} "${test_headers}" "${test_files}" "${link_targets}" "${project_config_unit_tests_extra_libraries}")
    set_target_properties(${PROJECT_NAME}.tests PROPERTIES LINKER_LANGUAGE CXX FOLDER ${internals_project_folder})
endif()

### Configure the target itself
if (src_files)
    if (NOT ARTIFACT_TYPE STREQUAL EXE)
        add_library(${PROJECT_NAME} ${ARTIFACT_TYPE} ${src_files} ${header_files})
        if (test_files)
            target_link_libraries(${PROJECT_NAME}.tests ${PROJECT_NAME})
        endif()
        if (benchmark_files)
            target_link_libraries(${PROJECT_NAME}.benchmark ${PROJECT_NAME})
        endif()
    else()
        add_executable(${PROJECT_NAME} ${src_files} ${header_files})
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    endif()
    enable_warnings(${PROJECT_NAME} false)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${link_targets} ${link_libraries})
    set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
    if (${project_config_use_clang_tidy})
        setup_clang_tidy(${PROJECT_NAME})
    endif()
elseif (header_files)
    add_library(${PROJECT_NAME} INTERFACE ${header_files})
    enable_warnings(${PROJECT_NAME} true)
endif()

if (project_config_${PROJECT_NAME}_make_external AND (src_files OR header_files))
    set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX FOLDER ${project_folder})
endif()

setup_visual_studio_filters("${project_config_vs_header_filters_erase_tokens}" header_files)
setup_visual_studio_filters("${project_config_vs_source_filters_erase_tokens}" src_files)