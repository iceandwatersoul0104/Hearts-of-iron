cmake_minimum_required(VERSION 3.16)
project(HeartsOfIronGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

set(SOURCES
    main.cpp
    core/Player.cpp
    core/GameEngine.cpp
    core/SaveManager.cpp
    core/MusicPlayer.cpp
    core/ScenarioBase.cpp
    scenarios/FallGelbScenario.cpp
    scenarios/BritainScenario.cpp
    scenarios/WolfPackScenario.cpp
    scenarios/StalingradScenario.cpp
    scenarios/BerlinScenario.cpp
    ui/MainWindow.cpp
    ui/MenuWidget.cpp
    ui/GameWidget.cpp
    ui/CharacterCreateWidget.cpp
    ui/SaveLoadDialog.cpp
)

set(HEADERS
    core/GameState.h
    core/StoryNode.h
    core/Player.h
    core/GameEngine.h
    core/SaveManager.h
    core/MusicPlayer.h
    core/ScenarioBase.h
    scenarios/FallGelbScenario.h
    scenarios/BritainScenario.h
    scenarios/WolfPackScenario.h
    scenarios/StalingradScenario.h
    scenarios/BerlinScenario.h
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

# Copy music directory to build folder if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/music")
    add_custom_command(TARGET HeartsOfIronGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/music"
        "$<TARGET_FILE_DIR:HeartsOfIronGame>/music"
    )
endif()

set_target_properties(HeartsOfIronGame PROPERTIES
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME "钢铁意志_第三帝国"
)
