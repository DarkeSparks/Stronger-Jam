#ifndef BUTTON_H
#define BUTTON_H

#include "UIBasics.h"
#include <iostream>

class Button : public UIBasics {
private:
    bool CollidingWithPoint(sf::Vector2f point);
public:
    sf::Color buttonColor;
    sf::Color textColor;

    sf::Vector2f size;
    sf::Vector2f textPos;

    std::string stringText;

    // : UIBasics(pos, color, stringText) {}

    Button(sf::Vector2f size, sf::Vector2f pos, sf::Vector2f textPos, sf::Color color, std::string stringText);

    sf::RectangleShape InitShape(sf::RectangleShape shape);

    void InitText(sf::Text& text, sf::Color color);

    bool ButtonInput(sf::Vector2f point);
};

#endif // BUTTON_H