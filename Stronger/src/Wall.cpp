#include "Wall.h"

Wall::Wall(sf::Vector2f pos, sf::Vector2f size, sf::Color color) {
    this->pos = pos;
    this->size = size;
    this->color = color;
}

sf::RectangleShape Wall::InitShape(sf::RectangleShape shape) { 
    shape.setFillColor(this->color);
    shape.setPosition(this->pos);
    shape.setSize(this->size);
    return shape;
}