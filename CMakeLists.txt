cmake_minimum_required(VERSION 3.16)
project(HeartsOfIronGame VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

set(SOURCES
    main.cpp
    engine/PlayerSystem.cpp
    engine/DiceSystem.cpp
    engine/DlcManager.cpp
    engine/NodeEngine.cpp
    engine/SaveManager.cpp
    engine/MusicPlayer.cpp
    ui/MainWindow.cpp
    ui/MenuWidget.cpp
    ui/GameWidget.cpp
    ui/CharacterCreateWidget.cpp
    ui/SaveLoadDialog.cpp
)

set(HEADERS
    engine/DlcTypes.h
    engine/PlayerSystem.h
    engine/DiceSystem.h
    engine/DlcManager.h
    engine/NodeEngine.h
    engine/SaveManager.h
    engine/MusicPlayer.h
    ui/MainWindow.h
    ui/MenuWidget.h
    ui/GameWidget.h
    ui/CharacterCreateWidget.h
    ui/SaveLoadDialog.h
)

qt_add_executable(HeartsOfIronGame
    ${SOURCES}
    ${HEADERS}
    resources/resources.qrc
)

target_link_libraries(HeartsOfIronGame PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
)

target_include_directories(HeartsOfIronGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Copy dlc directory to build folder
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/dlc")
    add_custom_command(TARGET HeartsOfIronGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/dlc"
        "$<TARGET_FILE_DIR:HeartsOfIronGame>/dlc"
    )
endif()

set_target_properties(HeartsOfIronGame PROPERTIES
    WIN32_EXECUTABLE FALSE
    OUTPUT_NAME "HeartsOfIron"
)
