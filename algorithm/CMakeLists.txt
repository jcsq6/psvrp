file(GLOB ALGORITHM_SOURCE "src/*.cpp")

add_executable(algorithm ${ALGORITHM_SOURCE})

target_include_directories(algorithm PUBLIC "include")

find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(algorithm PUBLIC vrp ${Boost_LIBRARIES})