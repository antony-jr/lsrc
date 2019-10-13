CMAKE_MINIMUM_REQUIRED( VERSION 2.8)
project(lsrc)

# Let cmake know that this is a release build.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# add git commit to bulid
if(GIT_COMMIT_STR)
	add_definitions(-DGIT_COMMIT_STR="${GIT_COMMIT_STR}")
endif()

# Include Directories.
include_directories(include)

add_executable(lsrc include/cw.h
	            include/cutelog.h
		    include/cstr.h
		    include/fwriter.h
		    include/cstr_list.h
		    src/cutelog.c
		    src/cw.c
		    src/cstr.c
		    src/cstr_list.c
		    src/fwriter.c
		    src/main.c)
target_include_directories(lsrc PUBLIC . include)
install(
	TARGETS lsrc
	RUNTIME DESTINATION bin
)
