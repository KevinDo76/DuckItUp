#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "string"
#include "textureAsset.h"
class mapManager; //thx chatgpt, didn't know about forward declaration until u <3

class tileData {
public:
	int posX;
	int posY;
	float offx;
	float offy;
	const int ID;
	int textureID;
	bool collidable;
	sf::Sprite sprite;
	sf::Texture internal;
	mapManager* parentMap;
	tileData(mapManager* parentMap, int posX, int posY, int ID, int textureID);
	~tileData();

	void updateTextureID(int ID);
	void drawBoundBox(sf::RenderWindow& window, sf::Color color);
private:
};

class mapManager {
public:
	//variables
	const int sizeX;
	const int sizeY;
	const int scaleX;
	const int scaleY;
	float posX;
	float posY;
	textureAsset* textureSet;
	sf::Texture nullTexture;
	std::vector<tileData> tiles;

public:
	//methods
	mapManager(int x, int y, int sx, int sy);
	~mapManager();

	static void getMouseWorldCoord(sf::RenderWindow& window, sf::Vector2f& worldCoord);
	int getCurrentTileIndexMouse(sf::RenderWindow& window);
	int getCurrentTileIndexPosition(sf::RenderWindow& window,const sf::Vector2f pos);
	void getCurrentTile(const sf::Vector2f worldCoord, sf::Vector2i& tileCoord);
	bool loadTextureAsset(textureAsset& texture);
	bool loadDataIntoTile(const std::string path);
	bool saveTileIntoFile(const std::string path);
	bool isTextureAssetLoaded();
	void setPosition(const float px,const float py);

	void draw(sf::RenderWindow& window);
private:
	bool textureAssetLoaded;
};