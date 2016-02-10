
#include "log.h"
#include <iostream>

using namespace std;

Log::LOGLEVEL Log::_level = INFO;

static string ToString(Log::LOGLEVEL level)
{
    switch (level)
    {
    case Log::ERROR:
        return "Error";
    case Log::WARNING:
        return "Warning";
    case Log::INFO:
        return "Info";
    case Log::DEBUG:
        return "Debug";
    default:
        return "UnknownLogLevel";
    }
}

void Log::Message(const std::string &sText, LOGLEVEL level)
{
    if (level > _level) {
        return;
    }
    cerr << ToString(level) << ": " << sText << endl;
}
