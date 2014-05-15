#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>

namespace sse
{
bool insideGUI(std::vector<sf::FloatRect> allocations, sf::Vector2i mousepos);
}
#endif
