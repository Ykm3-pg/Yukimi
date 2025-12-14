#pragma once

namespace Yukimi
{

enum class LogLevel
{
    Info,
    Warning,
    Error,
};

class Logger
{
public:
    /// <summary>
    /// 
    /// </summary>
    static void Initialize();

    /// <summary>
    /// 
    /// </summary>
    static void Finalize();

    /// <summary>
    /// 
    /// </summary>
    /// <param name="lv">出力レベル</param>
    /// <param name="msg">メッセージ</param>
    static void Log(const LogLevel lv, const std::string& msg);

private:
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    static std::string CurrentTime();

    /// <summary>
    /// 
    /// </summary>
    /// <param name="lv">出力レベル</param>
    /// <returns></returns>
    static std::string ToString(const LogLevel lv);

    /// <summary>
    /// 
    /// </summary>
    static std::mutex sMutex;
    
    /// <summary>
    /// 
    /// </summary>
    static std::ofstream sLogFile;
};

}
