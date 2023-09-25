#
cmake_minimum_required(VERSION 3.16)

project(ucan-monitor VERSION 0.3)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/git-info)
include(GitInfo)
GitInfoSetup()

################################################################################
# Threads
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

################################################################################
# Qt
find_package(Qt6 COMPONENTS Widgets Quick Gui QuickControls2 REQUIRED)

qt_standard_project_setup()



# find_package(Qt5 COMPONENTS Core Quick Charts REQUIRED)
# find_package(Qt5QuickCompiler REQUIRED)

# if (${Qt5_FOUND})
#     message(STATUS "Found Qt " ${Qt5_VERSION})
# else()
#     message(FATAL_ERROR "Couldn't find Qt")
# endif()

# set(CMAKE_AUTOMOC ON)
# # set(CMAKE_AUTORCC ON)

# cmake_policy(SET CMP0071 NEW)

################################################################################
# Set default build type
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified")
    set(CMAKE_BUILD_TYPE ${default_build_type})
endif()

################################################################################
# Headers and Sources
set(app_incdirs
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

file(GLOB_RECURSE app_sources CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/include/*.c
    ${PROJECT_SOURCE_DIR}/include/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# qtquick_compiler_add_resources(app_resources ./resources.qrc)

################################################################################
# Executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(executable ${CMAKE_PROJECT_NAME})

qt_add_executable(${executable}
    ${app_sources}
    #${app_resources}
)

qt_add_qml_module(${executable}
    URI ${CMAKE_PROJECT_NAME}
    QML_FILES
        main.qml
        serverselector_window.qml
    #     FramedImage.qml
    # RESOURCES
    #     img/world.png
)

target_compile_definitions(${executable} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    #${GTK4_CFLAGS_OTHER}
)

target_include_directories(${executable} PRIVATE
    ${app_incdirs}
)

target_link_libraries(${executable} PRIVATE
    Threads::Threads
    Qt6::Widgets
    Qt6::Gui
    Qt6::Quick
    Qt::QuickControls2
    # Qt5::Quick
    # Qt5::Charts
    git-info
)

################################################################################
# Compiler Options and Features
target_compile_options(${executable} PRIVATE
    #$<$<CONFIG:Debug>:-Og -g3 -ggdb>
    #$<$<CONFIG:Release>:-Og -g0>
    -Wall
    -Wextra
    -Wpedantic
    -fdiagnostics-color=always
)

target_compile_features(${executable} PRIVATE
    c_std_11
    cxx_std_23
)

# target_link_options(${executable} PRIVATE
#     -Wl,--export-dynamic # sic! for signals
# )

################################################################################
# Misc Options
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

################################################################################
# Set install target for flatpak-builder
install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME
    DESTINATION /app/bin
)


