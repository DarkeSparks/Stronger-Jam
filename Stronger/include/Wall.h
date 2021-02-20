#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>

class Wall {
public:
    sf::Vector2f pos;
    sf::Vector2f size;
    sf::Color color;

    Wall(sf::Vector2f pos, sf::Vector2f size, sf::Color color);

    sf::RectangleShape InitShape(sf::RectangleShape shape);
};

#endif // WALL_H