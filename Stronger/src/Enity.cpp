#include "Enity.h"

Enity::Enity() : Enity::Enity(sf::Vector2f(100, 100), sf::Vector2f(20, 20), sf::Vector2f(20, 20), sf::Color::Red) {};

Enity::Enity(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f speed, sf::Color color) {
    this->pos = pos;
    this->size = size;
    this->speed = speed;
    this->color = color;
}

void Enity::StopMovingWithWall(Wall* wall) {
    if (this->HasCollidedWithWall(wall)) {
        /* Check For Which Side */
        sf::Vector2f entry, exit;
        if (this->pos.x > wall->pos.x + wall->size.x || this->pos.y > wall->pos.y + wall->size.y)
            this->speed.x = std::abs(this->speed.x) * -1;
        if (this->pos.x + this->size.x > wall->pos.x || this->pos.y > wall->pos.y + wall->size.y)
            this->speed.x = std::abs(this->speed.x);
        
    }
}

sf::RectangleShape Enity::InitShape(sf::RectangleShape shape) {
    shape.setFillColor(this->color);
    shape.setPosition(this->pos);
    shape.setSize(this->size);
    return shape;
}

bool Enity::HasCollidedWithWall(Wall* wall) {
    return !(this->pos.x > wall->pos.x + wall->size.x || this->pos.x + this->size.x < wall->pos.x ||
             this->pos.y > wall->pos.y + wall->size.y || this->pos.y + this->size.y < wall->pos.y);
}

bool Enity::HasCollidedWithEnity(Enity* e) {
    return !(this->pos.x > e->pos.x + e->size.x || this->pos.x + this->size.x < e->pos.x ||
             this->pos.y > e->pos.y + e->size.y || this->pos.y + this->size.y < e->pos.y);
}

float Enity::Normalize(float value, float max, float min) {
    return (value - std::min(value, min)) / (std::max(value, max) - std::min(value, min));
}
