#include "button.h"
#include "game.h"

const float Button::SCALING_TIME = 0.2f;
const float Button::SCALING_VALUE = 0.2f;

Button::Button(sf::Vector2f position_, std::string text_)
{
    scale_factor = 0.f;

    sprite.setTexture(*Game::GetTexture("button"), true);
    sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().height / 2.f);
    sprite.setPosition(position_);

    text.setFont(*Game::GetFont("giant_robot_army"));
    text.setCharacterSize(70);
    text.setString(text_);
    text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f + 15);
    text.setPosition(position_);

    sound_press.setBuffer(*Game::GetSoundBuffer("button_press"));
    sound_press.setVolume(100);
}

std::string Button::Update(float changeInTime_, bool isLeftClickReleased)
{
    sf::Vector2f cursorPosition = Game::GetCursorPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();
    std::string message = "";

    if(cursorPosition.x > bounds.left && cursorPosition.x < bounds.left + bounds.width &&
       cursorPosition.y > bounds.top && cursorPosition.y < bounds.top + bounds.height)
    {
        scale_factor += changeInTime_ / SCALING_TIME;
        if(isLeftClickReleased)
        {
            message = text.getString();
            sound_press.play();
        }
    }
    else scale_factor -= changeInTime_ / SCALING_TIME;

    if(scale_factor < 0)
        scale_factor = 0;
    if(scale_factor > 1)
        scale_factor = 1;

    float scale = 1 + SCALING_VALUE * sin(90 * scale_factor * (22.f / 7.f / 180));
    sprite.setScale(scale, scale);
    text.setScale(scale, scale);

    return message;
}

void Button::Draw(sf::RenderWindow& rw)
{
    rw.draw(sprite);
    rw.draw(text);
}
