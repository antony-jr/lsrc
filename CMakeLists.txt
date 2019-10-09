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

add_executable(lrc )
target_include_directories(hpack PUBLIC . include)
install(
	TARGETS lsrc
	RUNTIME DESTINATION bin
)
