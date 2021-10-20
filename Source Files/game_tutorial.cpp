#include "game.h"
#include "drawgroup.h"

void Game::Tutorial()
{
    DrawGroup drawSession[3];
    int sessionIndex = 0;

    drawSession[0].AddText("giant_robot_army", sf::Vector2f(44, 12), 65, "How to play");
    drawSession[0].AddText("giant_robot_army", sf::Vector2f(926, 12), 65, "#1");
    drawSession[0].AddRectangle(sf::FloatRect(44, 87, 927, 6), sf::Color::White);
    drawSession[0].AddText("giant_robot_army", sf::Vector2f(44, 112), 30,
                           "Control the the pad using the below keys:");

    drawSession[0].AddSprite("tutorial_image1", sf::Vector2f(44, 163));
    drawSession[0].AddRectangle(sf::FloatRect(44, 314, 927, 6), sf::Color::White);
    drawSession[0].AddSprite("ball", sf::Vector2f(726, 404), 1.49f);
    drawSession[0].AddSprite("ball_corrupt", sf::Vector2f(881, 393), 1.4);
    drawSession[0].AddText("giant_robot_army", sf::Vector2f(44, 348), 30, (std::string)
                           "There are two ball types, the regular ball\n" +
                           "and the corrupted ball. Missing a\n" +
                           "corrupted ball will cost you one health.\n" +
                           "Its okay to miss a regular ball. The more\n" +
                           "balls you have active one the screen, the\n" +
                           "higher your score multiplier. You get some\n" +
                           "points from balls bouncing of any surface.");

    drawSession[0].AddText("giant_robot_army", sf::Vector2f(697, 473), 30, "Regular\n\tBall");
    drawSession[0].AddText("giant_robot_army", sf::Vector2f(843, 473), 30, "Corrupted\n\t  Ball");

    drawSession[1].AddText("giant_robot_army", sf::Vector2f(44, 12), 65, "How to play");
    drawSession[1].AddText("giant_robot_army", sf::Vector2f(901, 12), 65, "#2");
    drawSession[1].AddRectangle(sf::FloatRect(44, 87, 927, 6), sf::Color::White);
    drawSession[1].AddText("giant_robot_army", sf::Vector2f(44, 112), 30, (std::string)
                           "You can use you special ability, slow motion, to slow down the\n" +
                           "time. Depleting your special completely will cause a usability\n" +
                           "delay. Use any of the keys to activate and deactivate your\n" +
                           "special.");

    drawSession[1].AddSprite("tutorial_image2", sf::Vector2f(44, 274));
    drawSession[1].AddRectangle(sf::FloatRect(44, 413, 927, 6), sf::Color::White);
    drawSession[1].AddText("giant_robot_army", sf::Vector2f(44, 454), 30, (std::string)
                           "Enemy blobs will pop up from time to time. Hitting\n" +
                           "them with balls kills them. Blobs have a  chance\n" +
                           "of corrupting a regular ball when they torch\n" +
                           "them. Blobs also have a chance of dropping\n" +
                           "pick-ups. If you have too many of them on the\n" +
                           "screen, it can be a pain.");

    drawSession[1].AddSprite("tutorial_image4", sf::Vector2f(795, 467));
    drawSession[1].AddText("giant_robot_army", sf::Vector2f(855, 609), 30, "Blobs");

    drawSession[2].AddText("giant_robot_army", sf::Vector2f(44, 12), 65, "How to play");
    drawSession[2].AddText("giant_robot_army", sf::Vector2f(903, 12), 65, "#3");
    drawSession[2].AddRectangle(sf::FloatRect(44, 87, 927, 6), sf::Color::White);
    drawSession[2].AddText("giant_robot_army", sf::Vector2f(44, 114), 30, (std::string)
                           "You can switch between fullscreen and windowed screen\n" +
                           "anytime by pressing the F1 key.");

    drawSession[2].AddSprite("tutorial_image3", sf::Vector2f(44, 204));
    drawSession[2].AddRectangle(sf::FloatRect(44, 378, 927, 6), sf::Color::White);
    drawSession[2].AddText("giant_robot_army", sf::Vector2f(44, 419), 30, (std::string)
                           "The game speed increases with time, so it might get pretty\n" +
                           "difficult over time. Pick-ups might show up from time to time.\n" +
                           "Some of the pick-ups are good, some are bad so choose wisely.\n" +
                           "You get points with each ball hitting a surface and even more\n" +
                           "points killing a blob. Obstacles might show up falling from the\n" +
                           "top of the screen from time to time to reduce visibility.\n" +
                           "Have fun!");

    sf::Clock timer_framerate;

    SetFadeTransmitState(false);
    while(window.isOpen() && gameState == TutorialState)
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

        if(GetFadeTransmitState() == true && isFadeTransmitComplete())
        {
            sessionIndex++;
            if(sessionIndex >= 3)
            {
                gameState = MainMenuState;
                sessionIndex = 2;
            }
            else SetFadeTransmitState(false);
        }

        HandleBgColor(changeInTime);

        window.clear(GetBgColor());
        drawSession[sessionIndex].Draw(window);
        DrawFadeTransmit();

        window.display();
    }
    gameState = MainMenuState;
}
