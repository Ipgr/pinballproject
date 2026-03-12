#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include "Ball.h"
#include "Flipper.h"
#include "Physics.h"
#include "UI.h"

enum class GameState { Start, Playing, GameOver };

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void restart();
    void addScore(int points);

    sf::RenderWindow m_window;
    sf::Font         m_font;

    std::unique_ptr<Ball>    m_ball;
    std::unique_ptr<Flipper> m_leftFlipper;
    std::unique_ptr<Flipper> m_rightFlipper;
    std::unique_ptr<Physics> m_physics;
    std::unique_ptr<UI>      m_ui;

    // Bumpers
    struct Bumper {
        sf::CircleShape shape;
        int             score;
    };
    std::vector<Bumper> m_bumpers;

    // Sounds (optional – loaded if files exist)
    sf::SoundBuffer m_hitBuffer;
    sf::SoundBuffer m_scoreBuffer;
    sf::Sound       m_hitSound;
    sf::Sound       m_scoreSound;

    // Walls
    std::vector<sf::RectangleShape> m_walls;

    GameState m_state;
    int       m_score;
    int       m_highScore;

    static constexpr unsigned int WINDOW_WIDTH  = 480;
    static constexpr unsigned int WINDOW_HEIGHT = 720;
    static constexpr float        BALL_START_X  = WINDOW_WIDTH / 2.f;
    static constexpr float        BALL_START_Y  = 200.f;
};
