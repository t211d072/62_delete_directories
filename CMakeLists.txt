cmake_minimum_required(VERSION 2.8)
project(delete_directories)
add_definitions("-std=c++11")
 
if(WIN32)
  include_directories(C:/local/include)
  link_directories(C:/local/lib)
else()
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
endif()

add_library(signal_detector SHARED
  lib/file_operator.cpp
)

set(TN1 "delete_directories") 
add_executable(${TN1} src/${TN1}.cpp)
target_link_libraries(${TN1} boost_system boost_filesystem opencv_core opencv_highgui opencv_imgcodecs opencv_imgproc opencv_videoio signal_detector)
