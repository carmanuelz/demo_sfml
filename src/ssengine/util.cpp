#include "util.h"

bool sse::insideGUI(std::vector<sf::FloatRect> allocations, sf::Vector2i mousepos)
{
    for(auto allocation : allocations)
    {
        if (mousepos.x > allocation.left &&
                mousepos.x < allocation.left+allocation.width &&
                mousepos.y > allocation.top &&
                mousepos.y < allocation.top+allocation.height)
            return false;
    }
    return true;
}
