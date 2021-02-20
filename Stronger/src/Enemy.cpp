#include "Enemy.h"

sf::Vector2f Enemy::Movement(Enity* e, sf::Vector2f size) {
    sf::Vector2f distance = sf::Vector2f((e->pos.x - this->pos.x), (e->pos.y - this->pos.y));

    //std::cout << (Normalize(distance.x, size.x, 0)) * this->speed.x * deltaTime << std::endl;

    return sf::Vector2f(distance.x / this->speed.x, distance.y / this->speed.y);

    // return sf::Vector2f(Normalize(distance.x, size.x, 0) * this->speed.x, Normalize(distance.y, size.y, 0) * this->speed.y);
    //return sf::Vector2f(((Normalize(distance.x, size.x, 0)) * this->speed.x) * deltaTime, (Normalize(distance.y, size.y, 0) * this->speed.y) * deltaTime);
}

sf::Vector2f Enemy::EnemyDistance(Enemy* e) {
    return sf::Vector2f(this->pos.x - e->pos.x, this->pos.y - e->pos.y);
}

bool Enemy::IsTooClose(Enemy* e, sf::Vector2f size, float dist) {
    return (Normalize(std::abs(this->EnemyDistance(e).x), size.x, 0) <= dist && (Normalize(std::abs(this->EnemyDistance(e).y), size.y, 0) <= dist));
}

bool Enemy::LineCollision(sf::Vertex line[2], sf::Vector2f pos1, sf::Vector2f pos2) {
    //sf::Vector2f pos1 = this->pos, pos2 = sf::Vector2f(this->pos + this->size);
    sf::Vector2f pos3 = line[0].position, pos4 = line[1].position;
  // calculate the distance to intersection point
    float uA = ((pos4.x-pos3.x)*(pos1.y-pos3.y) - (pos4.y-pos3.y)*(pos1.x-pos3.x)) / ((pos4.y-pos3.y)*(pos2.x-pos1.x) - (pos4.x-pos3.x)*(pos2.y-pos1.y));
    float uB = ((pos2.x-pos1.x)*(pos1.y-pos3.y) - (pos2.y-pos1.y)*(pos1.x-pos3.y)) / ((pos4.y-pos3.y)*(pos2.x-pos1.x) - (pos4.x-pos3.y)*(pos2.y-pos1.y));

    // if uA and uB are between 0-1, lines are colliding
    return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
    //return true;
  
  //return false;
}

bool Enemy::HasCollidedWithLine(sf::Vertex line[2]) {
    sf::Vector2f pos1 = this->pos, pos2 = sf::Vector2f(this->pos + this->size);

    bool left =   this->LineCollision(line, this->pos, sf::Vector2f(this->pos.x, this->pos.y+this->size.y));
    bool right =  this->LineCollision(line, sf::Vector2f(this->pos.x + this->size.x, this->pos.y), sf::Vector2f(this->pos.x+this->size.x,this->pos.y+this->size.y));
    bool top =    this->LineCollision(line, this->pos, sf::Vector2f(this->pos.x + this->size.x, this->pos.y));
    bool bottom = this->LineCollision(line, sf::Vector2f(this->pos.x,this->pos.y+this->size.y), sf::Vector2f(this->pos.x+this->size.x,this->pos.y+this->size.y));

  // if ANY of the above are true, the line
  // has hit the rectangle

  return (left || right || top || bottom);
  // if (left || right || top || bottom) {
  //   return true;
  // }
  // return false;
   
}

sf::Vector2f Enemy::MoveAway(std::vector<Enemy*> es, sf::Vector2f max, sf::Vector2f min) {
    if (es.size() > 1) {
        for(Enemy* e : es) {
            std::vector<float> distancesX;
            std::vector<float> distancesY;
            distancesX.push_back(this->EnemyDistance(e).x);
            distancesY.push_back(this->EnemyDistance(e).y);

            for (float dX : distancesX)
                for (float dY : distancesY) {
                if (dX > 0 && dY > 0) {
                    dX *= -1;
                    dY *= -1;
                }
            }

            float valueX = std::accumulate(distancesX.begin(), distancesX.end(), 0);
            float valueY = std::accumulate(distancesY.begin(), distancesY.end(), 0);

            return sf::Vector2f(Normalize(valueX, max.x, min.x), Normalize(valueY, max.y, min.y));
        } 
    }

    return sf::Vector2f(NULL, NULL);
}