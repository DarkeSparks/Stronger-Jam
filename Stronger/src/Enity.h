#ifndef ENITY_H
#define ENITY_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <any>

#include "Wall.h"

class Enity {
public:
    sf::Vector2f pos;
    sf::Vector2f size;
    sf::Vector2f speed;
    sf::Color color;

    int health = 10;

    Enity();
    Enity(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f speed, sf::Color color);

    sf::RectangleShape InitShape(sf::RectangleShape shape);

    void StopMovingWithWall(Wall* wall);

    bool HasCollidedWithWall(Wall* wall);
    bool HasCollidedWithEnity(Enity* e);

    float Normalize(float value, float max, float min);
};

#endif // ENITY_H