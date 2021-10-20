#include "obstacle.h"
#include "game.h"

const float Obstacle::MIN_SPAWN_TIME = 1.f;
const float Obstacle::SPAWN_TIME_VARIATION = 60.f;
const float Obstacle::SPAWN_TIME_VARIATION_FAST = 5.f;
short textureIndex = 1;

Obstacle::Obstacle()
{
    velocityY = SPEED;

    sprite.setTexture(*Game::GetTexture("obstacle" + Game::ConvertNumberToString(textureIndex)));
    sprite.setPosition(Game::GenerateRandomNumber(Game::SCREEN_WIDTH - 380), -sprite.getTextureRect().height);

    textureIndex++;
    if(textureIndex > NUM_OF_TEXTURE)
        textureIndex = 1;
}

void Obstacle::Update(float changeInTime_)
{
    sprite.move(0, velocityY * changeInTime_);
}

bool Obstacle::IsOutOfScreen()
{
    if(sprite.getGlobalBounds().top > Game::SCREEN_HEIGHT)
        return true;
    return false;
}

void Obstacle::Draw(sf::RenderWindow& rw)
{
    rw.draw(sprite);
}
