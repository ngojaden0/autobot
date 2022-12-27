cmake_minimum_required(VERSION 3.8)
project(autobot_gazebo)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclpy REQUIRED)
find_package(gazebo_ros)
find_package(autobot_description)
find_package(rclcpp)
find_package(sensor_msgs)
find_package(tf2)
find_package(tf2_ros)
find_package(geometry_msgs)
find_package(xacro)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(encoder_information src/encoder.cpp)
ament_target_dependencies(
  encoder_information 
  rclcpp 
  sensor_msgs
  tf2
  tf2_ros
  geometry_msgs
  autobot_description
)

install(
  DIRECTORY
    launch
    worlds
    meshes
  DESTINATION
    share/${PROJECT_NAME}/
)

install(
  TARGETS
    encoder_information 
  DESTINATION 
    lib/${PROJECT_NAME}
)

ament_package()