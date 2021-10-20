#ifndef BLOB_H_INCLUDED
#define BLOB_H_INCLUDED

#include "ball.h"

class Blob
{
public:
    static const int LIMIT_BOTTOM = 550;
    static const int RATIO_SPAWN_BOSS = 15;
    static const int RATIO_SPAWN_DEFAULT = 40;
    static const int SPEED_BOSS = 80;
    static const int SPEED_DEFAULT = 100;
    static const int SPEED_SMALL = 120;
    static const int HIT_COUNT_SMALL = 5;
    static const int HIT_COUNT_DEFAULT = 10;
    static const int HIT_COUNT_BOSS = 25;
    static const float SCALING_TIME;
    static const int SCORE_KILLED_SMALL = 200;
    static const int SCORE_KILLED_DEFUALT = 500;
    static const int SCORE_KILLED_BOSS = 2000;
    static const float SPAWN_DELAY_VARRIATION;
    static const float SPAWN_DELAY_SMALL;
    static const float SPAWN_DELAY_DEFAULT;
    static const float SPAWN_DELAY_BOSS;
    static const float HIT_EFFECT_TIME;
    static const int PICKUP_DROP_CHANCE = 25;
    static const int BALL_CORRUPT_CHANCE = 15;
    static const int MAX_AREA_OCCUPIABLE = 300000;

    enum BlobType{SmallBlobType, DefaultBlobType, BossBlobType};

    Blob();

    void Update(float changeInTime_);
    void Draw(sf::RenderWindow& rw);
    void HandleBallCollision(Ball* ball);
    void HandleBlobCollision(Blob* blob);
    sf::FloatRect GetBounds();
    sf::Vector2f GetVelocity();
    BlobType GetType();
    sf::Vector2f GetPosition();
    int GetHitCount();
    float GetArea();
    void Move(float x, float y);
    void ModifyVelocityDirection(short xDir, short yDir);
    bool ShouldDelete();
    void Kill();

private:
    sf::Sprite sprite;
    int hitCount;
    sf::Vector2f velocity;
    float scale;
    float scalingTime;
    float lastHitTime;
    bool shouldDelete;
    BlobType type;
};
#endif // BLOB_H_INCLUDED
