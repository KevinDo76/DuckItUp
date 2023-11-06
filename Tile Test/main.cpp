#include <SFML/Graphics.hpp>
#include "mapManager.h"
#include "textureAsset.h"
#include "sfmlPanZoomHandler.h"
#include "Entity.h"
#include <sstream>
#include <iostream>
#include <cmath>
#define WORLD_RES_X 1000
#define WORLD_RES_Y 1000
int main()
{
    float lastElapse = 0;
    sfmlPanZoomHandler winObj(sf::VideoMode(WORLD_RES_X, WORLD_RES_Y), "Tile Engine");

    sf::View UIView;
    UIView.setViewport(sf::FloatRect(0, 0, 1, 1));
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text frameTime;
    frameTime.setFont(font);
    frameTime.setCharacterSize(20);
    frameTime.setStyle(sf::Text::Regular);

    mapManager map = mapManager(25,25,32, 32);
    textureAsset mainMap = textureAsset(32, 32, 5);
    textureAsset characterTexture = textureAsset(32, 32, 1);
    characterTexture.loadTextureMap("C:\\Users\\Owner\\Pictures\\daniel.png");
    mainMap.loadTextureMap("maptiles.png");

    Entity testEnt{ 200, 20, 32, 32, characterTexture };
    map.loadTextureAsset(mainMap);
    sf::Clock Clock;

    winObj.window.setFramerateLimit(200);
    map.loadDataIntoTile("save.txt");

    while (winObj.window.isOpen())
    {
        testEnt.velX = 0;
        testEnt.velY = 0;
        while (winObj.window.pollEvent(winObj.event)) {
            if (winObj.window.hasFocus() && map.isTextureAssetLoaded()) {
                if (winObj.event.type == sf::Event::MouseButtonPressed && winObj.event.mouseButton.button == sf::Mouse::Button::Left) {
                    int index = map.getCurrentTileIndexMouse(winObj.window);
                    if (index >= 0) {
                        map.tiles[index].updateTextureID((map.tiles[index].textureID + 1) % mainMap.getSize());
                    }
                }
                else if (winObj.event.type == sf::Event::MouseButtonPressed && winObj.event.mouseButton.button == sf::Mouse::Button::Right) {
                    int index = map.getCurrentTileIndexMouse(winObj.window);
                    if (index >= 0) {
                        map.tiles[index].updateTextureID(std::abs((map.tiles[index].textureID - 1)) % mainMap.getSize());
                    }
                }
                else if (winObj.event.type == sf::Event::KeyPressed && winObj.event.key.code == sf::Keyboard::S) {
                    map.saveTileIntoFile("save.txt");
                    std::cout << "saved\n";
                }
                else if (winObj.event.type == sf::Event::KeyPressed && winObj.event.key.code == sf::Keyboard::C) {
                    int index = map.getCurrentTileIndexMouse(winObj.window);
                    if (index >= 0) {
                        map.tiles[index].collidable = !map.tiles[index].collidable;
                    }
                }
            }
            if (winObj.event.type == sf::Event::Resized) {
                //sf::Vector2u windowSize = winObj.window.getSize();
                //UIView.setSize(sf::Vector2f(int(((double)windowSize.x / (double)windowSize.y) * winObj.defaultWidth) + (winObj.defaultWidth / 2), winObj.defaultWidth));
                sf::FloatRect visibleArea(0.f, 0.f, (int)winObj.event.size.width, (int)winObj.event.size.height);
                UIView.reset(visibleArea);
            }
            winObj.handleEventPanZoom();
        }
        float tempVX = 0;
        float tempVY = 0;
        if (sf::Keyboard().isKeyPressed(sf::Keyboard::Up)) {
            tempVY -= 100;
        }
        if (sf::Keyboard().isKeyPressed(sf::Keyboard::Down)) {
            tempVY += 100;
        }

        if (sf::Keyboard().isKeyPressed(sf::Keyboard::Left)) {
            tempVX -= 100;
        }
        if (sf::Keyboard().isKeyPressed(sf::Keyboard::Right)) {
            tempVX += 100;
        }

        testEnt.velX = tempVX;
        testEnt.velY = tempVY;

        winObj.window.clear();
        map.draw(winObj.window);
        testEnt.computePhysic(lastElapse, map, winObj.window);
        winObj.view.setCenter(sf::Vector2f(testEnt.posX + testEnt.sizeX / 2, testEnt.posY + testEnt.sizeY / 2));
        testEnt.renderSelf(winObj.window);

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