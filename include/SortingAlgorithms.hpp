#pragma once
#include "Visualizer.hpp"
#include <vector> 


class SortAlgorithm {
public:
    virtual ~SortAlgorithm() = default;
    virtual bool step() = 0;
protected:
    Visualizer& m_viz;
    SortAlgorithm(Visualizer& viz) : m_viz(viz) {}
};


class BubbleSort : public SortAlgorithm {
public:
    BubbleSort(Visualizer& viz);
    bool step() override;
private:
    size_t i, j;
    bool swapped;
};

class InsertionSort : public SortAlgorithm {
private:
    enum Phase { Start, Shifting, MarkSorted };
public:
    InsertionSort(Visualizer& viz);
    bool step() override;
private:
    size_t i, j;
    Phase phase;
};


class MergeSort : public SortAlgorithm {
private:
    // Fases do algoritmo iterativo
    enum Phase { Initialize, Merging, CopyingBack, NextRange, NextPass, Done };

    size_t m_n; 
    size_t m_curr_size;
    size_t m_left_start;

    
    size_t m_mid;
    size_t m_right_end;
    size_t m_i;             
    size_t m_j;             
    size_t m_k;             
    std::vector<int> m_temp_buffer;

    Phase m_phase;          

    void drawHighlights();

public:
    MergeSort(Visualizer& viz);
    bool step() override;
};


class QuickSort : public SortAlgorithm {
    private:
        struct PartitionState {
            size_t low, high;
            size_t i, j;
            size_t pivot;
            enum Phase { PickPivot, Partition } phase;
        };
        std::vector<PartitionState> m_stack;
        bool m_done;
    
    public:
        QuickSort(Visualizer& viz);
        bool step() override;
    };
    

    class HeapSort : public SortAlgorithm {
        public:
            explicit HeapSort(Visualizer& viz);
        
            bool step() override;
        
        private:
            enum Phase { BuildHeap, ExtractMax, SiftDown, Done };
            Phase phase;
        
            size_t buildIndex;
            size_t heapSize;
            size_t siftRoot;
        };