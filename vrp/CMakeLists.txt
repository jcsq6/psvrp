file(GLOB VRP_SOURCE "src/*.cpp")

add_executable(vrp ${VRP_SOURCE})
target_include_directories(vrp PUBLIC "include")
target_link_libraries(vrp PUBLIC tuning)