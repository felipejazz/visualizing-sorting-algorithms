# Sorting Algorithm Visualizer

A C++ application that visualizes various sorting algorithms using SFML (assumed). Built with CMake.

## Description

This project implements several common sorting algorithms and provides a visual representation of how they operate on a dataset. It's designed to help understand the behavior and performance differences between algorithms.

## Features

* Visualization of sorting algorithm steps.
* Implementation of various sorting algorithms (e.g., Bubble Sort, Insertion Sort, Merge Sort, Quick Sort - *Please update with actual implemented algorithms*).
* Configuration options (via `include/Config.hpp`).
* Cross-platform build system using CMake.

## Dependencies

* **CMake** (>= version specified in CMakeLists.txt, *please check*)
* **C++ Compiler** (with C++ standard support, e.g., C++11/14/17/20 - *please check CMakeLists.txt*)
* **SFML Library** (*Assumed based on `Visualizer.hpp` and `.ttf` resource. Please confirm and specify the required version/modules*)
* **GoogleTest** (*Assumed for testing based on `tests/` directory. Please confirm*)

## Building

Use CMake to build and run the project:

```bash

mkdir build
cd build

cmake ..

make
./SortingVisualizer 

``` 