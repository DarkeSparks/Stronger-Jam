#include "Button.h"

Button::Button(sf::Vector2f size, sf::Vector2f pos, sf::Vector2f textPos, sf::Color color, std::string stringText) : UIBasics(pos, color) {
        this->buttonColor = color;
        this->textPos = textPos;
        this->stringText = stringText;

        this->pos = pos;
        this->size = size;
}

sf::RectangleShape Button::InitShape(sf::RectangleShape shape) {
    shape.setSize(this->size);
    shape.setFillColor(this->buttonColor);
    shape.setPosition(this->pos);

    return shape;
}

void Button::InitText(sf::Text& text, sf::Color color) {
    this->textColor = color;

    text.setPosition(this->textPos);
    text.setColor(this->textColor);
}

bool Button::ButtonInput(sf::Vector2f point) {

    return (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->CollidingWithPoint(point));
    // if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
    //     if (this->CollidingWithPoint(point)) 
    //         return true;
    //     
    // return false;
}

bool Button::CollidingWithPoint(sf::Vector2f point) {
    return  (point.x >= this->pos.x && point.x <= this->pos.x + this->size.x && point.y >= this->pos.y && point.y <= this->pos.y + this->size.y); //!(this->pos.x > point.x || this->pos.x + this->size.x < point.x || this->pos.y > point.y || this->pos.y + this->size.y < point.y);
}