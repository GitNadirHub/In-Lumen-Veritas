#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <vector>
#include "resources.hpp"
#include "animator.hpp"

using namespace sf;

const float maxSpeed=15, acceleration = 0.5, deceleration = 1.f, jumpVelocity = 10.f;
bool isOnGround = 0;
const float g = 0.4;


Clock animClock, deltaClock;

struct Wall
{
    Vector2f position, size;
    RectangleShape hitbox;
    Wall(Vector2f position, Vector2f size)
    {
        this->position = position;
        this->size = size;
        hitbox.setSize(size);
        hitbox.setPosition(position);
    }
};

struct playerObj
{
    RectangleShape hitbox;
    Vector2f position;
    Vector2f velocity;
};

struct Level
{
    std::vector<Wall> walls;
    Vector2f defaultPlayerPos = { 0, 0 };
    Level() {}
    Level(const std::vector<Wall>& ws) : walls(ws) {}
};

Level levels[5];
int levelIndex = 0;

bool isWallDown(playerObj player)
{
    FloatRect playerBounds = player.hitbox.getGlobalBounds();

    player.hitbox.setPosition({player.position.x, player.position.y - player.velocity.y });
    for (auto wall : levels[levelIndex].walls)
    {
        FloatRect wallBounds = wall.hitbox.getGlobalBounds();

        if (playerBounds.findIntersection(wallBounds))
        {
            return true;
        }
    }
    return false;
}

bool isWallUp(playerObj player)
{
    FloatRect playerBounds = player.hitbox.getGlobalBounds();

    for (auto wall : levels[levelIndex].walls)
    {
        FloatRect wallBounds = wall.hitbox.getGlobalBounds();
        if (playerBounds.findIntersection(wallBounds))
        {
            float playerTop = playerBounds.position.y;
            float wallBottom = wallBounds.position.y + wallBounds.size.y;

            if (playerTop <= wallBottom + 5.f && playerTop >= wallBottom - 5.f)
                return true;
        }
    }
    return false;
}

float dt;


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

void updatePlayerPos(playerObj &player)
{

    isOnGround = isWallDown(player);


    Vector2f lastPos = player.position;

    if (player.position.y < 400 && !isOnGround)
        player.velocity.y += g * dt * (1 + !pressJump());
    else
        player.velocity.y = 0.f;

    if (pressJump() && (isOnGround || player.position.y>=400))
    {
        player.velocity.y -= jumpVelocity;
        isOnGround = false;
    }

    if (isWallUp(player))
        player.velocity.y = std::max(player.velocity.y, 0.f);

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

    player.position.x += player.velocity.x*dt;
    player.position.y += player.velocity.y * dt;

    for (auto wall : levels[levelIndex].walls)
    {
        player.hitbox.setPosition(player.position);
        if (!isOnGround)
            if (player.hitbox.getGlobalBounds().findIntersection(wall.hitbox.getGlobalBounds()))
            {
                player.position = { lastPos.x, player.position.y };
                player.hitbox.setPosition(player.position);
                player.velocity.x = 0;
            }
    }
}

void loadLevels()
{
    levels[0].walls =
    {
        Wall({250, 250}, {50, 50}),
        Wall({350, 350}, {500, 500})
    };
}

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({ 720, 480 }), "CMake SFML Project");
    window.setFramerateLimit(144);

    playerObj player;
    player.hitbox.setSize({ 32, 32 });

    RectangleShape ground({720, 50});
    ground.setPosition({ 0, 440 });

    loadLevels();

    while (window.isOpen())
    {

        dt = deltaClock.restart().asSeconds() * 60; //60fps is considered the base

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        updatePlayerPos(player);

        window.clear();
        window.draw(player.hitbox);
        window.draw(ground);
        for (auto wall : levels[levelIndex].walls)
            window.draw(wall.hitbox);
        window.display();
    }
}
