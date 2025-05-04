#include <SFML/Graphics.hpp>
#include "Visualizer.hpp"
#include "Config.hpp"

#include <stdexcept>
#include <thread>
#include <chrono>
#include <random>
#include <algorithm>
#include <iostream>
#include <map>
#include <optional>

Visualizer::Visualizer()
  : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Sorting Visualizer"),
    m_barWidth(0.f),
    m_isRunning(true),
    m_maxHeightScale(0.f)
{
    m_window.setFramerateLimit(60);
    initializeData();
    setupBars();
}

void Visualizer::initializeData() {
    m_data.resize(NUM_ELEMENTS);
    m_barColors.assign(NUM_ELEMENTS, sf::Color::White);
    for (int i = 0; i < NUM_ELEMENTS; ++i)
        m_data[i] = i + 1;

    std::shuffle(m_data.begin(), m_data.end(), std::mt19937{std::random_device{}()});
}

void Visualizer::setupBars() {
    m_bars.resize(NUM_ELEMENTS);
    m_barWidth = static_cast<float>(WINDOW_WIDTH) / NUM_ELEMENTS;
    m_maxHeightScale = static_cast<float>(WINDOW_HEIGHT) / *std::max_element(m_data.begin(), m_data.end());
    updateBarsFromData();
}

void Visualizer::updateBarsFromData() {
    if (m_data.empty()) return;
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        float h = m_data[i] * m_maxHeightScale;
        m_bars[i].setSize({m_barWidth - 1, h});
        m_bars[i].setPosition({i * m_barWidth, WINDOW_HEIGHT - h});
        m_bars[i].setFillColor(m_barColors[i]);
        m_bars[i].setOutlineThickness(1);
        m_bars[i].setOutlineColor(sf::Color(50,50,50));
    }
}

void Visualizer::drawState() {
    drawState(std::vector<std::pair<int, sf::Color>>{});
}

void Visualizer::handleEvents() {
    while (std::optional<sf::Event> ev = m_window.pollEvent()) {
        const sf::Event& event = *ev;
        if (event.is<sf::Event::Closed>()) {
            m_window.close();
            m_isRunning = false;
        }
        if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Escape) {
                m_window.close();
                m_isRunning = false;
            }
        }
    }
}

void Visualizer::setBarColor(int idx, sf::Color c) {
    if (idx>=0 && idx<NUM_ELEMENTS) {
        m_barColors[idx] = c;
        m_bars[idx].setFillColor(c);
    }
}

void Visualizer::drawState(const std::vector<std::pair<int, sf::Color>>& highlights) {
    if (!m_window.isOpen() || !m_isRunning) return;
    handleEvents();
    if (!m_isRunning) return;

    m_window.clear(sf::Color::Black);
    updateBarsFromData();

    std::map<int,sf::Color> orig;
    for (auto& hl : highlights) {
        int i = hl.first;
        if (i>=0 && i<NUM_ELEMENTS) {
            orig[i] = m_bars[i].getFillColor();
            m_bars[i].setFillColor(hl.second);
        }
    }

    for (auto& bar : m_bars)
        m_window.draw(bar);

    m_window.display();
    if (DELAY_MS>0)
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
}

size_t Visualizer::getSize() const {
    return m_data.size();
}

int Visualizer::getValue(int idx) const {
    if (idx<0 || idx>=m_data.size())
        throw std::out_of_range("getValue index");
    return m_data[idx];
}

CompareResult Visualizer::compareIndexes(int i1, int i2, bool show) {
    if (!m_isRunning) return CompareResult::EQUAL;
    if (i1<0||i1>=m_data.size()||i2<0||i2>=m_data.size())
        throw std::out_of_range("compareIndexes");
    if (show) drawState({{i1,sf::Color::Blue},{i2,sf::Color::Cyan}});
    int v1=m_data[i1], v2=m_data[i2];
    if (v1<v2) return CompareResult::LESS;
    if (v1>v2) return CompareResult::GREATER;
    return CompareResult::EQUAL;
}

void Visualizer::swapIndexes(int i1, int i2) {
    if (!m_isRunning || i1==i2) return;
    if (i1<0||i1>=m_data.size()||i2<0||i2>=m_data.size())
        throw std::out_of_range("swapIndexes");
    std::swap(m_data[i1], m_data[i2]);
    std::swap(m_barColors[i1], m_barColors[i2]);
    drawState({{i1, sf::Color::Red},{i2, sf::Color::Red}});
}

void Visualizer::setValueAtIndex(int idx, int value) {
    if (!m_isRunning) return;
    if (idx<0||idx>=m_data.size()) throw std::out_of_range("setValueAtIndex");
    m_data[idx] = value;
    drawState({{idx, sf::Color::Yellow}});
}

void Visualizer::markAsSorted(int idx) {
    if (!m_isRunning) return;
    setBarColor(idx, sf::Color::Green);
    drawState({{idx, sf::Color::Green}});
}

bool Visualizer::isRunning() const {
    return m_isRunning && m_window.isOpen();
}

void Visualizer::run(const std::function<void(Visualizer&)>& sortAlgorithm) {
    drawState();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    try {
        sortAlgorithm(*this);
    } catch (std::exception& e) {
        std::cerr<<"Error during sort: "<<e.what()<<"\n";
    }
    // marca tudo verde ao final
    if (isRunning()) {
        for (int i=0;i<NUM_ELEMENTS;++i)
            setBarColor(i, sf::Color::Green);
        drawState();
        std::cout<<"Sorting complete\n";
    }
    // mantém janela aberta até ESC ou fechar
    while (m_window.isOpen()) {
        handleEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
