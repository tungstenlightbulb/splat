add_executable(splat main.cpp splat_run.cpp dem.cpp path.cpp itwom3.0.cpp)
target_link_libraries(splat bz2)
install(TARGETS splat DESTINATION bin)
