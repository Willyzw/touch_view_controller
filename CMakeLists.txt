cmake_minimum_required(VERSION 3.0.2)
project(touch_view_controller)

# Add this to help debug linking issues
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(catkin REQUIRED COMPONENTS
  rviz
  pluginlib
  qt_gui
  std_msgs
  sensor_msgs
  geometry_msgs
  tf2
  tf2_ros
)

## Find Qt5 
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

## This setting causes Qt's "MOC" generation to happen automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES touch_view_controller
  CATKIN_DEPENDS rviz pluginlib std_msgs sensor_msgs geometry_msgs tf2 tf2_ros
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
  src/touch_view_controller.cpp
  include/${PROJECT_NAME}/touch_view_controller.h
)

# Make sure the compiler can find Qt's meta-object compiler output
target_include_directories(${PROJECT_NAME} PRIVATE 
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Explicitly link against Qt libraries
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  Qt5::Core
  Qt5::Widgets
  Qt5::Gui
)

# Add a compile definition for Qt
add_definitions(-DQT_NO_KEYWORDS)

## Install rules
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

install(FILES plugin_description.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)