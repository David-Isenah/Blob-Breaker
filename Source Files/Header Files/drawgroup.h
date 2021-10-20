#ifndef DRAWGROUP_H_INCLUDED
#define DRAWGROUP_H_INCLUDED

#include <vector>
#include <string>
#include "SFML/Graphics.hpp"

class DrawGroup
{
public:
    ~DrawGroup();

    void AddSprite(std::string id_, sf::Vector2f positon_, float scale_ = 1.f);
    void AddRectangle(sf::FloatRect rect_, sf::Color color_);
    void AddText(std::string id_, sf::Vector2f position_, int size_, std::string text_);
    void Draw(sf::RenderWindow& rw);
    void Clear();

private:
    std::vector<sf::Drawable*> drawObjects;
};

#endif // DRAWGROUP_H_INCLUDED
