#ifndef SRC_ADMIN_H
#define SRC_ADMIN_H
#include <iostream>
#include <fcntl.h>
#include <bits/stdc++.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "util/logger.h"
#include "message.h"
#include "client.h"

class Admin
{
public:
    static const int number_of_process = 3; // @todo: change this to terminal input
    int pipefd[number_of_process][2];

    std::vector<pid_t> client_list;
    std::map<pid_t, int> pipe_id;
    pid_t server_pid = -1;
    Logger logger = Logger(LogActor::ADMIN, LogType::CONSOLE, LogLevel::ANNOUNCEMENT);

    void AssignServer();
    void SendToClient(MessageType type, int pipe_id);
    bool ReceiveFromServer(pid_t server);
    bool AdminUpdate();
    void AddClient(pid_t client_pid);

    bool CreateServerPipeForAll();
};

#endif