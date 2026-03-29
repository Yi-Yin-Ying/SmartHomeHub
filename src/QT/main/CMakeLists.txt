cmake_minimum_required(VERSION 3.16)

project(main VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt6 组件（添加 Mqtt 组件）
find_package(Qt6 REQUIRED COMPONENTS Widgets Mqtt)

qt_standard_project_setup()

qt_add_executable(main
    main.cpp
    widget.cpp
    widget.h
    widget.ui
)

# 统一链接所有 Qt 库
target_link_libraries(main PUBLIC
    Qt6::Widgets
    Qt6::Mqtt
)

set_target_properties(main PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS main
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)