#include "Physics.h"
#include <cmath>
#include <algorithm>

Physics::Physics(float windowWidth, float windowHeight)
    : m_width(windowWidth)
    , m_height(windowHeight)
{
}

bool Physics::update(Ball& ball, Flipper& leftFlipper, Flipper& rightFlipper)
{
    resolveWallCollisions(ball);
    resolveFlipperCollision(ball, leftFlipper);
    resolveFlipperCollision(ball, rightFlipper);

    // Game over: ball fell below screen
    return ball.getPosition().y > m_height + ball.getRadius();
}

void Physics::resolveWallCollisions(Ball& ball)
{
    sf::Vector2f pos = ball.getPosition();
    sf::Vector2f vel = ball.getVelocity();
    float r = ball.getRadius();

    // Left wall
    if (pos.x - r < WALL_THICKNESS) {
        ball.setPosition(WALL_THICKNESS + r, pos.y);
        ball.setVelocity(-vel.x * WALL_BOUNCE, vel.y);
        vel = ball.getVelocity();
    }
    // Right wall
    if (pos.x + r > m_width - WALL_THICKNESS) {
        ball.setPosition(m_width - WALL_THICKNESS - r, pos.y);
        ball.setVelocity(-vel.x * WALL_BOUNCE, vel.y);
        vel = ball.getVelocity();
    }
    // Top wall
    if (pos.y - r < WALL_THICKNESS) {
        ball.setPosition(pos.x, WALL_THICKNESS + r);
        ball.setVelocity(vel.x, -vel.y * WALL_BOUNCE);
    }
    // No bottom wall – falling below triggers game over
}

bool Physics::resolveFlipperCollision(Ball& ball, Flipper& flipper)
{
    sf::FloatRect bounds = flipper.getBounds();
    sf::Vector2f ballPos = ball.getPosition();
    float r = ball.getRadius();

    // Broad-phase: enlarged bounds check
    sf::FloatRect expanded = bounds;
    expanded.left   -= r;
    expanded.top    -= r;
    expanded.width  += 2 * r;
    expanded.height += 2 * r;

    if (!expanded.contains(ballPos)) {
        return false;
    }

    // Narrow-phase: closest point on the flipper AABB to ball centre
    float closestX = std::clamp(ballPos.x, bounds.left, bounds.left + bounds.width);
    float closestY = std::clamp(ballPos.y, bounds.top,  bounds.top  + bounds.height);

    float dx = ballPos.x - closestX;
    float dy = ballPos.y - closestY;
    float distSq = dx * dx + dy * dy;

    if (distSq > r * r) {
        return false;
    }

    float dist = std::sqrt(distSq);
    if (dist < 0.001f) {
        dist = 0.001f;
        dx = 0.f;
        dy = -1.f;
    }

    // Push ball out of flipper
    float nx = dx / dist;
    float ny = dy / dist;
    float penetration = r - dist;
    ball.setPosition(ballPos.x + nx * penetration, ballPos.y + ny * penetration);

    // Reflect velocity with a bounce multiplier
    sf::Vector2f vel = ball.getVelocity();
    float dot = vel.x * nx + vel.y * ny;
    if (dot < 0) {
        vel.x -= (1.f + FLIPPER_BOUNCE) * dot * nx;
        vel.y -= (1.f + FLIPPER_BOUNCE) * dot * ny;
        // Add some upward impulse when flipper is being activated
        vel.y -= 150.f;
        ball.setVelocity(vel.x, vel.y);
    }
    return true;
}
