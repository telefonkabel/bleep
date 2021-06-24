//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file game.cpp
/// \brief Central game class - manages the game flow based on the settings provided by the parser.
//==============================================================================

#include "game.h"
#include "helper/exception.h"
#include "objects/object.h"
#include "objects/star.h"
#include "objects/blackHole.h"
#include "objects/asteroid.h"
#include "components/kinetics.h"


namespace bleep
{

    CGame::CGame(std::filesystem::path& currentPath) :
        m_startTime{ std::chrono::steady_clock::now() },
        m_currentPath{ currentPath },
        m_parser{ CParser(currentPath) },
        m_sound{ CSound(this, currentPath) },
        m_objects{},
        m_velocity{ m_parser.getV2D<parser::Game, parser::Velocity>() },
        m_acceleration{ m_parser.getV2D<parser::Game, parser::Acceleration>() },
        m_maxSpeed{ m_parser.getFloat<parser::Game, parser::MaxSpeed>() },
        m_maxSpeed2{ static_cast<float>(std::pow(m_maxSpeed, 2)) },
        m_radiusView{ m_parser.getInt<parser::Window, parser::ScreenHeight>() / 2 },
        m_radiusMap{ static_cast<int>(m_radiusView * 1.5f) },
        m_fogOfWar{ static_cast<int>(0.2f * m_radiusView) },
        m_center{ m_parser.getFloat<parser::Window, parser::ScreenWidth>() / 2,
            m_parser.getFloat<parser::Window, parser::ScreenHeight>() / 2 },
        m_playerColor{ m_parser.color(m_parser.getString<parser::Player, parser::Color>()) },
        m_startMass{ m_parser.getInt<parser::Player, parser::Mass>() },
        m_pPlayer{},
        m_starCardinality{ m_parser.getInt<parser::Game, parser::StarCard>() },
        m_debrisChance{ m_parser.getFloat<parser::Debris, parser::SpawnChance>() },
        m_debrisTimer{},
        m_debrisTimerReload{ m_parser.getFloat<parser::Debris, parser::SpawnReload>() },
        m_effectEaten{},
        m_effectEatenTime{ 0.2f },
        m_circling{}
    {
        sAppName = m_parser.getString<parser::Window, parser::Name>();
        Construct(m_parser.getInt<parser::Window, parser::ScreenWidth>(), m_parser.getInt<parser::Window, parser::ScreenHeight>(),
            m_parser.getInt<parser::Window, parser::PixelWidth>(), m_parser.getInt<parser::Window, parser::PixelHeight>(),
            m_parser.getBool<parser::Window, parser::FullScreen>());

        m_sound.playSound(sounds::MUSIC0, true);
    }

    bool CGame::OnUserCreate()
    {
        try
        {
            initPlayer();
            initStars();
        }
        catch (CException& exception)
        {
            exception.print();
            return false;
        }
        catch (...)
        {
            std::cerr << "Unhandled exception in " << S(__FILE__);
            return false;
        }

        return true;
    }

    bool CGame::OnUserUpdate(float deltaTime)
    {
        try
        {
            ////world input
            //close game
            if (GetKey(olc::ESCAPE).bHeld)
            {
                std::cout << "Bleep terminated with escape." << std::endl;
                return false;
            }
            //move the world relatively to the player
            if (GetKey(olc::W).bHeld)
                m_velocity.y += m_acceleration.y * deltaTime;
            if (GetKey(olc::S).bHeld)
                m_velocity.y -= m_acceleration.y * deltaTime;
            if (GetKey(olc::A).bHeld)
                m_velocity.x += m_acceleration.x * deltaTime;
            if (GetKey(olc::D).bHeld)
                m_velocity.x -= m_acceleration.x * deltaTime;
            if (m_velocity.mag2() > m_maxSpeed2)
                m_velocity = m_velocity.norm() * m_maxSpeed;
            //mouse input
            auto pPlayer{ player() };
            if (GetMouse(static_cast<int>(mouse::LEFT)).bHeld && pPlayer)
                pPlayer->fireHawking(getCursor(), deltaTime);

            ////objects update & display
            spawnDebris(deltaTime);

            //olc - needs to be called before drawing
            Clear(olc::BLACK);
            SetPixelMode(olc::Pixel::MASK);

            //User interface
            drawGUI(deltaTime);

            //objects update & clean up
            for (auto& objectType : m_objects)
                for (auto objItr = objectType.begin(); objItr != objectType.end(); ++objItr)
                {
                    if ((*objItr)->state() == objectStates::ALIVE)
                        (*objItr)->update(deltaTime);
                    else
                        objectType.erase(objItr++);

                    if (objItr == objectType.end())
                        break;
                }

            ////effects
            globalEffects(deltaTime);

            //olc - needs to be called after drawing
            SetPixelMode(olc::Pixel::NORMAL);
        }
        catch (CException& exception)
        {
            exception.print();
            return false;
        }
        catch (...)
        {
            std::cerr << "Unhandled exception in " << S(__FILE__);
            return false;
        }

        return true;
    }

    void CGame::globalEffects(float deltaTime)
    {
        //jet-effect when something got devoured by a black hole
        if (m_effectEaten > 0.0f)
        {
            m_effectEaten -= deltaTime;
            SetPixelMode(olc::Pixel::ALPHA);
            SetPixelBlend(0.5f * maths::fadeInOut((m_effectEatenTime - m_effectEaten) / m_effectEatenTime));
            FillRect(0, 0, ScreenWidth(), ScreenHeight(), m_playerColor / 4);
            SetPixelMode(olc::Pixel::NORMAL);
        }
    }

    void CGame::effectEaten()
    {
        m_effectEaten = m_effectEatenTime;
        m_sound.playSound(sounds::JET, false);
    }

    void CGame::addObject(const std::shared_ptr<CObject>&& object)
    {
        m_objects.at(static_cast<int>(object->type())).push_back(object);
    }

    void CGame::initStars()
    {
        for (size_t i{ 0 }; i < m_starCardinality; ++i)
        {
            v2d starPos(static_cast<float>(rand() % ScreenWidth()), static_cast<float>(rand() % ScreenHeight()));

            colors starColor;
            if (i > m_starCardinality * 0.9f)
                starColor = olc::WHITE;
            else if (i > m_starCardinality * 0.7f)
                starColor = olc::GREY;
            else if (i > m_starCardinality * 0.4f)
                starColor = olc::DARK_GREY;
            else
                starColor = olc::VERY_DARK_GREY;

            std::shared_ptr<CStar> star{ std::make_shared<CStar>(this, objectTypes::STAR, starPos, 0, starColor) };
            if ((center() - starPos).mag() <= radiusView())
            {
                star->kinetics()->speedFactor((1.5f - static_cast<float>(std::log10(m_starCardinality - i + 1) / std::log10(m_starCardinality + 1))) * 0.5f);
                addObject(std::move(star));
            }
        }
    }

    void CGame::initPlayer()
    {
        std::shared_ptr<CBHole> player{ std::make_shared<CBHole>(this, objectTypes::BLACKHOLE, center(), m_startMass, m_playerColor) };
        addObject(std::move(player));
        m_pPlayer = std::static_pointer_cast<CBHole>(m_objects.at(static_cast<int>(objectTypes::BLACKHOLE)).front());
    }

    void CGame::spawnDebris(float deltaTime)
    {
        if (static_cast<float>(rand()) / RAND_MAX < m_debrisChance)
        {
            m_debrisTimer -= deltaTime;
            if (m_debrisTimer <= 0.0f)
            {
                //dice not more than once per time unit
                m_debrisTimer = m_debrisTimerReload;

                std::shared_ptr<CAsteroid> debris{ std::make_shared<CAsteroid>(this, objectTypes::DEBRIS) };
                addObject(std::move(debris));
            }
        }
    }

    void CGame::drawGUI(float deltaTime)
    {
        auto pPlayer{ player() };

        //draw cursor
        drawCursor(deltaTime);
        //mass info
        DrawStringDecal(v2d(10, static_cast<float>(ScreenHeight() - 30)), "MASS:", olc::WHITE, v2d(1, 1));
        DrawStringDecal(v2d(10, static_cast<float>(ScreenHeight() - 10)), massInfo(pPlayer, 5), olc::WHITE, v2d(1, 1));

        //game over
        if (!pPlayer)
            DrawStringDecal(center() + v2d{ -40, -10 }, "Game Over!");
    }

    void CGame::drawCursor(float deltaTime)
    {
        m_circling = m_circling < 1 ? (m_circling + deltaTime / 1.5f) : 0; //in interval [0,1[
        v2d circleVec{ 0.0f, 4.0f };
        maths::rotate(circleVec, m_circling * 2 * maths::PI);
        v2d cursor{ getCursor() };
        FillCircle(cursor + circleVec, 1, m_playerColor);
        DrawCircle(cursor + circleVec, 2, m_playerColor / 3.0f);
        DrawCircle(cursor + circleVec, 3, m_playerColor / 6.0f);
        FillCircle(cursor - circleVec, 1, m_playerColor);
        DrawCircle(cursor - circleVec, 2, m_playerColor / 3.0f);
        DrawCircle(cursor - circleVec, 3, m_playerColor / 6.0f);
        maths::rotate(circleVec, -0.8f);
        FillCircle(cursor + circleVec, 1, m_playerColor / 3.0f);
        FillCircle(cursor - circleVec, 1, m_playerColor / 3.0f);
    }

    std::string CGame::massInfo(const std::shared_ptr<CObject>& obj, int shownDecimals) const
    {
        if (!obj)
            return "0";

        int shownMass{ obj->mass() };
        int powerIndex{ 0 };
        while (shownMass >= pow(10, shownDecimals))
        {
            powerIndex++;
            shownMass = static_cast<int>(shownMass / 10);
        }

        return std::to_string(shownMass) + "E" + std::to_string(powerIndex);
    }

    std::chrono::seconds CGame::playtime() const
    {
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_startTime);
    }

} //end of namespace bleep