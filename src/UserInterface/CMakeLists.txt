add_definitions(-DUSERINTERFACE_EXPORT)

set(PROJECT_SOURCES
    src/UserInterfaceLib.h
    src/MainWindow.cpp
    src/MainWindow.h
    src/MainWindow.ui
    src/AboutDialog.cpp
    src/AboutDialog.h
    src/AboutDialog.ui
    src/SimulationVariablesDialog.cpp
    src/SimulationVariablesDialog.h
    src/SimulationVariablesDialog.ui
)

qt5_add_resources(PROJECT_SOURCES res/SkyDolly.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(UserInterface SHARED
        ${PROJECT_SOURCES}
    )
else()
    add_library(UserInterface SHARED
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(UserInterface PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Kernel SkyConnect)
set_target_properties(UserInterface PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(UserInterface PROPERTIES SOVERSION 1)
