#include "SortingAlgorithms.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm> // Para std::min
#include <iostream>  // Para debug (opcional)


// === BubbleSort ===
BubbleSort::BubbleSort(Visualizer& viz)
  : SortAlgorithm(viz), i(0), j(0), swapped(false) // Chama construtor base
{}

// --- Implementação COMPLETA do step() do BubbleSort ---
bool BubbleSort::step() {
    auto n = m_viz.getSize();
    if (i >= n-1) {
        // Garante que o último elemento também seja marcado como verde ao final
        if (n > 0) m_viz.setHighlight(0, sf::Color::Green);
        return false; // Terminou
    }

    // Comparação dentro da passagem atual
    if (j < n - 1 - i) {
        bool greater = m_viz.getValue(j) > m_viz.getValue(j + 1);
        // Não chama draw aqui, apenas define highlights persistentes se necessário
        // Os highlights temporários serão tratados no loop principal
        // ou poderíamos retornar os highlights desejados.
        // Por simplicidade agora, não fazemos highlights temporários aqui.

        if (greater) {
            m_viz.swapData(j, j + 1);
            swapped = true;
            // Destaca a troca (persistente até a próxima atualização)
            m_viz.setHighlight(j, sf::Color::Red);
            m_viz.setHighlight(j+1, sf::Color::Red);
        } else {
            // Limpa highlights de trocas anteriores (se houver)
             m_viz.setHighlight(j, sf::Color::White);
             // j+1 será verificado/limpo na próxima iteração de j ou no fim da passagem
             if (j+1 == n - 1 - i) { // Se j+1 é o último da passagem atual
                m_viz.setHighlight(j+1, sf::Color::Green); // Marca como ordenado
             } else {
                 m_viz.setHighlight(j+1, sf::Color::White);
             }
        }
        ++j;
    } else { // Fim de uma passagem (j atingiu n-1-i)
        // Marca o último elemento da passagem como 'ordenado' (verde)
        m_viz.setHighlight(n - 1 - i, sf::Color::Green);

        // Se não houve trocas nesta passagem, o array está ordenado
        if (!swapped) {
            // Marca todos os elementos restantes (que ainda não são verdes) como verdes
            for (size_t k = 0; k < n - 1 - i; ++k) {
                 m_viz.setHighlight(k, sf::Color::Green);
            }
            return false; // Terminou
        }

        // Prepara para a próxima passagem
        swapped = false;
        j = 0;
        ++i;
         // Limpa highlight do primeiro elemento da nova passagem
         if (i < n - 1) {
             m_viz.setHighlight(0, sf::Color::White);
         }
    }
    // Não chama m_viz.draw() aqui. Loop principal fará isso.
    return true; // Continua rodando
}


// === InsertionSort ===
InsertionSort::InsertionSort(Visualizer& viz)
  : SortAlgorithm(viz), i(1), j(1), phase(Start) // Chama construtor base
{}

// --- Implementação COMPLETA do step() do InsertionSort ---
bool InsertionSort::step() {
    auto n = m_viz.getSize();
    if (i >= n) return false; // Terminou

    switch(phase) {
        case Start:
            j = i;
            // Marca o elemento 'i' que está sendo inserido
            m_viz.setHighlight(i, sf::Color::Yellow);
            phase = Shifting;
            // Cai para Shifting se j>0
            break; // Sai do switch por agora, próximo step fará o shift

        case Shifting:
            // Compara j com j-1 e troca se necessário
            if (j > 0 && m_viz.getValue(j - 1) > m_viz.getValue(j)) {
                 // Destaca os elementos sendo comparados/trocados
                 m_viz.setHighlight(j - 1, sf::Color::Red);
                 m_viz.setHighlight(j, sf::Color::Red); // Elemento amarelo sendo movido
                 m_viz.swapData(j - 1, j);
                 m_viz.setHighlight(j - 1, sf::Color::White); // Limpa highlight do anterior
                 m_viz.setHighlight(j, sf::Color::Yellow); // Mantém o movido amarelo
                --j;
                 // Se j chegou a 0, ou se o próximo não for maior, vai para MarkSorted
                 if (j == 0 || !(m_viz.getValue(j - 1) > m_viz.getValue(j))) {
                     phase = MarkSorted;
                 }
            } else {
                 // Elemento 'i' (agora em j) está na posição correta
                phase = MarkSorted;
            }
            break; // Sai do switch

        case MarkSorted:
            // Marca a parte ordenada [0..i] como verde
            for (size_t k = 0; k <= i; ++k) {
                m_viz.setHighlight(k, sf::Color::Green);
            }
            ++i; // Passa para o próximo elemento a ser inserido
            phase = Start; // Recomeça o ciclo para o novo 'i'
            break; // Sai do switch
    }
    // Não chama m_viz.draw() aqui. Loop principal fará isso.
    return true; // Continua rodando
}


// === MergeSort ===
MergeSort::MergeSort(Visualizer& viz)
    : SortAlgorithm(viz), // Chama construtor base
      m_n(viz.getSize()),
      m_curr_size(0),
      m_left_start(0),
      m_mid(0),
      m_right_end(0),
      m_i(0),
      m_j(0),
      m_k(0),
      m_temp_buffer(),
      m_phase(Initialize)
{}

// --- Implementação COMPLETA do step() do MergeSort ---
// (Como fornecido anteriormente, mas sem a chamada drawHighlights interna)
bool MergeSort::step() {
    if (m_phase == Done) return false;

    // Variáveis locais para highlights temporários (se necessário passá-los para draw)
    // std::vector<std::pair<int, sf::Color>> current_highlights;

    switch (m_phase) {
        case Initialize:
            m_curr_size = 1;
            m_left_start = 0;
            m_phase = Merging;
            if (m_n <= 1) {
                 m_phase = Done;
                 // Marcar como verde se n=1?
                 if (m_n == 1) m_viz.setHighlight(0, sf::Color::Green);
                 break;
            }
             // Cai para Merging

        case Merging: {
             if (m_k == 0) {
                m_mid = std::min(m_left_start + m_curr_size, m_n);
                m_right_end = std::min(m_left_start + 2 * m_curr_size, m_n);

                if (m_mid >= m_right_end) {
                    m_phase = NextRange;
                    goto next_phase_check; // Usa goto para evitar warnings de fallthrough e reavaliar
                }

                m_i = m_left_start;
                m_j = m_mid;
                m_k = 0;
                 m_temp_buffer.assign(m_right_end - m_left_start, 0);
                 // Limpa highlights anteriores nos ranges atuais (opcional)
                 for(size_t h_idx = m_left_start; h_idx < m_right_end; ++h_idx)
                    m_viz.setHighlight(h_idx, sf::Color::White);
            }

            // Destaca elementos sendo comparados
            if (m_i < m_mid) m_viz.setHighlight(m_i, sf::Color::Blue);
            if (m_j < m_right_end) m_viz.setHighlight(m_j, sf::Color::Cyan);


            if (m_i < m_mid && m_j < m_right_end) {
                if (m_viz.getValue(m_i) <= m_viz.getValue(m_j)) {
                    m_temp_buffer[m_k] = m_viz.getValue(m_i);
                    m_viz.setHighlight(m_i, sf::Color::White); // Limpa highlight após comparação
                    m_i++;
                } else {
                    m_temp_buffer[m_k] = m_viz.getValue(m_j);
                     m_viz.setHighlight(m_j, sf::Color::White); // Limpa highlight
                    m_j++;
                }
            }
            else if (m_i >= m_mid && m_j < m_right_end) {
                m_temp_buffer[m_k] = m_viz.getValue(m_j);
                 m_viz.setHighlight(m_j, sf::Color::White); // Limpa highlight
                m_j++;
            }
            else if (m_j >= m_right_end && m_i < m_mid) {
                 m_temp_buffer[m_k] = m_viz.getValue(m_i);
                 m_viz.setHighlight(m_i, sf::Color::White); // Limpa highlight
                m_i++;
            }
             else {
                 m_phase = CopyingBack;
                 m_k = 0;
                 goto next_phase_check;
             }

            m_k++;

             if (m_k == m_temp_buffer.size()) {
                 m_phase = CopyingBack;
                 m_k = 0;
             }

            break; // Fim do case Merging
        }

        case CopyingBack:
            if (m_k < m_temp_buffer.size()) {
                m_viz.setValue(m_left_start + m_k, m_temp_buffer[m_k]);
                m_viz.setHighlight(m_left_start + m_k, sf::Color::Red); // Destaca cópia (temporário até prox draw)
                m_k++;
            } else {
                 // Limpa highlight vermelho após cópia
                 for(size_t h_idx = m_left_start; h_idx < m_right_end; ++h_idx)
                    m_viz.setHighlight(h_idx, sf::Color::White);
                m_phase = NextRange;
                // Cai para NextRange
            }
             if (m_phase == CopyingBack) break;


        case NextRange:
            m_left_start += 2 * m_curr_size;
            m_k = 0;

            if (m_left_start >= m_n) {
                m_phase = NextPass;
                 // Cai para NextPass
            } else {
                 m_phase = Merging;
                 break;
            }

        case NextPass:
            m_curr_size *= 2;
            m_left_start = 0;
            m_k = 0;

            if (m_curr_size >= m_n) {
                m_phase = Done;
                 for (size_t k_final=0; k_final < m_n; ++k_final) {
                     m_viz.setHighlight(k_final, sf::Color::Green);
                 }
                 return false; // Terminou
            } else {
                m_phase = Merging;
                break;
            }

        case Done:
             return false;

    } // Fim do switch

next_phase_check: // Label para reavaliar após mudança de fase com goto
    if (m_phase == NextRange || m_phase == NextPass || m_phase == CopyingBack || m_phase == Merging) {
         // Se mudou para uma fase que precisa continuar no próximo step, retorna true
         // Ou se for para CopyingBack / Merging e precisa recalcular/iniciar algo
         return true;
    } else if (m_phase == Done) {
         return false; // Se terminou
    }

    // Caso padrão, continua rodando
    return true;
}
// === QuickSort ===
QuickSort::QuickSort(Visualizer& viz)
    : SortAlgorithm(viz)
    , m_done(false)
{
    size_t n = m_viz.getSize();
    if (n > 0) {
        // Inicia com o intervalo completo [0..n-1]
        // Os campos não usados (i, j, pivot) podem ser inicializados com 0 ou qualquer valor.
        m_stack.push_back(
            PartitionState{ 0, n - 1, 0, 0, 0, PartitionState::Phase::PickPivot }
        );
        // Limpa highlights iniciais (opcional)
        for(size_t k=0; k<n; ++k) m_viz.setHighlight(k, sf::Color::White);
    } else {
        m_done = true;
    }
}

bool QuickSort::step() {
    // 1) Nothing left?
    if (m_stack.empty()) {
        if (!m_done) {
            // mark everything green exactly once
            for (size_t k = 0; k < m_viz.getSize(); ++k)
                m_viz.setHighlight(k, sf::Color::Green);
            m_done = true;
        }
        return false;
    }

    // Work on the top of the stack
    auto &st = m_stack.back();

    // Phase 1: pick pivot
    if (st.phase == PartitionState::Phase::PickPivot) {
        if (st.low >= st.high) {
            // zero‐ or one‐element segment → already "sorted"
            if (st.low == st.high)
                m_viz.setHighlight(st.low, sf::Color::Green);
            m_stack.pop_back();
            return !m_stack.empty();
        }

        st.pivot = st.high;
        st.i     = st.low;
        st.j     = st.low;
        st.phase = PartitionState::Phase::Partition;

        // highlight pivot and clear the rest
        m_viz.setHighlight(st.pivot, sf::Color::Cyan);
        for (size_t k = st.low; k < st.high; ++k)
            m_viz.setHighlight(k, sf::Color::White);

        return true;
    }

    // Phase 2: partition loop
    if (st.phase == PartitionState::Phase::Partition) {
        if (st.j < st.high) {
            // compare & maybe swap
            m_viz.setHighlight(st.j, sf::Color::Blue);
            if (m_viz.getValue(st.j) < m_viz.getValue(st.pivot)) {
                m_viz.setHighlight(st.i, sf::Color::Red);
                m_viz.setHighlight(st.j, sf::Color::Red);
                m_viz.swapData(st.i, st.j);
                // clear red on st.i (it'll be white or blue next step)
                m_viz.setHighlight(st.i, sf::Color::White);
                ++st.i;
            }
            ++st.j;
            return true;
        }

        // done partitioning: swap pivot into final place
        size_t pivotPos = st.i;
        // capture the range before we pop
        size_t low  = st.low;
        size_t high = st.high;

        // highlight + swap
        m_viz.setHighlight(pivotPos, sf::Color::Red);
        m_viz.setHighlight(st.high,  sf::Color::Red);
        m_viz.swapData(pivotPos, st.high);
        // now mark the pivot green
        m_viz.setHighlight(pivotPos, sf::Color::Green);
        // clear the old pivot slot if different
        if (pivotPos != high)
            m_viz.setHighlight(high, sf::Color::White);

        // remove this partition task
        m_stack.pop_back();

        // push right then left
        if (pivotPos + 1 < high)
            m_stack.push_back(
              PartitionState{pivotPos + 1, high, 0, 0, 0, PartitionState::Phase::PickPivot}
            );
        if (low < pivotPos)
            m_stack.push_back(
              PartitionState{low, pivotPos - 1, 0, 0, 0, PartitionState::Phase::PickPivot}
            );

        return !m_stack.empty();
    }

    // should never get here
    return false;
}

HeapSort::HeapSort(Visualizer& viz)
  : SortAlgorithm(viz),
    phase(BuildHeap),
    buildIndex(viz.getSize() / 2),
    heapSize(viz.getSize()),
    siftRoot(0)
{}

bool HeapSort::step() {
    auto n = m_viz.getSize();

    // 1) BUILD heap: heapify subtrees from buildIndex−1 down to 0
    if (phase == BuildHeap) {
        if (buildIndex > 0) {
            --buildIndex;
            siftRoot = buildIndex;
            phase    = SiftDown;
            return true;
        } else {
            phase = ExtractMax;
            return true;
        }
    }

    // 2) EXTRACT max: swap root with last element, mark last sorted
    if (phase == ExtractMax) {
        if (heapSize > 1) {
            // highlight root & last
            m_viz.setHighlight(0, sf::Color::Red);
            m_viz.setHighlight(heapSize - 1, sf::Color::Red);

            // swap and mark sorted
            m_viz.swapData(0, heapSize - 1);
            m_viz.setHighlight(heapSize - 1, sf::Color::Green);

            --heapSize;
            siftRoot = 0;
            phase    = SiftDown;
            return true;
        } else {
            // single element left → sorted
            if (heapSize == 1)
                m_viz.setHighlight(0, sf::Color::Green);
            phase = Done;
            return false;
        }
    }

    // 3) SIFT DOWN: restore heap property at siftRoot
    if (phase == SiftDown) {
        size_t left    = 2 * siftRoot + 1;
        size_t right   = left + 1;
        size_t largest = siftRoot;

        // find largest among root and children
        if (left < heapSize &&
            m_viz.getValue(left) > m_viz.getValue(largest))
        {
            largest = left;
        }
        if (right < heapSize &&
            m_viz.getValue(right) > m_viz.getValue(largest))
        {
            largest = right;
        }

        // highlight comparison
        m_viz.setHighlight(siftRoot, sf::Color::Blue);
        m_viz.setHighlight(largest, sf::Color::Cyan);

        if (largest != siftRoot) {
            // swap and continue sifting
            m_viz.setHighlight(siftRoot, sf::Color::Red);
            m_viz.setHighlight(largest,  sf::Color::Red);
            m_viz.swapData(siftRoot, largest);
            siftRoot = largest;
            return true;
        } else {
            // no swap → clear and move on
            m_viz.setHighlight(siftRoot, sf::Color::White);
            // if we were still building, continue build; otherwise extract
            phase = (buildIndex > 0 ? BuildHeap : ExtractMax);
            return true;
        }
    }

    // 4) DONE
    return false;
}