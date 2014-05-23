#include "JSonLevel.h"

JSonLevel::JSonLevel(std::string incode):code(incode)
{
    std::string errorMsg;

    std::string codestr = "assets/maps/"+code+".json";
    const char* filename = codestr.c_str();
    std::ifstream ifs;
    ifs.open(filename, std::ios::in);
    if (!ifs)
    {
        //std::cout << "Could not open file " << filename << " for reading\n";
        errorMsg = std::string("Could not open file '") + std::string(filename) + std::string("' for reading");
        return;
    }

    Json::Reader reader;
    if ( ! reader.parse(ifs, LevelValue) )
    {
        //std::cout  << "Failed to parse " << filename << std::endl << reader.getFormattedErrorMessages();
        errorMsg = std::string("Failed to parse '") + std::string(filename) + std::string("' : ") + reader.getFormatedErrorMessages();
        ifs.close();
        return;
    }
    ifs.close();
    rubesource = LevelValue["rubesource"].asString();
    codeplayer = LevelValue["codeplayer"].asString();
    tilezise = LevelValue["tilesize"].asFloat();
    tileswidth = LevelValue["tileswidth"].asFloat();
    tilesheight = LevelValue["tilesheight"].asFloat();

    const Json::Value jsonOP = LevelValue["originplayer"];
    OriginPlayer.x = jsonOP["x"].asFloat();
    OriginPlayer.y = jsonOP["y"].asFloat();

    const Json::Value jsonChList = LevelValue["characterlist"];
    for ( int index = 0; index < jsonChList.size(); ++index )
    {
       const Json::Value character = jsonChList[index];
       CharacterList.push_back(new dataCharacter(character["code"].asString(),
                                                 character["x"].asInt(),
                                                 character["y"].asInt(),
                                                 character["reset"].asBool()));
    }

}

const char* JSonLevel::getRubeSource()
{
    return rubesource.c_str();
}

sf::Vector2f JSonLevel::getOriginCharacter()
{
    return OriginPlayer;
}

std::string JSonLevel::getCodePlayer()
{
    return codeplayer;
}

float JSonLevel::getTileSize()
{
    return tilezise;
}

int JSonLevel::getDimWidth()
{
    return tileswidth;
}

int JSonLevel::getDimHeight()
{
    return tilesheight;
}
