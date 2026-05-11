#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class ClashInterface {
private:
    sf::RenderWindow window;
    sf::Texture texLoading, texArena;
    sf::Sprite sprBackground;
    sf::Font font;
    std::vector<sf::Text> menuOptions;

    enum class GameState { LOADING_MENU, ARENA, RANKING };
    GameState currentState;

    void adjustBackground(const sf::Texture& texture) {
        sprBackground.setTexture(texture);
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u textureSize = texture.getSize();
        sprBackground.setScale(
            static_cast<float>(windowSize.x) / textureSize.x,
            static_cast<float>(windowSize.y) / textureSize.y
        );
    }

    void setupMenu() {
        menuOptions.clear();
        std::vector<std::string> names = { "JUGARrrr", "RANKING", "SALIR" };

        // AJUSTA ESTOS VALORES SI SIGUEN SIN ENCAJAR:
        // Estos números son la altura (Y) de cada tablón de madera.
        float positionsY[] = { 275.f, 360.f, 440.f };

        for (int i = 0; i < names.size(); ++i) {
            sf::Text text;
            text.setFont(font);
            text.setString(names[i]);
            text.setCharacterSize(40); // Tamaño ajustado para los tablones
            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(3);

            // --- LÓGICA DE CENTRADO CRÍTICA ---
            // 1. Calculamos el área que ocupa el texto
            sf::FloatRect textRect = text.getLocalBounds();
            // 2. Ponemos el "punto de agarre" (origin) en el centro exacto del texto
            text.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);

            // 3. Lo posicionamos en el centro de la pantalla (640) y en su altura Y
            text.setPosition(640.f, positionsY[i]);

            menuOptions.push_back(text);
        }
    }

public:
    ClashInterface() : currentState(GameState::LOADING_MENU) {
        // La normativa exige Visual Studio 2026 [cite: 100]
        window.create(sf::VideoMode(1280, 720), "Archon Royale - ETSIDI Edition");
        window.setFramerateLimit(60);
    }

    bool loadAssets() {
        // Carga de recursos obligatoria [cite: 176, 190]
        if (!texLoading.loadFromFile("assets/loading_royale.png") ||
            !texArena.loadFromFile("assets/arena_grass.png") ||
            !font.loadFromFile("assets/arial.ttf")) {
            return false;
        }

        adjustBackground(texLoading);
        setupMenu();
        return true;
    }

    void handleInput() {
        sf::Event event;
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (currentState == GameState::LOADING_MENU) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < menuOptions.size(); ++i) {
                        if (menuOptions[i].getGlobalBounds().contains(mousePos)) {
                            if (i == 0) { // JUGAR
                                currentState = GameState::ARENA;
                                adjustBackground(texArena);
                            }
                            else if (i == 2) { // SALIR
                                window.close();
                            }
                        }
                    }
                }
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                currentState = GameState::LOADING_MENU;
                adjustBackground(texLoading);
            }
        }

        // Efecto hover y personalización visual [cite: 180, 184]
        if (currentState == GameState::LOADING_MENU) {
            for (auto& text : menuOptions) {
                if (text.getGlobalBounds().contains(mousePos)) {
                    text.setFillColor(sf::Color(255, 200, 0)); // Dorado Clash Royale
                }
                else {
                    text.setFillColor(sf::Color::White);
                }
            }
        }
    }

    void run() {
        while (window.isOpen()) {
            handleInput();
            window.clear();
            window.draw(sprBackground);

            if (currentState == GameState::LOADING_MENU) {
                for (const auto& text : menuOptions) {
                    window.draw(text);
                }
            }

            window.display();
        }
    }
};

int main() {
    ClashInterface game;
    if (game.loadAssets()) {
        game.run();
    }
    return 0;
}