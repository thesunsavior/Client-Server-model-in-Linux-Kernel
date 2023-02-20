#include <iostream>

#include "logger.h"
void Logger::UpdateLogFile()
{
    if (current_actor == LogActor::SERVER)
    {
        output_file.open("Server_log.txt", std::ofstream::app);
        return;
    }
    else if (current_actor == LogActor::CLIENT)
    {
        output_file.open("Clients_log.txt", std::ofstream::app);
        return;
    }
    else
    {
        output_file.open("Admin_log.txt", std::ofstream::app);
    }
}

void Logger::SetType(LogType type)
{
    this->current_type = type;
}

void Logger::SetActor(LogActor actor)
{
    this->current_actor = actor;
}

void Logger::SetLevel(LogLevel level)
{
    this->current_level = level;
}

void Logger::Log(std::string msg, LogType type, LogLevel level)
{
    this->SetType(type);
    this->SetLevel(level);

    if (current_type == LogType::OUTPUT_FILE || current_type == LogType::BOTH)
    {
        UpdateLogFile();
    }

    std::string log_string = "";
    switch (current_level)
    {
    case DEBUG:
        log_string += ">>>DEBUG: ";
        break;
    case ANNOUNCEMENT:
        log_string += "======== ANNOUNCEMENT: ";
        break;
    case ERROR:
        log_string += "+++++ ERROR: ";
    }

    switch (current_actor)
    {
    case ADMIN:
        log_string += "ADMIN-";
        break;
    case CLIENT:
        log_string += "CLient " + std::to_string(getpid()) + "-";
        break;
    case SERVER:
        log_string += "SERVER " + std::to_string(getpid()) + "-";
    }

    if (current_type == CONSOLE || current_type == BOTH)
    {
        std::cout << log_string + msg + "\n";
    }

    if (current_type == OUTPUT_FILE || current_type == BOTH)
    {
        output_file << log_string + msg + "\n";
        output_file.close();
    }
}