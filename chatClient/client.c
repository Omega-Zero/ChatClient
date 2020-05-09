#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>  

void readLine(char line[]);


int main(){

	unsigned int port; 
	char IP[15];
	char username[21]; 
	int usernamesize = 0;

	printf("Enter IP: ");
	scanf("%s", IP);

	printf("Enter port: ");	
	scanf("%u", &port);

	printf("Enter user name: ");
	scanf("%s", username);
	usernamesize = strlen(username);
	
	



	//create socket()
	int sockFD= -1;
	sockFD= socket(AF_INET, SOCK_STREAM, 0);

	//sockaddr struct
	struct sockaddr_in address;	
	memset(&address, 0, sizeof(address)); 
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, IP, &( address.sin_addr));

	//see if able to connect to server
	if(connect(sockFD, (struct sockaddr*)&address, sizeof(address)) != -1){//connects successfully
		
		printf("\n*Chat Started*\n\n");

		int session = 1;
		getchar();
		//chat session active
		while(session == 1){
			char message[522];		
			char line[500];
			
			//clear arrays
			memset(message, '\0', 522*sizeof(message[0]));
			memset(line, '\0', 500*sizeof(line[0]));

			//start accepting messages
			printf("%s: ", username);
			readLine(line);
		
			//check if user wishes to quit
			if(!strcmp(line, "quit")){
				session = 0;
				close(sockFD);
				strcpy(message, line);//put msg in send without username
				send(sockFD, message, strlen(message)+1, 0);
				printf("\n*Chat Ended*\n");
				return 0;
			}


			strcpy(message, username);//put username into beginning of message to be sent
			message[usernamesize] = ':'; //formatting
			message[usernamesize+1] = ' ';
			strcat(message, line);	//fill in message	
			
			//send message to server
			send(sockFD, message, strlen(message)+1, 0);
		}

		close(sockFD);

	}else{//doesnt connect to server
		close(sockFD);
		printf("Unable to connect to server\n");
		return 0;
	}


	

	
	return 0;
}



//helper to get the input from user
void readLine(char line[])
{	
	int c = 0;	
	int i = 0;

	while( (c = getchar()) != '\n' )
	{
		line[i] = c;
		i++;
	}

	line[i] = '\0';
}

