#
# SDL_eda : CMakeLists.txt
#

cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# -----------------------------------------------------------------------------
# Project basic setting
# -----------------------------------------------------------------------------
project(SDL_ext)
set(serial "0.0.0")
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")

# -----------------------------------------------------------------------------
# App information setting
# -----------------------------------------------------------------------------
if (CMAKE_GENERATOR MATCHES "MinGW"
    OR CMAKE_GENERATOR MATCHES "MSYS"
    OR (WIN32 AND CMAKE_GENERATOR MATCHES "Unix"))
elseif (APPLE)
  set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME}")
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.s6jr.${PROJECT_NAME}")
  set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version ${serial}")
  set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${serial})
  set(MACOSX_BUNDLE_BUNDLE_VERSION ${serial})
  set(MACOSX_BUNDLE_COPYRIGHT "Copyright TLC. All Rights Reserved.")
elseif (UNIX)
endif ()

# -----------------------------------------------------------------------------
# Compiler setting
# -----------------------------------------------------------------------------
if (CMAKE_GENERATOR MATCHES "MinGW"
    OR CMAKE_GENERATOR MATCHES "MSYS"
    OR (WIN32 AND CMAKE_GENERATOR MATCHES "Unix"))
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)
elseif (APPLE)
elseif (UNIX)
endif ()

# -----------------------------------------------------------------------------
# Resource files setting
# -----------------------------------------------------------------------------
file(GLOB examples ${PROJECT_SOURCE_DIR}/example/*.c ${PROJECT_SOURCE_DIR}/example/*.cpp)
file(GLOB resource ${PROJECT_SOURCE_DIR}/resource/*)
file(GLOB source ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB headers ${PROJECT_SOURCE_DIR}/hdr/SDL2/*.h)
list(FILTER resource EXCLUDE REGEX .git)
set_property(TARGET PROPERTY PUBLIC_HEADER ${headers})

# -----------------------------------------------------------------------------
# Dependent CMake Projects & existing libraries setting
# -----------------------------------------------------------------------------
set(DEPEND_LIBS SDL2 SDL2_ttf)

# below syntax order should be followed...
# refer directories, add libraries or executables and target link libraries

if (CMAKE_GENERATOR MATCHES "MinGW"
    OR CMAKE_GENERATOR MATCHES "MSYS"
    OR (WIN32 AND CMAKE_GENERATOR MATCHES "Unix"))
  set(CMAKE_C_COMPILER clang)
  include_directories(/mingw64/include)
  link_directories(/mingw64/lib)
elseif (APPLE)
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
elseif(UNIX)
endif ()
include_directories(${PROJECT_SOURCE_DIR}/hdr/SDL2/)

add_library(${PROJECT_NAME} STATIC ${source} ${headers})

# -----------------------------------------------------------------------------
# add executable and link libraries
# -----------------------------------------------------------------------------
foreach(example IN LISTS examples)
  if (IS_ABSOLUTE "${example}")
    file(RELATIVE_PATH example_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${example}")
  else()
    set(example_rel "${example}")
  endif()
    get_filename_component(example_name "${example_rel}" NAME_WE)
    add_executable(${example_name} WIN32 MACOSX_BUNDLE ${header} ${example_rel} ${resource} ${Frameworks})
    add_dependencies(${example_name} ${PROJECT_NAME})
    target_link_libraries(${example_name} ${DEPEND_LIBS} ${PROJECT_NAME})

  if (CMAKE_GENERATOR MATCHES "MinGW"
      OR CMAKE_GENERATOR MATCHES "MSYS"
      OR (WIN32 AND CMAKE_GENERATOR MATCHES "Unix"))
  elseif (APPLE)
    set(INFO_PLIST ${PROJECT_SOURCE_DIR}/macos/TemplateInfo.plist)
    set_target_properties(${example_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${INFO_PLIST})
    foreach (resrc IN LISTS resource)
      set_source_files_properties(${resrc} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    endforeach ()
  elseif (UNIX)
  endif ()
endforeach()

install ( TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install (DIRECTORY hdr/SDL2 DESTINATION include FILES_MATCHING PATTERN "*.h")

