cmake_minimum_required(VERSION 3.16)

project(CarIntegrator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Gui Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appCarIntegrator
    main.cpp
)

qt_add_qml_module(appCarIntegrator
    URI CarIntegrator
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES debughelper.h debughelper.cpp
        SOURCES servicebus.h servicebus.cpp
        SOURCES appcore/applicationcore.h appcore/applicationcore.cpp
        SOURCES
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appCarIntegrator PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCarIntegrator
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_subdirectory(libs/device-common)

target_link_libraries(appCarIntegrator
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Network
    PRIVATE Qt6::Core
    PRIVATE device-common
)

target_include_directories(appCarIntegrator
    PRIVATE libs/device-common/include
)

include(GNUInstallDirs)

install(TARGETS appCarIntegrator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
