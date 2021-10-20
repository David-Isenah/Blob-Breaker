#include "ball.h"
#include "game.h"
#include <math.h>

const float Ball::MIN_SPAWN_TIME = 1.f;
const float Ball::MAX_SPAWN_TIME = 3.f;

Ball::Ball()
{
    if(Game::GenerateRandomNumber(100) <= RATIO_CORRUPT)
    {
        type = CorruptBallType;
        corruptCount = 1 + Game::GenerateRandomNumber(MAX_CORRUPT_COUNT - 1);
    }
    else
    {
        type = RegularBallType;
        corruptCount = -1;
    }
    padCollide = false;

    if(type == CorruptBallType)
        sprite.setTexture(*Game::GetTexture("ball_corrupt"), true);
    else sprite.setTexture(*Game::GetTexture("ball"), true);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);

    velocityMagn = MIN_SPEED + Game::GenerateRandomNumber(MAX_SPEED - MIN_SPEED);
    float projAngle = Game::GenerateRandomNumber(180 - MIN_SPAWN_ANGLE * 2);
    float angleInRadian = -(MIN_SPAWN_ANGLE + projAngle) * (22.f / 7.f) / 180.f;
    velocity.x = velocityMagn * cos(angleInRadian);
    velocity.y = velocityMagn * sin(angleInRadian);

    sprite.setPosition(Game::GenerateRandomNumber(Game::SCREEN_WIDTH), Game::SCREEN_HEIGHT + sprite.getGlobalBounds().height);
}

void Ball::Update(float changeInTime_)
{
    sprite.move(velocity * changeInTime_);

    if(sprite.getGlobalBounds().left < Game::WALL_LEFT)
    {
        sprite.setPosition(Game::WALL_LEFT + sprite.getGlobalBounds().width / 2.f, sprite.getPosition().y);
        if(velocity.x < 0)
            velocity.x = -velocity.x;
        Game::IncrementGameScore(SCORE_WALL_HIT);
    }
    if(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width > Game::WALL_RIGHT)
    {
        sprite.setPosition(Game::WALL_RIGHT - sprite.getGlobalBounds().width / 2.f, sprite.getPosition().y);
        if(velocity.x > 0)
            velocity.x = -velocity.x;
        Game::IncrementGameScore(SCORE_WALL_HIT);
    }
    if(sprite.getGlobalBounds().top < Game::WALL_TOP)
    {
        sprite.setPosition(sprite.getPosition().x, Game::WALL_TOP + sprite.getGlobalBounds().height / 2.f);
        if(velocity.y < 0)
            velocity.y = -velocity.y;
        Game::IncrementGameScore(SCORE_WALL_HIT);
        padCollide = true;
    }
}

void Ball::Draw(sf::RenderWindow& rw)
{
    if(padCollide)
        sprite.setColor(sf::Color::White);
    else sprite.setColor(sf::Color(255, 255, 255, 70));

    rw.draw(sprite);
}

void Ball::Move(float x, float y)
{
    sprite.move(x, y);
}

bool Ball::IsPadCollidable()
{
    return padCollide;
}

void Ball::ActivatePadCollision()
{
    padCollide = true;
}

sf::Vector2f Ball::GetPosition()
{
    return sprite.getPosition();
}

Ball::BallType Ball::GetType()
{
    return type;
}

void Ball::SetVelocity(sf::Vector2f velocity_)
{
    velocity = velocity_;
}

float Ball::GetVelocityMagn()
{
    return velocityMagn;
}

sf::Vector2f Ball::GetVelocity()
{
    return velocity;
}

void Ball::ModifyVelocityDirection(short xDir, short yDir)
{
    if(xDir != 0)
        velocity.x *= xDir / std::abs(xDir);
    if(yDir != 0)
        velocity.y *= yDir / std::abs(yDir);
}

sf::FloatRect Ball::GetBounds()
{
    return sprite.getGlobalBounds();
}

bool Ball::IsBallOutOfScreen()
{
    if(sprite.getGlobalBounds().top + sprite.getGlobalBounds().height < 0 && velocity.y < 0)
        return true;
    if(sprite.getGlobalBounds().top > Game::SCREEN_HEIGHT && velocity.y > 0)
        return true;
    return false;
}

void Ball::Uncorrupt()
{
    if(corruptCount <= 0)
    {
        type = RegularBallType;
        sprite.setTexture(*Game::GetTexture("ball"), true);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    }
    else corruptCount--;
}

void Ball::Corrupt()
{
    type = CorruptBallType;
    sprite.setTexture(*Game::GetTexture("ball_corrupt"), true);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
}
