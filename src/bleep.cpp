//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file bleep.cpp
/// \brief Provides main function for this game.
//==============================================================================


#define USE_WINDOWS
#define OLC_PGE_APPLICATION
#define OLC_PGEX_SOUND

#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
#include "imported/olcPixelGameEngine.h"
#include "imported/olcPGEX_Sound.h"
#pragma warning(pop)

#include "game.h"
#include "parser.h"

int main()
{
    //rand() functionality is more than enough for this game
    srand(static_cast<unsigned int>(time(0)));

    //load settings
    CParser settings;

    CGame game("Bleep", olc::DARK_MAGENTA, 1000000, 640, 480, 2, 2, false);
    game.Start();

    return 0;
}