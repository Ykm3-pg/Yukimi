#include "EnginePCH.h"
#include "Logger.h"

namespace Yukimi
{

std::mutex Logger::sMutex;
std::ofstream Logger::sLogFile;

void Logger::Initialize()
{
    std::string filename = "Log/" + CurrentTime() + "_log.txt";
    sLogFile.open(filename);
}

void Logger::Finalize()
{
    sLogFile.close();
}

void Logger::Log(const LogLevel lv, const std::string& msg)
{
    std::lock_guard<std::mutex> lock(sMutex);

    std::string lvstr = ToString(lv);
    std::ostringstream oss;
    oss << "[" << CurrentTime() << "]" << "[" << lvstr << "] " << msg;
    std::cout << oss.str() << std::endl;

    if (sLogFile.is_open())
    {
        sLogFile << oss.str() << std::endl;
    }
}

std::string Logger::CurrentTime()
{
    // 現在時刻を取得
    auto now = std::chrono::system_clock::now();

    // time_t（秒単位）に変換
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    // ローカル時刻に変換（スレッドセーフに）
    std::tm localTime{};
    localtime_s(&localTime, &t);

    // 時刻を文字列に整形
    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d_%H-%M-%S");

    // std::string に変換して返す
    return oss.str();
}

std::string Logger::ToString(const LogLevel lv)
{
    switch (lv) {
    case LogLevel::Info: return "INFO";
    case LogLevel::Warning: return "WARN";
    case LogLevel::Error: return "ERROR";
    default: return "UNKNOWN";
    }
}

}
