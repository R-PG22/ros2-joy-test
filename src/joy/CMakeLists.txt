cmake_minimum_required(VERSION 3.5)

project(joy)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_ros REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(sdl2_vendor REQUIRED)
find_package(sdl2_custom REQUIRED)
find_package(sensor_msgs REQUIRED)

add_library(joy SHARED src/joy.cpp)
target_include_directories(joy PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>")
target_link_libraries(joy PUBLIC
  rclcpp::rclcpp
  ${sensor_msgs_TARGETS}
  SDL2::SDL2)
target_link_libraries(joy PRIVATE
  rclcpp_components::component)
install(TARGETS joy EXPORT export_joy
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
rclcpp_components_register_node(joy
  PLUGIN "joy::Joy"
  EXECUTABLE joy_node)

install(DIRECTORY include/ DESTINATION include/${PROJECT_NAME})

install(DIRECTORY config launch
  DESTINATION share/${PROJECT_NAME})

ament_export_targets(export_joy)
ament_export_dependencies(
  "rclcpp"
  "sensor_msgs"
  "sdl2_vendor"
  "sdl2_custom")

add_executable(listener src/listener.cpp)
ament_target_dependencies(listener rclcpp sensor_msgs)
target_link_libraries(listener joy)
install(TARGETS
  listener
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
