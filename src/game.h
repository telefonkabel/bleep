//==============================================================================
/// 2021, Benedikt Michael.
//==============================================================================
/// game.h
/// Central game class - manages the game flow based on the settings provided by the parser.
//==============================================================================

#pragma once

#include "object.h"
#include "maths.h"
#include "asteroid.h"

#include <filesystem>


class CGame : public olc::PixelGameEngine
{
public:
    CGame(std::filesystem::path& currentPath, std::string gameName, olc::Pixel playerColor, int startMass, int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h, bool fullscreen);
    ~CGame();

    v2d velocity() const;
    v2d center() const;
    v2d accelerationX() const;
    v2d accelerationY() const;
    int radiusView() const;
    int radiusMap() const;
    int fog() const;
    olc::Pixel playerColor() const;
    std::filesystem::path currentPath() const;

    //whenever the mass of an object is needed as output in scientific notation with shown decimals as parameter
    std::string massInfo(const std::shared_ptr<CObject>& obj, int shownDecimals) const;

    //provides a handle to the player object specifically and all objects
    const std::shared_ptr<CObject>& player() const;
    const std::array<std::list<std::shared_ptr<CObject>>, static_cast<int>(objectTypes::count)>& gameObjects() const;


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

    std::filesystem::path m_currentPath;

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

    //need to be overridden for olc game engine:
    //initialization
    bool OnUserCreate() override;
    //game loop
    bool OnUserUpdate(float deltaTime) override;

    void drawUI();
};