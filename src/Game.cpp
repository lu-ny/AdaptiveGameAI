#include "Game.h"
#include <iostream>


Game::Game() : window(sf::VideoMode(800, 600), "AI Combat Game"),
               player(100, 100), ai(400, 300) {
    // Initialize other game state variables if necessary
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

//game states
void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        switch (gameState) {
            case GameState::StartScreen:
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        handleStartScreenClick(event);
                    }
                }
                break;

            case GameState::Playing:
                handlePlayerInput(event, event.type == sf::Event::KeyPressed);
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    gameState = GameState::Paused;
                }
                break;

            case GameState::Paused:
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        handlePauseScreenClick(event);
                    }
                }
                break;

            case GameState::Settings:
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        handleSettingsScreenClick(event);
                    }
                }
                break;
        }
    }
    if (gameState == GameState::Playing) {
        handleContinuousInput();
    }
}

//UI interaction
void Game::resetButtonColors() {
    // Reset colors for all buttons
    startButton.setFillColor(sf::Color::Blue);
    difficultyButton.setFillColor(sf::Color::Blue);
    settingsButton.setFillColor(sf::Color::Blue);
    resumeButton.setFillColor(sf::Color::Blue);
    titleScreenButton.setFillColor(sf::Color::Blue);
    quitButton.setFillColor(sf::Color::Blue);
    soundButton.setFillColor(sf::Color::Blue);
    videoButton.setFillColor(sf::Color::Blue);
    aiDataButton.setFillColor(sf::Color::Blue);
}

void Game::handleStartScreenClick(sf::Event& event) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (startButton.getGlobalBounds().contains(mousePos)) {
        startButton.setFillColor(sf::Color::Green);
        gameState = GameState::Playing;
    } else if (difficultyButton.getGlobalBounds().contains(mousePos)) {
        difficultyButton.setFillColor(sf::Color::Green);
        // Logic to change difficulty
    } else if (settingsButton.getGlobalBounds().contains(mousePos)) {
        settingsButton.setFillColor(sf::Color::Green);
        gameState = GameState::Settings;
    }
    resetButtonColors(); // Reset colors for next frame
}

void Game::handlePauseScreenClick(sf::Event& event) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (resumeButton.getGlobalBounds().contains(mousePos)) {
        resumeButton.setFillColor(sf::Color::Green);
        gameState = GameState::Playing;
    } else if (titleScreenButton.getGlobalBounds().contains(mousePos)) {
        titleScreenButton.setFillColor(sf::Color::Green);
        gameState = GameState::StartScreen;
    } else if (quitButton.getGlobalBounds().contains(mousePos)) {
        quitButton.setFillColor(sf::Color::Green);
        window.close();
    }
    resetButtonColors();
}

void Game::handleSettingsScreenClick(sf::Event& event) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (soundButton.getGlobalBounds().contains(mousePos)) {
        soundButton.setFillColor(sf::Color::Green);
        // Toggle sound settings
    } else if (videoButton.getGlobalBounds().contains(mousePos)) {
        videoButton.setFillColor(sf::Color::Green);
        // Adjust video settings
    } else if (aiDataButton.getGlobalBounds().contains(mousePos)) {
        aiDataButton.setFillColor(sf::Color::Green);
        // Toggle AI data upload setting
    }
    resetButtonColors();
}



void Game::update(sf::Time deltaTime) {
    player.update(/* game bounds */, ai);
    ai.update(/* game bounds */, player);
    // Additional game logic
}

void Game::render() {
    window.clear();
    player.draw(window);
    ai.draw(window);
    window.display();
}

// Implement handlePlayerInput and handleContinuousInput
void Game::handlePlayerInput(sf::Event event, bool isEvent) {
    if (isEvent) {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::J:  // Light Attack
                    player.lightAttack(ai);
                    break;
                case sf::Keyboard::K:  // Heavy Attack
                    player.heavyAttack(ai);
                    break;
                case sf::Keyboard::Space:  // Dodge
                    player.dodge(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
                    break;
                case sf::Keyboard::LShift:  // Block
                    player.block();
                    break;
                // Add more cases for other keys if needed
            }
        }
    }
}

//More Input handling
void Game::handleContinuousInput() {
    float moveSpeed = 5.0f; // Adjust speed as necessary
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.move(-moveSpeed, 0); // Move left
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.move(moveSpeed, 0); // Move right
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        player.move(0, -moveSpeed); // Move up
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        player.move(0, moveSpeed); // Move down
    }
}


// UI
void Game::startScreen() {
    sf::Font font;
    if (!font.loadFromFile("fonts/font.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    // Create rectangles for options
    sf::RectangleShape startButton(sf::Vector2f(200, 50));
    sf::RectangleShape difficultyButton(sf::Vector2f(200, 50));
    sf::RectangleShape settingsButton(sf::Vector2f(200, 50));

    startButton.setPosition(300, 200);
    difficultyButton.setPosition(300, 300);
    settingsButton.setPosition(300, 400);

    startButton.setFillColor(sf::Color::Blue);
    difficultyButton.setFillColor(sf::Color::Blue);
    settingsButton.setFillColor(sf::Color::Blue);

    // Create text for each option
    sf::Text startText("Start", font, 20);
    sf::Text difficultyText("Difficulty", font, 20);
    sf::Text settingsText("Settings", font, 20);

    startText.setPosition(320, 210);
    difficultyText.setPosition(320, 310);
    settingsText.setPosition(320, 410);

    window.draw(startButton);
    window.draw(difficultyButton);
    window.draw(settingsButton);

    window.draw(startText);
    window.draw(difficultyText);
    window.draw(settingsText);
}

void Game::pausedScreen() {
    sf::Font font;
    if (!font.loadFromFile("fonts/font.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    // Create rectangles for pause menu options
    sf::RectangleShape resumeButton(sf::Vector2f(200, 50));
    sf::RectangleShape titleScreenButton(sf::Vector2f(200, 50));
    sf::RectangleShape quitButton(sf::Vector2f(200, 50));

    resumeButton.setPosition(300, 200);
    titleScreenButton.setPosition(300, 300);
    quitButton.setPosition(300, 400);

    resumeButton.setFillColor(sf::Color::Blue);
    titleScreenButton.setFillColor(sf::Color::Blue);
    quitButton.setFillColor(sf::Color::Blue);

    // Create text for each option
    sf::Text resumeText("Resume", font, 20);
    sf::Text titleScreenText("Title Screen", font, 20);
    sf::Text quitText("Quit", font, 20);

    resumeText.setPosition(320, 210);
    titleScreenText.setPosition(320, 310);
    quitText.setPosition(320, 410);

    window.draw(resumeButton);
    window.draw(titleScreenButton);
    window.draw(quitButton);
    window.draw(resumeText);
    window.draw(titleScreenText);
    window.draw(quitText);
}


void Game::settingsScreen() {
    sf::Font font;
    if (!font.loadFromFile("fonts/font.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    // Create rectangles for settings options
    sf::RectangleShape soundButton(sf::Vector2f(200, 50));
    sf::RectangleShape videoButton(sf::Vector2f(200, 50));
    sf::RectangleShape aiDataButton(sf::Vector2f(200, 50));

    soundButton.setPosition(300, 200);
    videoButton.setPosition(300, 300);
    aiDataButton.setPosition(300, 400);

    soundButton.setFillColor(sf::Color::Blue);
    videoButton.setFillColor(sf::Color::Blue);
    aiDataButton.setFillColor(sf::Color::Blue);

    // Create text for each option
    sf::Text soundText("Sound", font, 20);
    sf::Text videoText("Video", font, 20);
    sf::Text aiDataText("AI Data Upload", font, 20);

    soundText.setPosition(320, 210);
    videoText.setPosition(320, 310);
    aiDataText.setPosition(320, 410);

    window.draw(soundButton);
    window.draw(videoButton);
    window.draw(aiDataButton);
    window.draw(soundText);
    window.draw(videoText);
    window.draw(aiDataText);
}


