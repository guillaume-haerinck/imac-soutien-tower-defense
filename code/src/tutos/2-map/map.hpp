#pragma once

#include <vector>
#include <glm/glm.hpp>

enum mapTile { constructible = 0, path = 1, locked = 2 };

class Map {
public:
    Map();
    ~Map();

    glm::vec2 windowToGrid(float x, float y);
    glm::vec2 gridToWindow(unsigned int x, unsigned int y);

private:
    std::vector<mapTile> m_grid;
    unsigned int m_tileSize;
    unsigned int m_gridWidth;
    unsigned int m_gridHeight;
};
