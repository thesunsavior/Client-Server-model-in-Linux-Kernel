#include "admin.h"

#include <errno.h>
#include <string.h>

// return false if there is error, true otherwise
bool Admin::AdminUpdate()
{
    // check whether any process has died
    bool dead_server = false;
    for (int i = 0; i < client_list.size(); i++)
    {
        int status = -1;
        status = waitpid(client_list[i], nullptr, WNOHANG);

        // if process is already terminated
        if (status != 0)
        {
            // report dead server
            if (server_pid == client_list[i])
            {
                dead_server = true;
                this->logger.Log("Server is dead, reassigning..", BOTH);
                // std::cout << "Server is dead, reassigning...\n";
                sleep(2);
            }

            // delete from client list
            client_list.erase(client_list.begin() + i);
        }
    }

    if (server_pid == -1 || dead_server)
    {
        if (client_list.size() == 0)
        {
            this->logger.Log("All process are gone, exiting");
            // std::cerr << "All process are gone, exiting " << std::endl;
            exit(0);
        }

        AssignServer();

        // wait for server to confirm its init
        // bool result = ReceiveFromServer(server_pid);
        // if (!result)
        // {
        //     std::cerr << "Unexpected message to admin" << std::endl;
        //     return false;
        // }
        // else
        // {
        //     std::cout << "Admin receive server confirmation" << std::endl;
        // }
    }

    // confirm that client and server can now carry on with their task
    for (int i = 0; i < client_list.size(); i++)
    {
        if (client_list[i] == server_pid)
            continue;
        SendToClient(MessageType::GoodMorning, pipe_id[client_list[i]]);
    }

    return true;
}

void Admin::AssignServer()
{
    std::cout << "+++++++++++++++++++++++++++++++++ Assigning server... ++++++++++++++++++++++++++++++++++++++++" << std::endl;
    if (client_list.size() <= 0)
    {
        std::cerr << "List is currently empty!" << std::endl;
        return;
    }
    server_pid = client_list[0]; 
    SendToClient (MessageType::ServerAssign,pipe_id[server_pid]);
    sleep(2); // make sure client read it
    logger.Log("assignment sent to client " + std::to_string(server_pid), BOTH);
    // std::cout << "assignment sent to client" << std::endl;
}

void Admin::SendToClient(MessageType type, int pipe_id)
{
    sleep(2);
    char msg = '0' + type;
    write(pipefd[pipe_id][1], &msg, 1);
}

//blocking read
bool Admin::ReceiveFromServer(pid_t server_pid)
{
    char buf;
    std::cout << "Admin waiting for response... from " << server_pid << std::endl;
    sleep(2);
    read(pipefd[pipe_id[server_pid]][0], &buf, 1);
    std::cout << "Received response " << buf << std::endl;

    int msg = buf - '0';
    if (msg == MessageType::ServerInit)
        return true;
    else
        std::cerr << "Message number is " << msg << std::endl;

    return false; // any other type of message should not be send to Admin
}

// return false means creating pipe failed
bool Admin::CreateServerPipeForAll()
{
    for(int i = 0 ; i < number_of_process; i++) {
        std::string temp = "/home/thesunsavior/lab/OS_Proj/pipe/pipe_client" + std::to_string(i);
        char *PipeName = (char *)temp.c_str();
        // const char *PipeName = "Home/pipe/pipe_client" + i;
        int result = mkfifo(PipeName, 0666);
        if (result == -1)
        {
            logger.Log("Error making server pipes", CONSOLE, ERROR);
            logger.Log(strerror(errno), BOTH, ERROR);

            // std::cerr << "Error making server pipes" << std::endl;
            // std::cerr << strerror(errno) << std::endl;
            return false;
        }
        else
            // std::cout << "Successfully created pipe " << std::endl;
            logger.Log("Successfully created pipe ", CONSOLE);
    }
    return true ;
}

void Admin::AddClient(pid_t client_id)
{
    client_list.push_back(client_id);
}