#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <inttypes.h>

#include "Player.h"
#include "Enemy.h"
#include "Wall.h"

namespace level {
    void CreateLevels(Player* pls[2], std::vector<Wall*> &walls, std::vector<Enemy*> &e, std::string name);
    void DestroyLevels(std::vector<Wall*> &walls, std::vector<Enemy*> &e);
};

/* class Level {
public:
    void CreateLevels();
}; */

#endif // LEVEL_H