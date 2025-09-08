#include <qk/PCH.h>
#include <qk/Commons.h>

namespace qk
{
    Crash::Crash(const char* msg)
        : std::runtime_error{ std::format("{}\n{}", msg, std::stacktrace::current(1)) }
    {
    }

    Stopwatch::Stopwatch()
        : m_last_time{ std::chrono::high_resolution_clock::now() }
        , m_elapsed_sec{}
    {
    }
    void Stopwatch::Mark() noexcept
    {
        Time now{ std::chrono::high_resolution_clock::now() };
        auto dt{ std::chrono::duration_cast<std::chrono::duration<float>>(now - m_last_time) };
        m_last_time = now;
        m_elapsed_sec = dt.count();
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

#include <qk/W32.h>

namespace qk
{
    std::string StrFromWStr(const std::wstring& wstr)
    {
        int buf_size{ WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr) };
        if (buf_size > 0)
        {
            std::unique_ptr<char[]> buf{ std::make_unique<char[]>(buf_size) };
            int written{ WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buf.get(), buf_size, nullptr, nullptr) };
            qk_Check(written == buf_size);
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
            qk_Check(written == buf_size);
            return buf.get();
        }
        else
        {
            return L"";
        }
    }
}
