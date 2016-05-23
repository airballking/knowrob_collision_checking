cmake_minimum_required(VERSION 2.8.3)
project(knowrob_moveit)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
  roscpp 
  roslib
  moveit_core 
  message_generation
  message_runtime
  moveit_msgs
  visualization_msgs
  sensor_msgs
)

find_package(PkgConfig)
pkg_check_modules(SWIPL REQUIRED swipl)
message( "SWI prolog INCLUDE_DIRS " ${SWIPL_INCLUDE_DIRS} )

add_message_files(
  FILES
  ContactList.msg
)

add_service_files(
  FILES
  CheckCollisions.srv
  UInt64Trigger.srv
)

generate_messages(
  DEPENDENCIES
  sensor_msgs
  moveit_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES knowrob_moveit_predicates
  CATKIN_DEPENDS message_runtime moveit_msgs visualization_msgs sensor_msgs
  DEPENDS swipl
)

include_directories(
  include 
  ${catkin_INCLUDE_DIRS}
  ${SWIPL_INCLUDE_DIRS})

link_directories(${SWIPL_LIBRARY_DIRS})

add_library(knowrob_moveit_predicates src/${PROJECT_NAME}/prolog_predicates.cpp)
add_dependencies(knowrob_moveit_predicates ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(knowrob_moveit_predicates ${catkin_LIBRARIES} ${SWIPL_LIBRARIES})

add_executable(planning_scene_server src/${PROJECT_NAME}/planning_scene_server.cpp)
add_dependencies(planning_scene_server ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(planning_scene_server ${catkin_LIBRARIES})

add_executable(planning_scene_client src/${PROJECT_NAME}/planning_scene_client.cpp)
add_dependencies(planning_scene_client ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(planning_scene_client ${catkin_LIBRARIES})

add_executable(contact_marker_visualizer src/${PROJECT_NAME}/contact_marker_visualizer.cpp)
add_dependencies(contact_marker_visualizer ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(contact_marker_visualizer ${catkin_LIBRARIES})


## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_knowrob_collision_checking.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
