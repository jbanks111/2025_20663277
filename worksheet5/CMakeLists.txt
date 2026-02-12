cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(calc)

# --------------------------------------
# Library
# --------------------------------------

add_library(maths adder.cpp)

# If building static library, define macro
if(NOT BUILD_SHARED_LIBS)
    add_definitions(-Dmaths_STATIC)
endif()

# --------------------------------------
# Executable
# --------------------------------------

add_executable(calc calc.cpp)
target_link_libraries(calc maths)

# --------------------------------------
# Install rules
# --------------------------------------

install(TARGETS calc maths
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)

# --------------------------------------
# CPack configuration
# --------------------------------------

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
else()
    set(CPACK_GENERATOR "ZIP")
endif()

include(CPack)
