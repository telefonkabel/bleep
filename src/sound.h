//==============================================================================
/// \copyright (c) 2021, Benedikt Michael
//==============================================================================
/// \file sound.h
/// \brief Handles all game sounds.
//==============================================================================

#pragma once
#include "thirdParty/olcPixelGameEngine.h"
#include "thirdParty/olcPGEX_Sound.h"

#include <array>
#include <filesystem>


class CGame;

//must not have initializers (and will be replaced in the future)
enum class sounds { MUSIC0, JET, CRASH0, SHOT0, IMPACT0, count};

class CSound
{
public:
	/// \brief Constructor of sound class.
	/// @param pGame Handle to the central game object.
	/// @param currentPath Execution path of the application.
	CSound(CGame* const pGame, std::filesystem::path currentPath);
	/// \brief Destructor of sound class.
	/// Starts clean up of the olc audio system.
	~CSound();

	/// \brief Plays a sound.
	/// @param index Id of the audio file to play.
	/// @param repeat Flag to set loop or not.
	void playSound(sounds index, bool repeat) const;

private:
	/// \brief Handle to central game object.
	CGame* const m_pGame;
	/// \brief Execution path of the application.
	std::filesystem::path m_soundPath;

	/// \brief Handle to olc sound obj.
	std::unique_ptr<olc::SOUND> m_pSound;
	/// \brief Array which wraps the olc sound id.
	/// Improves readability of olc sound ID through custom enum.
	std::array<int, static_cast<int>(sounds::count)> m_soundIDs;

	/// \brief Initializes a specific sound.
	/// This initializes an olc sound and links its ID to its corresponding sounds enum value.
	/// @param index The sounds enum value which is linked to.
	/// @param file The file of the audio which is initialized - e.g. "sound.wav"
	/// \attention Only .wav files are supported.
	void initSound(sounds index, std::string file);
};