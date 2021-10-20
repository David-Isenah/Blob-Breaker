#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class Button
{
public:
    static const float SCALING_TIME;
    static const float SCALING_VALUE;

    Button(sf::Vector2f position_, std::string text_);
    std::string Update(float changeInTime_, bool isLeftClickReleased);
    void Draw(sf::RenderWindow& rw);

private:
    sf::Sprite sprite;
    sf::Text text;
    float scale_factor;
    sf::Sound sound_press;
};

#endif // BUTTON_H_INCLUDED
