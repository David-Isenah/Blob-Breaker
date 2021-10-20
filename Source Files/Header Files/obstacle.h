#ifndef OBSTACLE_H_INCLUDED
#define OBSTACLE_H_INCLUDED

#include "SFML/Graphics.hpp"

class Obstacle
{
public:
    static const int SPEED = 80;
    static const float MIN_SPAWN_TIME;
    static const float SPAWN_TIME_VARIATION;
    static const float SPAWN_TIME_VARIATION_FAST;
    static const int NUM_OF_TEXTURE = 4;

    Obstacle();

    void Update(float changeInTime_);
    void Draw(sf::RenderWindow& rw);
    bool IsOutOfScreen();

private:
    sf::Sprite sprite;
    float velocityY;
};

#endif // OBSTACLE_H_INCLUDED
