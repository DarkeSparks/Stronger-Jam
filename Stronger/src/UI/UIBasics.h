#ifndef UIBASICS_H
#define UIBASICS_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>

class UIBasics {
public:
    sf::Vector2f pos;

    sf::Color color;

    UIBasics();
    UIBasics(sf::Vector2f pos, sf::Color color);
};

#endif // UIBASICS_H