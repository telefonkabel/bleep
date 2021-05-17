//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file bleep.cpp
/// \brief Provides the main function for this game.
//==============================================================================

#ifdef _WIN32
    #define USE_WINDOWS
#endif
#define OLC_PGE_APPLICATION
#define OLC_PGEX_SOUND

#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
#include "thirdParty/olcPixelGameEngine.h"
#include "thirdParty/olcPGEX_Sound.h"
#include "thirdParty/rapidjson/document.h"
#pragma warning(pop)

#include "game.h"
#include "parser.h"


int main()
{
    //setting up rand() - that rng is good enough for this purpose
    srand(static_cast<unsigned int>(time(0)));

    //load settings
    rapidjson::Document settings;
    CParser parser;
    parser.parse(settings);

    //start game
    CGame game("Bleep", olc::DARK_MAGENTA, 1000000, 640, 480, 2, 2, false);
    game.Start();

    return 0;
}