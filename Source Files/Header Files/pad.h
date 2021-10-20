#ifndef PAD_H_INCLUDED
#define PAD_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "ball.h"
#include "pickup.h"

class Pad
{
public:
    static const int MOVEMENT_SPEED = 8000;
    static const int MAX_SPEED = 800;
    static const int REDIRECT_DISTANCE = 50;
    static const int SCORE_BALL_HIT = 100;
    static const int SCORE_CORRUPT_BALL_HIT = 100;
    static const float EFFECT_DISPLAY_DURATION;

    enum EffectType{BigSizeEffect = 0, SmallSizeEffect, ReversedEffect, SlowEffect, GhostEffect, NumOfEffectType};

    Pad();

    void Draw(sf::RenderWindow& window_);
    void Update(float changeInTime_);
    void HandleBallCollision(Ball* ball);
    bool HandlePickUpCollision(PickUp* pickup);
    void ActivateEffect(EffectType effect_);

private:
    sf::Sprite sprite;
    float velocityX;

    float effect_time[NumOfEffectType];
    float effect_change_time;
    short numOfActiveEffects;
    sf::Sprite sprite_effect;
    sf::Sound sound_pickup;
};

#endif // PAD_H_INCLUDED
