cmake_minimum_required(VERSION 3.7)

project(ScreenSaverConfigDialog)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(DtkWidget REQUIRED)

set (SRC
    configwidget.h
    configwidget.cpp
    configsettingsdialog.h
    configsettingsdialog.cpp
    screensaverlistview.h
    screensaverlistview.cpp
    )

add_executable(${PROJECT_NAME} "main.cpp" ${SRC})

target_include_directories(ScreenSaverConfigDialog PUBLIC ${DtkWidget_INCLUDE_DIRS} ${OBJECT_BINARY_DIR})
target_link_libraries(ScreenSaverConfigDialog
    ${DtkWidget_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
)
