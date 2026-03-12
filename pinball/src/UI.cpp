#include "UI.h"
#include <string>
#include <sstream>

UI::UI(const sf::Font& font, float windowWidth, float windowHeight)
    : m_font(font)
    , m_width(windowWidth)
    , m_height(windowHeight)
{
}

static sf::Text makeText(const sf::Font& font, const std::string& str,
                         unsigned int size, sf::Color color)
{
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    return text;
}

void UI::drawHUD(sf::RenderWindow& window, int score, int highScore) const
{
    // Score
    sf::Text scoreText = makeText(m_font, "Score: " + std::to_string(score),
                                  22, sf::Color::White);
    scoreText.setPosition(10.f, 5.f);
    window.draw(scoreText);

    // High score
    sf::Text hiText = makeText(m_font, "Best: " + std::to_string(highScore),
                               22, sf::Color(200, 200, 100));
    sf::FloatRect hiBounds = hiText.getLocalBounds();
    hiText.setPosition(m_width - hiBounds.width - 14.f, 5.f);
    window.draw(hiText);
}

void UI::drawStartScreen(sf::RenderWindow& window) const
{
    // Semi-transparent overlay
    sf::RectangleShape overlay({m_width, m_height});
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    sf::Text title = makeText(m_font, "PINBALL", 60, sf::Color(255, 200, 0));
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    title.setPosition(m_width / 2.f, m_height / 2.f - 80.f);
    window.draw(title);

    sf::Text sub = makeText(m_font, "Press SPACE to start", 28, sf::Color::White);
    sf::FloatRect sb = sub.getLocalBounds();
    sub.setOrigin(sb.left + sb.width / 2.f, sb.top + sb.height / 2.f);
    sub.setPosition(m_width / 2.f, m_height / 2.f);
    window.draw(sub);

    sf::Text controls = makeText(m_font,
        "LEFT / RIGHT arrows to flip\nSPACE to restart",
        20, sf::Color(180, 180, 180));
    sf::FloatRect cb = controls.getLocalBounds();
    controls.setOrigin(cb.left + cb.width / 2.f, cb.top);
    controls.setPosition(m_width / 2.f, m_height / 2.f + 60.f);
    window.draw(controls);
}

void UI::drawGameOver(sf::RenderWindow& window, int score) const
{
    sf::RectangleShape overlay({m_width, m_height});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::Text over = makeText(m_font, "GAME OVER", 56, sf::Color(220, 50, 50));
    sf::FloatRect ob = over.getLocalBounds();
    over.setOrigin(ob.left + ob.width / 2.f, ob.top + ob.height / 2.f);
    over.setPosition(m_width / 2.f, m_height / 2.f - 90.f);
    window.draw(over);

    sf::Text sc = makeText(m_font, "Score: " + std::to_string(score),
                           36, sf::Color::White);
    sf::FloatRect scb = sc.getLocalBounds();
    sc.setOrigin(scb.left + scb.width / 2.f, scb.top + scb.height / 2.f);
    sc.setPosition(m_width / 2.f, m_height / 2.f);
    window.draw(sc);

    sf::Text restart = makeText(m_font, "Press SPACE to restart",
                                24, sf::Color(180, 180, 180));
    sf::FloatRect rb = restart.getLocalBounds();
    restart.setOrigin(rb.left + rb.width / 2.f, rb.top + rb.height / 2.f);
    restart.setPosition(m_width / 2.f, m_height / 2.f + 70.f);
    window.draw(restart);
}
