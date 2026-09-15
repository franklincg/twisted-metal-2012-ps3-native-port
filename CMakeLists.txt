cmake_minimum_required(VERSION 3.20)
project(TM2012PublicHostLab LANGUAGES C)
option(TM_PUBLIC_SANITIZERS "Sanitize the public synthetic host probe only" OFF)
option(TM_PUBLIC_H10_REFERENCE "Build the Windows-only historical H10 host reference suite" OFF)

add_executable(tm2012_public_layout_probe labs/taskset_layout/probe.c)
target_compile_features(tm2012_public_layout_probe PRIVATE c_std_11)
if(MSVC)
  target_compile_options(tm2012_public_layout_probe PRIVATE /W4)
else()
  target_compile_options(tm2012_public_layout_probe PRIVATE -Wall -Wextra -Wpedantic)
endif()
if(TM_PUBLIC_SANITIZERS)
  if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU" AND NOT MSVC)
    target_compile_options(tm2012_public_layout_probe PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(tm2012_public_layout_probe PRIVATE -fsanitize=address,undefined)
  else()
    message(FATAL_ERROR "This sanitizer switch is only configured for non-MSVC GCC/Clang host builds")
  endif()
endif()

enable_testing()
add_test(NAME public_layout_probe COMMAND tm2012_public_layout_probe)

if(TM_PUBLIC_H10_REFERENCE)
  if(NOT WIN32)
    message(FATAL_ERROR "The historical H10 reference suite models the Windows host contract and is intentionally Windows-only")
  endif()
  set(H10_REF ${CMAKE_CURRENT_SOURCE_DIR}/research/h10_reference)
  set(H10_UPSTREAM ${H10_REF}/upstream/ps3recomp)
  add_executable(tm2012_h10_reference
    ${H10_REF}/representation.c
    ${H10_REF}/legacy.c
    ${H10_REF}/spurs_taskset.c
    ${H10_REF}/spurs_pm.c
    ${H10_REF}/test_representation.c)
  target_compile_features(tm2012_h10_reference PRIVATE c_std_17)
  target_include_directories(tm2012_h10_reference PRIVATE
    ${H10_REF}
    ${H10_UPSTREAM}/libs/spurs
    ${H10_UPSTREAM}/runtime/ppu
    ${H10_UPSTREAM}/include)
  if(MSVC)
    target_compile_options(tm2012_h10_reference PRIVATE /W4 /std:c17 /experimental:c11atomics)
  else()
    target_compile_options(tm2012_h10_reference PRIVATE -Wall -Wextra)
  endif()
  add_test(NAME h10_reference_host COMMAND tm2012_h10_reference)
endif()

# No game, lifted game workload, original asset, runtime archive, network service
# or private path is built by this public project.
