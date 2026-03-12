#include "Game.h"
#include <cmath>
#include <algorithm>

static constexpr float PI = 3.14159265f;

Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pinball",
               sf::Style::Close | sf::Style::Titlebar)
    , m_state(GameState::Start)
    , m_score(0)
    , m_highScore(0)
{
    m_window.setFramerateLimit(60);

    // Font – try to load from assets, fall back to a default if missing
    if (!m_font.loadFromFile("assets/fonts/font.ttf")) {
        // SFML has no built-in font path; we embed a fallback notice silently
        // The game still runs but text will be invisible without a font
    }

    restart();

    // Sounds (optional)
    if (m_hitBuffer.loadFromFile("assets/sounds/hit.wav")) {
        m_hitSound.setBuffer(m_hitBuffer);
    }
    if (m_scoreBuffer.loadFromFile("assets/sounds/score.wav")) {
        m_scoreSound.setBuffer(m_scoreBuffer);
    }
}

void Game::restart()
{
    m_score = 0;

    // Ball starts near the top-centre
    m_ball = std::make_unique<Ball>(BALL_START_X, BALL_START_Y);
    m_ball->setVelocity(120.f, 0.f);

    // Flippers near the bottom – pivot at inner edge
    float flipY = WINDOW_HEIGHT - 100.f;
    float leftX  = static_cast<float>(WINDOW_WIDTH) / 2.f - 10.f;
    float rightX = static_cast<float>(WINDOW_WIDTH) / 2.f + 10.f;
    m_leftFlipper  = std::make_unique<Flipper>(leftX,  flipY, FlipperSide::Left);
    m_rightFlipper = std::make_unique<Flipper>(rightX, flipY, FlipperSide::Right);

    m_physics = std::make_unique<Physics>(
        static_cast<float>(WINDOW_WIDTH),
        static_cast<float>(WINDOW_HEIGHT));

    m_ui = std::make_unique<UI>(
        m_font,
        static_cast<float>(WINDOW_WIDTH),
        static_cast<float>(WINDOW_HEIGHT));

    // Build walls (left, right, top)
    m_walls.clear();
    auto makeWall = [](float x, float y, float w, float h) {
        sf::RectangleShape wall({w, h});
        wall.setPosition(x, y);
        wall.setFillColor(sf::Color(80, 80, 100));
        wall.setOutlineColor(sf::Color(120, 120, 150));
        wall.setOutlineThickness(1.f);
        return wall;
    };
    // Left wall
    m_walls.push_back(makeWall(0.f, 0.f, 20.f, static_cast<float>(WINDOW_HEIGHT)));
    // Right wall
    m_walls.push_back(makeWall(
        static_cast<float>(WINDOW_WIDTH) - 20.f, 0.f,
        20.f, static_cast<float>(WINDOW_HEIGHT)));
    // Top wall
    m_walls.push_back(makeWall(0.f, 0.f, static_cast<float>(WINDOW_WIDTH), 20.f));

    // Bumpers
    m_bumpers.clear();
    auto makeBumper = [](float x, float y, float r, int sc) -> Game::Bumper {
        Game::Bumper b;
        b.shape.setRadius(r);
        b.shape.setOrigin(r, r);
        b.shape.setPosition(x, y);
        b.shape.setFillColor(sf::Color(200, 60, 60));
        b.shape.setOutlineColor(sf::Color(255, 100, 100));
        b.shape.setOutlineThickness(3.f);
        b.score = sc;
        return b;
    };

    float w = static_cast<float>(WINDOW_WIDTH);
    m_bumpers.push_back(makeBumper(w / 2.f,         160.f, 22.f, 100));
    m_bumpers.push_back(makeBumper(w / 2.f - 90.f,  260.f, 18.f,  50));
    m_bumpers.push_back(makeBumper(w / 2.f + 90.f,  260.f, 18.f,  50));
    m_bumpers.push_back(makeBumper(w / 2.f,         350.f, 18.f,  50));
    m_bumpers.push_back(makeBumper(w / 2.f - 120.f, 450.f, 15.f,  25));
    m_bumpers.push_back(makeBumper(w / 2.f + 120.f, 450.f, 15.f,  25));
}

void Game::addScore(int points)
{
    m_score += points;
    if (m_score > m_highScore) {
        m_highScore = m_score;
    }
    if (m_scoreSound.getBuffer()) {
        m_scoreSound.play();
    }
}

void Game::run()
{
    sf::Clock clock;
    while (m_window.isOpen()) {
        float dt = clock.restart().asSeconds();
        // Cap dt to avoid spiral of death on lag spikes
        if (dt > 0.05f) dt = 0.05f;

        processEvents();
        if (m_state == GameState::Playing) {
            update(dt);
        }
        render();
    }
}

void Game::processEvents()
{
    sf::Event event{};
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Space) {
                if (m_state == GameState::Start ||
                    m_state == GameState::GameOver) {
                    restart();
                    m_state = GameState::Playing;
                }
            }
            if (event.key.code == sf::Keyboard::Escape) {
                m_window.close();
            }
        }
    }
}

void Game::update(float dt)
{
    bool leftKey  = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool rightKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

    m_leftFlipper->handleInput(leftKey,   dt);
    m_rightFlipper->handleInput(rightKey, dt);

    m_ball->update(dt);

    // Ball vs bumpers
    for (auto& bumper : m_bumpers) {
        sf::Vector2f ballPos = m_ball->getPosition();
        sf::Vector2f bumperPos = bumper.shape.getPosition();
        float r = m_ball->getRadius() + bumper.shape.getRadius();
        float dx = ballPos.x - bumperPos.x;
        float dy = ballPos.y - bumperPos.y;
        float distSq = dx * dx + dy * dy;
        if (distSq < r * r) {
            float dist = std::sqrt(distSq);
            if (dist < 0.001f) dist = 0.001f;
            float nx = dx / dist;
            float ny = dy / dist;
            // Push out
            float overlap = r - dist;
            m_ball->setPosition(ballPos.x + nx * overlap,
                                ballPos.y + ny * overlap);
            // Reflect
            sf::Vector2f vel = m_ball->getVelocity();
            float dot = vel.x * nx + vel.y * ny;
            vel.x -= 2.f * dot * nx;
            vel.y -= 2.f * dot * ny;
            // Ensure minimum bounce speed
            float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
            if (speed < 350.f) {
                vel *= 350.f / speed;
            }
            m_ball->setVelocity(vel.x, vel.y);
            addScore(bumper.score);
            if (m_hitSound.getBuffer()) {
                m_hitSound.play();
            }
        }
    }

    bool gameOver = m_physics->update(*m_ball, *m_leftFlipper, *m_rightFlipper);
    if (gameOver) {
        m_state = GameState::GameOver;
    }
}

void Game::render()
{
    m_window.clear(sf::Color(20, 20, 40));

    // Draw walls
    for (const auto& wall : m_walls) {
        m_window.draw(wall);
    }

    // Draw bumpers
    for (const auto& bumper : m_bumpers) {
        m_window.draw(bumper.shape);
    }

    m_leftFlipper->draw(m_window);
    m_rightFlipper->draw(m_window);
    m_ball->draw(m_window);

    // HUD always visible while playing or after game over
    m_ui->drawHUD(m_window, m_score, m_highScore);

    if (m_state == GameState::Start) {
        m_ui->drawStartScreen(m_window);
    } else if (m_state == GameState::GameOver) {
        m_ui->drawGameOver(m_window, m_score);
    }

    m_window.display();
}
