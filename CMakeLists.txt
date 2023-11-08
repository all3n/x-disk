cmake_minimum_required (VERSION 3.21)
set(PROJECT x-disk)
project (x-disk
  VERSION 0.0.1
  DESCRIPTION "x-disk"
  HOMEPAGE_URL https://github.com/all3n/x-disk
  LANGUAGES C
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_STANDARD 11)


if(APPLE)
  include_directories(/opt/homebrew/include)
  link_directories(/opt/homebrew/lib)
endif()

include(cmake/thirdparty.cmake)
# 根据构建类型设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
endif()

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*tests.c")
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

if (DEBUG)
  get_cmake_property(_variableNames VARIABLES)
  list (SORT _variableNames)
  foreach (_variableName ${_variableNames})
      message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endif()

include_directories(src)
add_executable(${PROJECT} ${SOURCES})
target_include_directories(${PROJECT} PRIVATE ${caph_SOURCE_DIR}/src)
target_link_libraries(${PROJECT} PRIVATE json-c curl readline caph)


# install(FILES ${HEADER_FILES} DESTINATION include)
install(TARGETS ${PROJECT}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
