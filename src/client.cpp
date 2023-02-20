#include <string.h>
#include "client.h"

void Server::Update()
{
    std::cout << "======================================== SERVER UPDATE ===========================================" << std::endl;
    for (int i = 0; i < number_of_process; i++)
    {
        int fd;
        std::string temp = "/home/thesunsavior/lab/OS_Proj/pipe/pipe_client" + std::to_string(i);
        logger.Log("create pipe > " + temp);
        // std::cout << "create pipe > " << temp << std::endl;
        threads[i] = std::thread(&Server::BroadCastToClient, temp);
        threads[i].detach();
    }

    while (true)
        ;
}

void Server::BroadCastToClient(std::string pipe_file)
{
    char *pipe_name = (char *)pipe_file.c_str();

    std::cout << "Name check: " << pipe_name << "\n"; // name check  delete afterward

    int fd = open(pipe_name, O_WRONLY | O_NONBLOCK);
    if (fd == -1)
    {
        std::cerr << "-Wrong named pipe in thread-\n"
                  << strerror(errno)
                  << "\n ----------------------------"
                  << std::endl; // name check  delete afterward

        // stall thread
        while (true)
            usleep(1000);
    }
    while (true)
    {
        sleep(2);
        write(fd, "Hi", sizeof("Hi"));
    }
}

int Client::ReadFromAdmin()
{
    char buf;
    int result = read(admin_pipe[0], &buf, 1);
    if (result == -1)
    {
        logger.Log("read from admin failed", BOTH, ERROR);
        // std::cerr << "Client " << getpid() << " read from admin failed" << std::endl;
    }

    return buf - '0';
}

void Client::Update()
{

    char *message;
    message = ReadFromServer();

    logger.Log(" receieve a message: \" " + (std::string)message + "\"");

    std::cout << getpid() << " receieve a message: \" " << message << "\"" << std::endl;
}

char *Client::ReadFromServer()
{
    int result = read(fd, &buf, MAX_BUF);
    if (result == -1)
    {
        logger.Log("pipe is empty", CONSOLE, ERROR);

        std::cout << "pipe is empty" << std::endl;
    }
    else
    {
        logger.Log("Received msg", CONSOLE, ANNOUNCEMENT);

        // std::cout << "Received msg" << std::endl;
    }
    return buf;
}

void Client::SendToAdmin(MessageType type)
{
    char msg = '0' + type;
    logger.Log("Sending to admin...", BOTH, ANNOUNCEMENT);

    // std::cout << "Sending to admin..." << std::endl;
    write(admin_pipe[1], &msg, 1);
    logger.Log("Sent to admin...", BOTH, ANNOUNCEMENT);
    // std::cout << "Sent to admin" << std::endl;
}