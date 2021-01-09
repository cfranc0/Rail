#LEONARDO PINTON

cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)

project ("Rail")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/line_description.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/timetables.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable (
  ${PROJECT_NAME}

  Line/RailLine.cpp
  Station/Station.cpp
  Train/Train.cpp
  Rail_Class/Rail.cpp
  Rail_Class/coutLine.cpp
  Rail_Class/simulate.cpp

  main.cpp
)
