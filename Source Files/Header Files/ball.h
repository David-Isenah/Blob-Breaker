#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class Ball
{
public:
    static const int MIN_SPEED = 200;
    static const int MAX_SPEED = 400;
    static const float MIN_SPAWN_TIME;
    static const float MAX_SPAWN_TIME;
    static const int MIN_SPAWN_ANGLE = 30;
    static const int RATIO_CORRUPT = 20;
    static const int SCORE_WALL_HIT = 50;
    static const int MAX_CORRUPT_COUNT = 6;

    enum BallType{RegularBallType, CorruptBallType};

    Ball();

    void Update(float changeInTime_);
    void Draw(sf::RenderWindow& rw);

    void Move(float x, float y);
    void ModifyVelocityDirection(short xDir, short yDir);
    void SetVelocity(sf::Vector2f velocity_);
    BallType GetType();
    sf::FloatRect GetBounds();
    sf::Vector2f GetPosition();
    sf::Vector2f GetVelocity();
    float GetVelocityMagn();
    bool IsBallOutOfScreen();
    bool IsPadCollidable();
    void ActivatePadCollision();

    void Corrupt();
    void Uncorrupt();

private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float velocityMagn;
    int corruptCount;
    bool padCollide;
    BallType type;
};

#endif // BALL_H_INCLUDED
