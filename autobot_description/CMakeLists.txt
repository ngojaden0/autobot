cmake_minimum_required(VERSION 3.8)
project(autobot_description)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclpy REQUIRED)
find_package(gazebo_ros REQUIRED)
find_package(autobot_gazebo REQUIRED)
find_package(xacro)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(
  DIRECTORY
    urdf
    launch
    rviz
  DESTINATION
    share/${PROJECT_NAME}/
)

install(
  DIRECTORY
    rviz
    urdf
  DESTINATION 
    lib/${PROJECT_NAME}
)

ament_package()