add_library(gamelib DefinitionLoading.cpp)
target_include_directories(gamelib PUBLIC ../include)

add_executable(parsertest parsertest.cpp)
target_link_libraries(parsertest gamelib)
