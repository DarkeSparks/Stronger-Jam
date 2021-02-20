#include "UIBasics.h"

UIBasics::UIBasics() : UIBasics::UIBasics(sf::Vector2f(0, 0), sf::Color::White) {};

UIBasics::UIBasics(sf::Vector2f pos, sf::Color color) {
    this->pos = pos;
    this->color = color;
}