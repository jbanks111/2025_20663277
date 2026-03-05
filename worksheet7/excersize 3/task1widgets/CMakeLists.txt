cmake_minimum_required(VERSION 3.19)

project(task1widgets LANGUAGES CXX)

set(CMAKE_AUTORCC ON)

# Find Qt
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

# Find VTK (include Qt support and STL I/O)
find_package(VTK REQUIRED COMPONENTS
    CommonCore
    RenderingCore
    RenderingOpenGL2
    InteractionStyle
    GUISupportQt
    IOGeometry        # For vtkSTLReader
)

qt_standard_project_setup()

# Add executable
qt_add_executable(task1widgets
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    modelPart.cpp
    modelPart.h
    modelPartList.cpp
    modelPartList.h
    icons.qrc
    optiondialog.h
    optiondialog.cpp
    optiondialog.ui
)

# Link libraries
target_link_libraries(task1widgets
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        VTK::CommonCore
        VTK::RenderingCore
        VTK::RenderingOpenGL2
        VTK::InteractionStyle
        VTK::GUISupportQt
        VTK::IOGeometry
)

# Required for VTK module initialization
vtk_module_autoinit(
    TARGETS task1widgets
    MODULES
        VTK::CommonCore
        VTK::RenderingCore
        VTK::RenderingOpenGL2
        VTK::InteractionStyle
        VTK::GUISupportQt
        VTK::IOGeometry
)

# Install rules
include(GNUInstallDirs)

install(TARGETS task1widgets
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET task1widgets
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

# Copy across OpenVR bindings
add_custom_target(VRBindings)

add_custom_command(TARGET VRBindings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/vrbindings
            ${CMAKE_BINARY_DIR}/vrbindings
)