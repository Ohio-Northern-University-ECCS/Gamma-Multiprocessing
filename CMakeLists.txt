cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( DisplayImage DisplayImage.cpp )
add_executable( DisplayImageCut DisplayImageCut.cpp )
add_executable( DisplayImageShmem DisplayImageShmem.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )
target_link_libraries( DisplayImageCut ${OpenCV_LIBS} )
target_link_libraries( DisplayImageShmem ${OpenCV_LIBS} )
