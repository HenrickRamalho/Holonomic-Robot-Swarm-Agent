cmake_minimum_required(VERSION 2.8.3)
project(triangle_description)

find_package(catkin REQUIRED)

catkin_package()

# Install relevant directories
install(DIRECTORY
  config
  def
  launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
