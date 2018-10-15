/* tcp_ client.c */ 
/* Programmed by Adarsh Sethi */
/* Sept. 13, 2018 */     

#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset, memcpy, and strlen */
#include <netdb.h>          /* for struct hostent and gethostbyname */
#include <sys/socket.h>     /* for socket, connect, send, and recv */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024

int main(void) {

   int sock_client;  /* Socket used by client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE]; /* Server's hostname */
   unsigned short server_port;  /* Port number used by server (remote port) */

   char sentence[STRING_SIZE];  /* send message */
   char operation[STRING_SIZE];
   char account[STRING_SIZE];
   char amount[STRING_SIZE];
   char modifiedSentence[STRING_SIZE]; /* receive message */
   unsigned int msg_len;  /* length of message */                      
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
  
   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("Client: can't open stream socket");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information 
            unless you want to specify a specific local port
            (in which case, do it the same way as in udpclient.c).
            The local address initialization and binding is done automatically
            when the connect function is called later, if the socket has not
            already been bound. */

   /* initialize server address information */

   printf("Enter hostname of server: ");
   scanf("%s", server_hostname);
   if ((server_hp = gethostbyname(server_hostname)) == NULL) {
      perror("Client: invalid server hostname");
      close(sock_client);
      exit(1);
   }

   printf("Enter port number for server: ");
   scanf("%hu", &server_port);

   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
                                    server_hp->h_length);
   server_addr.sin_port = htons(server_port);

    /* connect to the server */
 		
   if (connect(sock_client, (struct sockaddr *) &server_addr, 
                                    sizeof (server_addr)) < 0) {
      perror("Client: can't connect to server");
      close(sock_client);
      exit(1);
   }
  
   /* user interface */
    // while(1){
        printf("\nWelcome, what can we do for you to today?\n");
        printf("\nCheck my balance (b).\n");
        printf("Make a deposit (d).\n");
        printf("Make a withdrawl (w).\n");
        printf("Perform a transfer (t).\n");
        scanf("%s",operation);

            if (strcmp(operation, "t")==0){
                    printf("Enter FROM account ((c)hecking or (s)avings).\n");
                    scanf("%s",account);
            }//check for transfer
            
            else if ((strcmp(operation, "w")==0) ||(strcmp(operation, "d")==0)||(strcmp(operation, "b")==0)){
                    printf("Enter account ((c)hecking or (s)avings).\n");
                    scanf("%s",account);

            }//other valid responses
            
            if  ((strcmp(operation, "w")==0) ||(strcmp(operation, "d")==0)||(strcmp(operation, "t")==0)){
                    printf("Enter amount\n");
                    scanf("%s",amount);
                    char tmp[STRING_SIZE];
                    int len = 7-strlen(amount);
                    printf("amount len:%d\n",len);
                    for (int i = 0; i < len; i++){
                        strcpy(tmp,"0");
                        strcat(tmp,amount);
                        strcpy(amount,tmp);
                    }    
            }//if
            else{
                strcpy(amount,"0");
            }
            
            strcpy(sentence,operation);
            strcat(sentence,account);
            strcat(sentence,amount);
            

        //    scanf("%s", sentence);
        //    msg_len = strlen(sentence) + 1;


        msg_len = strlen(sentence) + 1;
        /* send message */
        
        bytes_sent = send(sock_client, sentence, msg_len, 0);

        /* get response from server */
        
        bytes_recd = recv(sock_client, modifiedSentence, STRING_SIZE, 0); 

        printf("\nThe response from server is:\n");
        printf("%s\n\n", modifiedSentence);

        /* close the socket */
// }//while loop
   close (sock_client);
}




// char *operationConvert(char o){
//     char op[STRING_SIZE];
//     if (o == 'w'){
//         op = "00";
//     }

// }//operationConvert