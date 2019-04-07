#include "map.hpp"

#include <glad/glad.h>
#include <Box2D/Box2D.h>
#include "core/constants.hpp"

Map::Map() : m_tileSize(50)
{
    m_gridWidth = WINDOW_WIDTH / m_tileSize;
    m_gridHeight = WINDOW_HEIGHT / m_tileSize;
    m_grid.resize(m_gridWidth * m_gridHeight);

    for (int i = 0; i < m_grid.size(); i++) {
        if (i % 2 == 0) {
            m_grid[i] = MapTile::constructible;
        } else {
            m_grid[i] = MapTile::locked;
        }
    }
}

Map::~Map()
{
}

glm::vec2 Map::windowToGrid(float x, float y) {
    unsigned int tileX = x / WINDOW_WIDTH * m_gridWidth;
    unsigned int tileY = y / WINDOW_HEIGHT * m_gridHeight;
    return glm::vec2(tileX, tileY);
}

glm::vec2 Map::gridToWindow(unsigned int x, unsigned int y) {
    float posX = WINDOW_WIDTH / m_gridWidth * x + m_tileSize / 2;
    float posY = WINDOW_HEIGHT / m_gridHeight * y + m_tileSize / 2;
    return glm::vec2(posX, posY);
}

MapTile Map::getTile(unsigned int x, unsigned int y) {
    return m_grid.at(y * m_gridHeight + x);
}

void Map::draw() {
    for (int x = m_tileSize; x < WINDOW_WIDTH; x++) {
        m_dd.DrawSegment(b2Vec2(x, 0), b2Vec2(x, WINDOW_HEIGHT), b2Color(255, 0, 0, 1));
        x += m_tileSize;
    }

    for (int y = m_tileSize; y < WINDOW_HEIGHT; y++) {
        m_dd.DrawSegment(b2Vec2(0, y), b2Vec2(WINDOW_WIDTH, y), b2Color(255, 0, 0, 1));
        y += m_tileSize;
    }
}
