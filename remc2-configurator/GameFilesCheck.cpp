#include "GameFilesCheck.h"
#include "PathHelpers.h"

#include <filesystem>

namespace
{
	using GameName = Config::Settings::GameName;
	using PathKey = Config::Settings::PathKey;

	wxString DescribeFolder(const std::string& folder)
	{
		return folder.empty() ? wxString("(no folder set)") : ToAbsolute(wxString::FromUTF8(folder));
	}

	std::filesystem::path FolderPath(const std::string& folder)
	{
		return std::filesystem::path(ToAbsolute(wxString::FromUTF8(folder)).ToStdWstring());
	}

	bool FolderExists(const std::string& folder)
	{
		if (folder.empty())
			return false;
		std::error_code error;
		return std::filesystem::is_directory(FolderPath(folder), error);
	}

	bool FolderHasFiles(const std::string& folder)
	{
		if (!FolderExists(folder))
			return false;
		std::error_code error;
		for (std::filesystem::recursive_directory_iterator it(FolderPath(folder), std::filesystem::directory_options::skip_permission_denied, error), end;
			!error && it != end; it.increment(error))
		{
			if (it->is_regular_file(error))
				return true;
		}
		return false;
	}

	// the entry folder/parts[0]/parts[1]... matched without case (the names of the CD are upper case, an installed or
	// copied game may have any case), empty if there is none
	std::filesystem::path FindEntry(const std::string& folder, const std::vector<std::string>& parts)
	{
		if (!FolderExists(folder))
			return {};
		std::filesystem::path current = FolderPath(folder);
		for (const auto& part : parts)
		{
			const wxString wanted = wxString::FromUTF8(part);
			std::filesystem::path found;
			std::error_code error;
			for (std::filesystem::directory_iterator it(current, std::filesystem::directory_options::skip_permission_denied, error), end;
				!error && it != end; it.increment(error))
			{
				if (wanted.IsSameAs(wxString(it->path().filename().wstring()), false))
				{
					found = it->path();
					break;
				}
			}
			if (found.empty())
				return {};
			current = found;
		}
		return current;
	}

	bool HasFileInAny(const std::vector<std::string>& folders, const std::string& file)
	{
		std::vector<std::string> parts;
		for (const auto& part : wxSplit(wxString::FromUTF8(file), '/'))
			parts.push_back(part.ToStdString());
		for (const auto& folder : folders)
		{
			const std::filesystem::path path = FindEntry(folder, parts);
			std::error_code error;
			if (!path.empty() && std::filesystem::is_regular_file(path, error))
				return true;
		}
		return false;
	}
}

std::vector<wxString> GameFilesCheck::Missing(const Config::Settings::Paths& paths, const std::string& gameName)
{
	std::vector<wxString> missing;

	const std::string gameFolder = paths.Get(gameName, PathKey::GameFolder);
	const std::string cdFolder = paths.Get(gameName, PathKey::CdFolder);

	if (gameName == GameName::MagicCarpet2)
	{
		if (!FolderExists(gameFolder))
			missing.push_back("Game folder: " + DescribeFolder(gameFolder));
		if (!FolderExists(cdFolder))
			missing.push_back("CD folder: " + DescribeFolder(cdFolder));
		else
		{
			const std::vector<std::string> gameSubfolders = std::vector<std::string>{ "CDATA", "LANGUAGE", "CLEVELS" };
			const std::vector<std::string> cdSubfolders = std::vector<std::string>{ "DATA", "INTRO", "LANGUAGE", "LEVELS", "SOUND" };

			for (const auto& subfolder : gameSubfolders)
				if (FindEntry(gameFolder, { subfolder }).empty())
					missing.push_back("Game folder: " + DescribeFolder(gameFolder) + "/" + subfolder);

			for (const auto& subfolder : cdSubfolders)
				if (FindEntry(cdFolder, { subfolder }).empty())
					missing.push_back("CD folder: " + DescribeFolder(cdFolder) + "/" + subfolder);
		}
	}

	if (gameName == GameName::MagicCarpet)
	{
		if (!FolderExists(gameFolder))
			missing.push_back("Game folder: " + DescribeFolder(gameFolder));
		if (!FolderExists(cdFolder))
			missing.push_back("CD folder: " + DescribeFolder(cdFolder));
		else
		{
			const std::vector<std::string> gameSubfolders = std::vector<std::string>{ "DATA", "LEVELS" };
			const std::vector<std::string> cdSubfolders = std::vector<std::string>{ "DATA", "INTRO", "LEVELS", "MOVIE" };

			for (const auto& subfolder : gameSubfolders)
				if (FindEntry(gameFolder, { subfolder }).empty())
					missing.push_back("Game folder: " + DescribeFolder(gameFolder) + "/" + subfolder);

			for (const auto& subfolder : cdSubfolders)
				if (FindEntry(cdFolder, { subfolder }).empty())
					missing.push_back("CD folder: " + DescribeFolder(cdFolder) + "/" + subfolder);
		}
	}

	if (gameName == GameName::HiddenWorlds)
	{
		if (!FolderExists(gameFolder))
			missing.push_back("Game folder: " + DescribeFolder(gameFolder));
		if (!FolderExists(cdFolder))
			missing.push_back("CD folder: " + DescribeFolder(cdFolder));
		else
		{
			const std::vector<std::string> gameSubfolders = std::vector<std::string>{ "DATA", "LEVELS" };
			const std::vector<std::string> cdSubfolders = std::vector<std::string>{ "DATA", "INTRO", "LEVELS", "MOVIE" };

			for (const auto& subfolder : gameSubfolders)
				if (FindEntry(gameFolder, { subfolder }).empty())
					missing.push_back("Game folder: " + DescribeFolder(gameFolder) + "/" + subfolder);

			for (const auto& subfolder : cdSubfolders)
				if (FindEntry(cdFolder, { subfolder }).empty())
					missing.push_back("CD folder: " + DescribeFolder(cdFolder) + "/" + subfolder);
		}

		// the game reads them from the install folder, else from the CD
		for (const char* file : { "DATA/TMAPS1-0.DAT", "DATA/TMAPS1-0.TAB", "LEVELS/DDLEVELS.DAT", "LEVELS/DDLEVELS.TAB" })
			if (!HasFileInAny({ gameFolder }, file))
				missing.push_back(wxString("Hidden Worlds file: ") + file);
	}
	return missing;
}

wxString GameFilesCheck::ToText(const std::vector<wxString>& missing)
{
	wxString text;
	for (const auto& line : missing)
		text += (text.empty() ? "" : "\n") + line;
	return text;
}
