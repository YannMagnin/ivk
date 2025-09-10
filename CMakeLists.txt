# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)
project(ivk LANGUAGES C ASM)

include(GenerateG1A)
include(GenerateG3A)
include(GenerateHH2Bin)
include(Fxconv)
find_package(Gint 2.9 REQUIRED)
find_package(Python3 REQUIRED)

set(SOURCES
  src/main.c
  src/menu/instruction.c
  src/menu/_list.c
  src/instructions_cpu.S
  src/instructions_cpu.c
  src/ivk/_code.S
  src/ivk/_data.S
  src/ivk/prepare.S
  src/ivk/exec.S
)

add_custom_command(
    OUTPUT ../src/instructions_cpu.S ../src/instructions_cpu.c
    COMMAND Python3::Interpreter ../scripts/instructions_generator.py
    DEPENDS ../scripts/instructions_generator.py ../src/instruction-cpu.def
)

add_executable(ivk ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(ivk PRIVATE -Wall -Wextra -Os -g -flto=auto)
target_include_directories(ivk PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(ivk Gint::Gint)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
  generate_g1a(TARGET ivk OUTPUT "ivk.g1a"
    NAME "ivk" ICON assets-fx/icon.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET ivk OUTPUT "ivk.g3a"
    NAME "ivk" ICONS assets/cg/icon-uns.png assets/cg/icon-sel.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G_G3A)
  generate_g3a(TARGET ivk OUTPUT "ivk-fx.g3a"
    NAME "ivk-fx" ICONS assets/cg/icon-uns.png assets/cg/icon-sel.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCP)
  generate_hh2_bin(TARGET ivk OUTPUT "ivk-hh2.bin")
endif()
