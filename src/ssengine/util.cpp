#include "util.h"

bool sse::insideGUI(std::vector<sf::FloatRect> allocations, sf::Vector2i mousepos)
{
    for(auto allocaton : allocations)
    {
        if (mousepos.x > allocaton.left &&
            mousepos.x < allocaton.left+allocaton.width &&
            mousepos.y > allocaton.top &&
            mousepos.y < allocaton.top+allocaton.height)
            return false;
    }
    return true;
}
