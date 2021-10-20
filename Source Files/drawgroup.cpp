#include "drawgroup.h"
#include "game.h"

DrawGroup::~DrawGroup()
{
    Clear();
}

void DrawGroup::AddSprite(std::string id_, sf::Vector2f positon_, float scale_)
{
    sf::Sprite* sprite = new sf::Sprite();
    sprite->setTexture(*Game::GetTexture(id_));
    sprite->setPosition(positon_);
    sprite->setScale(scale_, scale_);

    drawObjects.push_back(sprite);
}

void DrawGroup::AddText(std::string id_, sf::Vector2f position_,int size_, std::string text_)
{
    sf::Text* text = new sf::Text(text_, *Game::GetFont(id_), size_);
    text->setPosition(position_);

    drawObjects.push_back(text);
}

void DrawGroup::AddRectangle(sf::FloatRect rect_, sf::Color color_)
{
    sf::RectangleShape* rectangle = new sf::RectangleShape();
    rectangle->setPosition(rect_.left, rect_.top);
    rectangle->setSize(sf::Vector2f(rect_.width, rect_.height));
    rectangle->setFillColor(color_);

    drawObjects.push_back(rectangle);
}

void DrawGroup::Draw(sf::RenderWindow& rw)
{
    for(auto obj : drawObjects)
        rw.draw(*obj);
}

void DrawGroup::Clear()
{
    for(auto obj : drawObjects)
        delete obj;
    drawObjects.clear();
}
