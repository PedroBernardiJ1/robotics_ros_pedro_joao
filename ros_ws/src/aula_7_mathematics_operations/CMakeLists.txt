cmake_minimum_required(VERSION 3.8)
project(mathematics_operations)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_interfaces REQUIRED)

add_executable(add_ints_server_node src/add_ints_server_node.cpp)
ament_target_dependencies(add_ints_server_node rclcpp custom_interfaces)

add_executable(add_ints_cliente_node src/add_ints_cliente_node.cpp)
ament_target_dependencies(add_ints_cliente_node rclcpp custom_interfaces)

install(TARGETS
  add_ints_server_node
  add_ints_cliente_node
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
