file(GLOB VRP_SOURCE "src/*.cpp")

add_executable(vrp ${VRP_SOURCE})

target_include_directories(vrp PUBLIC "include")

find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(vrp PUBLIC tuning ${Boost_LIBRARIES})