cmake_minimum_required(VERSION 3.15)

project(SortingVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)


add_subdirectory(src)

add_executable(SortingVisualizer src/Main.cpp)

target_link_libraries(SortingVisualizer PRIVATE
    visualizer_lib
    sfml-graphics
    sfml-window
    sfml-system
)


option(BUILD_TESTING "Build tests" ON)

if(BUILD_TESTING)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(tests)
endif()


set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
set(RESOURCES_DESTINATION "${CMAKE_BINARY_DIR}/resources")

if(EXISTS ${RESOURCES_DIR})
    file(GLOB ASSET_FILES "${RESOURCES_DIR}/*.*")
    if(ASSET_FILES)
        # --- CORREÇÃO APLICADA AQUI ---
        add_custom_command(TARGET SortingVisualizer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${RESOURCES_DESTINATION}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ASSET_FILES} ${RESOURCES_DESTINATION}
            COMMENT "Copying RESOURCES..."
        )
    else()
        message(WARNING "RESOURCES directory exists at ${RESOURCES_DIR}, but no files matching '*.*' were found. Skipping asset copying.")
    endif()
else()
    message(WARNING "RESOURCES directory not found at ${RESOURCES_DIR}. Skipping asset copying.")
endif()

include(GNUInstallDirs)
install(TARGETS SortingVisualizer DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS visualizer_lib ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})