file(GLOB VRP_SOURCE "src/*.cpp")

add_library(vrp STATIC ${VRP_SOURCE})

target_include_directories(vrp PUBLIC "include")
target_link_libraries(vrp PUBLIC tuning)