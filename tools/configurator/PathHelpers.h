#pragma once
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/string.h>
#include <wx/utils.h>
#include <wx/msgdlg.h>

// Returns the directory that contains the running executable.
wxString GetExeDir();

// Converts an absolute path to a path relative to the exe directory.
// Falls back to the absolute path if it can't be made relative (e.g. different drive).
wxString ToRelative(const wxString& absPath);

// Resolves a (possibly relative) path against the exe directory to an absolute path.
// Useful for pre-populating directory dialogs.
wxString ToAbsolute(const wxString& path);

bool LaunchGame(const wxString& args);

