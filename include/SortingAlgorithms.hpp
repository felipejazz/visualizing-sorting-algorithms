#pragma once
#include "Visualizer.hpp"
#include <vector> // Para std::vector

// Classe base abstrata (SE NÃO EXISTIR, CRIE ALGO ASSIM)
class SortAlgorithm {
public:
    virtual ~SortAlgorithm() = default;
    virtual bool step() = 0; // Executa um passo e retorna true se ainda rodando
protected:
    Visualizer& m_viz;
    SortAlgorithm(Visualizer& viz) : m_viz(viz) {}
};


// === BubbleSort (Declaração existente) ===
class BubbleSort : public SortAlgorithm {
public:
    BubbleSort(Visualizer& viz);
    bool step() override;
private:
    size_t i, j;
    bool swapped;
};

// === InsertionSort (Declaração existente) ===
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


// === MergeSort (Nova Declaração) ===
class MergeSort : public SortAlgorithm {
private:
    // Fases do algoritmo iterativo
    enum Phase { Initialize, Merging, CopyingBack, NextRange, NextPass, Done };

    size_t m_n;             // Tamanho total do array
    size_t m_curr_size;     // Tamanho atual dos subarrays a serem mesclados
    size_t m_left_start;    // Início do subarray esquerdo atual

    // Variáveis para o passo de merge
    size_t m_mid;
    size_t m_right_end;
    size_t m_i;             // Índice para subarray esquerdo
    size_t m_j;             // Índice para subarray direito
    size_t m_k;             // Índice para buffer temporário
    std::vector<int> m_temp_buffer; // Buffer auxiliar para merge

    Phase m_phase;          // Fase atual do algoritmo

    // Função auxiliar interna para desenhar highlights
    void drawHighlights();

public:
    MergeSort(Visualizer& viz);
    bool step() override;
};


// === QuickSort ===
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
        
            // Perform one atomic step. Return true if more work remains.
            bool step() override;
        
        private:
            enum Phase { BuildHeap, ExtractMax, SiftDown, Done };
            Phase phase;
        
            // buildIndex: which subtree to heapify during build
            // heapSize:   current “active” heap size during extract
            // siftRoot:   root index of the current sift-down
            size_t buildIndex;
            size_t heapSize;
            size_t siftRoot;
        };