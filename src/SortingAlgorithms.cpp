#include "SortingAlgorithms.hpp"
#include "Visualizer.hpp"
#include <vector>
#include <utility>


void bubbleSort(Visualizer& viz) {
    size_t n = viz.getSize(); 

    if (n <= 1) {
        if (n == 1) viz.markAsSorted(0);
        return;
    }

    bool swapped;
    for (size_t i = 0; i < n - 1; ++i) {
    
        if (!viz.isRunning()) return;

        swapped = false; 

        // Passagem interna: compara elementos adjacentes
        // O limite superior j < n - 1 - i otimiza, pois os últimos 'i' elementos já estão ordenados
        for (size_t j = 0; j < n - 1 - i; ++j) {
            // Verifica frequentemente se a visualização deve parar
            if (!viz.isRunning()) return;

            // Compara os elementos nos índices j e j+1
            // viz.compareIndexes já desenha o estado com destaque azul/ciano
            if (viz.compareIndexes(j, j + 1) == CompareResult::GREATER) {
                // Se m_data[j] > m_data[j+1], troca-os
                // viz.swapIndexes já desenha o estado com destaque vermelho
                viz.swapIndexes(j, j + 1);
                swapped = true; // Indica que uma troca ocorreu nesta passagem
            }
        } // Fim do loop interno (j)

        // Após cada passagem completa do loop interno, o elemento na posição (n - 1 - i)
        // está garantido como sendo o maior elemento da porção não ordenada,
        // então ele está em sua posição final correta.
        // Marca esse elemento como ordenado (verde).
        viz.markAsSorted(n - 1 - i);

        if (!swapped) {
             // Marca todos os elementos restantes (que ainda não foram marcados) como ordenados
             // pois sabemos que o array inteiro está ordenado agora.
             // Os elementos de n-1-i até 0 ainda não foram explicitamente marcados.
             for(size_t k = 0; k < n - 1 - i; ++k) {
                 if (!viz.isRunning()) return; // Verifica antes de marcar
                 viz.markAsSorted(k);
             }
            break;
        }

    }

    if (viz.isRunning() && n > 0) {
         viz.markAsSorted(0);
    }
}