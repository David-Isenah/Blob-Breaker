#include "blob.h"
#include "game.h"

const float Blob::SCALING_TIME = 1.f;
const float Blob::HIT_EFFECT_TIME = 0.1f;
const float Blob::SPAWN_DELAY_VARRIATION = 2.f;
const float Blob::SPAWN_DELAY_SMALL = 1.f;
const float Blob::SPAWN_DELAY_DEFAULT = 3.f;
const float Blob::SPAWN_DELAY_BOSS = 5.f;

Blob::Blob()
{
    scale = 0;
    shouldDelete = false;
    scalingTime = 0.f;
    lastHitTime = 0.f;

    int ratioType = Game::GenerateRandomNumber(100);
    if(ratioType <= RATIO_SPAWN_BOSS)
        type = BossBlobType;
    else if(ratioType <= RATIO_SPAWN_BOSS + RATIO_SPAWN_DEFAULT)
        type = DefaultBlobType;
    else type = SmallBlobType;

    if(type == BossBlobType)
    {
        sprite.setTexture(*Game::GetTexture("blob_boss"), true);
        hitCount = HIT_COUNT_BOSS;
    }
    else if(type == DefaultBlobType)
    {
        sprite.setTexture(*Game::GetTexture(Game::GenerateRandomNumber(1) ? "blob_default1" : "blob_default2"), true);
        hitCount = HIT_COUNT_DEFAULT;
    }
    else
    {
        sprite.setTexture(*Game::GetTexture("blob_small"), true);
        hitCount = HIT_COUNT_SMALL;
    }

    sprite.setTextureRect(sf::IntRect(0, 0, sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height));

    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    sprite.setPosition(Game::GenerateRandomNumber(Game::SCREEN_WIDTH), Game::WALL_TOP + Game::GenerateRandomNumber(LIMIT_BOTTOM - Game::WALL_TOP));
    sprite.setScale(scale, scale);

    float velocityMagn = SPEED_SMALL;
    if(type == BossBlobType)
        velocityMagn = SPEED_BOSS;
    else if(type == DefaultBlobType)
        velocityMagn = SPEED_DEFAULT;

    float projAngle = Game::GenerateRandomNumber(360);
    float angleInRadian = projAngle * (22.f / 7.f) / 180.f;
    velocity.x = velocityMagn * cos(angleInRadian);
    velocity.y = velocityMagn * sin(angleInRadian);
}

void Blob::Draw(sf::RenderWindow& rw)
{
    rw.draw(sprite);
}

sf::FloatRect Blob::GetBounds()
{
    return sprite.getGlobalBounds();
}

sf::Vector2f Blob::GetVelocity()
{
    return velocity;
}

sf::Vector2f Blob::GetPosition()
{
    return sprite.getPosition();
}

void Blob::Kill()
{
    hitCount = 0;
    sprite.setColor(sf::Color(255, 255, 255, 100));
    sprite.setTextureRect(sf::IntRect(sprite.getTextureRect().width, 0, sprite.getTextureRect().width, sprite.getTextureRect().height));
}

Blob::BlobType Blob::GetType()
{
    return type;
}

int Blob::GetHitCount()
{
    return hitCount;
}

void Blob::Move(float x, float y)
{
    sprite.move(x, y);
}

float Blob::GetArea()
{
    return sprite.getTextureRect().width * sprite.getTextureRect().height;
}

void Blob::HandleBallCollision(Ball* ball)
{
    sf::Vector2f moveVec;
    if(Game::HandleRectCollision(GetBounds(), ball->GetBounds(), moveVec))
    {
        moveVec /= 2.f;
        Move(moveVec.x, moveVec.y);

        moveVec = -moveVec;
        ball->Move(moveVec.x, moveVec.y);

        bool itHit = false;
        if(moveVec.x < 0 && ball->GetVelocity().x > 0 || moveVec.x > 0 && ball->GetVelocity().x < 0)
        {
            ball->ModifyVelocityDirection(-1, 0);
            if(Game::GenerateRandomNumber(100) <= BALL_CORRUPT_CHANCE)
                ball->Corrupt();
            itHit = true;
        }
        if(moveVec.y < 0 && ball->GetVelocity().y > 0 || moveVec.y > 0 && ball->GetVelocity().y < 0)
        {
            if(Game::GenerateRandomNumber(100) <= BALL_CORRUPT_CHANCE && ball->GetVelocity().y > 0)
                ball->Corrupt();
            ball->ModifyVelocityDirection(0, -1);
            itHit = true;
        }

        if(itHit)
        {
            hitCount--;
            lastHitTime = HIT_EFFECT_TIME;
            if(hitCount <= 0)
            {
                sprite.setColor(sf::Color(255, 255, 255, 100));
                sprite.setTextureRect(sf::IntRect(sprite.getTextureRect().width, 0, sprite.getTextureRect().width, sprite.getTextureRect().height));
            }
            ball->ActivatePadCollision();
        }
    }
}

void Blob::HandleBlobCollision(Blob* blob)
{
    sf::Vector2f moveVec;
    Game::HandleRectCollision(GetBounds(), blob->GetBounds(), moveVec);

    moveVec /= 2.f;
    Move(moveVec.x , moveVec.y);
    if((moveVec.x > 0 && GetVelocity().x < 0) || (moveVec.x < 0 && GetVelocity().x > 0))
        ModifyVelocityDirection(-1, 0);
    if((moveVec.y > 0 && GetVelocity().y < 0) || (moveVec.y < 0 && GetVelocity().y > 0))
        ModifyVelocityDirection(0, -1);

    moveVec = -moveVec;
    blob->Move(moveVec.x , moveVec.y);
    if((moveVec.x > 0 && blob->GetVelocity().x < 0) || (moveVec.x < 0 && blob->GetVelocity().x > 0))
        blob->ModifyVelocityDirection(-1, 0);
    if((moveVec.y > 0 && blob->GetVelocity().y < 0) || (moveVec.y < 0 && blob->GetVelocity().y > 0))
        blob->ModifyVelocityDirection(0, -1);
}

void Blob::ModifyVelocityDirection(short xDir, short yDir)
{
    if(xDir != 0)
        velocity.x *= xDir / std::abs(xDir);
    if(yDir != 0)
        velocity.y *= yDir / std::abs(yDir);
}

void Blob::Update(float changeInTime_)
{
    if(hitCount > 0)
    {
        if(scalingTime < SCALING_TIME)
        {
            scalingTime += changeInTime_;
            if(scalingTime > SCALING_TIME)
                scalingTime = SCALING_TIME;

            scale = scalingTime / SCALING_TIME;
        }

        if(lastHitTime > 0.f)
        {
            lastHitTime -= changeInTime_;
            if(lastHitTime < 0.f)
                lastHitTime = 0.f;

            sprite.setColor(sf::Color(255, 255, 255, 100));
        }
        else sprite.setColor(sf::Color::White);

        sprite.move(velocity * changeInTime_);
        if(sprite.getGlobalBounds().left < Game::WALL_LEFT)
        {
            sprite.setPosition(Game::WALL_LEFT + sprite.getGlobalBounds().width / 2.f, sprite.getPosition().y);
            if(velocity.x < 0)
                velocity.x = -velocity.x;
        }
        if(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width > Game::WALL_RIGHT)
        {
            sprite.setPosition(Game::WALL_RIGHT - sprite.getGlobalBounds().width / 2.f, sprite.getPosition().y);
            if(velocity.x > 0)
                velocity.x = -velocity.x;
        }
        if(sprite.getGlobalBounds().top < Game::WALL_TOP)
        {
            sprite.setPosition(sprite.getPosition().x, Game::WALL_TOP + sprite.getGlobalBounds().height / 2.f);
            if(velocity.y < 0)
                velocity.y = -velocity.y;
        }
        if(sprite.getGlobalBounds().top + sprite.getGlobalBounds().height > LIMIT_BOTTOM)
        {
            sprite.setPosition(sprite.getPosition().x, LIMIT_BOTTOM - sprite.getGlobalBounds().height / 2.f);
            if(velocity.y > 0)
                velocity.y = -velocity.y;
        }
    }
    else
    {
        if(scalingTime > 0)
        {
            scalingTime -= changeInTime_;
            if(scalingTime < 0)
                scalingTime = 0;

            scale = scalingTime / SCALING_TIME;

            if(scalingTime == 0)
                shouldDelete = true;
        }
    }

    sprite.setScale(scale, scale);
}

bool Blob::ShouldDelete()
{
    return shouldDelete;
}
