//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file game.h
/// \brief Central game class - manages the game flow based on the settings provided by the parser.
//==============================================================================

#pragma once

#include "jsParser.h"

#include "sound.h"
#include "helper/maths.h"
#include "objects/object.h"
#include "components/graphics.h"
#include "thirdParty/olcPixelGameEngine.h"

#include <filesystem>
#include <chrono>

namespace bleep
{

    class CObject;
    class CBHole;
    /// \brief Enum for mouse button IDs used by olc.
    /// \note To reflect the right olc IDs this mustn't have initializers.
    enum class mouse { LEFT, RIGHT, MIDDLE };

    /// \brief Central game class.
    /// This class overwrites the functions of olcPGE for initialization OnUserCreate() and game flow OnUserUpdate().
    class CGame : public olc::PixelGameEngine
    {
    public:
        /// \brief Constructor of game class.
        /// @param currentPath Execution path of the application.
        CGame(std::filesystem::path& currentPath);

        //===== getter =====
        /// \brief Getter for overall game velocity.
        /// @returns 2D vector of CGame::m_velocity.
        v2d velocity() const { return m_velocity; };
        /// \brief Getter for the center of the map.
        /// @returns 2D vector of CGame::m_center.
        v2d center() const { return m_center; };
        /// \brief Getter for the overall game acceleration.
        /// @returns 2D vector of CGame::m_acceleration.
        v2d acceleration() const { return m_acceleration; };
        /// \brief Getter for the view radius.
        /// @returns int of CGame::m_radiusView.
        int radiusView() const { return m_radiusView; };
        /// \brief Getter for the map radius.
        /// @returns int of CGame::m_radiusMap.
        int radiusMap() const { return m_radiusMap; };
        /// \brief Getter for the fog of war density.
        /// @returns int of CGame::m_radiusMap.
        int fog() const { return m_fogOfWar; };
        /// \brief Getter for player color.
        /// @returns olc color representation of CGame::m_playerColor.
        olc::Pixel playerColor() const { return m_playerColor; };
        /// \brief Getter for execution path of the application.
        /// @returns std path of CGame::m_currentPath.
        std::filesystem::path currentPath() const { return m_currentPath; };

        /// \brief Handle to parser obj.
        /// @returns Reference to parser obj CGame::m_parser which has static lifetime.
        const CParser& parser() const { return m_parser; };

        /// \brief Handle to sound obj.
        /// @returns Reference to sound obj CGame::m_sound which has static lifetime.
        const CSound& sound() { return m_sound; };

        /// \brief Handle to the player object.
        std::shared_ptr<CBHole> player() const { return m_pPlayer.lock(); };

        /// \brief Handle to all game objects.
        /// @returns Ref to the array of all objects, ordered in lists of specific object types.
        const std::array<std::list<std::shared_ptr<CObject>>, static_cast<int>(objectTypes::count)>& gameObjects() const { return m_objects; };

        /// \brief Cursor position.
        /// @returns 2D vector to the current position of the mouse cursor.
        v2d getCursor() const { return v2d{ static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY()) }; };

        /// \brief Models mass info of an object.
        /// Models the mass in scientific notation with a given amount of decimals. 
        /// @param obj The object which mass should be modeled.
        /// @param shownDecimals The amount of shown decimals
        /// @returns Mass as string in desired notation.
        std::string massInfo(const std::shared_ptr<CObject>& obj, int shownDecimals) const;

        /// \brief Calculates the elapsed time since application start.
        /// @returns The elapsed time in seconds.
        std::chrono::seconds playtime() const;

        /// \brief Add object to the array of all game objects.
        /// Each object has to be represented in the array of all game objects.
        /// \attention The obj will be classified with its object type.
        /// @returns Ref to the array of all objects, ordered in lists of specific object types.
        void addObject(const std::shared_ptr<CObject>&& object);

        /// \brief Plays the eaten effect.
        void effectEaten();

    private:
        /// \brief Holds the time of application start.
        std::chrono::time_point<std::chrono::steady_clock> m_startTime;

        /// \brief Holds the path where the application has been started.
        std::filesystem::path m_currentPath;

        /// \brief Parser object.
        CParser m_parser;

        /// \brief Sound object.
        CSound m_sound;

        /// \brief Central array of all game objects.
        /// Each item is a shared pointer to the game object, classified in a list with its object type.
        std::array<std::list<std::shared_ptr<CObject>>, static_cast<int>(objectTypes::count)> m_objects;

        //===== world attributes applying to all (movable) objects =====
        /// \brief Overall game velocity.
        /// Parsed out of settings.
        /// This is relatively to the player.
        v2d m_velocity;
        /// \brief Overall game acceleration.
        /// Parsed out of settings.
        /// This is relatively to the player and is divided in its x and y component.
        v2d m_acceleration;
        /// \brief Maximum player speed.
        /// Parsed out of settings.
        float m_maxSpeed;
        /// \brief Maximum player speed squared.
        /// Stored in an own var, since it's often needed.
        float m_maxSpeed2;

        //===== view attributes =====
        /// \brief View radius.
        /// Radius of area where game objects are shown - calculated as half of screen height.
        int m_radiusView;
        /// \brief Map radius.
        /// Radius of area where game objects could exist - calculated as 1.5 * screen height.
        int m_radiusMap;
        /// \brief Fog of war density.
        /// Calculated as 0.2 * view radius.
        int m_fogOfWar;
        /// \brief Center of the screen.
        /// This equals to the position of the player.
        v2d m_center;

        //===== player attributes =====
        /// \brief Player color.
        /// Parsed out of settings.
        olc::Pixel m_playerColor;
        /// \brief Player start mass.
        /// Parsed out of settings.
        int m_startMass;

        //===== init objects =====
        /// \brief Initialize the player object.
        /// \attention The player needs to be the first initialized object.
        void initPlayer();
        /// \brief Handle to the player object.
        /// Got an own handle, since it is the most used object.
        std::weak_ptr<CBHole> m_pPlayer;
        /// \brief Initialize background star objects.
        void initStars();
        /// \brief Amount of background stars.
        /// Parsed out of settings.
        int m_starCardinality;

        //===== spawn objects =====
        /// \brief Chance that a debris is spawned.
        /// Parsed out of settings.
        /// This should be in [0, 1].
        /// It gets diced once per interval CGame::m_debrisTimerReload.
        float m_debrisChance;
        /// \brief Helps determining when to dice next.
        /// This gets counted down by deltaTime. Once 0 a debris could spawn and this will be set back to CGame::m_debrisTimerReload.
        float m_debrisTimer;
        /// \brief The time interval in which one debris could spawn.
        /// Parsed out of settings.
        float m_debrisTimerReload;
        /// \brief Potentially spawns a debris.
        /// This gets influenced by CGame::m_debrisChance and CGame::m_debrisTimerReload.
        /// @param deltaTime Elapsed time since last frame.
        void spawnDebris(float deltaTime);

        //===== effects =====
        /// \brief Helps to determine when the next effect could be shown.
        /// This gets counted down by deltaTime. Once 0 this will be set back to CGame::m_effectEatenTime.
        float m_effectEaten;
        /// \brief The time interval in which one debris could spawn.
        /// This effect is fixed to 0.2 seconds.
        float m_effectEatenTime;
        /// \brief Plays all kinds of effects.
        /// This checks if an effect should be played and if it is ready to play.
        /// @param deltaTime Elapsed time since last frame.
        void globalEffects(float deltaTime);

        //===== needs to be overridden for olc game engine =====
        /// \brief Initializes the game.
        /// This is used by olcPGE 
        bool OnUserCreate() override;
        /// \brief The actual game loop.
        /// This is used by olcPGE 
        /// @param deltaTime Elapsed time since last frame.
        bool OnUserUpdate(float deltaTime) override;

        //===== GUI =====
        /// \brief Helps printing the circling cursor.
        /// The value is in [0, 1[ and is counted up by deltaTime - once 1 it is set to back to 0.
        /// The value is then used to calculate a point around the cursor position.
        float m_circling;
        /// \brief Draws all GUI objects.
        void drawGUI(float deltaTime);
        /// \brief Draws the cursor.
        void drawCursor(float deltaTime);
    };

} //end of namespace bleep