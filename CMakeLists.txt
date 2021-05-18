cmake_minimum_required(VERSION 3.14)

project(animeDownloadConsoleV2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)
find_package(Qt5Sql)
#find_package(unofficial-mongoose CONFIG REQUIRED)
find_package(Qt5Network)

add_executable(animeDownloadConsoleV2
    model.cpp
    model.h
  main.cpp
  database.cpp
  database.h
  episodeRepository.cpp
  episodeRepository.h
  repository.h
  animeRepository.cpp
  animeRepository.h
  mjson.c
  mjson.h
  mongoose.c
  mongoose.h
  webui.h
  webui.cpp
  mainwindow.cpp
  mainwindow.h
  searchhandler.cpp
  searchhandler.h
  utils.cpp
  utils.h
  webserver.cpp
  webserver.h
  searchservice.cpp
  searchservice.h
  searchanime2.cpp
  searchanime2.h
)
#  mongoose.c
# mongoose.h

if(WIN32)
  target_link_libraries(animeDownloadConsoleV2 PRIVATE wsock32 ws2_32)
endif()

target_link_libraries(animeDownloadConsoleV2 PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(animeDownloadConsoleV2 PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(animeDownloadConsoleV2 PRIVATE Qt${QT_VERSION_MAJOR}::Network)
#target_link_libraries(animeDownloadConsoleV2 PRIVATE  unofficial::mongoose::mongoose)
