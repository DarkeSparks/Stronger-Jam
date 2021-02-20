#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <array>
#include <any>

#include "Wall.h"
#include "Enity.h"

class Player : public Enity {
private:
    bool moveLeft = false, moveRight = false;
    bool moveUp = false, moveDown = false;

    bool LineCollision(std::array<sf::Vertex, 2> line, sf::Vector2f pos1, sf::Vector2f pos2);
public:
    sf::Vector2f futurePos;
    bool isDead = false;

    Player(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f speed, sf::Color color) : Enity(pos, size, speed, color) {};

    sf::RectangleShape Movement(sf::RectangleShape shape, float deltaTime);
    void UpdatedMovement(std::array<sf::Vertex, 8> lines, float deltaTime, Wall* wall, sf::RectangleShape &shape);

    void Input(sf::Event event, sf::Keyboard::Key keys[4]);
    void NoMovement();

    bool IsTooClose(Player* pl, sf::Vector2f size, float dist);
    sf::Vector2f PlayerDistance(Player* pl);
};

#endif // PLAYER_H