//==============================================================================
/// 2021, Benedikt Michael.
//==============================================================================
/// sounds.cpp
/// Component class which handles all object sounds.
//==============================================================================

#include "sounds.h"


//initialization of static members
bool CSounds::m_firstInit = true;
olc::SOUND* CSounds::m_sound;
std::array<int, static_cast<int>(sounds::count)> CSounds::m_soundIDs = std::array<int, static_cast<int>(sounds::count)>();

CSounds::CSounds(CObject* pObject) :
	CComponent(pObject),
	m_soundPath(pObject->game()->currentPath() / "sounds")
{
	if (m_firstInit)
		initSounds();
}

void CSounds::initSounds()
{
	m_firstInit = false;

	olc::SOUND::InitialiseAudio();
	initSound(sounds::MUSIC0, m_sound->LoadAudioSample((m_soundPath / "music0.wav").string()));
	initSound(sounds::JET, m_sound->LoadAudioSample((m_soundPath / "jet.wav").string()));
	initSound(sounds::CRASH0, m_sound->LoadAudioSample((m_soundPath / "crash0.wav").string()));
}

void CSounds::update(float deltaTime)
{
	;
}

void CSounds::initSound(sounds position, int id)
{
	if (static_cast<int>(position) == -1)
		; //throw
	else
		m_soundIDs.at(static_cast<int>(position)) = id;
}

void CSounds::playSound(sounds position, bool repeat) const
{
	m_sound->PlaySample(m_soundIDs.at(static_cast<int>(position)), repeat);
}