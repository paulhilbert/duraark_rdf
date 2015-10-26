cmake_minimum_required(VERSION 2.8.8)
project(duraark_rdf)

set (duraark_rdf_VERSION_MAJOR 0)
set (duraark_rdf_VERSION_MINOR 1)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-fPIC)
	add_definitions(-O3)
	add_definitions(-g)
	add_definitions(-std=c++1y)
    add_definitions(-Wall)
    add_definitions(-Wno-unused-function)
    add_definitions(-Wno-unused-local-typedefs)
    add_definitions(-Wno-deprecated-declarations)
endif()

find_package(Boost COMPONENTS system)
find_package(Raptor)
find_package(Eigen)

file (GLOB_RECURSE obj RELATIVE "${PROJECT_SOURCE_DIR}" "src/*.cpp")
if (RAPTOR_FOUND AND EIGEN_FOUND AND Boost_FOUND)
	include_directories(${RAPTOR_INCLUDE_DIRS})
	include_directories(${EIGEN_INCLUDE_DIRS})
	include_directories(${Boost_INCLUDE_DIRS})

    add_library(duraark_rdf SHARED ${obj})
    target_link_libraries(duraark_rdf ${RAPTOR_LIBRARIES} ${OPTIONAL_LIBRARIES} "dl")

	# install binary
    install (TARGETS duraark_rdf DESTINATION lib)
	# install header
    install (DIRECTORY include/ DESTINATION include/duraark_rdf)
    # install find module
    file (GLOB find_modules RELATIVE "${PROJECT_SOURCE_DIR}" "cmake/modules/self/*.cmake")
    install (FILES ${find_modules} DESTINATION share/DuraarkRDF)
endif()
