#include "pickup.h"
#include "game.h"

const int PickUpChance[PickUp::NumOfPickUps] = {25, 10, 5, 5, 15, 15, 20, 5};
const float PickUp::TIME_SCALE = 1.5f;
const float PickUp::SCALE_FACTOR = 0.2f;

PickUp::PickUp(sf::Vector2f  position, PickUp::PickUpType type_)
{
    time_scale = 0.f;
    type = type_;

    sprite.setTexture(*Game::GetTexture("pickups"));
    sprite.setTextureRect(sf::IntRect(type * 64, 0, 64, 64));
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    sprite.setPosition(position);
}

PickUp::PickUp(sf::Vector2f  position)
{
    time_scale = 0.f;
    int chance = Game::GenerateRandomNumber(100);

    int currentPickUpChance = 0;
    for(int i = 0; i < NumOfPickUps; i++)
    {
        currentPickUpChance += PickUpChance[i];
        if(chance <= currentPickUpChance)
        {
            type = (PickUpType)i;
            break;
        }
    }

    sprite.setTexture(*Game::GetTexture("pickups"));
    sprite.setTextureRect(sf::IntRect(type * 64, 0, 64, 64));
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    sprite.setPosition(position);
}

sf::FloatRect PickUp::GetBounds()
{
    return sprite.getGlobalBounds();
}

PickUp::PickUpType PickUp::GetType()
{
    if(type == RandomPickUp)
    {
        type = (PickUpType)Game::GenerateRandomNumber(NumOfPickUps - 1);
        if(type == RandomPickUp)
            type = (PickUpType)(type + 1);
    }

    return type;
}

void PickUp::Draw(sf::RenderWindow& rw)
{
    rw.draw(sprite);
}

void PickUp::Update(float changeInTime_)
{
    time_scale += changeInTime_;
    if(time_scale > TIME_SCALE)
        time_scale = 0.f;

    float scale = 1 + sin(time_scale / TIME_SCALE * 2 * (22.f / 7.f)) * SCALE_FACTOR;

    sprite.move(0, SPEED_MOVEMENT * changeInTime_);
    sprite.setScale(scale, scale);
}
