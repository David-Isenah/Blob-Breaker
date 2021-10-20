#ifndef PICKUP_H_INCLUDED
#define PICKUP_H_INCLUDED

#include "SFML/Graphics.hpp"

class PickUp
{
public:
    static const int SPEED_MOVEMENT = 300;
    static const float TIME_SCALE;
    static const float SCALE_FACTOR;

    enum PickUpType{BigSiZePickUp = 0, SmallSizePickUp, ReversedPickUp, SlowPickUp, RandomPickUp, HealthPickUp, GhostPickUp, BlobPickUp, NumOfPickUps};

    PickUp(sf::Vector2f  position);
    PickUp(sf::Vector2f  position, PickUp::PickUpType type_);
    void Update(float changeInTime_);
    void Draw(sf::RenderWindow &rw);
    sf::FloatRect GetBounds();
    PickUpType GetType();

private:
    sf::Sprite sprite;
    PickUpType type;
    float time_scale;
};

#endif // PICKUP_H_INCLUDED
