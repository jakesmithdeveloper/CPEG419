/* tcpserver.c */
/* Programmed by Adarsh Sethi */
/* Sept. 13, 2018 */    

#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, bind, listen, accept */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024   

/* SERV_TCP_PORT is the port number on which the server listens for
   incoming requests from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_TCP_PORT 58803 //65000

//defining the struct for the bank accounts
struct Account {    
    int balance;
    int prevBalance;
};   //account 

//function declarations
int checkConditions(char *msg, int msg_len,struct Account c, struct Account s);


int main(void) {

    struct Account checking;
    checking.balance = 0;
    struct Account savings;
    savings.balance = 0;


   int sock_server;  /* Socket on which server listens to clients */
   int sock_connection;  /* Socket on which server exchanges data with client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned int server_addr_len;  /* Length of server address structure */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   char sentence[STRING_SIZE];  /* receive message */
   char response[STRING_SIZE]; /* send message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */

   int cond;

   /* open a socket */

   if ((sock_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("Server: can't open stream socket");
      exit(1);                                                
   }

   /* initialize server address information */
    
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */ 
   server_port = SERV_TCP_PORT; /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   if (bind(sock_server, (struct sockaddr *) &server_addr,
                                    sizeof (server_addr)) < 0) {
      perror("Server: can't bind to local address");
      close(sock_server);
      exit(1);
   }                     

   /* listen for incoming requests from clients */

   if (listen(sock_server, 50) < 0) {    /* 50 is the max number of pending */
      perror("Server: error on listen"); /* requests that will be queued */
      close(sock_server);
      exit(1);
   }
   printf("I am here to listen ... on port %hu\n\n", server_port);
  
   client_addr_len = sizeof (client_addr);

   /* wait for incoming connection requests in an indefinite loop */

   for (;;) {

      sock_connection = accept(sock_server, (struct sockaddr *) &client_addr, 
                                         &client_addr_len);
                     /* The accept function blocks the server until a
                        connection request comes from a client */
      if (sock_connection < 0) {
         perror("Server: accept() error\n"); 
         close(sock_server);
         exit(1);
      }
 
      /* receive the message */

      bytes_recd = recv(sock_connection, sentence, STRING_SIZE, 0);

//******************************************************************************************************************************
    if (bytes_recd > 0){
        printf("Received Sentence is:\n");
        printf("%s", sentence);
        printf("\nwith length %d\n\n", bytes_recd);
        msg_len = bytes_recd;
        
        //check to ensure that the request conforms to the given conditions.
        cond = checkConditions(sentence,msg_len,checking,savings);
        printf("The check condition result is %d\n",cond);
        
        switch (cond){
            case 1:
                strcpy(response,"You may not withdrawl from a savings account");
                msg_len = strlen(response);
                break;

            case 2:
                strcpy(response,"You may only withdrawl in multiples of 20");
                msg_len = strlen(response);                
                break;

            case 3:
                strcpy(response,"Your request exceeds the balance in your checking account");
                msg_len = strlen(response);
                break;

            case 4:
                strcpy(response,"Insufficient funds to process your transfer");
                msg_len = strlen(response);
                break;
            default:
                strcpy(response,"ok");
                msg_len = strlen(response);
                
        }//switch | formulating the response if the request fails any of the conditions.

         /* send message */
         bytes_sent = send(sock_connection, response, msg_len, 0);
      }

      /* close the socket */

      close(sock_connection);
   } 
}

int checkConditions(char *msg,int msg_len, struct Account c, struct Account s){

    int condition = 0;
    char amountStr[STRING_SIZE];
    int amountInt;
    char op;
    char act;

    for (int i=2; i<msg_len; i++){
        amountStr[i-2] = msg[i];
    }//for | pull out substring that holds amount

    op=msg[0];      //pull operation char
    act=msg[1];     //pull account char

    sscanf(amountStr, "%d", &amountInt);        //convert amount string to int
    printf("%d\n",amountInt);
    
    
    if ((op=='w') && (act=='s')){
        condition = 1;
    }//if | withdrawl from saving account
    
    else if ((op=='w') && (act=='c')){
        if (amountInt%20 != 0){
            condition = 2;
        }//if | Check if multiple of 20

        else if (amountInt > c.balance){
            condition = 3;
        }//esle if | check if exceeds balance

    }//else if | examine conditions for withdrawl from checking
    
    else if ((op=='t') && (act=='s') && (amountInt>s.balance)){
        condition = 4;
    }//else if | transfer condition
    
    else if ((op=='t') && (act=='c') && (amountInt>c.balance)){
        condition = 4;
    }//else if | transfer condition
    
    return condition;
}//checkConditions | function to verify that client request is valid

char *getResponseAndUpdateAccounts(char op, char act, int amt, struct Account c, struct Account s){
    
    



    static char resp[2];
    return resp; 
    }