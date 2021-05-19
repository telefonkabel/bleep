//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// sounds.cpp
/// Handles all game sounds.
//==============================================================================

#include "sounds.h"
#include "exception.h"

#include <iostream>


CSounds::CSounds(std::filesystem::path currentPath) :
	m_soundPath{currentPath / "sounds" },
	m_sound{},
	m_soundIDs{}
{
	try
	{
		olc::SOUND::InitialiseAudio();
		initSound(sounds::MUSIC0, m_sound->LoadAudioSample((m_soundPath / "music0.wav").string()));
		initSound(sounds::JET, m_sound->LoadAudioSample((m_soundPath / "jet.wav").string()));
		initSound(sounds::CRASH0, m_sound->LoadAudioSample((m_soundPath / "crash0.wav").string()));
	}
	catch (CException& exception)
	{
		std::cerr << "Exception in sounds: " << exception.msg();
	}
	catch (...)
	{
		std::cerr << "Unhandled exception in sounds.";
	}
}

void CSounds::initSound(sounds index, int id)
{
	if (static_cast<int>(id) == -1)
		throw CException{ "olcPGEX_Sound can't load audio sample with sound ID: " + std::to_string(id) };
	else
		m_soundIDs.at(static_cast<int>(index)) = id;
}

void CSounds::playSound(sounds index, bool repeat) const
{
	m_sound->PlaySample(m_soundIDs.at(static_cast<int>(index)), repeat);
}