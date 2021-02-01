cmake_minimum_required(VERSION 3.5)

project(myFavNotes LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Sql REQUIRED)


set(CMAKE_AUTOUIC_SEARCH_PATHS forms)

include_directories("headers")
include_directories(${CMAKE_BINARY_DIR})


set(PROJECT_SOURCES
        sources/main.cpp
        sources/MainWindow.cpp
        sources/MainWindow_database.cpp
        sources/MainWindow_settings.cpp
        sources/MainWindow_actions.cpp
        sources/NoteWidget.cpp
        sources/TextInputDialog.cpp
        sources/DBDialog.cpp
        sources/NoteListView.cpp
        sources/SortFilterProxyModelNotes.cpp
        sources/NoteListDelegate.cpp
        sources/SideBarDelegate.cpp
        sources/SideBarListView.cpp
)

set(PROJECT_HEADERS
        headers/MainWindow.h
        headers/NoteWidget.h
        headers/TextInputDialog.h
        headers/DBDialog.h
        headers/NoteListView.h
        headers/SortFilterProxyModelNotes.h
        headers/NoteListDelegate.h
        headers/SideBarDelegate.h
        headers/SideBarListView.h
)

set(PROJECT_FORMS
        forms/MainWindow.ui
        forms/NoteWidget.ui
        forms/TextInputDialog.ui
        forms/DBDialog.ui
)

set(PROJECT_RESOURCES
        resources/resources.qrc
        resources/icon.rc
)


set(PROJECT_FILES
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_RESOURCES}
        ${PROJECT_FORMS_HEADERS}
)

#set(MACOSX_BUNDLE_ICON_FILE icon.icns)
#set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.icns)
#set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(myFavNotes
            ${PROJECT_FILES}
    )
else()
    if(ANDROID)
        add_library(myFavNotes SHARED
                ${PROJECT_FILES}
        )
    else()
        add_executable(myFavNotes
                ${PROJECT_FILES}
        )
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET myFavNotes PROPERTY WIN32_EXECUTABLE true)
endif()

target_link_libraries(myFavNotes PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(myFavNotes PRIVATE Qt6::Sql)
