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
	# set(CMAKE_EXE_LINKER_FLAGS "-pthread")
	set(CMAKE_CXX_FLAGS_DEBUG "-O3 -Wall -g -pg")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
endif (CMAKE_COMPILER_IS_GNUCXX)

# +----------------------------------------------+
# | Third Party Libray                           |
# +--------------------------------------------- +
find_package(Eigen3 REQUIRED)
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})

find_package(VTK COMPONENTS 
	vtkCommonColor
	vtkRenderingContextOpenGL2
	vtkRenderingGL2PSOpenGL2
	vtkCommonCore
	vtkCommonDataModel
	vtkFiltersSources
	vtkInteractionStyle
	vtkRenderingCore
	vtkRenderingFreeType
	vtkRenderingOpenGL2 QUIET)
if (NOT VTK_FOUND)
  message("Skipping Animation: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")

# +----------------------------------------------+
# | Build executable                             |
# +--------------------------------------------- +
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
	include(${VTK_USE_FILE})
	add_executable(${target_name}
		MACOSX_BUNDLE
		${source_files}
	)
	target_link_libraries(${target_name} PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
	add_executable(${target_name}
		MACOSX_BUNDLE
		${source_files}
	)
	target_link_libraries(${target_name} PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
	vtk_module_autoinit(
		TARGETS ${target_name}
		MODULES ${VTK_LIBRARIES}
	)
endif () 