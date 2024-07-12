#include "CollisionHandler.h"

CollisionHandler* CollisionHandler::s_pInstance = 0;
void CollisionHandler::attachObserver(SDLGameObject* go)
{
	m_gameObjects.push_back(go);
}
void CollisionHandler::detachObserver(SDLGameObject* go)
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

bool CollisionHandler::checkCollisionAll(SDLGameObject* g1)
{
    for (SDLGameObject* g2 : m_gameObjects)
    {
        if (checkCollision(g1, g2))
        {
            return true;
        }
    }
    return false;
}

bool CollisionHandler::checkCollision(SDLGameObject* g1, SDLGameObject* g2)
{
    bool result = false;
    //AABB·¨¼ì²âÅö×²
    int ax, ay, aw, ah;
    int bx, by, bw, bh;
    ax = g1->getPosition().x;
    ay = g1->getPosition().y;
    aw = g1->getSize().x;
    ah = g1->getSize().y;
    bx = g2->getPosition().x;
    by = g2->getPosition().y;
    bw = g2->getSize().x;
    bh = g2->getSize().y;
    if (ax<bx + bw && ax + aw>bx && ay<by + bh && ay + ah>by)
    {
        result = true;
        g1->onCollision();
        g2->onCollision();
    }
    return result;
}