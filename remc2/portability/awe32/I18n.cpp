#include "I18n.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <vector>

#ifdef AWE32EMU_NLS
#include <clocale>
#include <filesystem>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#endif

#ifdef AWE32EMU_NLS
namespace
{
    // Directory of the running executable. argv[0] need not contain a path
    // (a start through PATH), so the OS is asked first.
    std::filesystem::path ExeDir(const char* argv0)
    {
        std::error_code ec;
#if defined(_WIN32)
        wchar_t buf[4096];
        const DWORD n = GetModuleFileNameW(nullptr, buf, 4096);
        if (n > 0 && n < 4096)
            return std::filesystem::path(buf).parent_path();
#elif defined(__linux__)
        const auto self = std::filesystem::read_symlink("/proc/self/exe", ec);
        if (!ec)
            return self.parent_path();
#endif
        if (argv0 && *argv0)
            return std::filesystem::absolute(argv0, ec).parent_path();
        return {};
    }
}
#endif

void I18n::Init(const char* argv0)
{
#ifdef AWE32EMU_NLS
    std::setlocale(LC_ALL, "");
    // Numbers are written into traces, CSV dumps and WAV headers; a decimal
    // comma there would break the tools that read them.
    std::setlocale(LC_NUMERIC, "C");

    std::string dir;
    if (const char* env = std::getenv("AWE32EMU_LOCALEDIR"); env && *env)
        dir = env;
    else
        dir = (ExeDir(argv0) / "locale").string();

    bindtextdomain("awe32emu", dir.c_str());
    bind_textdomain_codeset("awe32emu", "UTF-8");
    textdomain("awe32emu");
#else
    (void)argv0;
#endif
}

std::string StrFormat(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    va_list ap2;
    va_copy(ap2, ap);
    const int n = std::vsnprintf(nullptr, 0, fmt, ap);
    va_end(ap);
    if (n <= 0)
    {
        va_end(ap2);
        return {};
    }
    std::vector<char> buf(static_cast<size_t>(n) + 1);
    std::vsnprintf(buf.data(), buf.size(), fmt, ap2);
    va_end(ap2);
    return std::string(buf.data(), static_cast<size_t>(n));
}
