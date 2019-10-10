CMAKE_MINIMUM_REQUIRED( VERSION 2.8)
project(lsrc)

# Let cmake know that this is a release build.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Include Directories.
include_directories(include)

add_executable(lsrc include/cw.h include/cstr.h src/cw.c src/cstr.c src/main.c)
target_include_directories(lsrc PUBLIC . include)
install(
	TARGETS lsrc
	RUNTIME DESTINATION bin
)
