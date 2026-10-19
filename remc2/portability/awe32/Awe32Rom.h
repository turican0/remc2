#pragma once
#ifndef AWE32_ROM_H
#define AWE32_ROM_H

#include <string>

// Wave ROM of the Sound Blaster AWE32 (EMU8000), 1 MB. It is Creative's and
// is not part of remc2: it is looked up as <folder>/awe32.raw among the data
// paths of remc2 and, when allowed, downloaded there.
namespace Awe32Rom
{
	// Path of a usable ROM, or an empty string; then `error` says why.
	std::string Locate(const std::string& folder, bool download, const std::string& url, std::string& error);

	// A github.com ".../blob/<ref>/<path>" page address turned into the raw file address.
	std::string RawUrl(const std::string& url);
}

#endif //AWE32_ROM_H
