#pragma once
#ifndef AWE32_ROM_H
#define AWE32_ROM_H

#include <cstdint>
#include <string>
#include <vector>

// Wave ROM of the Sound Blaster AWE32 (EMU8000), 1 MB. It is looked up among
// the data paths of remc2 as <folder>/data.binz (the ROM packed by gzip) or
// <folder>/data.bin (the plain image) and, when allowed, downloaded there.
namespace Awe32Rom
{
	// Path of a usable ROM, or an empty string; then `error` says why.
	std::string Locate(const std::string& folder, std::string& error);

	// Contents of a ROM file, unpacked when it is gzip data.
	bool Read(const std::string& path, std::vector<uint8_t>& data, std::string& error);

	// A github.com ".../blob/<ref>/<path>" page address turned into the raw file address.
	std::string RawUrl(const std::string& url);
}

#endif //AWE32_ROM_H
