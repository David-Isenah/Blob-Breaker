#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <map>
#include <string>
#include <iostream>
#include <math.h>

class Game
{
public:
    static const int SCREEN_WIDTH = 1024;
    static const int SCREEN_HEIGHT = 768;
    static const int SCREEN_WIDTH_LR = 640;
    static const int SCREEN_HEIGHT_LR = 480;
    static const int NUM_OF_BG_COLOURS = 8;
    static const int WALL_TOP = 63;
    static const int WALL_LEFT = 72;
    static const int WALL_RIGHT = SCREEN_WIDTH - WALL_LEFT;
    static const float MAX_GAMEPLAY_SPEED_TIME;
    static const float MAX_GAMEPLAY_SPEED_MULTIPLIER;
    static const float SPECIAL_FRAME_RATE_MULTIPLIER;
    static const float BG_COLOR_CHANGE_DURATION;
    static const int PER_SCORE_MULTIPIER_BALLS = 5;
    static const float HEALTH_DROP_TIME;
    static const float FADE_TRANSMIT_TIME;
    static const float OVERTIME_SCORE_MULTIPIER;

    enum GameState{IntroState, MainMenuState, PlayState, TutorialState, CreditState, ExitState};

    static bool InitializeResources();
    static void InitializeGame();
    static std::string ConvertNumberToString(int number);
    static int GenerateRandomNumber(int range);
    static void IncrementGameScore(unsigned int score_);
    static void SetGameScoreMultiplier(float multipier);
    static void ResetScore();
    static void HandleBgColor(float changeInTime_);
    static sf::Color GetBgColor();
    static sf::Vector2f GetCursorPosition();
    static bool HandleRectCollision(sf::FloatRect subjRect, sf::FloatRect objRect, sf::Vector2f& moveVec);
    static void ChangeScreenMode();

    static void Run();
    static void Play();
    static void MainMenu();
    static void Credit();
    static void Tutorial();

    static sf::Texture* GetTexture(std::string id);
    static sf::Font* GetFont(std::string id);
    static sf::SoundBuffer* GetSoundBuffer(std::string id);
    static sf::Music& GetBgMusic();
    static unsigned long GetGameScore();

    static void QuickDrawRectangle(sf::Color color, sf::FloatRect rect);
    static void DrawFadeTransmit();
    static void SetFadeTransmitState(bool state_);
    static bool GetFadeTransmitState();
    static bool isFadeTransmitComplete();

private:
    static sf::RenderWindow window;
    static GameState gameState;
};

#endif // GAME_H_INCLUDED
