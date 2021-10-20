#include "game.h"
#include <sstream>
#include <random>

const float Game::MAX_GAMEPLAY_SPEED_TIME = 60 * 2.f;
const float Game::MAX_GAMEPLAY_SPEED_MULTIPLIER = 2.2f;
const float Game::SPECIAL_FRAME_RATE_MULTIPLIER = 0.3f;
const float Game::BG_COLOR_CHANGE_DURATION = 30.f;
const float Game::HEALTH_DROP_TIME = 30.f;
const float Game::FADE_TRANSMIT_TIME = 0.5f;
const float Game::OVERTIME_SCORE_MULTIPIER = 6.f;

std::map<std::string, sf::Texture> game_textures;
std::map<std::string, sf::SoundBuffer> game_soundBuffers;
std::map<std::string, sf::Font> game_fonts;
sf::Color game_bg_colors[Game::NUM_OF_BG_COLOURS];
sf::Music game_bg_music;
unsigned long game_score = 0;
float game_score_multipier = 1.f;
float game_color_time = 0.f;
short game_color_index = 0;
bool game_fullscreen = false;
sf::RectangleShape quick_rectangle;

bool fade_transmit_state = true;
bool supports_fullscreen = false;
sf::Clock fade_transmit_clock;

sf::RenderWindow Game::window;
Game::GameState Game::gameState;

bool Game::InitializeResources()
{
    game_fonts.clear();
    game_textures.clear();
    game_soundBuffers.clear();

    if(!game_fonts["giant_robot_army"].loadFromFile("Resources/Fonts/giant_robot_army.ttf")) return false;
    if(!game_fonts["pricedown"].loadFromFile("Resources/Fonts/pricedown.ttf")) return false;

    if(!game_textures["ball"].loadFromFile("Resources/Images/ball.png")) return false;
    if(!game_textures["ball_corrupt"].loadFromFile("Resources/Images/ball_corrupt.png")) return false;
    if(!game_textures["blob_boss"].loadFromFile("Resources/Images/blob_boss.png")) return false;
    if(!game_textures["blob_default1"].loadFromFile("Resources/Images/blob_default1.png")) return false;
    if(!game_textures["blob_default2"].loadFromFile("Resources/Images/blob_default2.png")) return false;
    if(!game_textures["blob_small"].loadFromFile("Resources/Images/blob_small.png")) return false;
    if(!game_textures["button"].loadFromFile("Resources/Images/button.png")) return false;
    if(!game_textures["gameover_score"].loadFromFile("Resources/Images/gameover_score.png")) return false;
    if(!game_textures["health_bar"].loadFromFile("Resources/Images/health_bar.png")) return false;
    if(!game_textures["obstacle1"].loadFromFile("Resources/Images/obstacle1.png")) return false;
    if(!game_textures["obstacle2"].loadFromFile("Resources/Images/obstacle2.png")) return false;
    if(!game_textures["obstacle3"].loadFromFile("Resources/Images/obstacle3.png")) return false;
    if(!game_textures["obstacle4"].loadFromFile("Resources/Images/obstacle4.png")) return false;
    if(!game_textures["pad"].loadFromFile("Resources/Images/pad.png")) return false;
    if(!game_textures["pad_effects"].loadFromFile("Resources/Images/pad_effects.png")) return false;
    if(!game_textures["pickups"].loadFromFile("Resources/Images/pickups.png")) return false;
    if(!game_textures["special_bar"].loadFromFile("Resources/Images/special_bar.png")) return false;
    if(!game_textures["tutorial_image1"].loadFromFile("Resources/Images/tutorial_image1.png")) return false;
    if(!game_textures["tutorial_image2"].loadFromFile("Resources/Images/tutorial_image2.png")) return false;
    if(!game_textures["tutorial_image3"].loadFromFile("Resources/Images/tutorial_image3.png")) return false;
    if(!game_textures["tutorial_image4"].loadFromFile("Resources/Images/tutorial_image4.png")) return false;
    if(!game_textures["blob_breaker_logo"].loadFromFile("Resources/Images/blob_breaker_logo.png")) return false;
    if(!game_textures["blob_breaker_icon"].loadFromFile("Resources/Images/blob_breaker_icon.png")) return false;
    if(!game_textures["wall"].loadFromFile("Resources/Images/wall.png")) return false;

    if(!game_soundBuffers["button_press"].loadFromFile("Resources/Sound FX/button_press.wav")) return false;
    if(!game_soundBuffers["miss"].loadFromFile("Resources/Sound FX/miss.wav")) return false;
    if(!game_soundBuffers["pickup"].loadFromFile("Resources/Sound FX/pickup.wav")) return false;

    for(auto itr = game_textures.begin(); itr != game_textures.end(); itr++)
        itr->second.setSmooth(true);

    game_bg_colors[0] = sf::Color(149, 237, 90);
    game_bg_colors[1] = sf::Color(138, 230, 175);
    game_bg_colors[2] = sf::Color(230, 216, 122);
    game_bg_colors[3] = sf::Color(240, 156, 156);
    game_bg_colors[4] = sf::Color(177, 219, 151);
    game_bg_colors[5] = sf::Color(172, 194, 230);
    game_bg_colors[6] = sf::Color(193, 230, 129);
    game_bg_colors[7] = sf::Color(205, 154, 230);

    game_bg_music.openFromFile("Resources/Music/background_music.ogg");
    game_bg_music.setLoop(true);

    return true;
}

void Game::InitializeGame()
{
    if(InitializeResources())
    {
        gameState = MainMenuState;

        sf::View view;
        view.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        view.setCenter(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
        view.setViewport(sf::FloatRect(0, 0, 1, 1));

        sf::Image icon = GetTexture("blob_breaker_icon")->copyToImage();
        window.create(sf::VideoMode(SCREEN_WIDTH_LR, SCREEN_HEIGHT_LR), "Blob Breaker", sf::Style::Close);
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        window.setVerticalSyncEnabled(false);
        window.setView(view);

        std::vector<sf::VideoMode> supportedVideoModes = sf::VideoMode::getFullscreenModes();
        for(sf::VideoMode mode : supportedVideoModes)
            if(mode.width == SCREEN_WIDTH && mode.height == SCREEN_HEIGHT)
            {
                supports_fullscreen = true;
                break;
            }
        fade_transmit_state = true;
        game_fullscreen = false;
        game_bg_music.play();
    }
    else gameState = ExitState;
}

sf::Texture* Game::GetTexture(std::string id)
{
    auto itr = game_textures.find(id);
    if(itr != game_textures.end())
        return &itr->second;

    std::cout << "Couldn't find \"" << id <<"\" in game textures.\n";
    return nullptr;
}

sf::Font* Game::GetFont(std::string id)
{
    auto itr = game_fonts.find(id);
    if(itr != game_fonts.end())
        return &itr->second;

    std::cout << "Couldn't find \"" << id <<"\" in game fonts.\n";
    return nullptr;
}

sf::SoundBuffer* Game::GetSoundBuffer(std::string id)
{
    auto itr = game_soundBuffers.find(id);
    if(itr != game_soundBuffers.end())
        return &itr->second;

    std::cout << "Couldn't find \"" << id <<"\" in game sound buffers.\n";
    return nullptr;
}

sf::Music& Game::GetBgMusic()
{
    return game_bg_music;
}

sf::Color Game::GetBgColor()
{
    short nextColorIndex = game_color_index + 1;
    if(nextColorIndex >= NUM_OF_BG_COLOURS)
        nextColorIndex = 0;

    float factor = game_color_time / BG_COLOR_CHANGE_DURATION;

    float rDiff = game_bg_colors[nextColorIndex].r - game_bg_colors[game_color_index].r;
    float gDiff = game_bg_colors[nextColorIndex].g - game_bg_colors[game_color_index].g;
    float bDiff = game_bg_colors[nextColorIndex].b - game_bg_colors[game_color_index].b;

    return sf::Color((float)game_bg_colors[game_color_index].r + (rDiff * factor),
                     (float)game_bg_colors[game_color_index].g + (gDiff * factor),
                     (float)game_bg_colors[game_color_index].b + (bDiff * factor));
}

void Game::IncrementGameScore(unsigned int score_)
{
    game_score += score_ * game_score_multipier;
}

void Game::SetGameScoreMultiplier(float multipier)
{
    if(multipier >= 1.f)
        game_score_multipier = multipier;
}

void Game::ResetScore()
{
    game_score = 0;
}

unsigned long Game::GetGameScore()
{
    return game_score;
}

void Game::HandleBgColor(float changeInTime_)
{
    game_color_time += changeInTime_;
    if(game_color_time > BG_COLOR_CHANGE_DURATION)
    {
        game_color_time = 0;
        game_color_index++;
        if(game_color_index >= NUM_OF_BG_COLOURS)
            game_color_index = 0;
    }
}

bool Game::HandleRectCollision(sf::FloatRect subjRect, sf::FloatRect objRect, sf::Vector2f& moveVec)
{
    if(subjRect.intersects(objRect))
    {
        float subjLeft = subjRect.left;
        float subjRight = subjRect.left + subjRect.width;
        float subjTop = subjRect.top;
        float subjBottom = subjRect.top + subjRect.height;

        float objLeft = objRect.left;
        float objRight = objRect.left + objRect.width;
        float objTop = objRect.top;
        float objBottom = objRect.top + objRect.height;

        float moveX = 0;
        float moveY = 0;

        if(subjLeft < objLeft && subjRight > objLeft)
            moveX = objLeft - subjRight;
        if(subjRight > objRight && subjLeft < objRight)
            moveX = objRight - subjLeft;

        if(subjTop < objTop && subjBottom > objTop)
            moveY = objTop - subjBottom;
        if(subjBottom > objBottom && subjTop < objBottom)
            moveY = objBottom - subjTop;

        if(sqrt(moveX * moveX) > 0 && moveY == 0)
            moveVec.x = moveX;
        else if(sqrt(moveY * moveY) > 0 && moveX == 0)
            moveVec.y = moveY;
        else if(sqrt(moveX * moveX) < sqrt(moveY * moveY))
            moveVec.x = moveX;
        else if(sqrt(moveY* moveY) < sqrt(moveX * moveX))
            moveVec.y = moveY;

        return true;
    }
    return false;
}

void Game::Run()
{
    InitializeGame();

    while(window.isOpen())
    {
        switch(gameState)
        {
        case(PlayState):
            Play();
            break;

        case(MainMenuState):
            MainMenu();
            break;

        case(TutorialState):
            Tutorial();
            break;

        case(CreditState):
            Credit();
            break;

        case(ExitState):
            window.close();
            break;

        default:
            window.close();
            break;
        }
    }
}

void Game::QuickDrawRectangle(sf::Color color, sf::FloatRect rect)
{
    quick_rectangle.setFillColor(color);
    quick_rectangle.setSize(sf::Vector2f(rect.width, rect.height));
    quick_rectangle.setPosition(rect.left, rect.top);
    window.draw(quick_rectangle);
}


std::string Game::ConvertNumberToString(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

int Game::GenerateRandomNumber(int range)
{
    return rand() % (range + 1);
}

void Game::SetFadeTransmitState(bool state_)
{
    if(fade_transmit_state != state_)
    {
        fade_transmit_clock.restart();
        fade_transmit_state = state_;
    }
}

bool Game::isFadeTransmitComplete()
{
    if(fade_transmit_clock.getElapsedTime().asSeconds() > FADE_TRANSMIT_TIME + 0.1f)
        return true;
    return false;
}

bool Game::GetFadeTransmitState()
{
    return fade_transmit_state;
}

void Game::DrawFadeTransmit()
{
    float factor_ = fade_transmit_clock.getElapsedTime().asSeconds() / FADE_TRANSMIT_TIME;;
    if(fade_transmit_state == false)
        factor_ = 1 - fade_transmit_clock.getElapsedTime().asSeconds() / FADE_TRANSMIT_TIME;

    if(factor_ < 0)
        factor_ = 0;
    if(factor_ > 1)
        factor_ = 1;
    QuickDrawRectangle(sf::Color(255, 255, 255, 255 * factor_), sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
}

sf::Vector2f Game::GetCursorPosition()
{
    sf::Vector2f cursorPosition = (sf::Vector2f)sf::Mouse::getPosition(window);

    if(game_fullscreen == false)
    {
        cursorPosition.x *= (float)SCREEN_WIDTH / window.getSize().x;
        cursorPosition.y *= (float)SCREEN_HEIGHT / window.getSize().y;
    }
    return cursorPosition;
}

void Game::ChangeScreenMode()
{
    if(supports_fullscreen)
    {
        if(game_fullscreen)
        {
            game_fullscreen = false;

            sf::View view;
            view.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
            view.setCenter(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
            view.setViewport(sf::FloatRect(0, 0, 1, 1));

            window.create(sf::VideoMode(SCREEN_WIDTH_LR, SCREEN_HEIGHT_LR), "Blob Breaker", sf::Style::Close);
            window.setView(view);
            window.setVerticalSyncEnabled(false);
        }
        else
        {
            game_fullscreen = true;
            window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Blob Breaker", sf::Style::Fullscreen);
            window.setVerticalSyncEnabled(true);
        }
    }
}
