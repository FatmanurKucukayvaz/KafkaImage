cmake_minimum_required(VERSION 3.7)
project(kafka_consumer)

set(CMAKE_CXX_STANDARD 11)
set( USE_AVX_INSTRUCTIONS ON )
find_package( OpenCV REQUIRED )
find_package( OpenCV REQUIRED )
find_package( Threads )
find_package( Boost REQUIRED COMPONENTS filesystem system )
include_directories( ${OpenCV_INCLUDE_DIRS} )

link_libraries(${Boost_LIBRARIES} cppkafka ${RDKAFKA_LIBRARY})

include_directories(/home/fatmanur/cppkafka/include)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${RDKAFKA_INCLUDE_DIR})


set(SOURCE_FILES main.cpp)
add_executable(kafka_consumer ${SOURCE_FILES})

target_link_libraries( kafka_consumer ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} Boost::filesystem )