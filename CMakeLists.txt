cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Set project name and version
project(Hashlife VERSION 0.1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Enable debug symbols by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release 
    CACHE STRING 
      "Choose the type of build (Debug or Release)" 
      FORCE
    )
endif()

# Generate config.h
#configure_file(config.h.in config.h)

# Find SFML shared libraries
find_package(SFML 2.5 
  COMPONENTS 
    system window graphics network audio REQUIRED
  )

# Compile executable
include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(Hashlife ${SOURCES})

# Set include directory search paths
target_include_directories(Hashlife 
  PRIVATE
  "${PROJECT_BINARY_DIR}"
  )
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
# Link executable to required SFML libraries
target_link_libraries(Hashlife sfml-graphics ${OPENGL_LIBRARIES})

# Install target
install(TARGETS Hashlife DESTINATION bin)

# CPack packaging
#include(InstallRequiredSystemLibraries)
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
#set(CPACK_PACKAGE_VERSION_MAJOR "${myproject_VERSION_MAJOR}")
#set(CPACK_PACKAGE_VERSION_MINOR "${myproject_VERSION_MINOR}")
#include(CPack)