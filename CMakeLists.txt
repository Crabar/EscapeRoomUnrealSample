cmake_minimum_required(VERSION 3.12)
project(EscapeRoom)

set(CMAKE_CXX_STANDARD 14)

include_directories(Intermediate/Build/Win64/UE4Editor/Development)
include_directories(Intermediate/Build/Win64/UE4Editor/Development/BuildingEscape)
include_directories(Intermediate/Build/Win64/UE4Editor/Inc/BuildingEscape)
include_directories(Source/BuildingEscape)

add_executable(EscapeRoom
        Intermediate/Build/Win64/UE4Editor/Development/BuildingEscape/Definitions.BuildingEscape.h
        Intermediate/Build/Win64/UE4Editor/Inc/BuildingEscape/BuildingEscape.init.gen.cpp
        Intermediate/Build/Win64/UE4Editor/Inc/BuildingEscape/BuildingEscapeClasses.h
        Intermediate/Build/Win64/UE4Editor/Inc/BuildingEscape/PositionReport.gen.cpp
        Intermediate/Build/Win64/UE4Editor/Inc/BuildingEscape/PositionReport.generated.h
        Source/BuildingEscape/BuildingEscape.cpp
        Source/BuildingEscape/BuildingEscape.h
        Source/BuildingEscape/PositionReport.cpp
        Source/BuildingEscape/PositionReport.h)
