#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Character.h"

int main()
{
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            Game game;
            game.run();
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}
