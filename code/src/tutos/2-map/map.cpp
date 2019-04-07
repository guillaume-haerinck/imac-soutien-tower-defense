#include "map.hpp"

#include <glad/glad.h>
#include <Box2D/Box2D.h>
#include "core/constants.hpp"

Map::Map() : m_tileSize(50)
{
    m_gridWidth = WINDOW_WIDTH / m_tileSize;
    m_gridHeight = WINDOW_HEIGHT / m_tileSize;
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
    float posX = x / m_gridWidth * WINDOW_WIDTH + m_tileSize / 2;
    float posY = y / m_gridHeight * WINDOW_HEIGHT + m_tileSize / 2;
    return glm::vec2(posX, posY);
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
    /*
    glColor4f(255, 0, 0, 1);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
        glVertex2f(m_x, m_y);
    glEnd();
    glPointSize(1.0f);
    */
}
