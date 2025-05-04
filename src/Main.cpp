// src/Main.cpp
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <optional> // Necessário para o retorno de std::optional<sf::Event> em SFML 3
#include <functional>
#include <iostream>
#include <algorithm>

#include "Visualizer.hpp"
#include "SortingAlgorithms.hpp"

enum class AppState { MainMenu, Visualizing };

// Declarações dos manipuladores de eventos
void handleMenuEvents(
    sf::RenderWindow& window,
    const sf::Event& event,
    AppState& currentState,
    std::vector<std::string>& selectedAlgorithms,
    const std::vector<std::string>& availableAlgorithms,
    const std::vector<sf::RectangleShape>& algoButtons,
    std::vector<sf::RectangleShape>& mutableAlgoButtons,
    const sf::RectangleShape& startButton,
    std::unique_ptr<Visualizer>& viz,
    const std::vector<std::function<void(Visualizer&)>>& algorithmFunctions
);

void handleVisualizationEvents(
    sf::RenderWindow& window,
    const sf::Event& event,
    AppState& currentState
);

int main() {
    // Janela principal
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Sorting Visualizer");
    window.setFramerateLimit(60);

    AppState currentState = AppState::MainMenu;

    // Nomes e funções dos algoritmos
    std::vector<std::string> availableAlgorithms = { "Bubble Sort" };
    std::vector<std::function<void(Visualizer&)>> algorithmFunctions = { bubbleSort };
    std::vector<std::string> selectedAlgorithms;

    // Carrega fonte
    sf::Font font;
    if (!font.openFromFile("resources/LiberationSans-Regular.ttf")) {
        std::cerr << "Erro: não foi possível carregar a fonte." << std::endl;
        return EXIT_FAILURE;
    }

    // Configura botões de seleção
    float startY = 100.f, btnH = 50.f, btnW = 200.f, spacing = 20.f;
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonTexts;
    for (size_t i = 0; i < availableAlgorithms.size(); ++i) {
        sf::RectangleShape btn({btnW, btnH});
        btn.setPosition({50.f, startY + i * (btnH + spacing)});
        btn.setFillColor(sf::Color::Blue);
        btn.setOutlineColor(sf::Color::Black);
        btn.setOutlineThickness(1.f);
        buttons.push_back(btn);

        sf::Text txt(font, availableAlgorithms[i], 20);
        auto bounds = txt.getLocalBounds();
        txt.setOrigin({bounds.position.x + bounds.size.x/2.f, bounds.position.y + bounds.size.y/2.f});
        txt.setPosition({btn.getPosition().x + btnW/2.f, btn.getPosition().y + btnH/2.f});
        txt.setFillColor(sf::Color::White);
        buttonTexts.push_back(txt);
    }

    // Botão Start
    sf::RectangleShape startButton({btnW, btnH});
    startButton.setPosition({
        window.getSize().x/2.f - btnW/2.f,
        startY + availableAlgorithms.size() * (btnH + spacing) + 50.f
    }
    );
    startButton.setFillColor(sf::Color::Green);
    sf::Text startText(font, "Start", 20);
    auto sb = startText.getLocalBounds();
    startText.setOrigin({sb.position.x + sb.size.x/2.f, sb.position.y + sb.size.y/2.f});
    startText.setPosition({startButton.getPosition().x + btnW/2.f,
                          startButton.getPosition().y + btnH/2.f});
    startText.setFillColor(sf::Color::White);

    std::unique_ptr<Visualizer> viz = nullptr;

    // Loop principal
    while (window.isOpen()) {
        // Eventos
        while (std::optional<sf::Event> ev = window.pollEvent()) {
            const sf::Event& event = *ev;
            if (event.is<sf::Event::Closed>())
                window.close();

            switch (currentState) {
                case AppState::MainMenu:
                    handleMenuEvents(
                        window, event, currentState,
                        selectedAlgorithms, availableAlgorithms,
                        buttons, buttons,
                        startButton,
                        viz,
                        algorithmFunctions
                    );
                    break;

                case AppState::Visualizing:
                    handleVisualizationEvents(window, event, currentState);
                    break;
            }
        }

        // Desenho
        window.clear(sf::Color::White);
        if (currentState == AppState::MainMenu) {
            // Título
            sf::Text title(font, "Select Algorithm", 30);
            auto tb = title.getLocalBounds();
            title.setOrigin({tb.position.x + tb.size.x/2.f, tb.position.y + tb.size.y/2.f});
            title.setPosition({window.getSize().x/2.f, 50.f});
            title.setFillColor(sf::Color::Black);
            window.draw(title);

            // Botões
            for (size_t i = 0; i < buttons.size(); ++i) {
                window.draw(buttons[i]);
                window.draw(buttonTexts[i]);
            }
            window.draw(startButton);
            window.draw(startText);

        } else if (currentState == AppState::Visualizing && viz) {
            // Enquanto estiver visualizando, nada no main.
        }

        window.display();
    }

    return 0;
}

void handleMenuEvents(
    sf::RenderWindow& window,
    const sf::Event& event,
    AppState& currentState,
    std::vector<std::string>& selectedAlgorithms,
    const std::vector<std::string>& availableAlgorithms,
    const std::vector<sf::RectangleShape>& algoButtons,
    std::vector<sf::RectangleShape>& mutableAlgoButtons,
    const sf::RectangleShape& startButton,
    std::unique_ptr<Visualizer>& viz,
    const std::vector<std::function<void(Visualizer&)>>& algorithmFunctions
) {
    if (const auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            sf::Vector2f mp = window.mapPixelToCoords(mb->position);
            // seleção dos algoritmos
            for (size_t i = 0; i < algoButtons.size(); ++i) {
                if (algoButtons[i].getGlobalBounds().contains(mp)) {
                    const auto& name = availableAlgorithms[i];
                    auto it = std::find(selectedAlgorithms.begin(), selectedAlgorithms.end(), name);
                    if (it != selectedAlgorithms.end()) {
                        selectedAlgorithms.erase(it);
                        mutableAlgoButtons[i].setFillColor(sf::Color::Blue);
                    } else {
                        selectedAlgorithms.push_back(name);
                        mutableAlgoButtons[i].setFillColor(sf::Color::Red);
                    }
                }
            }
            // start
            if (startButton.getGlobalBounds().contains(mp) && !selectedAlgorithms.empty()) {
                size_t idx = std::distance(
                    availableAlgorithms.begin(),
                    std::find(availableAlgorithms.begin(), availableAlgorithms.end(), selectedAlgorithms[0])
                );
                viz = std::make_unique<Visualizer>();
                viz->run(algorithmFunctions[idx]);
                viz.reset();
                selectedAlgorithms.clear();
                currentState = AppState::MainMenu;
            }
        }
    } else if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        if (kp->code == sf::Keyboard::Key::Escape) {
            window.close();
        }
    }
}

void handleVisualizationEvents(
    sf::RenderWindow& window,
    const sf::Event& event,
    AppState& currentState
) {
    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        if (kp->code == sf::Keyboard::Key::Escape) {
            currentState = AppState::MainMenu;
        }
    }
}
