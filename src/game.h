//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// game.h
/// Central game class - manages the game flow based on the settings provided by the parser.
//==============================================================================

#pragma once

#include "jsParser.h"
#include "sound.h"
#include "helper/maths.h"
#include "objects/object.h"
#include "thirdParty/olcPixelGameEngine.h"

#include <filesystem>
#include <chrono>


class CObject;
class CBHole;
enum class mouse {LEFT, RIGHT, MIDDLE};

class CGame : public olc::PixelGameEngine
{
public:
    CGame(std::filesystem::path& currentPath);

    //set & get
    v2d velocity() const;
    v2d center() const;
    v2d acceleration() const;
    int radiusView() const;
    int radiusMap() const;
    int fog() const;
    olc::Pixel playerColor() const;
    std::filesystem::path currentPath() const;

    //handle to parser obj with static lifetime
    const CParser& parser() const;

    //handle to sound obj with static lifetime
    const CSound& sound();

    //provides the mass of an object for possible outputs in scientific notation with the amount of shown decimals as parameter
    std::string massInfo(const std::shared_ptr<CObject>& obj, int shownDecimals) const;

    //provides a handle to all objects
    const std::array<std::list<std::shared_ptr<CObject>>, static_cast<int>(objectTypes::count)>& gameObjects() const;

    //runtime of game
    std::chrono::seconds playtime() const;

    //get position of cursor
    v2d getCursor() const;

    //add object to object container
    void addObject(const std::shared_ptr<CObject>&& object);

    //play effect
    void effectEaten();

    //shortcut to player
    std::shared_ptr<CBHole> player() const;

private:
    //to measure game runtime
    std::chrono::time_point<std::chrono::steady_clock> m_startTime;

    //the path where the prog is executed
    std::filesystem::path m_currentPath;

    //parser object which handles the settings
    CParser m_parser;

    //sound object which handles all sounds for the game
    CSound m_sound;

    //all active objects separated into types
    std::array<std::list<std::shared_ptr<CObject>>, static_cast<int>(objectTypes::count)> m_objects;

    //world attributes applying to all (moveable) objects
    v2d m_velocity;
    v2d m_acceleration; //acceleration in x and y component
    float m_maxSpeed;
    float m_maxSpeed2;

    //view attributes
    int m_radiusView;
    int m_radiusMap;
    int m_fogOfWar;
    v2d m_center;

    //player attributes
    olc::Pixel m_playerColor;
    int m_startMass;

    //init objects, player need to be first
    void initPlayer();
    std::weak_ptr<CBHole> m_pPlayer; //shortcut to the player object
    void initStars();
    int m_starCardinality;

    ////spawn objects
	//spawnchance per interval - should be in [0, 1]
    float m_debrisChance;
    //spawn interval in seconds
    float m_debrisTimer;
    float m_debrisTimerReload;
    void spawnDebris(float deltaTime);

    //effects
    float m_effectEaten;
    float m_effectEatenTime;
    void globalEffects(float deltaTime);

    ////needs to be overridden for olc game engine:
    //initialization
    bool OnUserCreate() override;
    //game loop
    bool OnUserUpdate(float deltaTime) override;

    ////UI
    //cursor circling effect
    float m_circling;
    void drawUI(float deltaTime);
    void drawCursor(float deltaTime);
};