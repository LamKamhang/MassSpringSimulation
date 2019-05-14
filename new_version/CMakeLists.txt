# +----------------------------------------------+
# | project settings                             |
# +--------------------------------------------- +
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(mass_spring_system)
set(target_name mass_spring_system)

set(source_files
	src/main.cxx
)

# +----------------------------------------------+
# | Complilation flags                           |
# +--------------------------------------------- +
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_EXE_LINKER_FLAGS "-pthread")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -pg -g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
endif (CMAKE_COMPILER_IS_GNUCXX)

# +----------------------------------------------+
# | Third Party Libray                           |
# +--------------------------------------------- +
find_package(Eigen3 REQUIRED)
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})

# +----------------------------------------------+
# | Build executable                             |
# +--------------------------------------------- +
add_executable(${target_name}
    ${source_files}
)