#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "resources.hpp"
#include "animator.hpp"

using namespace sf;

const float maxSpeed=15, acceleration = 0.5, deceleration = 1.f, jumpVelocity = 26.f;
bool isOnGround = 0;
const float g = 0.4;

Clock animClock, deltaClock;

struct playerObj
{
    RectangleShape hitbox;
    Vector2f position;
    Vector2f velocity;
};

bool pressRight()
{
    if (Keyboard::isKeyPressed(Keyboard::Key::Right))
        return true;
    if (Keyboard::isKeyPressed(Keyboard::Key::D))
        return true;
    return false;
}

bool pressLeft()
{
    if (Keyboard::isKeyPressed(Keyboard::Key::Left))
        return true;
    if (Keyboard::isKeyPressed(Keyboard::Key::A))
        return true;
    return false;
}

bool pressJump()
{
    if (Keyboard::isKeyPressed(Keyboard::Key::Up))
        return true;
    if (Keyboard::isKeyPressed(Keyboard::Key::W))
        return true;
    return false;
}

void updatePlayerPos(playerObj &player, const float dt)
{

    if (player.position.y < 400)
        player.velocity.y += g * dt * (1 + !pressJump());
    else
        player.position.y = 400, player.velocity.y = 0.f, isOnGround = true;

    if (pressJump() && isOnGround)
    {
        player.velocity.y -= jumpVelocity * dt;
        isOnGround = false;
    }

    if (pressRight())
    {
        player.velocity.x += acceleration * dt;
        if (player.velocity.x > maxSpeed)
            player.velocity.x = maxSpeed;
    }
    else if (pressLeft())
    {
        player.velocity.x -= acceleration * dt;
        if (player.velocity.x < -maxSpeed)
            player.velocity.x = -maxSpeed;
    }
    else
    {
        if (player.velocity.x > 0)
        {
            player.velocity.x -= deceleration * dt;
            if (player.velocity.x < 0)
                player.velocity.x = 0;
        }
        else if (player.velocity.x < 0)
        {
            player.velocity.x += deceleration * dt;
            if (player.velocity.x > 0)
                player.velocity.x = 0;
        }
    }
    player.position += player.velocity*dt;

    player.hitbox.setPosition(player.position);
}

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({ 720, 480 }), "CMake SFML Project");
    window.setFramerateLimit(144);

    playerObj player;
    player.hitbox.setSize({ 50, 50 });

    while (window.isOpen())
    {

        float dt = deltaClock.restart().asSeconds() * 60; //60fps is considered the base

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        updatePlayerPos(player, dt);

        window.clear();
        window.draw(player.hitbox);
        window.display();
    }
}
