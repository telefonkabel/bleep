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

#include <filesystem>


class CGame : public olc::PixelGameEngine
{
public:
    CGame(const std::shared_ptr<CSounds>& sound, std::filesystem::path& currentPath, std::string gameName, olc::Pixel playerColor, int startMass, int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h, bool fullscreen);
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

    //provides a handle to the player object specifically and all objects
    const std::shared_ptr<CObject>& player() const;
    const std::array<std::list<std::shared_ptr<CObject>>, static_cast<int>(objectTypes::count)>& gameObjects() const;

    //provieds a handle to the sound object
    const std::shared_ptr<CSounds>& sound() const;


private:
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

    //the path where the prog is executed
    std::filesystem::path m_currentPath;

    //ptr to object that handles all sounds for the game
    std::shared_ptr<CSounds> m_sound;

    //all active objects seperated into types
    std::array<std::list<std::shared_ptr<CObject>>, static_cast<int>(objectTypes::count)> m_objects;
    void addObject(const std::shared_ptr<CObject>&& object);

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