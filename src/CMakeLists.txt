cmake_minimum_required(VERSION 3.16)

project(PekaEDS VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

qt_standard_project_setup()

qt_add_executable(PekaEDS
    handler/EpisodeHandler.cpp
    handler/MapHandler.cpp

    io/FileHandler.h
    io/FileHandler.cpp
    io/LegacyMapFileHandler.cpp
    io/LegacySpriteFileReader.cpp

    pk2/ExpectedMapFormatException.h  
    pk2/LegacyMapConstants.h     
    pk2/PK2SpriteTypes.h
    pk2/MissingImageException.h       
    pk2/Scrolling.h
    pk2/PK2FileUtil.cpp        
    pk2/SDLMapConstants.h
    pk2/PK2LegacyMapData.h
    pk2/Weather.h
    pk2/PK2MapBaseMetadata.h          
    pk2/WrongSpriteFormatException.h
    pk2/PK2MapData.h

    ui/BackgroundRenderer.h 
    ui/RegionView.h
    ui/RegionView.cpp
    ui/Region.cpp
    ui/EDSErrorDialog.cpp        
    ui/ImageCache.h
    ui/ImageCache.cpp
    ui/EDSMainMenu.cpp    

    ui/ColorPaletteView.h
    ui/SpriteListView.cpp
    ui/EDSToolBar.cpp      
    ui/MiniMapView.cpp         
    ui/SpriteView.cpp
    ui/MapMetadataComponent.h  
    ui/PekaEDSMainView.cpp
    ui/TilesetViewRenderer.cpp
    ui/MapMetadataView.cpp     
    ui/PekaEDSWindow.cpp    
    ui/TilesetView.cpp
    ui/RegionsListView.h
    ui/RegionsListView.cpp
    ui/RegionMetadataView.h
    ui/RegionMetadataView.cpp
    ui/ColorPaletteView.cpp

    util/GamePaths.cpp

    util/LayerLayout.cpp  
    util/MapProfile.h
    util/Settings.cpp
    util/GFXUtil.h    
    util/Logger.cpp      
    util/MapUtil.h     
    util/Version.h
    util/Profile.h
    util/Profile.cpp

    main.cpp
    tools/Tool.h tools/Tool.cpp
    tools/BrushTool.h tools/BrushTool.cpp
    ui/NewRegionDialog.h ui/NewRegionDialog.cpp
    util/cachetree.h util/cachetree.cpp
    util/CacheEntry.h util/CacheEntry.cpp
    ui/Layers.h
    ui/SetGamePathView.h ui/SetGamePathView.cpp
    ui/ToolMode.h
    util/MapPositionDialog.h util/MapPositionDialog.cpp
    tools/SelectionTool.h tools/SelectionTool.cpp
    ui/ColorPreviewWidget.h ui/ColorPreviewWidget.cpp
)

qt_add_resources(PekaEDS "res"
    FILES
    res/missing.bmp
)

target_include_directories(PekaEDS PRIVATE ${PROJECT_SOURCE_DIR}/handler)
target_include_directories(PekaEDS PRIVATE ${PROJECT_SOURCE_DIR}/io)
target_include_directories(PekaEDS PRIVATE ${PROJECT_SOURCE_DIR}/pk2)
target_include_directories(PekaEDS PRIVATE ${PROJECT_SOURCE_DIR}/ui)
target_include_directories(PekaEDS PRIVATE ${PROJECT_SOURCE_DIR}/util)
target_include_directories(PekaEDS PRIVATE ${PROJECT_SOURCE_DIR}/tools)

target_link_libraries(PekaEDS PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Core5Compat)
target_link_libraries(PekaEDS PRIVATE nlohmann_json::nlohmann_json)

set_target_properties(PekaEDS PROPERTIES AUTOMOC ON)
set_target_properties(PekaEDS PROPERTIES AUTORCC ON)
