#include "Player.h"

void Player::Input(sf::Event event, sf::Keyboard::Key keys[4]) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == keys[3]) moveRight = true; 
        if (event.key.code == keys[1]) moveLeft = true;
        if (event.key.code == keys[0]) moveUp = true;
        if (event.key.code == keys[2]) moveDown = true;
    }
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == keys[3]) moveRight = false;
        if (event.key.code == keys[1]) moveLeft = false;
        if (event.key.code == keys[0]) moveUp = false;
        if (event.key.code == keys[2]) moveDown = false;
    }
}

void Player::NoMovement() {
    moveRight = false; moveLeft = false;
    moveUp = false; moveDown = false;
}

bool Player::IsTooClose(Player* pl, sf::Vector2f size, float dist) {
    return (Normalize(this->PlayerDistance(pl).x, size.x, 0) <= dist && Normalize(this->PlayerDistance(pl).y, size.y, 0) <= dist);
}

bool Player::LineCollision(std::array<sf::Vertex, 2> line, sf::Vector2f pos1, sf::Vector2f pos2) {
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

void Player::UpdatedMovement(std::array<sf::Vertex, 8> lines, float deltaTime, Wall* wall, sf::RectangleShape &shape) {
    sf::Vector2f lastPos = sf::Vector2f(this->pos.x + (this->size.x / 2) * 2, this->pos.y + (this->size.y / 2) * 2);
    sf::Vector2f currentPos = sf::Vector2f(0, 0);


    bool left =   this->LineCollision({lines[0], lines[1]}, lastPos, currentPos);
    bool right =  this->LineCollision({lines[2], lines[3]}, lastPos, currentPos);// sf::Vector2f(this->pos.x + this->size.x, this->pos.y), sf::Vector2f(this->pos.x+this->size.x,this->pos.y+this->size.y));
    bool top =    this->LineCollision({lines[4], lines[5]}, lastPos, currentPos);// this->pos, sf::Vector2f(this->pos.x + this->size.x, this->pos.y));
    bool bottom = this->LineCollision({lines[6], lines[7]}, lastPos, currentPos);
    
    std::cout << left << right << top << bottom << std::endl;

    //if (!HasCollidedWithWall(wall)) {
    if (this->moveRight != false && right !=  true) this->pos.x += this->speed.x * deltaTime;
    if (this->moveLeft != false  && left !=   true)  this->pos.x -= this->speed.x * deltaTime;
    if (this->moveDown != false  && bottom != true)  this->pos.y += this->speed.y * deltaTime;
    if (this->moveUp != false    && top !=    true)    this->pos.y -= this->speed.y * deltaTime;
    //}

    currentPos = sf::Vector2f(this->pos.x + (this->size.x / 2), this->pos.y + (this->size.y / 2));
    // sf::Vector2f(this->pos.x,this->pos.y+this->size.y), sf::Vector2f(this->pos.x+this->size.x,this->pos.y+this->size.y));

    
    shape.setPosition(this->pos);

    //return shape;
}

sf::Vector2f Player::PlayerDistance(Player* pl) {
    return sf::Vector2f(std::abs(this->pos.x - pl->pos.x), std::abs(this->pos.y - pl->pos.y));
}

sf::RectangleShape Player::Movement(sf::RectangleShape shape, float deltaTime) {

    // bool futurePosition = this->HasCollidedWithWall(void);
    if (this->moveRight != false) this->futurePos.x = this->speed.x * deltaTime;
    if (this->moveLeft != false)  this->futurePos.x = -this->speed.x * deltaTime;
    if (this->moveDown != false)  this->futurePos.y = this->speed.y * deltaTime;
    if (this->moveUp != false)    this->futurePos.y = -this->speed.y * deltaTime;

    if (this->moveRight != true && this->moveLeft != true) this->futurePos = sf::Vector2f(0, this->futurePos.y);
    if (this->moveUp != true && this->moveDown != true) this->futurePos = sf::Vector2f(this->futurePos.x, 0);

    if (this->moveRight != false) this->pos.x += this->speed.x * deltaTime;
    if (this->moveLeft != false)  this->pos.x -= this->speed.x * deltaTime;
    if (this->moveDown != false)  this->pos.y += this->speed.y * deltaTime;
    if (this->moveUp != false)    this->pos.y -= this->speed.y * deltaTime;

    //this->pos += this->futurePos;

    shape.setPosition(this->pos);

    return shape;
}