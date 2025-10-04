#include "Visualizer.hpp"
#include <algorithm>
#include <random>
#include <iostream>

Visualizer::Visualizer(sf::RenderWindow& window,
    const sf::FloatRect& viewport,
    const std::string& name)
: m_window(window)
, m_view(sf::FloatRect({0.f, 0.f}, {float(WINDOW_WIDTH), float(WINDOW_HEIGHT)}))
, m_name(name)
{
    m_window.setFramerateLimit(FPS);

    if (!m_font.openFromFile("resources/LiberationSans-Regular.ttf")) {
        std::cerr << "Warning: could not load font\n";
    }

    m_view.setViewport(viewport);

    initializeData();

    m_barWidth       = float(WINDOW_WIDTH)  / NUM_ELEMENTS;
    m_maxHeightScale = float(WINDOW_HEIGHT) / NUM_ELEMENTS;
    m_bars.resize(NUM_ELEMENTS);
    m_colors.assign(NUM_ELEMENTS, sf::Color::White);
    updateBars();
}

void Visualizer::initializeData() {
    m_data.resize(NUM_ELEMENTS);
    for (int k = 0; k < NUM_ELEMENTS; ++k)
        m_data[k] = k + 1;
    std::shuffle(m_data.begin(), m_data.end(),
                 std::mt19937{std::random_device{}()});
}


void Visualizer::setValue(int idx, int value) {
    if (idx >= 0 && idx < int(m_data.size())) {
        m_data[idx] = value;
    }

}

size_t Visualizer::getSize() const { return m_data.size(); }
bool   Visualizer::isRunning() const { return m_window.isOpen(); }
int    Visualizer::getValue(int idx) const { return m_data.at(idx); }

void Visualizer::swapData(int i1, int i2) {
    std::swap(m_data[i1], m_data[i2]);
    std::swap(m_colors[i1], m_colors[i2]);
}

void Visualizer::setHighlight(int idx, const sf::Color& color) {
    if (idx >= 0 && idx < int(NUM_ELEMENTS))
        m_colors[idx] = color;
}

void Visualizer::updateBars() {
    for (int k = 0; k < NUM_ELEMENTS; ++k) {
        float h = m_data[k] * m_maxHeightScale;
        m_bars[k].setSize({m_barWidth, h});
        m_bars[k].setPosition({k * m_barWidth, WINDOW_HEIGHT - h});
        m_bars[k].setFillColor(m_colors[k]);
        m_bars[k].setOutlineThickness(1);
        m_bars[k].setOutlineColor(sf::Color(50, 50, 50));
    }
}



void Visualizer::draw(const std::vector<std::pair<int, sf::Color>>& highlights) {
    m_window.setView(m_view);
    sf::Text title(m_font, m_name, 24);
    sf::Vector2f viewSize = m_view.getSize();
    sf::FloatRect textBounds = title.getLocalBounds();


    float centeredX = (viewSize.x / 2.f) - (textBounds.position.x + textBounds.size.x / 2.f);
    float topY = 10.f;

    title.setPosition({centeredX, topY});


    m_window.draw(title);

    updateBars();


    for (auto& hl : highlights) {
        int idx = hl.first;
        if (idx >= 0 && idx < int(m_bars.size())) {
            m_bars[idx].setFillColor(hl.second);
        }
    }
   


    for (auto& bar : m_bars)
        m_window.draw(bar);
}


void Visualizer::setData(const std::vector<int>& data) {
    m_data = data;
    
    m_colors.assign(m_data.size(), sf::Color::White);
    updateBars();  
}
