# Find required packages
FIND_PACKAGE(CGAL ${SYSTEM_PACKAGE_REQUIRED})

set (dep_libs)
set (dep_libs_debug)

if (WIN32)
  set(dep_libs Common Math IO)
  set(dep_libs_debug Common Math IO)
else()
  set(dep_libs Common Math IO)
endif()

# Find required packages
FIND_PACKAGE(CGAL ${SYSTEM_PACKAGE_REQUIRED})
if(CGAL_FOUND)
	include_directories(${CGAL_INCLUDE_DIRS})
	add_definitions(${CGAL_DEFINITIONS})
	link_directories(${CGAL_LIBRARY_DIRS})
	include_directories(${CMAKE_INSTALL_PREFIX}/gmp)
	link_directories(${CMAKE_INSTALL_PREFIX}/gmp)
	
	if (WIN32)
	  set(dep_libs ${dep_libs} mpfr mpir mpirxx)
	  set(dep_libs_debug ${dep_libs_debug} mpfrd mpird mpirxxd)
	else()
      set(dep_libs ${dep_libs} ${CGAL_LIBRARYS})
	endif()
endif()

FIND_PACKAGE(VCG ${SYSTEM_PACKAGE_REQUIRED})
if(VCG_FOUND)
	include_directories(${VCG_INCLUDE_DIRS})
	add_definitions(${VCG_DEFINITIONS})
endif()

set(CERES_LIBS "")
if(OpenMVS_USE_CERES)
	FIND_PACKAGE(CERES)
	if(CERES_FOUND)
		include_directories(${CERES_INCLUDE_DIRS})
		add_definitions(${CERES_DEFINITIONS})
	else()
		set(OpenMVS_USE_CERES OFF)
	endif()
	link_directories(${CERES_LIBRARY_DIRS})
	if (WIN32)
	  set(dep_libs ${dep_libs} ceres libcxsparse glog gflags_static shlwapi)
	  set(dep_libs_debug ${dep_libs_debug} ceres-debug libcxsparsed glogd gflags_staticd shlwapi)
	else()
      set(dep_libs ${dep_libs} ${CERES_LIBS})
	endif()
endif()

if(OpenMVS_USE_CUDA)
	set(dep_libs ${dep_libs} ${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY})
	set(dep_libs_debug ${dep_libs_debug} ${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY})
endif()

# List sources files
FILE(GLOB PCH_C "Common.cpp")

FILE(GLOB LIBRARY_FILES_C "*.cpp")
FILE(GLOB LIBRARY_FILES_H "*.h" "*.inl")

# Place Common.cpp as the first file in the list
# needed by cotire when setting PCH manually
LIST(REMOVE_ITEM LIBRARY_FILES_C ${PCH_C})
SET(LIBRARY_FILES_C "${PCH_C};${LIBRARY_FILES_C}")

cxx_library_with_type_no_pch(MVS "Libs" "" "${cxx_default}"
	${LIBRARY_FILES_C} ${LIBRARY_FILES_H}
)

# Manually set Common.h as the precompiled header
set_target_pch(MVS Common.h)

# Link its dependencies
#TARGET_LINK_LIBRARIES(MVS PRIVATE Common Math IO ${CERES_LIBS} ${CGAL_LIBS} ${CUDA_CUDA_LIBRARY} CGAL::CGAL)
if (WIN32)
  foreach(lib ${dep_libs})
    target_link_libraries(MVS optimized ${lib})
  endforeach()
  foreach(lib ${dep_libs_debug})
    target_link_libraries(MVS debug ${lib})
  endforeach()
else()
  TARGET_LINK_LIBRARIES(MVS ${CERES_LIBS} ${CGAL_LIBS} ${dep_libs})
endif()

# Install
SET_TARGET_PROPERTIES(MVS PROPERTIES
	PUBLIC_HEADER "${LIBRARY_FILES_H}")
INSTALL(TARGETS MVS
	EXPORT OpenMVSTargets
	RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
	LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
	ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib
	PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/MVS" COMPONENT dev)
