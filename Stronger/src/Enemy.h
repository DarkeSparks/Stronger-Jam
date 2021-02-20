#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <numeric>
#include <math.h>
#include <any>

#include "Wall.h"
#include "Enity.h"

class Enemy : public Enity {
private:
    sf::Vector2f EnemyDistance(Enemy* e);

    bool IsTooClose(Enemy* e, sf::Vector2f size, float dist);
    bool LineCollision(sf::Vertex line[2], sf::Vector2f pos3, sf::Vector2f pos4);
public:
    int health = 10;

    Enemy(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f speed, sf::Color color) : Enity(pos, size, speed, color) {};

    bool HasCollidedWithWall(Wall* wall);
    bool HasCollidedWithLine(sf::Vertex line[2]);

    sf::Vector2f Movement(Enity* e, sf::Vector2f size);
    sf::Vector2f MoveAway(std::vector<Enemy*> es, sf::Vector2f max, sf::Vector2f min);
    //void IsTooClose(Enity* pl, sf::Vector2f size, float dist);
};

#endif // PLAYER_H