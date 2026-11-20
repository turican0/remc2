#pragma once
#include "Globals.h"
#include <vector>

// Checks that a game has the folders and files it needs before it can start (the folder names as on the CD,
// matched without case):
//  - the game folder (the installed game) holds files
//  - the CD folder holds the folders the game reads: MC1 / Hidden Worlds data, intro, levels;
//    MC2 also language and sound
//  - Hidden Worlds: its tmaps (data/tmaps1-0) and levels (levels/ddlevels), in the install folder or on the CD
//    (a Magic Carpet CD without Hidden Worlds does not have them)
namespace GameFilesCheck
{
	// what the game (Config::Settings::GameName) misses, one line each; empty when it can start
	std::vector<wxString> Missing(const Config::Settings::Paths& paths, const std::string& gameName);

	// the lines as one text, each on its own line
	wxString ToText(const std::vector<wxString>& missing);
}
