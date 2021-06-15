//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file sound.cpp
/// \brief Handles all game sounds.
//==============================================================================

#include "sound.h"
#include "game.h"
#include "parser.h"
#include "helper/exception.h"

#include <iostream>


CSound::CSound(CGame* const pGame, std::filesystem::path currentPath) :
	m_pGame(pGame),
	m_soundPath{currentPath / "sounds" },
	m_pSound{},
	m_soundIDs{}
{
	std::cout << std::endl << "Initialize sound class, with its folder: " << m_soundPath << std::endl;
	olc::SOUND::InitialiseAudio();
	initSound(sounds::MUSIC0, pGame->parser().getString<parser::Game, parser::Music>());
	initSound(sounds::JET, pGame->parser().getString<parser::Player, parser::Jet>());
	initSound(sounds::CRASH0, pGame->parser().getString<parser::Debris, parser::Asteroid, parser::Crash>());
	initSound(sounds::SHOT0, pGame->parser().getString<parser::Missile, parser::Hawking, parser::Shot>());
	initSound(sounds::IMPACT0, pGame->parser().getString<parser::Missile, parser::Hawking, parser::Impact>());
}

CSound::~CSound()
{
	olc::SOUND::DestroyAudio();
}

void CSound::initSound(sounds index, std::string file)
{
	if(file != "")
	{
		std::filesystem::path filepath{ m_soundPath / file };
		int id{ m_pSound->LoadAudioSample(filepath.string()) };
		if (id == -1)
			throw CException{ "olcPGEX_Sound can't load audio sample: " + filepath.string(), INFO };
		else
		{
			m_soundIDs.at(static_cast<int>(index)) = id;
			std::cout << "Sound " + file + " with ID: " << std::to_string(id) << " initialized." << std::endl;
		}
	}
}

void CSound::playSound(sounds index, bool repeat) const
{
	int id{ m_soundIDs.at(static_cast<int>(index)) };
	if (id != 0)
		m_pSound->PlaySample(id, repeat);
}