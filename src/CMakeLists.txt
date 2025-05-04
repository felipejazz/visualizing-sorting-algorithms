add_library(visualizer_lib
    Visualizer.cpp
    Main.cpp
    SortingAlgorithms.cpp

)

target_include_directories(visualizer_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(visualizer_lib PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
)
