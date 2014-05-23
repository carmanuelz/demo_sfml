#ifndef JSONLEVEL_H
#define JSONLEVEL_H

#include <string>
#include <istream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <jsoncpp/json.h>

struct dataCharacter
{
    dataCharacter(std::string incode,int inx, int iny, bool inreset):code(incode),x(inx),y(iny),reset(inreset){}
    std::string code;
    int x;
    int y;
    bool reset;
};

class JSonLevel
{
    public:
        JSonLevel(std::string incode);
        virtual ~JSonLevel(){};
        std::vector<dataCharacter*> CharacterList;
        const char* getRubeSource();
        sf::Vector2f getOriginCharacter();
        std::string getCodePlayer();
        float getTileSize();
        int getDimWidth();
        int getDimHeight();
    protected:
    private:
        Json::Value LevelValue;
        std::string code;
        std::string rubesource;
        sf::Vector2f OriginPlayer;
        std::string codeplayer;
        float tilezise;
        int tileswidth;
        int tilesheight;
};

#endif // JSONLEVEL_H
