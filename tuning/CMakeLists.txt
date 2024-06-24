file(GLOB TUNING_SRC "src/*.cpp")

add_library(tuning STATIC ${TUNING_SRC})
target_include_directories(tuning PUBLIC "include")