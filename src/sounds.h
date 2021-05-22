//==============================================================================
/// 2021, Benedikt Michael
//==============================================================================
/// sounds.h
/// Handles all game sounds.
//==============================================================================

#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_Sound.h"

#include <array>
#include <filesystem>


enum class sounds { MUSIC0, JET, CRASH0, count};

class CSounds
{
public:
	CSounds(std::filesystem::path currentPath);
	~CSounds();

	void playSound(sounds index, bool repeat) const;

private:
	std::filesystem::path m_soundPath;

	//Handle to olc's sound
	std::unique_ptr<olc::SOUND> m_pSound;
	//wraps the int IDs of olc for better readability
	std::array<int, static_cast<int>(sounds::count)> m_soundIDs;

	//initialize individual sounds and links them to their corresponding array index - throws, if olc returns "-1"
	void initSound(sounds index, int id);
};