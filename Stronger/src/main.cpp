#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "UI/Button.h"

#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "Level.h"


#include <iostream>
#include <vector>
#include <math.h>
#include <sstream>

#define PLAYABLECOLOR sf::Color(58, 89, 224, 255)
#define UNPLAYABLECOLOR sf::Color(82, 60, 105, 255)

#define SOUNDNUMBER 2


sf::Vector2f windowSize(800, 600);

float DeltaTime(sf::Clock c);
float Normalize(float value, float max, float min);

void Restart(std::vector<Wall*> &walls, std::vector<Enemy*> &e, std::vector<sf::RectangleShape> &enemyShape, std::vector<sf::RectangleShape> &wallShape, Player* pls[2], sf::Clock &playerClock, std::vector<sf::Clock> &edc, std::string levelName);

// void CreateUiText(sf::Text text, sf::Font font, std::string stringText, sf::Color color, sf::Vector2f pos, int size);

int main(void)
{
    //Setup SFML
    sf::RenderWindow window( sf::VideoMode( windowSize.x,windowSize.y), "Stronger");

    sf::Font font;

    sf::Text timer;

    if (!font.loadFromFile("bin/Fonts/FFFFORWA.TTF")) return -1; 

    timer.setFont(font);
    timer.setCharacterSize(40);
    timer.setPosition(sf::Vector2f(windowSize.x / 2.75, 40));

    srand((unsigned)time(NULL));
    
    sf::SoundBuffer buffer[SOUNDNUMBER];
    sf::Sound soundEfx[SOUNDNUMBER];

    // sf::Sound deadEfx;
    // sf::Sound switchEfx;


    std::string soundNames[SOUNDNUMBER] = {
      "bin/SoundEfx/Dead_Sound.wav",
      "bin/SoundEfx/Switch.wav"
    };

    for (int i = 0; i < SOUNDNUMBER; i++) {
      if (!buffer[i].loadFromFile(soundNames[i])) 
        return -1;

      soundEfx[i].setBuffer(buffer[i]);

    }

    // deadEfx.setBuffer(buffer[0]);
    // switchEfx.setBuffer(buffer[1]);

    Button* restart = new Button(sf::Vector2f(125.0f, 45.0f), sf::Vector2f(windowSize.x / 2.25, windowSize.y / 2), 
      sf::Vector2f(windowSize.x / 2.18, windowSize.y / 1.9), sf::Color::White, "Restart");

    Button* quit = new Button(sf::Vector2f(90.0f, 45.0f), sf::Vector2f(windowSize.x / 2.15, windowSize.y / 1.5), 
      sf::Vector2f(windowSize.x / 2.05, windowSize.y / 1.45), sf::Color::White, "Quit");



    sf::Text restartText = sf::Text(restart->stringText, font, 20);
    sf::Text quitText = sf::Text(quit->stringText, font, 20);

    restart->InitText(restartText, sf::Color::Black);
    quit->InitText(quitText, sf::Color::Black);
  
    //restart->InitText(restartText, sf::Color::Black);
    
    //menus::RestartMenu({restart, quit}, windowSize, {restartText, quitText}, font);

    sf::RectangleShape restartShape;
    sf::RectangleShape quitShape;

    //quit->InitText(quitText, sf::Color::Black);

    restartShape = restart->InitShape(restartShape);
    quitShape = quit->InitShape(quitShape);


    Player* pls[2] = {
      new Player(sf::Vector2f(400, 300), sf::Vector2f(20, 20), sf::Vector2f(1.05, 1.05), PLAYABLECOLOR),
      new Player(sf::Vector2f(400, 300), sf::Vector2f(20, 20), sf::Vector2f(1.05, 1.05), UNPLAYABLECOLOR)
    };

    sf::RectangleShape playerShapes[2];
    for (int i = 0; i < 2; i++) playerShapes[i] = pls[i]->InitShape(playerShapes[i]);

    std::vector<Enemy*> enemys;
    std::vector<sf::RectangleShape> enemyShapes;

    std::vector<Wall*> walls;
    std::vector<sf::RectangleShape> wallShapes;


    level::CreateLevels(pls, walls, enemys, "level_1");

    for (int i = 0; i < walls.size(); i++) {
      wallShapes.push_back(sf::RectangleShape(walls.at(i)->size));
      wallShapes.at(i) = walls.at(i)->InitShape(wallShapes.at(i));
    }

    for (int i = 0; i < enemys.size(); i++) {
      enemyShapes.push_back(sf::RectangleShape(enemys.at(i)->size));
      enemyShapes.at(i) = enemys.at(i)->InitShape(enemyShapes.at(i));
    }

    sf::Keyboard::Key pl1Keys[4] {sf::Keyboard::W, sf::Keyboard::A, sf::Keyboard::S, sf::Keyboard::D};

    sf::Clock deltaClock;
    sf::Clock playersClock;
    std::vector<sf::Clock> enemyDamageClocks;

    for (int i = 0; i < enemys.size(); i++)
      enemyDamageClocks.push_back(sf::Clock());

    sf::Color deadColor = sf::Color(0, 0, 0, 125);
    sf::RectangleShape deadShape(windowSize);

    sf::Vertex playersLine[2] {
      sf::Vertex(sf::Vector2f(pls[0]->pos.x + pls[0]->size.x / 2, pls[0]->pos.y + pls[0]->size.y / 2), sf::Color::Yellow),
      sf::Vertex(sf::Vector2f(pls[1]->pos.x + pls[1]->size.x / 2, pls[1]->pos.y + pls[1]->size.y / 2), sf::Color::Yellow)
    };

    sf::Vertex topPlayerCollisionLine[2] {
      sf::Vertex(sf::Vector2f(pls[0]->pos.x, pls[0]->pos.y ), sf::Color::White),
      sf::Vertex(sf::Vector2f(pls[0]->pos.x + pls[0]->futurePos.x, pls[0]->pos.y + pls[0]->futurePos.y), sf::Color::White),
    };

    sf::Vector2f lastPos;

    std::array<sf::Vertex, 8> lines;


    deadShape.setFillColor(deadColor);

    short whichPlayer = 0;

    //float gradient = 0.0;
    // This is our little game loop.
    window.setFramerateLimit(60);
    while (window.isOpen()) {
      // Check if it's time to go
      sf::Event event;
      while (window.pollEvent( event)) {
        if( event.type == sf::Event::Closed)
          window.close();

        pls[whichPlayer]->Input(event, pl1Keys);

        /* Players Are Dead Button Logic */

        for (int i = 0; i < 2; i++) {
          if (pls[i]->isDead == true) {
            sf::Vector2f mousePoint(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

            if (restart->ButtonInput(mousePoint) == true) {
              whichPlayer = 0; pls[0]->color = PLAYABLECOLOR; pls[1]->color = UNPLAYABLECOLOR;
              Restart(walls, enemys, enemyShapes, wallShapes, pls, playersClock, enemyDamageClocks, "level_1");
            }

            if (quit->ButtonInput(mousePoint) == true) return -1;

            }
          }

        }


      /* Check For Player Collision */
      //for (Wall* wall : walls){
      //  if (pls[whichPlayer]->HasCollidedWithWall(wall) == true) std::cout << "Collision With Wall" << std::endl;
      //}

      /* Play Sounds If Player's Dead */
      for (Enemy* e : enemys)
        if (pls[whichPlayer]->HasCollidedWithEnity(e) || pls[0]->IsTooClose(pls[1], windowSize, 0.125)) 
          if (pls[whichPlayer]->isDead == false)
            soundEfx[0].play();
        

      for (Enemy* e : enemys) 
        if (pls[whichPlayer]->HasCollidedWithEnity(e) && pls[whichPlayer]->isDead == false)  
          pls[whichPlayer]->isDead = true;

      /* Lose Enemy Health */
        for (sf::Clock edc : enemyDamageClocks) 
          for (Enemy* e : enemys) {
            if (e->HasCollidedWithLine(playersLine) && std::floor(edc.getElapsedTime().asSeconds()) > 2) {
              //if (std::floor(edc.getElapsedTime().asSeconds()) > 2) {
                edc.restart();
                
                e->health--;
              //}
            }
        
            //std::cout << std::floor(edc.getElapsedTime().asSeconds()) << std::endl;
            //std::cout << e->health << std::endl;
        
            edc.restart();        
        }
        
        //for (sf::Clock edc : enemyDamageClocks) edc.restart();

      /* Kill Players If Too Close To each Over */
      for (int i = 0; i < 2; i++) 
        if (pls[0]->IsTooClose(pls[1], windowSize, 0.125) && pls[whichPlayer]->isDead == false) 
          pls[i]->isDead = true;
        
        for (Wall* w : walls)
        pls[whichPlayer]->StopMovingWithWall(w);

      //else std::cout << "You Can Live" << std::endl;

      std::stringstream ss;

      /* Move Here Only If Players Are Alive */
      if (pls[0]->isDead != true && pls[1]->isDead != true) {
        if (std::floor(playersClock.getElapsedTime().asSeconds()) > 3) {

          /* Move Only For Selected Player */
          pls[whichPlayer]->NoMovement();

          /* Play Switched Sound Efx */
          soundEfx[1].play();
          
          /* Select A Certain Player */
          if (whichPlayer > 0) { whichPlayer = 0; pls[1]->color = UNPLAYABLECOLOR; pls[0]->color = PLAYABLECOLOR; }
          else if (whichPlayer < 1) { whichPlayer = 1; pls[0]->color = UNPLAYABLECOLOR; pls[1]->color = PLAYABLECOLOR; }
  
          playersClock.restart();
        }
  
      for (Wall* w : walls) {
        lines = {
          sf::Vertex(sf::Vector2f(w->pos.x, w->pos.y),                         sf::Color::Blue),
          sf::Vertex(sf::Vector2f(w->pos.x, w->pos.y + w->size.y),             sf::Color::Blue),
          sf::Vertex(sf::Vector2f(w->pos.x + w->size.x, w->pos.y),             sf::Color::Blue),
          sf::Vertex(sf::Vector2f(w->pos.x + w->size.x, w->pos.y + w->size.y), sf::Color::Blue),
          sf::Vertex(sf::Vector2f(w->pos.x, w->pos.y),                         sf::Color::Blue),
          sf::Vertex(sf::Vector2f(w->pos.x + w->size.x, w->pos.y),             sf::Color::Blue),
          sf::Vertex(sf::Vector2f(w->pos.x, w->pos.y + w->size.y),             sf::Color::Blue),
          sf::Vertex(sf::Vector2f(w->pos.x + w->size.x, w->pos.y + w->size.y), sf::Color::Blue)
        };
      }
      
        for (int i = 0; i < 2; i++) {
          lastPos = sf::Vector2f(pls[whichPlayer]->pos.x + (pls[whichPlayer]->size.x / 2), pls[whichPlayer]->pos.y + (pls[whichPlayer]->size.y / 2));
          for (Wall* w : walls) pls[i]->UpdatedMovement(lines, DeltaTime(deltaClock), w, playerShapes[i]);
          //playerShapes[i] = pls[i]->Movement(playerShapes[i], DeltaTime(deltaClock));
          playerShapes[i].setFillColor(pls[i]->color);
        }
      //}
        /* Update Players Timer */
        ss << std::floor(playersClock.getElapsedTime().asSeconds());
      }
      

      /* Move Enemy If Players Are Alive */
      for (int i = 0; i < enemys.size(); i++) {
        if (pls[0]->isDead != true && pls[1]->isDead != true) {
          enemys.at(i)->pos.x += enemys.at(i)->Movement(pls[whichPlayer], windowSize).x + enemys.at(i)->MoveAway(enemys, windowSize, sf::Vector2f(0, 0)).x * 23;
          enemys.at(i)->pos.y += enemys.at(i)->Movement(pls[whichPlayer], windowSize).y + enemys.at(i)->MoveAway(enemys, windowSize, sf::Vector2f(0, 0)).y * 23;
          //enemys.at(i)->pos;// += enemys.at(i)->MoveAway(enemys);
        }

        enemyShapes.at(i).setPosition(enemys.at(i)->pos);
        //std::cout << e->pos.x << std::endl;
      }

      for (int i = 0; i < 2; i++) {
        playersLine[i].position = sf::Vector2f(pls[i]->pos.x + pls[i]->size.x / 2, pls[i]->pos.y + pls[i]->size.y / 2);
        playersLine[i].color = sf::Color(218, 224, 58, Normalize(std::sqrt((pls[1]->pos.x * pls[0]->pos.x) + (pls[1]->pos.y * pls[0]->pos.y)), 700, 0) * 255);
        //test[1].position = pls[1]->pos;
      }

      topPlayerCollisionLine[0].position = lastPos;
      topPlayerCollisionLine[1].position = sf::Vector2f(pls[whichPlayer]->pos.x + (pls[whichPlayer]->size.x / 2), 
        pls[whichPlayer]->pos.y + (pls[whichPlayer]->size.y / 2));

      
      // std::cout << enemys.at(enemys.size() - 1)->pos.x << std::endl;

      timer.setString("Timer: " + ss.str());

      deltaClock.restart();

      window.clear();

      window.draw(playersLine, 2, sf::Lines);

      /* Draw Lines */

      for (sf::RectangleShape es : enemyShapes) window.draw(es);
      for (sf::RectangleShape plshape : playerShapes) window.draw(plshape);
      for (sf::RectangleShape wall : wallShapes) window.draw(wall);

      window.draw(topPlayerCollisionLine, 2, sf::Lines);

      window.draw(timer);

      //window.draw(test);

      //window.draw(deadShape);

      if (pls[0]->isDead == true || pls[1]->isDead == true) {
        window.draw(deadShape);

        window.draw(restartShape);
        window.draw(restartText);
        window.draw(quitShape);
        window.draw(quitText);
      }

      window.display();
    }

    return 0;
} 

/* void CreateUiText(sf::Text text, sf::Font font, std::string stringText, sf::Color color, sf::Vector2f pos, int size) {
  text.setFont(font);
  text.setString(stringText); 
  text.setColor(color); 
  text.setPosition(pos); 
  text.setCharacterSize(size);

  std::cout << text.getPosition().x << std::endl;

  //return text;
} */

void Restart(std::vector<Wall*> &walls, std::vector<Enemy*> &e, std::vector<sf::RectangleShape> &enemyShape, std::vector<sf::RectangleShape> &wallShape, Player* pls[2], sf::Clock &playerClock, std::vector<sf::Clock> &edc, std::string levelName) {
  wallShape.clear(); enemyShape.clear(); edc.clear();
  level::DestroyLevels(walls, e);
  level::CreateLevels(pls, walls, e, levelName);

  for (int i = 0; i < walls.size(); i++) {
      wallShape.push_back(sf::RectangleShape(walls.at(i)->size));
      wallShape.at(i) = walls.at(i)->InitShape(wallShape.at(i));
  }

    for (int i = 0; i < e.size(); i++) {
      enemyShape.push_back(sf::RectangleShape(e.at(i)->size));
      enemyShape.at(i) = e.at(i)->InitShape(enemyShape.at(i));
      
      edc.push_back(sf::Clock());
  }

  for (int i = 0; i < 2; i++)
    pls[i]->isDead = false;

  playerClock.restart();
}

float Normalize(float value, float max, float min) {
    return (value - std::min(value, min)) / (std::max(value, max) - std::min(value, min));
}

float DeltaTime(sf::Clock c) {
    return c.restart().asSeconds();
}