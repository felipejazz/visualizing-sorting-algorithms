#include "SortingAlgorithms.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

BubbleSort::BubbleSort(Visualizer& viz)
  : SortAlgorithm(viz), i(0), j(0), swapped(false)
{}

bool BubbleSort::step() {
    auto n = m_viz.getSize();
    if (i >= n-1) {
        if (n > 0) m_viz.setHighlight(0, sf::Color::Green);
        return false;
    }

    if (j < n - 1 - i) {
        bool greater = m_viz.getValue(j) > m_viz.getValue(j + 1);

        if (greater) {
            m_viz.swapData(j, j + 1);
            swapped = true;

            m_viz.setHighlight(j, sf::Color::Red);
            m_viz.setHighlight(j+1, sf::Color::Red);
        } else {

             m_viz.setHighlight(j, sf::Color::White);

             if (j+1 == n - 1 - i) {
                m_viz.setHighlight(j+1, sf::Color::Green);
             } else {
                 m_viz.setHighlight(j+1, sf::Color::White);
             }
        }
        ++j;
    } else {

        m_viz.setHighlight(n - 1 - i, sf::Color::Green);

        if (!swapped) {
            for (size_t k = 0; k < n - 1 - i; ++k) {
                 m_viz.setHighlight(k, sf::Color::Green);
            }
            return false;
        }

        swapped = false;
        j = 0;
        ++i;
         if (i < n - 1) {
             m_viz.setHighlight(0, sf::Color::White);
         }
    }

    return true;
}




InsertionSort::InsertionSort(Visualizer& viz)
  : SortAlgorithm(viz), i(1), j(1), phase(Start)
{}

bool InsertionSort::step() {
    auto n = m_viz.getSize();
    if (i >= n) return false;

    switch(phase) {
        case Start:
            j = i;

            m_viz.setHighlight(i, sf::Color::Yellow);
            phase = Shifting;

            break;

        case Shifting:

            if (j > 0 && m_viz.getValue(j - 1) > m_viz.getValue(j)) {

                 m_viz.setHighlight(j - 1, sf::Color::Red);
                 m_viz.setHighlight(j, sf::Color::Red);
                 m_viz.swapData(j - 1, j);
                 m_viz.setHighlight(j - 1, sf::Color::White);
                 m_viz.setHighlight(j, sf::Color::Yellow);
                --j;
                 if (j == 0 || !(m_viz.getValue(j - 1) > m_viz.getValue(j))) {
                     phase = MarkSorted;
                 }
            } else {
                phase = MarkSorted;
            }
            break;

        case MarkSorted:
            for (size_t k = 0; k <= i; ++k) {
                m_viz.setHighlight(k, sf::Color::Green);
            }
            ++i;
            phase = Start;
            break;
    }
    return true;
}


// === MergeSort ===
MergeSort::MergeSort(Visualizer& viz)
    : SortAlgorithm(viz),
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

bool MergeSort::step() {
    if (m_phase == Done) return false;

    switch (m_phase) {
        case Initialize:
            m_curr_size = 1;
            m_left_start = 0;
            m_phase = Merging;
            if (m_n <= 1) {
                 m_phase = Done;
                 if (m_n == 1) m_viz.setHighlight(0, sf::Color::Green);
                 break;
            }

        case Merging: {
             if (m_k == 0) {
                m_mid = std::min(m_left_start + m_curr_size, m_n);
                m_right_end = std::min(m_left_start + 2 * m_curr_size, m_n);

                if (m_mid >= m_right_end) {
                    m_phase = NextRange;
                    goto next_phase_check;
                }

                m_i = m_left_start;
                m_j = m_mid;
                m_k = 0;
                 m_temp_buffer.assign(m_right_end - m_left_start, 0);
                 for(size_t h_idx = m_left_start; h_idx < m_right_end; ++h_idx)
                    m_viz.setHighlight(h_idx, sf::Color::White);
            }

            if (m_i < m_mid) m_viz.setHighlight(m_i, sf::Color::Blue);
            if (m_j < m_right_end) m_viz.setHighlight(m_j, sf::Color::Cyan);

            if (m_i < m_mid && m_j < m_right_end) {
                if (m_viz.getValue(m_i) <= m_viz.getValue(m_j)) {
                    m_temp_buffer[m_k] = m_viz.getValue(m_i);
                    m_viz.setHighlight(m_i, sf::Color::White);
                    m_i++;
                } else {
                    m_temp_buffer[m_k] = m_viz.getValue(m_j);
                     m_viz.setHighlight(m_j, sf::Color::White);
                    m_j++;
                }
            }
            else if (m_i >= m_mid && m_j < m_right_end) {
                m_temp_buffer[m_k] = m_viz.getValue(m_j);
                 m_viz.setHighlight(m_j, sf::Color::White);
                m_j++;
            }
            else if (m_j >= m_right_end && m_i < m_mid) {
                 m_temp_buffer[m_k] = m_viz.getValue(m_i);
                 m_viz.setHighlight(m_i, sf::Color::White);
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

            break;
        }

        case CopyingBack:
            if (m_k < m_temp_buffer.size()) {
                m_viz.setValue(m_left_start + m_k, m_temp_buffer[m_k]);
                m_viz.setHighlight(m_left_start + m_k, sf::Color::Red);
                m_k++;
            } else {
                 for(size_t h_idx = m_left_start; h_idx < m_right_end; ++h_idx)
                    m_viz.setHighlight(h_idx, sf::Color::White);
                m_phase = NextRange;
            }
             if (m_phase == CopyingBack) break;

        case NextRange:
            m_left_start += 2 * m_curr_size;
            m_k = 0;

            if (m_left_start >= m_n) {
                m_phase = NextPass;
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
                 return false;
            } else {
                m_phase = Merging;
                break;
            }

        case Done:
             return false;

    }

next_phase_check:
    if (m_phase == NextRange || m_phase == NextPass || m_phase == CopyingBack || m_phase == Merging) {
         return true;
    } else if (m_phase == Done) {
         return false;
    }

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