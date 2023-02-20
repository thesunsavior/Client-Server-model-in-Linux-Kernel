#ifndef SRC_CLIENT_H
#define SRC_CLIENT_H

#include <iostream>

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <thread>
#include <error.h>
#include <string.h>

#include "util/logger.h"
#include "message.h"

#define MAX_BUF 1024

class Client  {
public:
    int pipe_id;
    int *admin_pipe;
    int number_of_process;
    int fd;
    char buf[MAX_BUF];

    Logger logger = Logger(LogActor::CLIENT, LogType::CONSOLE, LogLevel::ANNOUNCEMENT);

    Client() = default;
    Client(int admin_pipe[2], int pipe_id,const int number_of_process){
        this->pipe_id = pipe_id;
        this->admin_pipe = admin_pipe;
        this->number_of_process = number_of_process;

        // connect to server pipe
        std::string temp = "/home/thesunsavior/lab/OS_Proj/pipe/pipe_client" + std::to_string(pipe_id);

        const char *named_pipe = (char *)temp.c_str();
        fd = open(named_pipe, O_RDONLY | O_NONBLOCK);

        if (fd == -1)
        {
            std::cerr << "-Client failed to open named pipe-\n"
                      << strerror(errno)
                      << "\n ----------------------------"
                      << std::endl; // name check  delete afterward
        }
        else
        {
            std::cout << "Client successfully opened named pipe" << std::endl;
        }
    }

    int ReadFromAdmin();
    char* ReadFromServer();
    void SendToAdmin(MessageType type);
    virtual void Update();
};

class Server : public Client{
public:
    int *fd;
    std::thread *threads;
    Logger logger = Logger(LogActor::SERVER, LogType::CONSOLE, LogLevel::ANNOUNCEMENT);

    Server() = default;
    Server(int admin_pipe[2], int pipe_id, const int number_of_process)
    {
        this->pipe_id = pipe_id;
        this->admin_pipe = admin_pipe;
        this->number_of_process = number_of_process;
        fd = new int [number_of_process];
        threads = new std::thread[number_of_process];
    }

    static void BroadCastToClient(std::string pipe_file);
    void Update();
};

#endif