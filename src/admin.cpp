#include "admin.h"

// return false if there is error, true otherwise 
bool AdminUpdate () {
    // check whether any process has died 
    bool dead_server = false;
    for (int i = 0; i < client_list.size(); i++) {
        int status= -1; 
        status =waitpid (client_list[i],nullptr,WNOHANG);
        
        // if process is already terminated 
        if (status != 0) {
            // report dead server 
            if (server_pid == client_list[i])
                dead_server = true ;
            
            // delete from client list 
            client_list.erase(client_list.begin()+i);
        }
    }

    if (server_pid == -1 || dead_server) {
        if (client_list.size() == 0)
            return false;

        AssignServer();
        // wait for server to confirm its init
        bool result = ReceiveFromServer (server_pid);
        if (!result) {
            std::cerr <<"Unexpected message to admin";
            return false;
        }
    }

    //confirm that client and server can now carry on with their task
    for(int i = 0; i < client_list.size(); i++) {
        if (client_list[i] == server_pid)
            continue;
        SendToClient (MessageType::GoodMorning, pipe_id[client_list[i]]);    
    }

    return true;
} 

void AssignServer()  {
    if(client_list.size() <= 0 )
        return;
    
    server_pid = client_list[0]; 
    SendToClient (MessageType::ServerAssign,pipe_id[server_pid]);
}

void SendToClient (MessageType type, int pipe_id) {
     char msg ='0'+ MessageType::ServerAssign; 
     write(pipefd[pipe_id][1], &msg, 1);
}

//blocking read
bool ReceiveFromServer(pid_t server_pid) {
     char buf;
     read(pipefd[pipe_id[server_pid]][0], &buf, 1);
     
     int msg = buf-'0';
     if (msg == MessageType::ServerInit)
        return true;

    return false; // any other type of message should not be send to Admin
} 

// return false means creating pipe failed
bool CreateServerPipeForAll() {
    for(int i = 0 ; i < number_of_process; i++) {
       const char* PipeName = "/pipe/pipe_client"+ i;
       int result = mkfifo(PipeName,0666); 
       if (result == -1) {
            std::cerr <<"Error making server pipes"<< std::endl;
            return false ;
       }
    }
    return true ;
}