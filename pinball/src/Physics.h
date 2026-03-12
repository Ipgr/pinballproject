#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "Flipper.h"

class Physics {
public:
    Physics(float windowWidth, float windowHeight);

    // Returns true if ball fell below screen (game over)
    bool update(Ball& ball, Flipper& leftFlipper, Flipper& rightFlipper);

private:
    void resolveWallCollisions(Ball& ball);
    bool resolveFlipperCollision(Ball& ball, Flipper& flipper);

    float m_width;
    float m_height;

    static constexpr float WALL_THICKNESS = 20.f;
    static constexpr float FLIPPER_BOUNCE  = 1.3f;
    static constexpr float WALL_BOUNCE     = 0.75f;
};
