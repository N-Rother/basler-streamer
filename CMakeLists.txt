cmake_minimum_required(VERSION 3.21)
project(basler_streamer)

set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )

find_package( Pylon QUIET )
if (NOT ${Pylon_FOUND})
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Findpylon.cmake")
endif()

include_directories(
        ${Pylon_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
)

add_executable(Sender src/Sender.cpp src/BaslerCapture.cpp src/BaslerCapture.h)
add_executable(Receiver src/Receiver.cpp)

target_link_libraries(Sender ${OpenCV_LIBS} ${Pylon_LIBRARIES})
target_link_libraries(Receiver ${OpenCV_LIBS})
