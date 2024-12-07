add_library(Config SHARED ConfigManager.cpp ConfigCSV.cpp)
target_link_libraries(Config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
