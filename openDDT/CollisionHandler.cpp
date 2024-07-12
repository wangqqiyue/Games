#include "CollisionHandler.h"

CollisionHandler* CollisionHandler::s_pInstance = 0;
void CollisionHandler::attachObserver(GameObject* go)
{
	m_gameObjects.push_back(go);
}
void CollisionHandler::detachObserver(GameObject* go)
{
    m_gameObjects.erase(
        std::remove(m_gameObjects.begin(), m_gameObjects.end(), go),
        m_gameObjects.end()
    );
}

void CollisionHandler::draw(int angle)
{

}

void CollisionHandler::update(State state)
{

}