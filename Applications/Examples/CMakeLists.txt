cmake_minimum_required(VERSION 2.8)

add_subdirectory( TestCam )
add_subdirectory( KeyboardCarCommander )
add_subdirectory( WaypointAddition )
if(${BUILD_CAR_PLANNER})
  add_subdirectory( PathPlannerTest )
endif()



