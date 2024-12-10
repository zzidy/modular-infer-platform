add_library(PreProcess SHARED PreProcessMan.cpp PreProcess.cpp)
target_link_libraries(PreProcess PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
