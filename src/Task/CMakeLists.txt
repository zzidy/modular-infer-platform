add_library(Tasklib SHARED Task.cpp)
target_link_libraries(Tasklib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
