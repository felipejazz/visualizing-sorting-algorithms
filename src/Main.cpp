#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <random>
#include <algorithm>
#include <optional>

#include "Config.hpp"
#include "Visualizer.hpp"
#include "SortingAlgorithms.hpp"

int main() {
    // --- Obter resolução do Desktop ---
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // --- Criar janela ---
    sf::RenderWindow window(desktop, "Sorting Visualizer");
    // Para fullscreen:
    // sf::RenderWindow window(desktop, "Sorting Visualizer", sf::Style::Fullscreen);

    window.setFramerateLimit(FPS);

    // --- Nomes dos algoritmos ---
    std::vector<std::string> names = {
        "Bubble Sort", 
        "Insertion Sort", 
        "Merge Sort",
        "Quick Sort",
        "Heap Sort"
    };
    size_t n = names.size();

    // --- Computar viewports em grid de 2 colunas ---
    const int cols = 2;
    int rows = (n + cols - 1) / cols;  // ceil(n / cols)

    std::vector<sf::FloatRect> viewports;
    viewports.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        int r = i / cols;           // linha
        int c = i % cols;           // coluna
        viewports.emplace_back(
            sf::FloatRect(
                { float(c) / cols,   // x offset
                  float(r) / rows }, // y offset
                { 1.f / cols,        // largura
                  1.f / rows }       // altura
            )
        );
    }

    // --- Gerar dados iniciais compartilhados ---
    std::vector<int> baseData(NUM_ELEMENTS);
    std::iota(baseData.begin(), baseData.end(), 1);
    std::shuffle(
        baseData.begin(), baseData.end(),
        std::mt19937{ std::random_device{}() }
    );

    // --- Criar visualizadores ---
    std::vector<std::unique_ptr<Visualizer>> vizs;
    vizs.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        vizs.emplace_back(
            std::make_unique<Visualizer>(window, viewports[i], names[i])
        );
        vizs.back()->setData(baseData);
    }

    // --- Instanciar algoritmos ---
    std::vector<std::unique_ptr<SortAlgorithm>> algos;
    algos.emplace_back(std::make_unique<BubbleSort>(*vizs[0]));
    algos.emplace_back(std::make_unique<InsertionSort>(*vizs[1]));
    algos.emplace_back(std::make_unique<MergeSort>(*vizs[2]));
    algos.emplace_back(std::make_unique<QuickSort>(*vizs[3]));
    algos.emplace_back(std::make_unique<HeapSort>(*vizs[4]));

    bool isPaused   = false;
    bool needsReset = false;
    bool anyRunning = true;

    while (window.isOpen()) {
        // --- Eventos ---
        while (auto evOpt = window.pollEvent()) {
            const auto& event = *evOpt;
            if (event.is<sf::Event::Closed>()) {
                window.close();
            } else if (event.is<sf::Event::KeyPressed>()) {
                auto code = event.getIf<sf::Event::KeyPressed>()->code;
                if (code == sf::Keyboard::Key::Escape) {
                    window.close();
                } else if (code == sf::Keyboard::Key::P) {
                    isPaused = !isPaused;
                    std::cout << (isPaused ? "Paused\n" : "Resumed\n");
                } else if (code == sf::Keyboard::Key::R) {
                    needsReset = true;
                    isPaused   = false;
                    std::cout << "Resetting...\n";
                }
            }
        }

        // --- Reset logic ---
        if (needsReset) {
            std::iota(baseData.begin(), baseData.end(), 1);
            std::shuffle(baseData.begin(), baseData.end(),
                         std::mt19937{std::random_device{}()});
            for (size_t i = 0; i < n; ++i) {
                vizs[i]->setData(baseData);
                if (names[i] == "Bubble Sort") {
                    algos[i] = std::make_unique<BubbleSort>(*vizs[i]);
                }
                else if (names[i] == "Insertion Sort") {
                    algos[i] = std::make_unique<InsertionSort>(*vizs[i]);
                }
                else if (names[i] == "Merge Sort") {
                    algos[i] = std::make_unique<MergeSort>(*vizs[i]);
                }
                else if (names[i] == "Quick Sort") {
                    algos[i] = std::make_unique<QuickSort>(*vizs[i]);
                }
                else if (names[i] == "Heap Sort") {
                    algos[i] = std::make_unique<HeapSort>(*vizs[i]);
                }
            }
            anyRunning = true;
            needsReset = false;
        }
        
        window.clear(sf::Color::Black);

        // --- Update & Draw ---
        if (!isPaused && anyRunning) {
            bool stillRunning = false;
            for (size_t i = 0; i < n; ++i) {
                if (algos[i] && algos[i]->step())
                    stillRunning = true;
            }
            anyRunning = stillRunning;

            for (auto &vz : vizs)
                vz->draw();

            if (anyRunning)
                sf::sleep(sf::milliseconds(DELAY_MS));
        } else {
            for (auto &vz : vizs)
                vz->draw();
            if (isPaused)
                sf::sleep(sf::milliseconds(50));
        }

        window.display();
    }

    return 0;
}
