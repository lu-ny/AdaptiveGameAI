#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <string>

class Character {
public:
    Character(float x, float y);
    void update(sf::FloatRect gameBounds, const Character& opponent);
    void draw(sf::RenderWindow& window);
    void move(float dx, float dy);
    void lightAttack(Character& target);
    void heavyAttack(Character& target);
    void dodge(sf::FloatRect gameBounds);
    void block();
    void aiUpdate(const Character& player, sf::FloatRect gameBounds);
    void setAIDifficulty(const std::string& difficulty);
    bool isAlive() const;
    void takeDamage(float amount);
    sf::Vector2f getPosition() const;

    // Additional methods and members may be added as needed

private:
    sf::Vector2f position;
    float health;
    bool isBlocking, isDodging, invincible, canMove;
    sf::RectangleShape shape, sword;
    sf::Clock heavyAttackCooldown, dodgeCooldown, blockCooldown, attackTimer, dodgeTimer, blockTimer, aiDecisionClock;
    float aiReactionTime;
    sf::Vector2f lastDirection;

    // Additional private methods and members as discussed
};

#endif // CHARACTER_H
