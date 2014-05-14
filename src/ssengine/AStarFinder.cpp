#include "AStarFinder.h"

using namespace sse;

#define SQRT2 = 1.4
AStarFinder::AStarFinder(float tilesize, int inwidth, int inheight, std::vector<b2Fixture*> *blocFixtures)
{
    width = inwidth;
    height = inheight;

    blockgrid = new int*[height];
    for (int i = 0; i <height; ++i)
        blockgrid[i] = new int[width];

    for(int i = 0 ; i < height; i++)
    {
        for(int j = 0 ; j <  width; j++)
        {
            bool isunblock = true;
            for(auto k = blocFixtures->cbegin() ; k != blocFixtures->cend() ; k++ )
            {
                b2Vec2  point = b2Vec2((j*tilesize +tilesize/2)/PPM,(i*tilesize +tilesize/2)/PPM);
                b2Fixture* testfixt = *k;
                isunblock &= !testfixt->TestPoint(point);
                if(!isunblock)
                    break;
            }
            if(isunblock)
            {
                blockgrid[i][j] = 0;
            }
            else
            {
                blockgrid[i][j] = 1;
            }
        }
    }

    Grid = new Nodo**[height];
    for (int i = 0; i < height; ++i)
    {
        Grid[i] = new Nodo*[height];
        for(int j = 0 ; j < height; j++)
            Grid[i][j] = new Nodo;
    }

    for (int j = 0; j < height; j++ )
        for (int k = 0; k < height; k++)
        {
            Grid[j][k]->x = k;
            Grid[j][k]->y = j;
            if (blockgrid[j][k])
            {
                Grid[j][k]->walkable = false;
            }
        }
}

bool AStarFinder::isWalkableAt(int x, int y) {
    return isInside(x, y) && Grid[y][x]->walkable;
};

bool AStarFinder::isInside(int x, int y) {
    return (x >= 0 && x < width) && (y >= 0 && y < height);
};

void AStarFinder::setWalkableAt(int x, int y, bool walkable) {
    Grid[y][x]->walkable = walkable;
};

std::vector<Nodo*> AStarFinder::getNeighbors(Nodo* node)
{
    int x, y;
    bool s0 = false, d0 = false,
        s1 = false, d1 = false,
        s2 = false, d2 = false,
        s3 = false, d3 = false;
    x = node->x;
    y = node->y;
    std::vector<Nodo*> neighbors;

    if (isWalkableAt(x,y-1))
    {
        neighbors.push_back(Grid[y-1][x]);
        s0 = true;
    }

    if (isWalkableAt(x+1,y))
    {
        neighbors.push_back(Grid[y][x+1]);
        s1 = true;
    }

    if (isWalkableAt(x,y+1))
    {
        neighbors.push_back(Grid[y+1][x]);
        s2 = true;
    }

    if (isWalkableAt(x-1,y))
    {
        neighbors.push_back(Grid[y][x-1]);
        s3 = true;
    }

    d0 = s3 && s0;
    d1 = s0 && s1;
    d2 = s1 && s2;
    d3 = s2 && s3;

    if (d0 && isWalkableAt(x-1,y-1))
        neighbors.push_back(Grid[y - 1][x - 1]);

    if (d1 && isWalkableAt(x+1,y-1))
        neighbors.push_back(Grid[y - 1][x + 1]);

    if (d2 && isWalkableAt(x+1,y+1))
        neighbors.push_back(Grid[y + 1][x + 1]);

    if (d3 && isWalkableAt(x-1,y+1))
        neighbors.push_back(Grid[y + 1][x - 1]);

    return neighbors;
}

bool AStarFinder::compareNode(Nodo* A, Nodo* B)
{
    return A->x == B->x && A->y == B->y;
}

std::vector<sf::Vector2i*> AStarFinder::backtrace(Nodo* node)
{
    sf::Vector2i *currentnode = new sf::Vector2i(node->parent->x,node->parent->y);
    std::vector<sf::Vector2i*> btrace;
    btrace.push_back(new sf::Vector2i(node->x,node->y));
    while(currentnode->x != -1 && currentnode->y != -1)
    {
        btrace.push_back(currentnode);
        currentnode = new sf::Vector2i(Grid[currentnode->y][currentnode->x]->parent->x,Grid[currentnode->y][currentnode->x]->parent->y);
    }
    return btrace;
}

std::vector<sf::Vector2i*> AStarFinder::findPath(sf::Vector2i Pstart, sf::Vector2i Pend)
{
    return findPath(Pstart.x, Pstart.y, Pend.x, Pend.y);
}

bool AStarFinder::compareByLF(const Nodo* a, const Nodo* b)
{
    return a->f > b->f;
}

std::vector<sf::Vector2i*> AStarFinder::findPath(int startX, int startY, int endX, int endY)
{
    resetGrid();
    std::vector<Nodo*> OpenList;
    Nodo* StartNode = Grid[startY][startX];
    Nodo* EndNode = Grid[endY][endX];

    StartNode->g = 0;
    StartNode->f = 0;

    OpenList.push_back(StartNode);
    StartNode->opened = true;

    while(!OpenList.empty())
    {
        std::sort(OpenList.begin(),OpenList.end(),compareByLF);
        Nodo *node = OpenList.back();
        OpenList.pop_back();
        node->closed = true;

        if(compareNode(node,EndNode))
        {
            return smoothenPath(backtrace(node));
        }

        std::vector<Nodo*> Vecinos = getNeighbors(node);
        for ( int i=0; i < Vecinos.size() ; i++)
        {
            Nodo* neighbor = Vecinos[i];
            if (neighbor->closed)
                continue;
            int x = neighbor->x, y = neighbor->y, dif = 0;
            if((x - node->x == 0 || y - node->y == 0))
                dif = 10;
            else
                dif = 14;
            float ng = node->g + dif;

            if (!neighbor->opened || ng < neighbor->g)
            {
                neighbor->g = ng;
                neighbor->parent->x = node->x;
                neighbor->parent->y = node->y;
                if (!neighbor->opened)
                {
                    neighbor->h = 10 * (abs(x - endX)+abs(y - endY));
                    OpenList.push_back(neighbor);
                    neighbor->opened = true;
                }
                neighbor->f = neighbor->g + neighbor->h;
            }
        }
    }
}

std::vector<sf::Vector2i*> AStarFinder::getLine(int x0, int y0, int x1, int y1)
{
    int sx, sy, dx, dy, err, e2;
    std::vector<sf::Vector2i*> line;

    dx = abs(x1 - x0);
    dy = abs(y1 - y0);

    if(x0 < x1)
        sx = 1;
    else
        sx =-1;

    if(y0 < y1)
        sy = 1;
    else
        sy =-1;

    err = dx - dy;

    while (true) {
        line.push_back(new sf::Vector2i(x0, y0));
        if (x0 == x1 && y0 == y1) {
            break;
        }
        e2 = 2 * err;
        if (e2 > -dy) {
            err = err - dy;
            x0 = x0 + sx;
        }
        if (e2 < dx) {
            err = err + dx;
            y0 = y0 + sy;
        }
    }
    return line;
}
std::vector<sf::Vector2i*> AStarFinder::smoothenPath(std::vector<sf::Vector2i*> path)
{
    std::vector<sf::Vector2i*> newPath;
    std::vector<sf::Vector2i*> line;
    int len = path.size();
    if(len>1)
    {
        int x0 = path[0]->x,        // path start x
        y0 = path[0]->y,        // path start y
        x1 = path[len - 1]->x,  // path end x
        y1 = path[len - 1]->y,  // path end y
        sx, sy,                 // current start coordinate
        ex, ey,                 // current end coordinate
        lx, ly;
        bool blocked;

        sx = x0;
        sy = y0;
        lx = path[1]->x;
        ly = path[1]->y;
        newPath.push_back(new sf::Vector2i(sx, sy));

        for (int i = 2; i < len; ++i) {
            sf::Vector2i* coord = new sf::Vector2i(path[i]->x,path[i]->y);
            ex = coord->x;
            ey = coord->y;
            line = getLine(sx, sy, ex, ey);
            blocked = false;
            for (int j = 1; j < line.size(); ++j) {
                if (!isWalkableAt(line[j]->x, line[j]->y)) {
                    blocked = true;
                    newPath.push_back(new sf::Vector2i(lx, ly));
                    sx = lx;
                    sy = ly;
                    break;
                }
            }
            if (!blocked) {
                lx = ex;
                ly = ey;
            }
        }
        newPath.push_back(new sf::Vector2i(x1, y1));
    }
    return newPath;
}

void AStarFinder::resetGrid()
{
    for (int j = 0; j < height; j++ )
    {
        for (int k = 0; k < width; k++)
        {
            Grid[j][k]->reset();
        }
    }
}


