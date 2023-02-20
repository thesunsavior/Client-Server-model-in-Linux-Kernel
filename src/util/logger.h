#ifndef SRC_UTIL_LOGGER_H
#define SRC_UTIL_LOGGER_H
#include <fstream>
#include <unistd.h>

enum LogType
{
    CONSOLE,
    OUTPUT_FILE,
    BOTH,
};

enum LogActor
{
    ADMIN,
    SERVER,
    CLIENT,
};

enum LogLevel
{
    ERROR,
    DEBUG,
    ANNOUNCEMENT,
};

class Logger
{
public:
    Logger(LogActor actor, LogType type, LogLevel level)
    {
        this->current_actor = actor;
        this->current_level = level;
        this->current_type = type;
    }

    void UpdateLogFile();
    void SetType(LogType type);
    void SetActor(LogActor actor);
    void SetLevel(LogLevel level);
    void Log(std::string msg, LogType type = CONSOLE, LogLevel level = ANNOUNCEMENT);

private:
    std::ofstream output_file;
    LogActor current_actor;
    LogType current_type;
    LogLevel current_level;
};

#endif