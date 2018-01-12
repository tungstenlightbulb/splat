add_executable(splat
    anf.cpp
    boundary_file.cpp
    city_file.cpp
    dem.cpp
    elevation_map.cpp
    gnuplot.cpp
    itwom3.0.cpp
    kml.cpp
    lrp.cpp
    main.cpp
    pat_file.cpp
    path.cpp
    ppm.cpp
    region.cpp
    report.cpp
    sdf.cpp
    site.cpp
    splat_run.cpp
    udt.cpp
    utilities.cpp)
target_link_libraries(splat bz2)
install(TARGETS splat DESTINATION bin)
