#include "PathHelpers.h"

wxString GetExeDir()
{
	wxFileName exe(wxStandardPaths::Get().GetExecutablePath());
	return exe.GetPath();
}

wxString ToRelative(const wxString& absPath)
{
	wxFileName fn(absPath);
	fn.MakeRelativeTo(GetExeDir());
	return fn.GetFullPath();
}

wxString ToAbsolute(const wxString& path)
{
	wxFileName fn(path);
	if (fn.IsRelative())
		fn.MakeAbsolute(GetExeDir());
	return fn.GetFullPath();
}

bool LaunchGame(const wxString& args)
{
	// Launch remc2.exe detached (wxEXEC_ASYNC = fire-and-forget)
	long pid = wxExecute("remc2.exe " + args, wxEXEC_ASYNC);
	if (pid == 0) {
		wxMessageBox("Failed to launch Magic Carpet 2 HD.\n"
			"Make sure it is in the same directory as this application.",
			"Launch Error", wxOK | wxICON_ERROR);
		return false;
	}
	return true;
}