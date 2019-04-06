#include "entity.hpp"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

Entity::Entity(float x, float y) : m_x(x), m_y(y) {
    spdlog::info("Entity created !");
}

Entity::~Entity() {
    spdlog::info("Entity destructed !");
}

void Entity::update() {
    glColor4f(255, 0, 0, 1);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
        glVertex2f(m_x, m_y);
    glEnd();
    glPointSize(1.0f);
}
