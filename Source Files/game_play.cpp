#include "game.h"
#include "pad.h"
#include "ball.h"
#include "blob.h"
#include "pickup.h"
#include "obstacle.h"
#include "drawgroup.h"

void Game::Play()
{
    Pad playerPad;
    std::vector<Blob*> game_blobs;
    sf::Clock timer_blob_spawner;
    float nextBlobSpawnTime = Blob::SPAWN_DELAY_SMALL;

    std::vector<Ball*> game_balls;
    sf::Clock timer_obstacle_spawner;
    float nextObstacleSpawnTime = Obstacle::MIN_SPAWN_TIME + (Obstacle::SPAWN_TIME_VARIATION) * GenerateRandomNumber(100) / 100.f;

    std::vector<Obstacle*> game_obstacles;
    sf::Clock timer_ball_spawner;
    float nextBallSpawnTime = Ball::MIN_SPAWN_TIME + (Ball::MAX_SPAWN_TIME - Ball::MIN_SPAWN_TIME) * GenerateRandomNumber(100) / 100.f;

    sf::Clock timer_health_drop;

    std::vector<PickUp*> game_pickups;

    float special = 100.f;
    bool specialDelay = false;
    bool isUsingSpecialNow = false;
    int health = 10;

    sf::Sprite sprite_left_wall(*GetTexture("wall"));
    sprite_left_wall.setPosition(0, 63);

    sf::Sprite sprite_right_wall(*GetTexture("wall"));
    sprite_right_wall.setOrigin(sprite_right_wall.getTextureRect().width / 2.f,
                               sprite_right_wall.getTextureRect().height / 2.f);
    sprite_right_wall.setPosition(988, 415);
    sprite_right_wall.rotate(180);

    sf::Text text_health;
    text_health.setFont(*GetFont("pricedown"));
    text_health.setCharacterSize(34);
    text_health.setPosition(72, 18);

    sf::Text text_score;
    text_score.setFont(*GetFont("pricedown"));
    text_score.setCharacterSize(45);
    text_score.setPosition(1013, 0);

    sf::Sound sound_miss;
    sound_miss.setBuffer(*GetSoundBuffer("miss"));
    sound_miss.setVolume(35);

    DrawGroup drawGroup;
    drawGroup.AddRectangle(sf::FloatRect(0, 57, 1024, 6), sf::Color::White);
    drawGroup.AddSprite("health_bar", sf::Vector2f(14, 11));
    drawGroup.AddSprite("special_bar", sf::Vector2f(267, 12));

    sf::Clock timer_fps;
    sf::Clock timer_gameplay;
    float actualFrameRateMultiplier = 1.f;
    float blobAreaOccupied = 0.f;

    SetFadeTransmitState(false);
    ResetScore();

    while(window.isOpen() && gameState == PlayState)
    {
        float changeInTime = timer_fps.restart().asSeconds();

        //Handling Events
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::KeyReleased)
            {
                if(event.key.code == sf::Keyboard::F1)
                    ChangeScreenMode();
                else if(event.key.code == sf::Keyboard::Escape)
                    SetFadeTransmitState(true);
            }
        }

        if((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) &&
           specialDelay == false)
        {
            special -= 15 * changeInTime;
            actualFrameRateMultiplier -= 4 * changeInTime;
            isUsingSpecialNow = true;
        }
        else
        {
            special += 10 * changeInTime;
            actualFrameRateMultiplier += 4 * changeInTime;
            isUsingSpecialNow = false;
        }

        if(special < 0)
        {
            special = 0.f;
            specialDelay = true;
        }
        else if(special > 100)
            special = 100;

        if(specialDelay && special > 20)
            specialDelay = false;

        //Handling Time Multiplier
        float timeFactor = timer_gameplay.getElapsedTime().asSeconds() / MAX_GAMEPLAY_SPEED_TIME;
        if(timeFactor > 1)
            timeFactor = 1.f;
        float timeMultiplier = 1 + (MAX_GAMEPLAY_SPEED_MULTIPLIER - 1) * timeFactor;

        if(actualFrameRateMultiplier < SPECIAL_FRAME_RATE_MULTIPLIER)
            actualFrameRateMultiplier = SPECIAL_FRAME_RATE_MULTIPLIER;
        else if(actualFrameRateMultiplier > timeMultiplier)
                actualFrameRateMultiplier = timeMultiplier;

        float actualChangeInTime = changeInTime * actualFrameRateMultiplier;
        float leastChangeInTime = actualChangeInTime < changeInTime ? actualChangeInTime : changeInTime;

        //Spawning objects
        if(timer_ball_spawner.getElapsedTime().asSeconds() >= nextBallSpawnTime)
        {
            timer_ball_spawner.restart();
            nextBallSpawnTime = Ball::MIN_SPAWN_TIME + (Ball::MAX_SPAWN_TIME - Ball::MIN_SPAWN_TIME) * GenerateRandomNumber(100) / 100.f;
            nextBallSpawnTime /= timeMultiplier;
            game_balls.push_back(new Ball());
        }

        if(timer_obstacle_spawner.getElapsedTime().asSeconds() >= nextObstacleSpawnTime)
        {
            timer_obstacle_spawner.restart();

            float timeVariation = Obstacle::SPAWN_TIME_VARIATION_FAST + (Obstacle::SPAWN_TIME_VARIATION - Obstacle::SPAWN_TIME_VARIATION_FAST) * (1 - timeFactor);
            nextObstacleSpawnTime = Obstacle::MIN_SPAWN_TIME + (timeVariation) * GenerateRandomNumber(100) / 100.f;
            game_obstacles.push_back(new Obstacle());
        }

        if(timer_blob_spawner.getElapsedTime().asSeconds() >= nextBlobSpawnTime && blobAreaOccupied < Blob::MAX_AREA_OCCUPIABLE)
        {
            timer_blob_spawner.restart();
            nextBlobSpawnTime = Blob::SPAWN_DELAY_VARRIATION * GenerateRandomNumber(100) / 100.f;
            game_blobs.push_back(new Blob());

            blobAreaOccupied += game_blobs[game_blobs.size() - 1]->GetArea();

            switch(game_blobs[game_blobs.size() - 1]->GetType())
            {
            case(Blob::SmallBlobType) :
                nextBlobSpawnTime += Blob::SPAWN_DELAY_SMALL;
                break;

            case(Blob::DefaultBlobType) :
                nextBlobSpawnTime += Blob::SPAWN_DELAY_DEFAULT;
                break;

            case(Blob::BossBlobType) :
                nextBlobSpawnTime += Blob::SPAWN_DELAY_BOSS;
                break;
            }
            nextBlobSpawnTime /= timeMultiplier;
        }

        //Free health drops after some time
        if(timer_health_drop.getElapsedTime().asSeconds() >= HEALTH_DROP_TIME)
        {
            timer_health_drop.restart();
            game_pickups.push_back(new PickUp(sf::Vector2f(104 + GenerateRandomNumber(816), -32), PickUp::HealthPickUp));
        }

        //Updating objects
        SetGameScoreMultiplier(game_balls.size() / (float)PER_SCORE_MULTIPIER_BALLS + OVERTIME_SCORE_MULTIPIER * timeFactor);

        for(int i = 0; i < game_balls.size(); i++)
        {
            game_balls[i]->Update(actualChangeInTime);
            if(game_balls[i]->IsBallOutOfScreen())
            {
                if(game_balls[i]->GetType() == Ball::CorruptBallType)
                {
                    health--;
                    sound_miss.play();
                }

                delete game_balls[i];
                game_balls.erase(game_balls.begin() + i);
                i--;
            }
        }
        for(int i = 0; i < game_blobs.size(); i++)
        {
            game_blobs[i]->Update(actualChangeInTime);
            if(game_blobs[i]->ShouldDelete())
            {
                blobAreaOccupied -= game_blobs[i]->GetArea();

                delete game_blobs[i];
                game_blobs.erase(game_blobs.begin() + i);
                i--;
            }
        }
        for(int i = 0; i < game_obstacles.size(); i++)
        {
            game_obstacles[i]->Update(leastChangeInTime);
            if(game_obstacles[i]->IsOutOfScreen())
            {
                delete game_obstacles[i];
                game_obstacles.erase(game_obstacles.begin() + i);
                i--;
            }
        }

        //Handling when to end game
        if(health <= 0)
            SetFadeTransmitState(true);

        if(GetFadeTransmitState() == true && isFadeTransmitComplete())
            break;

        HandleBgColor(actualChangeInTime);

        //Updating player and pickups
        playerPad.Update(leastChangeInTime < changeInTime * SPECIAL_FRAME_RATE_MULTIPLIER * 2 ? changeInTime * SPECIAL_FRAME_RATE_MULTIPLIER * 2 : leastChangeInTime);

        for(int i = 0; i < game_pickups.size(); i++)
        {
            game_pickups[i]->Update(leastChangeInTime);
            if(playerPad.HandlePickUpCollision(game_pickups[i]))
            {
                if(game_pickups[i]->GetType() == PickUp::HealthPickUp)
                    health++;
                else if(game_pickups[i]->GetType() == PickUp::BlobPickUp)
                {
                    int numOfBlobsToKill = GenerateRandomNumber(game_blobs.size());
                    if(game_blobs.size() <= 2)
                        numOfBlobsToKill = game_blobs.size();
                    else if(numOfBlobsToKill < 2)
                        numOfBlobsToKill = 2;

                    for(int k = 0; k < game_blobs.size() && k < numOfBlobsToKill; k++)
                        game_blobs[k]->Kill();
                }

                delete game_pickups[i];
                game_pickups.erase(game_pickups.begin() + i);
                i--;
            }
            else if(game_pickups[i]->GetBounds().top > SCREEN_HEIGHT)
            {
                delete game_pickups[i];
                game_pickups.erase(game_pickups.begin() + i);
                i--;
            }
        }

        //Handling Collision
        for(auto blob : game_blobs)
            for(auto blob_collide : game_blobs)
                blob->HandleBlobCollision(blob_collide);

        for(auto ball : game_balls)
        {
            playerPad.HandleBallCollision(ball);
            for(auto blob : game_blobs)
            {
                int prevHitCount = blob->GetHitCount();

                blob->HandleBallCollision(ball);
                if(blob->GetHitCount() <= 0 && prevHitCount > 0)
                {
                    if(blob->GetType() != Blob::BossBlobType && GenerateRandomNumber(100) < Blob::PICKUP_DROP_CHANCE)
                        game_pickups.push_back(new PickUp(blob->GetPosition()));
                    else if(blob->GetType() == Blob::BossBlobType)
                        game_pickups.push_back(new PickUp(blob->GetPosition()));
                }
            }
        }


        //Rendering to screen
        window.clear(GetBgColor());

        for(auto Ball : game_balls)
            Ball->Draw(window);
        for(auto blob : game_blobs)
            blob->Draw(window);
        playerPad.Draw(window);
        for(auto pickup : game_pickups)
            pickup->Draw(window);

        drawGroup.Draw(window);
        window.draw(sprite_right_wall);
        window.draw(sprite_left_wall);
        QuickDrawRectangle(sf::Color(255, 255, 255, specialDelay ? 100 : 255), sf::FloatRect(279, 24, 489 * special / 100.f, 11));

        text_health.setString(ConvertNumberToString(health));
        text_health.setOrigin(text_health.getGlobalBounds().width / 2.f, text_health.getGlobalBounds().height / 2.f);
        window.draw(text_health);

        text_score.setString(ConvertNumberToString(GetGameScore()));
        text_score.setOrigin(text_score.getGlobalBounds().width, 0);
        window.draw(text_score);

        for(auto obstacle : game_obstacles)
            obstacle->Draw(window);

        DrawFadeTransmit();

        window.display();
    }

    SetFadeTransmitState(false);
    drawGroup.Clear();
    drawGroup.AddSprite("gameover_score", sf::Vector2f(232, 127));

    text_score.setString(ConvertNumberToString(GetGameScore()));
    text_score.setCharacterSize(70);
    text_score.setOrigin(0, 0);
    text_score.setPosition(305, 520);

    if(health <= 0)
    {
        while(window.isOpen() && gameState == PlayState)
        {
            float changeInTime = timer_fps.restart().asSeconds();

             //Handling Inputs
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
                gameState = MainMenuState;

            //Rendering
            window.clear(GetBgColor());
            drawGroup.Draw(window);
            window.draw(text_score);
            DrawFadeTransmit();

            window.display();
        }
    }
    else gameState = MainMenuState;

    //Clearing or deleting objects
    for(auto ball : game_balls)
        delete ball;
    game_balls.clear();

    for(auto blob : game_blobs)
        delete blob;
    game_blobs.clear();

    for(auto pickup : game_pickups)
        delete pickup;
    game_pickups.clear();

    for(auto obstacle : game_obstacles)
        delete obstacle;
    game_obstacles.clear();
}
