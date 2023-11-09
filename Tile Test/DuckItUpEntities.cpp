#include "DuckItUpEntities.h"
#include "Entity.h"
#include "SFML/Graphics.hpp"
bulletObj::bulletObj(float posX, float posY, int sX, int sY, textureAsset& textureA) : Entity(posX,posY,sX,sX,textureA), timeLeft(2.f) {}
void bulletObj::computeBulletDir(Entity& gunObj, sf::RenderWindow& window) {
    sf::Vector2f mouseWorld;
    mapManager::getMouseWorldCoord(window, mouseWorld);
    sf::Vector2f direction = sf::Vector2f((gunObj.posX + gunObj.sizeX / 2) - mouseWorld.x + sizeX/2, (gunObj.posY + gunObj.sizeY / 2) - mouseWorld.y + sizeY/2);
    float length = std::sqrtf(direction.x * direction.x + direction.y * direction.y);
    direction /= -length;
    velX = direction.x * 300;
    velY = direction.y * 300;
    posX = (gunObj.posX + gunObj.sizeX / 2);
    posY = (gunObj.posY + gunObj.sizeY / 2);
    collidableWithMap = false;
}
