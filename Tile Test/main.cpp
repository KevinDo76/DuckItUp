#include <SFML/Graphics.hpp>
#include "mapManager.h"
#include "textureAsset.h"
#include "sfmlPanZoomHandler.h"
#include "Entity.h"
#include <sstream>
#include <iostream>
#include <cmath>
#include <chrono>
#define WORLD_RES_X 1000
#define WORLD_RES_Y 1000
int main()
{
    auto start = std::chrono::system_clock::now();
    float lastElapse = 0;
    sfmlPanZoomHandler winObj(sf::VideoMode(WORLD_RES_X, WORLD_RES_Y), "Tile Engine");

    //frame time counter
    sf::View UIView;
    UIView.setViewport(sf::FloatRect(0, 0, 1, 1));
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text frameTime;
    frameTime.setFont(font);
    frameTime.setCharacterSize(20);
    frameTime.setStyle(sf::Text::Regular);

    //textureAsset init
    mapManager map = mapManager(25,25,32, 32);
    textureAsset mainMap = textureAsset(32, 32, 5);
    textureAsset gunTexture = textureAsset(32, 32, 5);
    textureAsset characterTexture = textureAsset(32, 32, 5);
    textureAsset enemyTexture = textureAsset(32, 32, 5);

    //loading textures
    characterTexture.loadTextureMap("ducktiles.png");
    mainMap.loadTextureMap("maptiles.png");
    gunTexture.loadTextureMap("gun.png");
    enemyTexture.loadTextureMap("enemytile.png");

    //applying textures
    Entity Duck{ 200, 40, 32, 32, characterTexture };
    Entity gun{ 200, 40, 32, 32, gunTexture };
    std::vector<Entity> enemies;
    map.loadTextureAsset(mainMap);
    sf::Clock Clock;

    //settings
    winObj.window.setFramerateLimit(200);
    map.loadDataIntoTile("save.txt");

    //initialize enemies
    for (int i = 0; i < 5; i++) {
        std::cout << rand() << "\n";
        enemies.push_back({ (rand()/(float)RAND_MAX * 736.f + 50.f), (rand() / (float)RAND_MAX * 736.f + 50.f), 32,32, enemyTexture });
    }

    while (winObj.window.isOpen())
    {
        float timeNow = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch() - start.time_since_epoch()).count() / 1000.f;
        while (winObj.window.pollEvent(winObj.event)) {
            if (winObj.window.hasFocus() && map.isTextureAssetLoaded()) {
                if (winObj.event.type == sf::Event::MouseButtonPressed && winObj.event.mouseButton.button == sf::Mouse::Button::Left) {
                    int index = map.getCurrentTileIndexMouse(winObj.window);
                    if (index >= 0) {
                        //map.tiles[index].updateTextureID((map.tiles[index].textureID + 1) % mainMap.getSize());
                    }
                }
                else if (winObj.event.type == sf::Event::MouseButtonPressed && winObj.event.mouseButton.button == sf::Mouse::Button::Right) {
                    int index = map.getCurrentTileIndexMouse(winObj.window);
                    if (index >= 0) {
                        //map.tiles[index].updateTextureID(std::abs((map.tiles[index].textureID - 1)) % mainMap.getSize());
                    }
                }
                else if (winObj.event.type == sf::Event::KeyPressed && winObj.event.key.code == sf::Keyboard::S) {
                    //map.saveTileIntoFile("save.txt");
                    //std::cout << "saved\n";
                }
                else if (winObj.event.type == sf::Event::KeyPressed && winObj.event.key.code == sf::Keyboard::C) {
                    int index = map.getCurrentTileIndexMouse(winObj.window);
                    if (index >= 0) {
                        map.tiles[index].collidable = !map.tiles[index].collidable;
                    }
                }
            }
            if (winObj.event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0.f, 0.f, (int)winObj.event.size.width, (int)winObj.event.size.height);
                UIView.reset(visibleArea);
            }
            winObj.handleEventPanZoom();
        }

        Duck.velX = 0;
        Duck.velY = 0;
        float tempVX = 0;
        float tempVY = 0;
        if (sf::Keyboard().isKeyPressed(sf::Keyboard::Up)) {
            tempVY -= 100;
            Duck.textureID = (int)(timeNow * 5) % 2 + 3;
            gun.textureID = 2;

        } else if (sf::Keyboard().isKeyPressed(sf::Keyboard::Down)) {
            tempVY += 100;
            Duck.textureID = (int)(timeNow * 5) % 2 + 1;
            gun.textureID = 3;
        }
        else if (sf::Keyboard().isKeyPressed(sf::Keyboard::Left)) {
            tempVX -= 100;
            Duck.textureID = (int)(timeNow * 5) % 2 + 5;
            gun.textureID = 0;
        } else if (sf::Keyboard().isKeyPressed(sf::Keyboard::Right)) {
            tempVX += 100;
            Duck.textureID = (int)(timeNow * 5) % 2 + 7;
            gun.textureID = 1;
        }
        else {
            Duck.textureID = 0;
            gun.textureID = 1;
        }

        Duck.velX = tempVX;
        Duck.velY = tempVY;

        winObj.window.clear();
        map.draw(winObj.window);

        Duck.computePhysic(lastElapse, map, winObj.window);
        gun.posX = Duck.posX;
        gun.posY = Duck.posY-14 - (std::sinf(timeNow * 13));

        winObj.view.setCenter(sf::Vector2f(Duck.posX + Duck.sizeX / 2, Duck.posY + Duck.sizeY / 2));

        for (int i = 0; i < 5; i++) {
            enemies[i].renderSelf(winObj.window);
        }

        Duck.renderSelf(winObj.window);
        gun.renderSelf(winObj.window);

        winObj.window.setView(UIView);
        winObj.window.draw(frameTime);
        winObj.window.setView(winObj.view);
        winObj.window.display();

        //fps calc
        sf::Time Time = Clock.getElapsedTime();
        lastElapse = Time.asMicroseconds() / 1000000.f;

        Clock.restart();
        //std::cout << Time.asMilliseconds() << "ms/16.6ms ideal\n";
        std::stringstream frameTimeText;
        frameTimeText << Time.asMicroseconds() / 1000.f << "ms/16.6ms ideal\n";
        frameTime.setString(frameTimeText.str());
    }

    return 0;
}