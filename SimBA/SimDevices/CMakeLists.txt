cmake_minimum_required(VERSION 2.8)

#add_custom_command("${CMAKE_CURRENT_SOURCE_DIR}/lcmod.xml")


add_subdirectory(Sensor) # comment this if you don't want to use network
add_subdirectory(Controller)
