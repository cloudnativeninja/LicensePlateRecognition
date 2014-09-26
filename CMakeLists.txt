cmake_minimum_required(VERSION 2.8)
project(lpr)

#include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_FLAGS "-Ofast -g -Wall")

SET (CMAKE_CXX_COMPILER   "/usr/bin/clang++")

macro (add_headers)
    file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_hds ${ARGN})
        if (_relPath)
            list (APPEND HDS "${_relPath}/${_hds}")
        else()
            list (APPEND HDS "${_hds}")
        endif()
    endforeach()
#    message(STATUS "${SRCS}")
    if (_relPath)
        # propagate SRCS to parent directory
        set (HDS ${HDS} PARENT_SCOPE)
    endif()
endmacro()

macro (add_sources)
    file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SRCS "${_relPath}/${_src}")
        else()
            list (APPEND SRCS "${_src}")
        endif()
    endforeach()
#    message(STATUS "${SRCS}")
    if (_relPath)
        # propagate SRCS to parent directory
        set (SRCS ${SRCS} PARENT_SCOPE)
    endif()
endmacro()

#message(STATUS "${PROJECT_SOURCE_DIR}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/lib/")


find_package(TESSERACT)
find_package(OpenCV REQUIRED)


if(NOT TESSERACT_FOUND)
        MESSAGE (STATUS "TESSERACT LIB not found. Project will not be built.")
else(NOT TESSERACT_FOUND)
		MESSAGE (STATUS "TESSERACT LIB found. Project will be built.")
endif(NOT TESSERACT_FOUND)

if(NOT OpenCV_FOUND)
        MESSAGE (STATUS "OpenCV LIB not found. Project will not be built.")
else(NOT OpenCV_FOUND)
		MESSAGE (STATUS "OpenCV LIB found. Project will be built.")
endif(NOT OpenCV_FOUND)


set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

add_subdirectory("${SOURCE_DIR}/main")
add_subdirectory("${SOURCE_DIR}/tools")



include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

include_directories(${TESSERACT_INCLUDE_DIRS})
link_directories(${TESSERACT_LIBRARY_DIRS})

add_executable(
	lpr ${SRCS} ${HDS}
)
target_link_libraries(lpr ${TESSERACT_LIBRARIES})
target_link_libraries(lpr ${OpenCV_LIBRARIES})


message(STATUS "${SRCS}")