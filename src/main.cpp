#include <iostream>
#include <string>

#include <bits/stdc++.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "admin.h"

pid_t pid_holder;
bool IsServer = false;
int id=-1; 
int main () {


    // creating child process
    for (int i = 0; i < number_of_process; i++) {
        // create a pipe for each client 
        if (pipe(pipefd[i]) == -1)
        {
            std::cerr << "Error creating pipe";
            return 0;
        }

        id = i;
        pid_holder = fork();
        if (pid_holder == -1)
        {
            std::cerr <<"Error creating child process";
            return 0;
         }
         else if (pid_holder == 0){
            break;
         } 
         else {
            pipe_id[pid_holder] = i;
            std::cout << "child pid " << pid_holder << " was created." << std::endl;
         }
    }

    // 0 is read end   
    // 1 is write end 
    if (pid_holder > 0) {
         CreateServerPipeForAll();
        while(true)
            AdminUpdate();
    }else 
    if (pid_holder == 0) {
        Client *host = new Client(pipefd[id],id,number_of_process);

        while (!IsServer) {

            // read from server
            int msg;
            msg = host->ReadFromAdmin();
            if (msg == MessageType::ServerAssign && !IsServer)
            {
                delete host;
                host = new Server(pipefd[id], id, number_of_process);

                host->SendToAdmin(MessageType::ServerInit);

                msg = host->ReadFromAdmin();

                IsServer = true;
                break;
            }

            if (msg != MessageType::GoodMorning)
            {
                std::cerr << "Expected GoodMorning receive some different order" << std::endl;
                exit(0);
            }

            host->Update();
        }

    }
}    
 