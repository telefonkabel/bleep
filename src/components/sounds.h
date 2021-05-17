//==============================================================================
/// \copyright (c) 2021, Benedikt Michael.
//==============================================================================
/// \file sounds.h
/// \brief Component class which handles all object sounds.
//==============================================================================

#pragma once

#include "component.h"
#include "game.h"

#include <filesystem>

#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
#include "thirdParty/olcPGEX_Sound.h"
#pragma warning(pop)


enum class sounds { MUSIC0, JET, CRASH0, count};


class CSounds : public CComponent
{
public:
	CSounds(CObject* pObject);

	void update(float deltaTime) override;

	void playSound(sounds position, bool repeat) const;

private:
	std::filesystem::path m_soundPath;

	////encapsulates sounds in this component and shares them between all instances
	static bool m_firstInit;
	static olc::SOUND* m_sound;
	//wraps the int IDs of olc for better readability
	static std::array<int, static_cast<int>(sounds::count)> m_soundIDs;

	void initSounds();
	//initialize individual sounds and links them to their corresponding position in the array - throws, if olc returns "-1"
	void initSound(sounds position, int id);
};