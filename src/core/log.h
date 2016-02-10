
#ifndef LOG_H
#define LOG_H

#include <string>

// Log, version 0.1: a simple logging class
class Log
{
public:
    enum LOGLEVEL {ERROR, WARNING, INFO, DEBUG};
    static void Debug(const std::string& sText);
    static void Warning(const std::string& sText);
    static void Error(const std::string& sText);
    static void Info(const std::string& sText);
    static LOGLEVEL& SetLevel(LOGLEVEL level);

private:
    static void Message(const std::string& sText, LOGLEVEL level = INFO);
    Log(const Log&) = delete;
    Log& operator =(const Log&) = delete;

private:
    static LOGLEVEL _level;
};

inline void Log::Debug(const std::string& sText)
{
    return Message(sText, DEBUG);
}

inline void Log::Info(const std::string& sText)
{
    return Message(sText, INFO);
}

inline void Log::Warning(const std::string& sText)
{
    return Message(sText, WARNING);
}

inline void Log::Error(const std::string& sText)
{
    return Message(sText, ERROR);
}

#endif
