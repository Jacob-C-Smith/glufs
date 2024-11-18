# Need CMake 3.16.0
cmake_minimum_required (VERSION 3.16.0)

# The name of the repository
project ("glufs")

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set the output paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Build sync with semaphore
add_compile_definitions(BUILD_SYNC_WITH_SEMAPHORE)

# Build sync with monitor
add_compile_definitions(BUILD_SYNC_WITH_MONITOR)

# Find the log module
if ( NOT "${HAS_LOG}")
    
    # Add the log directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/log ${CMAKE_SOURCE_DIR}/build/log)

    # Let any parent project know that this project has log
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_LOG true PARENT_SCOPE)
        set(LOG_INCLUDE_DIR ${LOG_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the sync module
if ( NOT "${HAS_SYNC}")
    
    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_SYNC true PARENT_SCOPE)
        set(SYNC_INCLUDE_DIR ${SYNC_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the tree module
if ( NOT "${HAS_TREE}")
    
    # Add the tree directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/tree ${CMAKE_SOURCE_DIR}/build/tree)

    # Let any parent project know that this project has tree
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_TREE true PARENT_SCOPE)
        set(TREE_INCLUDE_DIR ${TREE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Add the glufs project
if ( NOT "${HAS_GLUFS}")

    set (GLUFS_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

    # Let any parent project know that it has the glufs project
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_GLUFS true PARENT_SCOPE)
        set(GLUFSL_INCLUDE_DIR ${GLUFS_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Add source to this project's executable.
add_executable (glufs_example "main.c")
add_dependencies(glufs_example glufs log sync)
target_include_directories(glufs_example PUBLIC ${GLUFS_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(glufs_example glufs log sync)

# Add source to this project's executable.
add_executable (archiver "archiver.c")
add_dependencies(archiver glufs tree log sync)
target_include_directories(archiver PUBLIC ${GLUFS_INCLUDE_DIR} ${TREE_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(archiver glufs tree log sync)

# Add source to this project's library
add_library (glufs SHARED "glufs.c" "entry.c" "metadata.c")
add_dependencies(glufs log sync)
target_include_directories(glufs PUBLIC ${GLUFS_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(glufs PRIVATE log sync)
