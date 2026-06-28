cmake_minimum_required(VERSION 3.15...3.27)

if (CMAKE_VERSION VERSION_LESS 3.18)
  set(DEV_MODULE Development)
else()
  set(DEV_MODULE Development.Module)
endif()

project(bind_template LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python 3.14 
  REQUIRED COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)
#  COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)

find_package(nanobind CONFIG REQUIRED)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

nanobind_add_module(process_scalar
    STABLE_ABI
    src/cpp/bindings/bindings.cpp
    src/cpp/bindings/bind_process_scalar.cpp
    src/cpp/lib/process_scalar.cpp
)

target_include_directories(process_scalar PRIVATE
    src/cpp/lib
)

# Install directive for scikit-build-core
install(TARGETS process_scalar LIBRARY DESTINATION bind_template/scalar)