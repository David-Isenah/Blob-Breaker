#include "game.h"
#include "button.h"

const float LOGO_SCALING_VALUE = 0.1f;
const float LOGO_SCALING_TIME = 3.f;

void Game::MainMenu()
{
    sf::Sprite logo;
    logo.setTexture(*GetTexture("blob_breaker_logo"));
    logo.setOrigin(368, 266);
    logo.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

    Button button[4] =
    {
        Button(sf::Vector2f(512, 163), "Play"),
        Button(sf::Vector2f(512, 311), "Tutorial"),
        Button(sf::Vector2f(512, 459), "Credit"),
        Button(sf::Vector2f(512, 605), "Exit"),
    };

    sf::Clock timer_logo_scaling;
    sf::Clock timer_framerate;
    float logo_scale = 1.f;
    SetFadeTransmitState(false);

    while(window.isOpen() && gameState == MainMenuState)
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

        if(timer_logo_scaling.getElapsedTime().asSeconds() > LOGO_SCALING_TIME)
            timer_logo_scaling.restart();
        logo_scale = 1 + LOGO_SCALING_VALUE * (1 + sin((-90 + 360 * timer_logo_scaling.getElapsedTime().asSeconds() / LOGO_SCALING_TIME) * (22.f / 7.f) / 180.f)) / 2.f;

        logo.setScale(logo_scale, logo_scale);

        window.clear(GetBgColor());
        window.draw(logo);
        DrawFadeTransmit();

        window.display();
    }

    GameState nextState = ExitState;
    SetFadeTransmitState(false);

    while(window.isOpen() && gameState == MainMenuState)
    {
        float changeInTime = timer_framerate.restart().asSeconds();

        bool isLeftClickReleased = false;
        std::string message;

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F1)
                ChangeScreenMode();
            else if(event.type == sf::Event::MouseButtonReleased)
                if(event.mouseButton.button == sf::Mouse::Left)
                    isLeftClickReleased = true;
        }

        for(int i = 0; i < 4; i++)
        {
            std::string gottenMessage = button[i].Update(changeInTime, isLeftClickReleased);
            if(gottenMessage != "")
                message = gottenMessage;
        }

        if(message == "Play")
        {
            SetFadeTransmitState(true);
            nextState = PlayState;
        }
        else if(message == "Tutorial")
        {
            SetFadeTransmitState(true);
            nextState = TutorialState;
        }
        if(message == "Credit")
        {
            SetFadeTransmitState(true);
            nextState = CreditState;
        }
        if(message == "Exit")
        {
            SetFadeTransmitState(true);
            nextState = ExitState;
        }

        HandleBgColor(changeInTime);
        if(GetFadeTransmitState() == true && isFadeTransmitComplete())
        {
            gameState = nextState;
            break;
        }

        window.clear(GetBgColor());
        for(int i = 0; i < 4; i++)
            button[i].Draw(window);
        DrawFadeTransmit();

        window.display();
    }
}
