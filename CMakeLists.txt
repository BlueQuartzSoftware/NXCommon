cmake_minimum_required(VERSION 3.19)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/Utility.cmake)

option(NXCOMMON_BUILD_PYTHON "Enable building Python building" OFF)
enable_vcpkg_manifest_feature(TEST_VAR NXCOMMON_BUILD_PYTHON FEATURE "python")

option(NXCOMMON_BUILD_TESTS "Enable building NXCOMMON tests" ON)
enable_vcpkg_manifest_feature(TEST_VAR NXCOMMON_BUILD_TESTS FEATURE "tests")

option(NXCOMMON_ENABLE_MULTICORE "Enable multicore support" ON)
enable_vcpkg_manifest_feature(TEST_VAR NXCOMMON_ENABLE_MULTICORE FEATURE "parallel")

project(NXCommon
  VERSION 0.1.1
  DESCRIPTION "Common types for use in NX projects"
  HOMEPAGE_URL "https://github.com/BlueQuartzSoftware/NXCommon"
  LANGUAGES CXX
)

set(NXCOMMON_BIN_DIR ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${NXCOMMON_BIN_DIR} CACHE PATH "Single Directory for all Libraries")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${NXCOMMON_BIN_DIR} CACHE PATH "Single Directory for all Executables.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${NXCOMMON_BIN_DIR} CACHE PATH "Single Directory for all static libraries.")

list(APPEND CMAKE_MODULE_PATH ${NXCommon_SOURCE_DIR}/cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(expected-lite CONFIG REQUIRED)
find_package(span-lite CONFIG REQUIRED)

if(NXCOMMON_ENABLE_MULTICORE)
  find_package(TBB CONFIG REQUIRED)
endif()

add_library(NXCommon SHARED)
add_library(NX::Common ALIAS NXCommon)

target_compile_features(NXCommon
  PUBLIC
  cxx_std_17
)

set_target_properties(NXCommon
  PROPERTIES
  DEBUG_POSTFIX "_d"
)

NXCommon_enable_warnings(TARGET NXCommon)

if(MSVC)
  target_compile_options(NXCommon
    PRIVATE
    /MP
  )
endif()

# Force HDF5 1.10 API
target_compile_definitions(NXCommon PUBLIC "H5_USE_110_API")

if(NXCOMMON_ENABLE_MULTICORE)
  target_compile_definitions(NXCommon PUBLIC "NXCOMMON_ENABLE_MULTICORE")
  target_link_libraries(NXCommon PUBLIC TBB::tbb)
endif()

target_link_libraries(NXCommon
  PUBLIC
  fmt::fmt
  nlohmann_json::nlohmann_json
  nonstd::expected-lite
  nonstd::span-lite
)

if(UNIX)
  target_link_libraries(NXCommon
    PRIVATE
    ${CMAKE_DL_LIBS}
  )
endif()

option(NXCOMMON_ENABLE_LINK_FILESYSTEM "Enables linking to a C++ filesystem library" OFF)

if(NXCOMMON_ENABLE_LINK_FILESYSTEM)
  set(NXCOMMON_FILESYSTEM_LIB "stdc++fs" CACHE STRING "C++ filesystem library to link to")
  set_property(CACHE NXCOMMON_FILESYSTEM_LIB PROPERTY STRINGS "stdc++fs" "c++fs")
  target_link_libraries(NXCommon
    PUBLIC
    ${NXCOMMON_FILESYSTEM_LIB}
  )
endif()

set(NXCOMMON_GENERATED_DIR ${NXCommon_BINARY_DIR}/generated)
set(NXCOMMON_GENERATED_HEADER_DIR ${NXCommon_BINARY_DIR}/generated/NX/Common)
set(NXCOMMON_EXPORT_HEADER ${NXCOMMON_GENERATED_HEADER_DIR}/NXCommon_export.hpp)

file(READ ${NXCommon_SOURCE_DIR}/cmake/NXCommon_export_extra.hpp NXCOMMON_EXPORT_HEADER_EXTRA)

include(GenerateExportHeader)
generate_export_header(NXCommon
  EXPORT_FILE_NAME ${NXCOMMON_EXPORT_HEADER}
  CUSTOM_CONTENT_FROM_VARIABLE NXCOMMON_EXPORT_HEADER_EXTRA
)

set(NXCOMMON_SOURCE_DIR ${NXCommon_SOURCE_DIR}/src/NX/Common)

set(NXCOMMON_HDRS
  ${NXCOMMON_SOURCE_DIR}/Any.hpp
  ${NXCOMMON_SOURCE_DIR}/Array.hpp
  ${NXCOMMON_SOURCE_DIR}/Bit.hpp
  ${NXCOMMON_SOURCE_DIR}/NXConstants.hpp
  ${NXCOMMON_SOURCE_DIR}/BoundingBox.hpp
  ${NXCOMMON_SOURCE_DIR}/DataVector.hpp
  ${NXCOMMON_SOURCE_DIR}/EulerAngle.hpp
  ${NXCOMMON_SOURCE_DIR}/Numbers.hpp
  ${NXCOMMON_SOURCE_DIR}/Point2D.hpp
  ${NXCOMMON_SOURCE_DIR}/Point3D.hpp
  ${NXCOMMON_SOURCE_DIR}/Range.hpp
  ${NXCOMMON_SOURCE_DIR}/Range2D.hpp
  ${NXCOMMON_SOURCE_DIR}/Range3D.hpp
  ${NXCOMMON_SOURCE_DIR}/Ray.hpp
  ${NXCOMMON_SOURCE_DIR}/Result.hpp
  ${NXCOMMON_SOURCE_DIR}/RgbColor.hpp
  ${NXCOMMON_SOURCE_DIR}/ScopeGuard.hpp
  ${NXCOMMON_SOURCE_DIR}/StringLiteral.hpp
  ${NXCOMMON_SOURCE_DIR}/TypeTraits.hpp
  ${NXCOMMON_SOURCE_DIR}/Types.hpp
  ${NXCOMMON_SOURCE_DIR}/TypesUtility.hpp
  ${NXCOMMON_SOURCE_DIR}/Uuid.hpp
)

set(NXCOMMON_GENERATED_HEADERS
  ${NXCOMMON_EXPORT_HEADER}
)

set(NXCOMMON_SRCS
  ${NXCOMMON_SOURCE_DIR}/Range.cpp
  ${NXCOMMON_SOURCE_DIR}/Range2D.cpp
  ${NXCOMMON_SOURCE_DIR}/Range3D.cpp
  ${NXCOMMON_SOURCE_DIR}/RgbColor.cpp
  ${NXCOMMON_SOURCE_DIR}/Uuid.cpp
)

set(NXCOMMON_ALL_HDRS
  ${NXCOMMON_HDRS}
  ${NXCOMMON_GENERATED_HEADERS}
)

target_sources(NXCommon
  PRIVATE
  ${NXCOMMON_ALL_HDRS}
  ${NXCOMMON_SRCS}
)

source_group(TREE ${NXCOMMON_SOURCE_DIR} PREFIX "NX/Common" FILES ${NXCOMMON_HDRS})
source_group(TREE ${NXCOMMON_GENERATED_HEADER_DIR} PREFIX "NX/Common/generated" FILES ${NXCOMMON_GENERATED_HEADERS})
source_group(TREE ${NXCOMMON_SOURCE_DIR} PREFIX "NX/Common" FILES ${NXCOMMON_SRCS})

set(NXCOMMON_INCLUDE_DIR ${NXCommon_SOURCE_DIR}/src)

target_include_directories(NXCommon
  PUBLIC
  $<BUILD_INTERFACE:${NXCOMMON_INCLUDE_DIR}>
  $<BUILD_INTERFACE:${NXCOMMON_GENERATED_DIR}>
  $<INSTALL_INTERFACE:include>
)

set(NXCOMMON_PACKAGE_NAMESPACE "NX::")

option(NXCOMMON_ENABLE_INSTALL "Enables NXCOMMON install rules" ON)

if(NXCOMMON_ENABLE_INSTALL)
  install(TARGETS NXCommon
    EXPORT NXCommonTargets
  )

  export(EXPORT NXCommonTargets NAMESPACE ${NXCOMMON_PACKAGE_NAMESPACE})

  include(CMakePackageConfigHelpers)

  set(NXCOMMON_INSTALL_CMAKE_PREFIX "lib/cmake" CACHE STRING "Relative path to install NXCommonConfig.cmake in")

  set(NXCOMMON_CONFIG_FILE ${NXCommon_BINARY_DIR}/NXCommonConfig.cmake)
  set(NXCOMMON_VERSION_FILE ${NXCommon_BINARY_DIR}/NXCommonConfigVersion.cmake)

  configure_package_config_file(${NXCommon_SOURCE_DIR}/cmake/NXCommonConfig.cmake.in ${NXCOMMON_CONFIG_FILE}
    INSTALL_DESTINATION ${NXCOMMON_INSTALL_CMAKE_PREFIX}
  )

  write_basic_package_version_file(${NXCOMMON_VERSION_FILE}
    COMPATIBILITY SameMajorVersion
  )

  install(FILES ${NXCOMMON_CONFIG_FILE} ${NXCOMMON_VERSION_FILE}
    DESTINATION ${NXCOMMON_INSTALL_CMAKE_PREFIX}
    COMPONENT develop
  )

  include(GNUInstallDirs)
  install(TARGETS NXCommon
    RUNTIME
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT runtime
    LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT runtime
    NAMELINK_COMPONENT develop
    ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT develop
  )

  install(EXPORT NXCommonTargets
    NAMESPACE ${NXCOMMON_PACKAGE_NAMESPACE}
    DESTINATION ${NXCOMMON_INSTALL_CMAKE_PREFIX}
  )

  install_with_directory(FILES ${NXCOMMON_HDRS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NX/Common
    COMPONENT develop
    BASE_DIR ${NXCOMMON_SOURCE_DIR}
  )

  install_with_directory(FILES ${NXCOMMON_GENERATED_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NX/Common
    COMPONENT develop
    BASE_DIR ${NXCOMMON_GENERATED_HEADER_DIR}
  )

  # Install the LICENSE.txt as copyright to make VCPKG happy
  install(FILES ${NXCommon_SOURCE_DIR}/LICENSE.md DESTINATION ${NXCOMMON_INSTALL_CMAKE_PREFIX} RENAME copyright)
endif()

option(NXCOMMON_ENABLE_PACKAGING "Enables NX::Common packaging" ON)

if(NXCOMMON_ENABLE_PACKAGING)
  if(WIN32)
    set(CPACK_GENERATOR ZIP)
  else()
    set(CPACK_GENERATOR TGZ)
  endif()

  include(CPack)
endif()

if(NXCOMMON_BUILD_TESTS)
  include(CTest)
  add_subdirectory(test)
endif()

if(NXCOMMON_BUILD_PYTHON)
  add_subdirectory(wrapping/python)
endif()

option(NXCOMMON_BUILD_DOCS "Enables building NX::Common documentation" OFF)

if(NXCOMMON_BUILD_DOCS)
  add_subdirectory(docs)
endif()
