#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include "Config.hpp"

class Visualizer {
public:
    // window: janela compartilhada
    // viewport: fração [0..1] x [0..1] indicando coluna
    // name: nome do algoritmo
    Visualizer(sf::RenderWindow& window,
               const sf::FloatRect& viewport,
               const std::string& name);

    size_t getSize() const;
    bool   isRunning() const;
    int    getValue(int idx) const;
    void setData(const std::vector<int>& data);
    void setValue(int idx, int value); // <<< ADICIONE ESTA LINHA
    // troca só nos dados e cores
    void swapData(int i1, int i2);
    // permite destacar uma célula (não altera dado)
    void setHighlight(int idx, const sf::Color& color);

    // desenha BARRAS + nome no topo
    // highlights: pares (idx,cor)
    void draw(const std::vector<std::pair<int, sf::Color>>& highlights = {});
    void initializeData();
private:
    sf::RenderWindow&        m_window;
    sf::View                 m_view;
    std::string              m_name;
    sf::Font                 m_font;
    std::vector<int>         m_data;
    std::vector<sf::RectangleShape> m_bars;
    std::vector<sf::Color>   m_colors;
    float                    m_barWidth;
    float                    m_maxHeightScale;

    
    void updateBars();
};
