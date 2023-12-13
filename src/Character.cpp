#include "Character.h"
#include <cmath>

Character::Character(float x, float y) 
    : position(x, y), health(100), isBlocking(false), isDodging(false), 
      invincible(false), canMove(true), aiReactionTime(0.25f) {
    shape.setSize(sf::Vector2f(40, 40)); // Player's shape
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);

    sword.setSize(sf::Vector2f(20, 5)); // Sword's shape
    sword.setFillColor(sf::Color::Red);
}

void Character::update(sf::FloatRect gameBounds, const Character& opponent) {
    updateDodge(gameBounds);
    updateBlock(opponent);
    updateAttack();
    sword.setPosition(position.x + shape.getSize().x, position.y + (shape.getSize().y / 2));
}

void Character::draw(sf::RenderWindow &window) {
    window.draw(shape);
    window.draw(sword);
}

void Character::move(float dx, float dy) {
    if (canMove) {
        lastDirection = sf::Vector2f(dx, dy);
        position.x += dx;
        position.y += dy;
        shape.setPosition(position);
    }
}

void Character::lightAttack(Character &target) {
    if (canAttack()) {
        target.takeDamage(5); // Light attack damage
        canMove = false;
        attackTimer.restart();
    }
}

void Character::heavyAttack(Character &target) {
    if (canAttack() && heavyAttackCooldown.getElapsedTime().asSeconds() > 2.0f) {
        isChargingHeavyAttack = true;
        heavyAttackChargeTimer.restart();
        heavyAttackCooldown.restart();
    }

    if (isChargingHeavyAttack && heavyAttackChargeTimer.getElapsedTime().asSeconds() > 0.5f) {
        isChargingHeavyAttack = false;
        target.takeDamage(15); // Heavy attack damage
        flashColor(sf::Color::Red, heavyAttackVulnerabilityTime); // Flash red indicating vulnerability
        canMove = false;
        attackTimer.restart();
    }
}

void Character::dodge(sf::FloatRect gameBounds) {
    if (canDodge() && dodgeCooldown.getElapsedTime().asSeconds() > dodgeCooldownTime) {
        isDodging = true;
        invincible = true;
        dodgeTimer.restart();
        dodgeCooldown.restart();
        sf::Vector2f dodgePosition = position + normalize(lastDirection) * dodgeDistance;
        if (gameBounds.contains(dodgePosition)) {
            position = dodgePosition;
            shape.setPosition(position);
        }
    }
}




void Character::block() {
    if (blockCooldown.getElapsedTime().asSeconds() > blockCooldownTime) {
        isBlocking = true;
        blockTimer.restart();
        blockCooldown.restart();
    }
}


void Character::aiUpdate(const Character& player, sf::FloatRect gameBounds) {
    if (aiDecisionClock.getElapsedTime().asSeconds() >= aiReactionTime) {
        // AI decision-making logic
        aiDecisionClock.restart();
    }

    update(gameBounds, player);
}

bool Character::isAlive() const {
    return health > 0;
}

void Character::takeDamage(float amount) {
    if (!invincible) {
        health -= amount;
        flashColor(sf::Color::Red, 0.2f);
    }
}

void Character::setAIDifficulty(const std::string& difficulty) {
    if (difficulty == "easy") {
        aiReactionTime = 0.3f; // 300ms
    } else if (difficulty == "medium") {
        aiReactionTime = 0.25f; // 250ms
    } else if (difficulty == "hard") {
        aiReactionTime = 0.2f; // 200ms
    } else if (difficulty == "expert") {
        aiReactionTime = 0.15f; // 150ms
    }
}

sf::Vector2f Character::getPosition() const {
    return position;
}

//update blocking, attacking, and dodging
void Character::updateBlock(const Character& opponent) {
    if (isBlocking && blockTimer.getElapsedTime().asSeconds() < blockDuration) {
        // Check if opponent is attacking and if the block is within the parry window
        if (opponent.isAttacking() && blockTimer.getElapsedTime().asSeconds() < parryWindow) {
            float reflectedDamage = opponent.isHeavyAttacking() ? heavyAttackDamage * parryDamageMultiplier : lightAttackDamage;
            opponent.takeDamage(reflectedDamage);
            flashColor(sf::Color::Green, 0.2f); // Flash green on successful parry
        }
    } else {
        isBlocking = false;
    }
}

void Character::updateDodge(sf::FloatRect gameBounds) {
    if (isDodging && dodgeTimer.getElapsedTime().asSeconds() < dodgeDuration) {
        invincible = true;
    } else {
        isDodging = false;
        invincible = false;
    }
}

void Character::updateAttack() {
    // Handle the cooldown after a light attack
    if (attackTimer.getElapsedTime().asSeconds() < 0.5f) { // Assuming 0.5 seconds for light attack cooldown
        canMove = false;
    } else {
        canMove = true;
    }

    // Handle the cooldown and vulnerability after a heavy attack
    if (heavyAttackCooldown.getElapsedTime().asSeconds() < 2.0f) { // Assuming 2 seconds for heavy attack cooldown
        if (heavyAttackCooldown.getElapsedTime().asSeconds() < 0.5f) { // Vulnerability period
            flashColor(sf::Color::Red, 0.2f); // Flash red indicating vulnerability
        }
        canMove = false;
    } else {
        canMove = true;
    }

    // Resetting the color after flashing effect
    if (flashTimer.getElapsedTime().asSeconds() >= 0.2f) {
        shape.setFillColor(sf::Color::White); // Reset to original color after flashing
    }
}

void Character::flashColor(sf::Color color, float duration) {
    originalColor = shape.getFillColor();
    shape.setFillColor(color);
    flashTimer.restart();

    if (flashTimer.getElapsedTime().asSeconds() >= duration) {
        shape.setFillColor(originalColor);
    }
}

sf::Vector2f Character::normalize(const sf::Vector2f& source) {
    float length = std::sqrt(source.x * source.x + source.y * source.y);
    if (length != 0)
        return sf::Vector2f(source.x / length, source.y / length);
    return source;
}

bool Character::canAttack() const {
    return !isDodging && canMove;
}

bool Character::canDodge() const {
    return !isChargingHeavyAttack;
}
