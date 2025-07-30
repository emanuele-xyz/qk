#include <qk_gui/PCH.h>
#include <qk_gui/Commons.h>

namespace qk_gui
{
    Crash::Crash(const char* msg)
        : std::runtime_error{ std::format("{}\n{}", msg, std::stacktrace::current(1)) }
    {
    }

    std::string GetBytesStr(size_t bytes)
    {
        const char* suffixes[]{ "B", "KB", "MB", "GB", "TB", "PB" };

        size_t i{};
        double val{ static_cast<double>(bytes) };
        while (val >= 1024.0 && i < std::size(suffixes) - 1)
        {
            val /= 1024.0;
            i++;
        }

        std::ostringstream out{};
        out << std::fixed << std::setprecision(2) << val << " " << suffixes[i];

        return out.str();
    }
}

#include <qk_gui/W32.h>

namespace qk_gui
{
    std::string StrFromWStr(const std::wstring& wstr)
    {
        int buf_size{ WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr) };
        if (buf_size > 0)
        {
            std::unique_ptr<char[]> buf{ std::make_unique<char[]>(buf_size) };
            int written{ WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buf.get(), buf_size, nullptr, nullptr) };
            qk_gui_Check(written == buf_size);
            return buf.get();
        }
        else
        {
            return "";
        }
    }
    std::wstring WStrFromStr(const std::string& wstr)
    {
        int buf_size{ MultiByteToWideChar(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0) };
        if (buf_size > 0)
        {
            std::unique_ptr<wchar_t[]> buf{ std::make_unique<wchar_t[]>(buf_size) };
            int written{ MultiByteToWideChar(CP_UTF8, 0, wstr.c_str(), -1, buf.get(), buf_size) };
            qk_gui_Check(written == buf_size);
            return buf.get();
        }
        else
        {
            return L"";
        }
    }
}
