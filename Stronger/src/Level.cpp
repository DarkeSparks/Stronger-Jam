#include "Level.h"

void level::CreateLevels(Player* pls[2], std::vector<Wall*> &walls, std::vector<Enemy*> &e, std::string name) {
    std::fstream file;
    std::string fileName = "bin/Levels/" + name + ".txt";

    std::string level;
    std::vector<std::string> fullLevel;

    int lines = 0;

    file.open(fileName, std::ios::in);
    if (file.is_open()) {
        file >> level;

        while (getline(file,level)) {
            // std::cout << level << std::endl;
            for (int x = 0; x <= 40; x++) if (level[x] == ' ') lines++;
            
            fullLevel.push_back(level);
        }

    file.close();
    

    for (int y = 0; y <= lines; y++)
        for (int x = 0; x <= 41; x++) {
            if (y + 1 <= lines)  level = fullLevel[static_cast<__int64_t>(y) + 1];

            //std::cout << level << std::endl;

            switch(level[x]) {
                case '#':
                    walls.push_back(new Wall(sf::Vector2f(20 * x, 20 * y), sf::Vector2f(20, 20), sf::Color::White));
                    //std::cout << "Platform at " << x << ": " << y << std::endl;
                    break;
                case 'E':
                    e.push_back(new Enemy(sf::Vector2f(20 * x, 20 * y), sf::Vector2f(20, 20), sf::Vector2f((rand() % 49 + 104)/4, 105/4), sf::Color::Red));
                    break;
                case '1':
                    pls[0]->pos = sf::Vector2f(pls[0]->size.x * x, pls[0]->size.y * y);
                    //std::cout << "Origin 2" << std::endl;
                    break;
                case '2':
                    pls[1]->pos = sf::Vector2f(pls[1]->size.x * x, pls[1]->size.y * y);
                    //std::cout << "Origin 2" << std::endl;
                    break;
            }

            //if (level[x] == '#') std::cout << "Poop" << std::endl;//printf("Poop");
        }
    } else {
        std::cout << "Cannot Find File" << std::endl;
        exit(1);
    }
}

void level::DestroyLevels(std::vector<Wall*> &walls, std::vector<Enemy*> &e) {
    walls.clear();
    e.clear();
}