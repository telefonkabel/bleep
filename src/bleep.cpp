//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file bleep.cpp
/// \brief Provides the main function for this game.
//==============================================================================

#define OLC_PGE_APPLICATION
#define OLC_PGEX_SOUND

#ifdef _WIN32
    #define USE_WINDOWS
#else
    #include <unistd.h>
#endif

#include <rapidjson/document.h>

#include "thirdParty/olcPixelGameEngine.h"
#include "thirdParty/olcPGEX_Sound.h"

#include "game.h"
#include "parser.h"

int main()
{
    //get the current path of the executable platform independently
    std::size_t len{};
    std::wstring currentPath{};

    #ifdef USE_WINDOWS
    {
        std::vector<wchar_t> buffer(0);
        do
        {
            buffer.resize(buffer.size() + 1);
            len = GetModuleFileName(NULL, &buffer.at(0), buffer.size());
        } while (buffer.size() == len);

        if (len == 0)
            ;//throw
        else
            currentPath = { buffer.data(), buffer.size() };

        std::wcout << currentPath;
    }
    #else
    {
        std::vector<char> buffer(0);
        do
        {
            buffer.resize(buffer.size() + 1);
            len = readlink("/proc/self/exe", &buffer.at(0), buffer.size());
        } while (buffer.size() == len);

        if (len == 0)
            ;//throw
        else
        {
            std::string path{ buffer.data(), buffer.size() };
            currentPath = { path.begin(), path.end() }
        }
    }
    #endif

    //setting up rand() - that rng is good enough for this purpose
    srand(static_cast<unsigned int>(time(0)));

    //load settings
    rapidjson::Document settings;
    CParser parser;
    parser.parse(settings);

    //start game
    CGame game("Bleep", olc::DARK_MAGENTA, 1000000, 640, 480, 2, 2, true);
    game.Start();

    return 0;
}