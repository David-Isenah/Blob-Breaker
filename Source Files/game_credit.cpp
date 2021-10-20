#include "game.h"
#include "drawgroup.h"

void Game::Credit()
{
    DrawGroup drawGroup;
    drawGroup.AddText("giant_robot_army", sf::Vector2f(44, 12), 65, "Credit");
    drawGroup.AddRectangle(sf::FloatRect(44, 87, 927, 6), sf::Color::White);
    drawGroup.AddText("giant_robot_army", sf::Vector2f(44, 116), 48, "Programming");
    drawGroup.AddText("giant_robot_army", sf::Vector2f(44, 172), 32, "David Isenah");
    drawGroup.AddRectangle(sf::FloatRect(44, 213, 78, 6), sf::Color::White);
    drawGroup.AddText("giant_robot_army", sf::Vector2f(44, 239), 48, "Graphics Design");
    drawGroup.AddText("giant_robot_army", sf::Vector2f(44, 289), 32, "David Isenah");
    drawGroup.AddRectangle(sf::FloatRect(44, 334, 78, 6), sf::Color::White);
    drawGroup.AddText("giant_robot_army", sf::Vector2f(44, 355), 48, "Sound Effects");
    drawGroup.AddText("giant_robot_army", sf::Vector2f(44, 405), 32, "David Isenah");
    drawGroup.AddRectangle(sf::FloatRect(44, 447, 78, 6), sf::Color::White);
    drawGroup.AddText("giant_robot_army", sf::Vector2f(44, 469), 48, "Music");
    drawGroup.AddText("giant_robot_army", sf::Vector2f(44, 525), 32, "Go Cart - Kevin MacLeod(www.incompetech.com)");
    drawGroup.AddRectangle(sf::FloatRect(44, 576, 78, 6), sf::Color::White);
    drawGroup.AddText("giant_robot_army", sf::Vector2f(44, 597), 48, "Fonts");
    drawGroup.AddText("giant_robot_army", sf::Vector2f(44, 654), 32, (std::string)
                      "Giant Robot Army - Gabor Vad(www.dafont.com)\n" +
                      "Pricedown - Typodermic Fonts(www.typodermicfonts.com)");

    SetFadeTransmitState(false);
    sf::Clock timer_framerate;

    while(window.isOpen() && gameState == CreditState)
    {
        float changeInTime = timer_framerate.restart().asSeconds();

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F1)
                ChangeScreenMode();
            else if(isFadeTransmitComplete() && (event.type == sf::Event::KeyReleased || event.type == sf::Event::MouseButtonPressed))
                SetFadeTransmitState(true);
        }

        HandleBgColor(changeInTime);
        if(GetFadeTransmitState() == true && isFadeTransmitComplete())
             break;

        window.clear(GetBgColor());
        drawGroup.Draw(window);
        DrawFadeTransmit();

        window.display();
    }
    gameState = MainMenuState;
}
