#ifndef ASTARFINDER_H
#define ASTARFINDER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm>

#include <Box2D/Box2D.h>
#include <math.h>

#define PPM 64.0f               //PIXELS POR METRO
#define MPP (1.0f/PPM)          //METROS POR PIXEL

namespace sse
{
struct Nodo
{
    bool walkable = true;
    bool closed = false, opened = false;
    int x = 0, y = 0, f = 0, g = 0, h = 0;
    sf::Vector2i *parent = new sf::Vector2i(-1,-1);

    void reset()
    {
        closed = false;
        opened = false;
        f = 0;
        g = 0;
        h = 0;
        parent->x = -1;
        parent->y = -1;
    }
};

class AStarFinder
{
public:
    AStarFinder(float intilesize, int width, int height, std::vector<b2Fixture*> *blocFixtures);
    std::vector<sf::Vector2i*> findPath(sf::Vector2i Pstart, sf::Vector2i Pend);
    std::vector<sf::Vector2i*> findPath(int startX, int startY, int endX, int endY);
    bool isWalkableAt(int x,int y);
    void drawdebug(sf::RenderWindow* rw);
private:
    int width;
    int height;
    int **blockgrid;
    Nodo ***Grid;
    float tilesize;
    std::vector<Nodo*> getNeighbors(Nodo* node);
    std::vector<sf::Vector2i*> backtrace(Nodo* node);
    std::vector<sf::Vertex*> griddebug;
    bool isInside(int x, int y);
    void setWalkableAt(int x, int y,bool walkable);
    bool compareNode(Nodo* A, Nodo* B);
    static bool compareByLF(const Nodo* a,const Nodo* b);
    std::vector<sf::Vector2i*> getLine(int x0, int y0, int x1, int y1);
    std::vector<sf::Vector2i*> smoothenPath(std::vector<sf::Vector2i*> path);
    void resetGrid();
};
}

#endif
