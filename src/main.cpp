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

    Admin ad;

    // creating child process
    for (int i = 0; i < ad.number_of_process; i++)
    {
        // create a pipe for each client
        if (pipe(ad.pipefd[i]) == -1)
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
            ad.pipe_id[pid_holder] = i;
            ad.AddClient(pid_holder);
            std::cout << "child pid " << pid_holder << " was created." << std::endl;
         }
    }

    // 0 is read end   
    // 1 is write end 
    if (pid_holder > 0) {
         ad.CreateServerPipeForAll();
         while (true)
            ad.AdminUpdate();
    }else 
    if (pid_holder == 0) {
         Client *host = new Client(ad.pipefd[id], id, ad.number_of_process);
         std::cout << "I am the child " << getpid() << std::endl;

         while (!IsServer)
         {

            // read from server
            int msg;
            msg = host->ReadFromAdmin();
            if (msg == MessageType::ServerAssign && !IsServer)
            {
                std::cout << getpid() << " has receive server assignment" << std::endl;
                delete host;
                host = new Server(ad.pipefd[id], id, ad.number_of_process);

                // host->SendToAdmin(MessageType::ServerInit);

                // msg = host->ReadFromAdmin();

                IsServer = true;
                std::cout << "i want too break freee" << std::endl;
                break;
            }

            if (msg != MessageType::GoodMorning)
            {
                std::cerr << "Expected GoodMorning receive some different order \n";
                continue; // spin until server is assigned
            }
            else
            {
                std::cout << "Client received good morning!"
                          << std::endl;
            }

            host->Update();
         }

         host->Update();
    }
}    
 