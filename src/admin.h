#include <iostream>
#include <fcntl.h>
#include <bits/stdc++.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "message.h"
#include "client.h"

const int number_of_process = 3; // @todo: change this to terminal input
int pipefd[number_of_process][2];

std::vector<pid_t> client_list;
std::map <pid_t,int> pipe_id; 
pid_t server_pid = -1;

void AssignServer();
void SendToClient(MessageType type,int pipe_id);
bool ReceiveFromServer(pid_t server); 
bool AdminUpdate();

bool CreateServerPipeForAll(); 

