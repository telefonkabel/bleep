//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// game.h
/// Central game class - manages the game flow based on the settings provided by the parser.
//==============================================================================

#pragma once

#include "object.h"
#include "maths.h"
#include "asteroid.h"
#include "sounds.h"
#include "parser.h"

#include "rapidjson/document.h"

#include <filesystem>


class CGame : public olc::PixelGameEngine
{
public:
    CGame(std::filesystem::path& currentPath);
    ~CGame();

    //set & get
    v2d velocity() const;
    v2d center() const;
    v2d accelerationX() const;
    v2d accelerationY() const;
    int radiusView() const;
    int radiusMap() const;
    int fog() const;
    olc::Pixel playerColor() const;
    std::filesystem::path currentPath() const;

    //provides the mass of an object for possible outputs in scientific notation with the amount of shown decimals as parameter
    std::string massInfo(const std::shared_ptr<CObject>& obj, int shownDecimals) const;

    //provides a handle to all objects
    const std::array<std::list<std::shared_ptr<CObject>>, static_cast<int>(objectTypes::count)>& gameObjects() const;

    //provieds a handle to the sound object
    const std::unique_ptr<CSounds>& sound() const;


private:
    //the path where the prog is executed
    std::filesystem::path m_currentPath;

    //parser class which handles the settings
    CParser m_parser;

    //ptr to object that handles all sounds for the game
    std::unique_ptr<CSounds> m_pSound;

    //all active objects seperated into types
    std::array<std::list<std::shared_ptr<CObject>>, static_cast<int>(objectTypes::count)> m_objects;
    void addObject(const std::shared_ptr<CObject>&& object);

    //world attributes applying to all (moveable) objects
    v2d m_velocity;
    v2d m_accelerationX;
    v2d m_accelerationY;
    v2d m_center;

    int m_radiusView;
    int m_radiusMap;
    int m_fogOfWar;
    olc::Pixel m_playerColor;
    int m_startMass;

    float m_maxSpeed;
    float m_maxSpeed2;

    int m_starCardinality;

    //init objects, player need to be first
    void initPlayer();
    void initStars();

    ////spawn objects
	//spawnchance should be in [0, 1]
    float m_debrisChance;
    float m_debrisTimer;
    float m_debrisTimerReload;
    void spawnDebris(float deltaTime);

    //effects
    float m_effectEaten;
    float m_effectEatenTime;
    void effectEaten(float deltaTime);

    ////needs to be overridden for olc game engine:
    //initialization
    bool OnUserCreate() override;
    //game loop
    bool OnUserUpdate(float deltaTime) override;

    //draws the UI
    void drawUI();
};