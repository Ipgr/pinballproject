#pragma once
#include <SFML/Graphics.hpp>

class UI {
public:
    explicit UI(const sf::Font& font, float windowWidth, float windowHeight);

    void drawHUD(sf::RenderWindow& window, int score, int highScore) const;
    void drawStartScreen(sf::RenderWindow& window) const;
    void drawGameOver(sf::RenderWindow& window, int score) const;

private:
    const sf::Font& m_font;
    float m_width;
    float m_height;
};
