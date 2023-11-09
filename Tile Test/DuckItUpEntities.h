#pragma once
#include "Entity.h"
class bulletObj : public Entity {
public:
	float timeLeft;
	bulletObj(float posX, float posY, int sX, int sY, textureAsset& textureA);
	void computeBulletDir(Entity& gunObj, sf::RenderWindow& window);
};