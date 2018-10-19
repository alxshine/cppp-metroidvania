add_library(gamelib DefinitionLoading.cpp LoadAllDefinitions.cpp)
target_include_directories(gamelib PUBLIC ../include)
target_link_libraries(gamelib stdc++fs)

add_executable(parsertest parsertest.cpp)
target_link_libraries(parsertest gamelib)
