add_executable(tester tester.cpp)
add_executable(generator generator.cpp)
add_executable(route_class_tester test_route_class.cpp)

target_link_libraries(tester vrp)
target_link_libraries(generator vrp)
target_link_libraries(route_class_tester vrp)