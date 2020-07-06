#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Logger.h"
#include "ImageLoader.h"
#include "Wall.h"
#include "Ghost.h"
#include "Player.h"
#include "Coin.h"
#include "Camera.h"
#include "Heart.h"
#include "Fire.h"

int WINDOW_WIDTH{1500};
int WINDOW_HEIGHT{1000};
int PLAYER_SPEED{6};
int INITIAL_PLAYER_HP{20};
int PLAYER_DAMAGE{5};
bool DAMAGE_PLAYER_WHEN_OUT_OF_SCREEN{true};
int LEVEL_WIDTH{10000};

int g_lastPlayerScore{};

/*
 * Initializes SDL and creates the window and the renderer.
 */
void initVideo(SDL_Window *&window, SDL_Renderer *&renderer)
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
            "Egyptian Jumper",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            0
    );

    if (!window) { Logger::fatal("Failed to create window: "+std::string(SDL_GetError())); std::exit(1); }

    renderer = SDL_CreateRenderer(
            window,
            -1,
           SDL_RENDERER_ACCELERATED
    );

    if (!renderer) { Logger::fatal("Failed to create renderer: "+std::string(SDL_GetError())); std::exit(1); }

    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND))
        Logger::error("SDL_SetRenderDrawBlendMode() failed: "+std::string(SDL_GetError()));

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

/*
 * Creates an ImageLoader object, returns a pointer to it and loads the images.
 */
ImageLoader *loadImages(SDL_Window *window, SDL_Renderer *renderer)
{
    ImageLoader *imageLoader = new ImageLoader{window, renderer, "assets/images"};
    imageLoader->loadImage("background_wall_sandstone.bmp", "background_wall");
    imageLoader->loadImage("character_right.bmp", "player_right");
    imageLoader->loadImage("character_left.bmp", "player_left");
    imageLoader->loadImage("ghost_right.bmp");
    imageLoader->loadImage("ghost_left.bmp");
    imageLoader->loadImage("logo.bmp");
    for (int i{}; i < 26; ++i)
    imageLoader->loadImage("coin_"+std::to_string(i)+".bmp");
    imageLoader->loadImage("hp_text_white.bmp");
    imageLoader->loadImage("score_text_white.bmp");
    imageLoader->loadImage("heart_hud.bmp");
    imageLoader->loadImage("heart_half_hud.bmp");
    imageLoader->loadImage("coin_hud.bmp");
    imageLoader->loadImage("vignette.bmp");
    imageLoader->loadImage("controls_text.bmp");
    imageLoader->loadImage("heart_0.bmp");
    imageLoader->loadImage("heart_1.bmp");
    imageLoader->loadImage("platform.bmp", "wall");
    imageLoader->loadImage("play_again_text.bmp");
    for (int i{}; i < 5; ++i)
    imageLoader->loadImage("fire_"+std::to_string(i)+".bmp");
    imageLoader->loadImage("press_space_text.bmp");
    imageLoader->loadImage("score_text.bmp");
    for (int i{}; i < 10; ++i)
    imageLoader->loadImage("digit_"+std::to_string(i)+".bmp");

    return imageLoader;
}

// returns true if the player is dead
bool mainLoop(SDL_Window *window, SDL_Renderer *renderer, ImageLoader *imageLoader)
{
    using size_type = std::vector<int>::size_type;

    Camera *camera = new Camera{-500, 0};

    int lastTileY{500};
    std::vector <Wall*> wallList;
    wallList.push_back(new Wall{490, 500, 80, 40, renderer, imageLoader});
    for (int j{}; j < 3; ++j)
    for (int i{}; i < LEVEL_WIDTH/100; ++i)
        wallList.push_back(new Wall{570+200*i+rand()%200, lastTileY+rand()%50-50/2+j*500, 80, 40, renderer, imageLoader});

    Player *player = new Player{500, 0, 45, 150, renderer, imageLoader, INITIAL_PLAYER_HP, PLAYER_DAMAGE, &wallList};

    std::vector<Ghost*> ghostList;
    for (int i{}; i < LEVEL_WIDTH/200; ++i)
        ghostList.push_back(new Ghost{1000+rand()%LEVEL_WIDTH, rand()%WINDOW_HEIGHT, 100, 100, renderer, imageLoader, 10, 1, &wallList});

    std::vector<Coin*> coinList;
    for (int i{}; i < LEVEL_WIDTH/100; ++i)
        coinList.push_back(new Coin{500+i*120+40+(rand()%200+1), 0, 40, 40, renderer, imageLoader, &wallList});

    std::vector<Heart*> heartList;
    for (int i{}; i < LEVEL_WIDTH/100; ++i)
        heartList.push_back(new Heart{500+i*480+60+(rand()%400+1), 0, 15, 15, renderer, imageLoader, &wallList});


    std::vector<Fire*> fireList;
    for (int i{}; i < LEVEL_WIDTH/100; ++i)
        fireList.push_back(new Fire{500+i*480+60+(rand()%400+1), 0, 30, 30, renderer, imageLoader, &wallList});

    // ----------------------------------------------------

    bool done{};
    bool isDebugMode{false};
    double cameraSpeed{0.5}; // real_speed = playerSpeed * cameraSpeed

    while (!done) // main loop
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = true;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_F3:
                    isDebugMode = !isDebugMode;
                    break;
                }
                break;
            }
        } // end of event processing loop

        if (player->getIsDead())
            done = true;

        if (done)
            break;

        // ------------ update objects --------------------
        for (auto ghost : ghostList)
            ghost->update();
        for (auto coin : coinList)
            coin->update();
        for (auto heart : heartList)
            heart->update();
        for (auto fire : fireList)
            fire->update();

        for (auto ghost : ghostList)
            if (ghost->getIsColliding(player))
                player->damage(ghost->getDamageAmount());

        for (auto fire : fireList)
            if (fire->getIsColliding(player))
                player->damage(fire->getDamageAmount());

        for (size_type i{}; i < coinList.size(); ++i)
        {
            if (coinList[i]->getIsColliding(player))
            {
                player->addScore(coinList[i]->getDamageAmount());
                coinList[i]->kill();
            }
        }

        for (size_type i{}; i < heartList.size(); ++i)
        {
            if (heartList[i]->getIsColliding(player))
            {
                player->addHealth(heartList[i]->getDamageAmount());
                heartList[i]->kill();
            }
        }

        player->update();
        // ------------------------------------------------

        // ---------------- handle player movement --------
        SDL_PumpEvents();
        auto keyState{SDL_GetKeyboardState(nullptr)};

        if (keyState[SDL_SCANCODE_A])
        {
            player->moveX(-PLAYER_SPEED);
        }
        if (keyState[SDL_SCANCODE_D])
        {
            player->moveX(PLAYER_SPEED);
        }
        if (keyState[SDL_SCANCODE_SPACE])
            player->jump();
        // ------------------------------------------------

        // ---------------- render objects ----------------
        if (SDL_RenderCopy(renderer, imageLoader->getImage("background_wall"), nullptr, nullptr))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        SDL_Rect logoRect{300-camera->getX(), 200-camera->getY(), 900, 130};
        if (SDL_RenderCopy(renderer, imageLoader->getImage("logo"), nullptr, &logoRect))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        SDL_Rect controlsRect{50-camera->getX(), 500-camera->getY(), 250, 120};
        if (SDL_RenderCopy(renderer, imageLoader->getImage("controls_text"), nullptr, &controlsRect))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        for (auto wall : wallList)
            wall->render(camera);
        for (auto coin : coinList)
            coin->render(camera);
        for (auto heart : heartList)
            heart->render(camera);
        for (auto fire : fireList)
            fire->render(camera);
        player->render(camera);
        for (auto ghost : ghostList)
            ghost->render(camera);


        if (SDL_RenderCopy(renderer, imageLoader->getImage("vignette"), nullptr, nullptr))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        SDL_Rect hudRect{0, 0, WINDOW_WIDTH, 70};
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 150);
        if (SDL_RenderFillRect(renderer, &hudRect))
            Logger::error("SDL_RenderFillRect() failed: "+std::string(SDL_GetError()));

        SDL_Rect hpTextRect{10, 10, 38, 20};
        if (SDL_RenderCopy(renderer, imageLoader->getImage("hp_text_white"), nullptr, &hpTextRect))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        SDL_Rect scoreTextRect{600, 10, 80, 20};
        if (SDL_RenderCopy(renderer, imageLoader->getImage("score_text_white"), nullptr, &scoreTextRect))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        {
            int i{};
            for (; i < player->getHealth()/2; ++i) // one heart means two HPs
            {
                SDL_Rect heartRect{60+i%15*20, 15+i/15*20, 16, 16};
                if (SDL_RenderCopy(renderer, imageLoader->getImage("heart_hud"), nullptr, &heartRect))
                    Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));
            }
            if (player->getHealth()%2) // so if we have odd number of HP, we have to render plus a half heart
            {
                SDL_Rect heartRect{60+i%15*20, 15+i/15*20, 16, 16};
                if (SDL_RenderCopy(renderer, imageLoader->getImage("heart_half_hud"), nullptr, &heartRect))
                    Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));
            }
        }

        for (int i{}; i < player->getScore(); ++i)
        {
            SDL_Rect coinRect{690+i%40*20, 15+20*(int)(i/40), 16, 16};
            if (SDL_RenderCopy(renderer, imageLoader->getImage("coin_hud"), nullptr, &coinRect))
                Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));
        }
        // ------------------------------------------------

        // ----- render hitboxes if in debug mode ---------
        if (isDebugMode)
        {
            for (auto coin : coinList)
                coin->renderHitbox(camera);
            for (auto heart : heartList)
                heart->renderHitbox(camera);
            for (auto fire : fireList)
                fire->renderHitbox(camera);
            player->renderHitbox(camera);
            for (auto wall : wallList)
                wall->renderHitbox(camera);
            for (auto ghost : ghostList)
                ghost->renderHitbox(camera);
        }
        // ------------------------------------------------

        if (DAMAGE_PLAYER_WHEN_OUT_OF_SCREEN)
        {
            camera->setX(camera->getX()+(PLAYER_SPEED*cameraSpeed));

            if (player->getX() < camera->getX()-45)
                player->damage(1);
        }
        else
        {
            camera->setX(player->getX()-WINDOW_WIDTH/2);
        }

        camera->setY(player->getY()-WINDOW_HEIGHT/2);

        if (cameraSpeed < 0.90)
            cameraSpeed += 0.0005;

        SDL_RenderPresent(renderer);
        SDL_Delay(16.67);

    } // end of main loop

    bool isPlayerDead{player->getIsDead()};

    g_lastPlayerScore = player->getScore();

    delete player;
    for (auto wall : wallList)
        delete wall;
    for (auto ghost : ghostList)
        delete ghost;
    for (auto coin : coinList)
        delete coin;
    for (auto heart : heartList)
        delete heart;
    for (auto fire : fireList)
        delete fire;

    delete camera;

    return isPlayerDead;
}

// returns true if the user wants to play again, otherwise returns false
bool askRestart(SDL_Window *window, SDL_Renderer *renderer, ImageLoader *imageLoader)
{
    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return false;
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    return false;

                case SDLK_SPACE:
                    return true;
                }
                break;
            }
        } // end of event processing loop

        if (SDL_RenderCopy(renderer, imageLoader->getImage("background_wall"), nullptr, nullptr))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        SDL_Rect logoRect{250, 300, 1000, 150};
        if (SDL_RenderCopy(renderer, imageLoader->getImage("logo"), nullptr, &logoRect))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        SDL_Rect scoreTextRect{500, 550, 300, 80};
        if (SDL_RenderCopy(renderer, imageLoader->getImage("score_text"), nullptr, &scoreTextRect))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        for (std::string::size_type i{}; i < std::to_string(g_lastPlayerScore).length(); ++i)
        {
            SDL_Rect scoreDigitRect{(int)(840+i*50), 565, 50, 50};
            std::string currentDigitStr{std::to_string(g_lastPlayerScore)[i]};

            if (SDL_RenderCopy(renderer, imageLoader->getImage("digit_"+currentDigitStr), nullptr, &scoreDigitRect))
                Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));
        }

        SDL_Rect playAgainTextRect{350, 700, 800, 400};
        if (SDL_RenderCopy(renderer, imageLoader->getImage("play_again_text"), nullptr, &playAgainTextRect))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        SDL_RenderPresent(renderer);
        SDL_Delay(40);
    }

    return false; // this is impossible
}

// returns true if the user did NOT close the window
bool showMenu(SDL_Window *window, SDL_Renderer *renderer, ImageLoader *imageLoader)
{
    uint8_t textAlpha{0};
    bool isTextAlphaIncrementing{true};

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return false;
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    return true;
                }
                break;
            }
        } // end of event processing loop

        if (SDL_RenderCopy(renderer, imageLoader->getImage("background_wall"), nullptr, nullptr))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        SDL_Rect logoRect{250, 400, 1000, 150};
        if (SDL_RenderCopy(renderer, imageLoader->getImage("logo"), nullptr, &logoRect))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        if (SDL_SetTextureAlphaMod(imageLoader->getImage("press_space_text"), textAlpha))
            Logger::error("SDL_SetTextureAlphaMod() failed: "+std::string(SDL_GetError()));
        SDL_Rect pressSpaceRect{550, 800, 400, 60};
        if (SDL_RenderCopy(renderer, imageLoader->getImage("press_space_text"), nullptr, &pressSpaceRect))
            Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));

        SDL_RenderPresent(renderer);
        SDL_Delay(40);

        if (textAlpha < 255 && isTextAlphaIncrementing)
            textAlpha += 5;
        else if (textAlpha > 0 && !isTextAlphaIncrementing)
            textAlpha -= 5;
        else if (textAlpha == 255)
            isTextAlphaIncrementing = false;
        else if (textAlpha == 0)
            isTextAlphaIncrementing = true;
    }

    return false; // this is impossible
}

int main()
{
    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
    initVideo(window, renderer);

    ImageLoader *imageLoader{loadImages(window, renderer)};

    // Seed std::rand()
    std::srand(std::time(nullptr));

    bool didUserExit{!showMenu(window, renderer, imageLoader)};


    // Don't start if the user closed the window while the main menu was being showed
    while (!didUserExit)
    {
        if (!mainLoop(window, renderer, imageLoader))
            break;

        if (!askRestart(window, renderer, imageLoader))
            break;
    }

    delete imageLoader;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    Logger::info("End");

    return 0;
}
