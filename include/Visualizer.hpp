#ifndef SORTING_ALGORITHMS_VISUALIZER_HPP
#define SORTING_ALGORITHMS_VISUALIZER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <utility>

#include "Config.hpp"

enum class CompareResult { LESS = -1, EQUAL = 0, GREATER = 1 };

class Visualizer {
public:
    Visualizer();

    size_t getSize() const;
    int    getValue(int index) const;

    CompareResult compareIndexes(int i1, int i2, bool show = true);

    void swapIndexes(int i1, int i2);
    void setValueAtIndex(int idx, int value);
    void markAsSorted(int idx);

    bool isRunning() const;

    // Remove the “={}” default here and supply an overload instead
    void drawState(const std::vector<std::pair<int, sf::Color>>& highlights);
    void drawState();  

    void run(const std::function<void(Visualizer&)>& sortAlgorithm);

private:
    void initializeData();
    void setupBars();
    void handleEvents();
    void updateBarsFromData();
    void setBarColor(int index, sf::Color color);

    sf::RenderWindow            m_window;
    std::vector<int>            m_data;
    std::vector<sf::RectangleShape> m_bars;
    std::vector<sf::Color>      m_barColors;
    float                       m_barWidth    = 0.f;
    bool                        m_isRunning   = true;
    float                       m_maxHeightScale = 0.f;
};

#endif // SORTING_ALGORITHMS_VISUALIZER_HPP
