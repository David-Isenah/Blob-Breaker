#include "pad.h"
#include "game.h"

const float Pad::EFFECT_DISPLAY_DURATION = 0.5f;
const float EFFECT_DURATION[Pad::NumOfEffectType] = {30.f, 20.f, 15.f, 8.f, 30.f};

Pad::Pad()
{
    velocityX = 0;

    for(int i = 0; i < NumOfEffectType; i++)
        effect_time[i] = 0.f;
    effect_change_time = 0.f;
    numOfActiveEffects = 0;

    sprite.setTexture(*Game::GetTexture("pad"), true);
    sprite.setTextureRect(sf::IntRect(109, 0, 156, 34));
    sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);
    sprite.setPosition(Game::SCREEN_WIDTH / 2.f, Game::SCREEN_HEIGHT - 32.f);

    sprite_effect.setTexture(*Game::GetTexture("pad_effects"), true);
    sprite_effect.setTextureRect(sf::IntRect(0, 0, 50, 50));

    sound_pickup.setBuffer(*Game::GetSoundBuffer("pickup"));
    sound_pickup.setVolume(100);
}

void Pad::Draw(sf::RenderWindow& window_)
{
    window_.draw(sprite);

    if(numOfActiveEffects > 0)
    {
        float dx = -25;
        float padRight = sprite.getGlobalBounds().left + sprite.getGlobalBounds().width;
        if(padRight + 25 > Game::WALL_RIGHT)
            dx = Game::WALL_RIGHT - (padRight + 50);

        sprite_effect.setPosition(padRight + dx, sprite.getGlobalBounds().top - 50);
        window_.draw(sprite_effect);

        if(effect_time[GhostEffect] > 0.f)
        {
            sf::Sprite ghost = sprite;

            float xDiff = Game::SCREEN_WIDTH / 2.f - sprite.getPosition().x;
            ghost.setPosition(Game::SCREEN_WIDTH / 2.f + xDiff, sprite.getPosition().y);
            ghost.setColor(sf::Color(255, 255, 255, 100));
            window_.draw(ghost);
        }
    }
}

void Pad::ActivateEffect(EffectType effect_)
{
    effect_time[effect_] = EFFECT_DURATION[effect_];

    if(effect_ == BigSizeEffect)
    {
        effect_time[SmallSizeEffect] = 0.f;
        sprite.setTextureRect(sf::IntRect(265, 0, 205, 34));
        sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);
    }
    if(effect_ == SmallSizeEffect)
    {
        effect_time[BigSizeEffect] = 0.f;
        sprite.setTextureRect(sf::IntRect(0, 0, 109, 34));
        sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);
    }

}

void Pad::Update(float changeInTime_)
{
    int moveDir = 0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        moveDir--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        moveDir++;

    if(effect_time[ReversedEffect] > 0.f)
        moveDir = -moveDir;

    float actualSpeed = MOVEMENT_SPEED;
    float actualMaxSpeed = MAX_SPEED;

    if(effect_time[SlowEffect] > 0.f)
    {
        actualSpeed *= 0.5f;
        actualMaxSpeed *= 0.5f;
    }

    if(moveDir == 0 && velocityX != 0)
    {
        int velDir = 1;
        if(velocityX < 0)
            velDir = -1;

        velocityX += (-velDir) * actualSpeed * changeInTime_;

        if(velocityX * velDir < 0)
            velocityX = 0;
    }
    else velocityX += moveDir * actualSpeed * changeInTime_;

    if(velocityX > actualMaxSpeed)
        velocityX = actualMaxSpeed;
    if(velocityX < -actualMaxSpeed)
        velocityX = -actualMaxSpeed;

    sprite.move(velocityX * changeInTime_, 0);

    if(sprite.getGlobalBounds().left < Game::WALL_LEFT)
    {
        sprite.setPosition(Game::WALL_LEFT + sprite.getTextureRect().width / 2.f, sprite.getPosition().y);
        velocityX = 0;
    }
    if(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width > Game::WALL_RIGHT)
    {
        sprite.setPosition(Game::WALL_RIGHT - sprite.getTextureRect().width / 2.f, sprite.getPosition().y);
        velocityX = 0;
    }

    numOfActiveEffects = 0;
    for(int i = 0; i < NumOfEffectType; i++)
        if(effect_time[i] > 0.f)
        {

            effect_time[i] -= changeInTime_;
            if(effect_time[i] > 0.f)
                numOfActiveEffects++;

            if((i == SmallSizeEffect || i == BigSizeEffect) && effect_time[i] < 0.f)
            {
                sprite.setTextureRect(sf::IntRect(109, 0, 156, 34));
                sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);
            }
        }

    if(numOfActiveEffects > 1)
    {
        effect_change_time -= changeInTime_;
        if(effect_change_time <= 0.f)
        {
            effect_change_time = EFFECT_DISPLAY_DURATION;
            int index = sprite_effect.getTextureRect().left / sprite_effect.getTextureRect().width;

            while(true)
            {
                index++;
                if(index >= NumOfEffectType)
                    index = 0;

                if(effect_time[index] > 0.f)
                {
                    sf::IntRect rect = sprite_effect.getTextureRect();
                    sprite_effect.setTextureRect(sf::IntRect(rect.width * index, rect.top, rect.width, rect.height));
                    break;
                }
            }

        }
    }
    else
    {
        short index = 0;
        for(int i = 0; i < NumOfEffectType; i++)
            if(effect_time[i] > 0.f)
            {
                index = i;
                break;
            }

        sf::IntRect rect = sprite_effect.getTextureRect();
        sprite_effect.setTextureRect(sf::IntRect(rect.width * index, rect.top, rect.width, rect.height));
    }
}

bool Pad::HandlePickUpCollision(PickUp* pickup)
{
    if(pickup != nullptr)
    {
        if(sprite.getGlobalBounds().intersects(pickup->GetBounds()))
        {
            switch(pickup->GetType())
            {
            case(PickUp::BigSiZePickUp) :
                ActivateEffect(BigSizeEffect);
                break;

            case(PickUp::SmallSizePickUp) :
                ActivateEffect(SmallSizeEffect);
                break;

            case(PickUp::ReversedPickUp) :
                ActivateEffect(ReversedEffect);
                break;

            case(PickUp::SlowPickUp) :
                ActivateEffect(SlowEffect);
                break;

            case(PickUp::GhostPickUp) :
                ActivateEffect(GhostEffect);
                break;
            }
            sound_pickup.play();
            return true;
        }
    }
    return false;
}

void Pad::HandleBallCollision(Ball* ball)
{
    if(ball != nullptr)
    {
        if(ball->GetVelocity().y > 0)
        {
            if(sprite.getGlobalBounds().intersects(ball->GetBounds()) && ball->IsPadCollidable())
            {
                sf::Vector2f redirectVec = ball->GetPosition() - sprite.getPosition() - sf::Vector2f(0, REDIRECT_DISTANCE);
                redirectVec /= (float)sqrt(redirectVec.x * redirectVec.x + redirectVec.y * redirectVec.y);
                ball->SetVelocity(redirectVec * ball->GetVelocityMagn());

                if(ball->GetType() == Ball::CorruptBallType)
                {
                    Game::IncrementGameScore(SCORE_CORRUPT_BALL_HIT);
                    ball->Uncorrupt();
                }
                else Game::IncrementGameScore(SCORE_BALL_HIT);
            }
        }

        if(effect_time[GhostEffect] > 0.f)
        {
            sf::Sprite ghost = sprite;
            float xDiff = Game::SCREEN_WIDTH / 2.f - sprite.getPosition().x;
            ghost.setPosition(Game::SCREEN_WIDTH / 2.f + xDiff, sprite.getPosition().y);

            if(ghost.getGlobalBounds().intersects(ball->GetBounds()) && ball->IsPadCollidable())
            {
                sf::Vector2f redirectVec = ball->GetPosition() - ghost.getPosition() - sf::Vector2f(0, REDIRECT_DISTANCE);
                redirectVec /= (float)sqrt(redirectVec.x * redirectVec.x + redirectVec.y * redirectVec.y);
                ball->SetVelocity(redirectVec * ball->GetVelocityMagn());

                if(ball->GetType() == Ball::CorruptBallType)
                {
                    Game::IncrementGameScore(SCORE_CORRUPT_BALL_HIT);
                    ball->Uncorrupt();
                }
                else Game::IncrementGameScore(SCORE_BALL_HIT);
            }
        }
    }
}
