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
            ad.logger.Log("Error creating pipe", BOTH, ERROR);
            return 0;
        }

        id = i;
        pid_holder = fork();
        if (pid_holder == -1)
        {
            ad.logger.Log("Error creating child process", BOTH, ERROR);
            return 0;
         }
         else if (pid_holder == 0){
            break;
         } 
         else {
            ad.pipe_id[pid_holder] = i;
            ad.AddClient(pid_holder);
            ad.logger.Log("child pid " + std::to_string(pid_holder) + " was created.", BOTH);

            // std::cout << "child pid " << pid_holder << " was created." << std::endl;
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
         host->logger.Log("I am the child " + std::to_string(getpid()), BOTH);
         //  std::cout << "I am the child " << getpid() << std::endl;

         while (!IsServer)
         {

            // read from server
            int msg;
            msg = host->ReadFromAdmin();
            sleep(2);
            if (msg == MessageType::ServerAssign && !IsServer)
            {
                host->logger.Log("Received server assignment", BOTH);
                std::cout << getpid() << " has receive server assignment" << std::endl;
                delete host;
                host = new Server(ad.pipefd[id], id, ad.number_of_process);

                // host->SendToAdmin(MessageType::ServerInit);

                // msg = host->ReadFromAdmin();

                IsServer = true;
                break;
            }

            if (msg != MessageType::GoodMorning)
            {
                host->logger.Log("Expected GoodMorning receive some different order", CONSOLE, ERROR);

                // std::cerr << "Expected GoodMorning receive some different order \n";
                continue; // spin until server is assigned
            }
            else
            {
                host->logger.Log("Client received good morning!", CONSOLE);

                // std::cout << "Client received good morning!"
                //           << std::endl;
            }

            host->Update();
         }

         host->Update();
    }
}    
 