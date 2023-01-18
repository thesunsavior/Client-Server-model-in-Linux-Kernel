#ifndef SRC_CLIENT_H
#define SRC_CLIENT_H

#include <iostream>

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <thread>

#include "message.h"

class Client  {
public:
    int pipe_id;
    int *admin_pipe;
    int number_of_process;
    int fd; 

    Client() = default;
    Client(int admin_pipe[2], int pipe_id,const int number_of_process){
        this->pipe_id = pipe_id;
        this->admin_pipe = admin_pipe;
        this->number_of_process = number_of_process;

        // connect to server pipe
        const char *named_pipe = "/pipe/pipe_client" + pipe_id;
        fd = open(named_pipe, O_RDONLY);
    }

    int ReadFromAdmin();
    char* ReadFromServer();
    void SendToAdmin(MessageType type); 
    void Update();
};

class Server : public Client{
public:
    int *fd;
    std::thread *threads;

    Server() = default;
    Server(int admin_pipe[2], int pipe_id, const int number_of_process)
    {
        this->pipe_id = pipe_id;
        this->admin_pipe = admin_pipe;
        this->number_of_process = number_of_process;
        fd = new int [number_of_process];
        threads = new std::thread[number_of_process];
    }

    static void BroadCastToClient(char *pipe_file);
    void Update();
};

#endif