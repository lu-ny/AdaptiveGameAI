#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Character.h"

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    Character player;
    Character ai;
    GameState gameState;
    sf::RectangleShape startButton, difficultyButton, settingsButton;
    sf::RectangleShape resumeButton, titleScreenButton, quitButton;
    sf::RectangleShape soundButton, videoButton, aiDataButton;
    sf::Font font;

    // methods for handling different screens
    void startScreen();
    void pausedScreen();
    void settingsScreen();
    void resetButtonColors();

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void handlePlayerInput(sf::Event event, bool isEvent);
    void handleContinuousInput();
    void processPauseInput(sf::Event event);
};

#endif // GAME_H
