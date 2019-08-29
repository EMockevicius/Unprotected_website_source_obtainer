#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <errno.h>
#include <netdb.h>
#include <locale.h>

#define MAXBUF 1024
#define PORT 80
int main(int Count, char *Strings[])
{
int sockfd, bytes_read;

struct sockaddr_in dest;
struct hostent *he;

char buffer[MAXBUF];
FILE *tinklapis;
tinklapis = fopen("tinklapis.html","a+");
setlocale(LC_ALL,NULL);

if ( Count < 1 || Count > 2 )

{

fprintf(stderr, "usage: %s <IP-addr>\n", Strings[0]);

}
if((he=gethostbyname(Strings[1])) == NULL)

{

perror("gethostbyname()");

}
else
printf("Klientas - Tinklapis: %s\n", Strings[1]);

//sukuriamas socket'as transliavimui
if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
{
perror("Socket");
}
printf("Klientas - The socket() sockfd is OK...\n");

//Inicijuojamas serverio adresas ir portas
dest.sin_family = AF_INET;
dest.sin_addr = *((struct in_addr *)he->h_addr);
dest.sin_port = htons(PORT);
printf("Serveris - naudojamas tinklapis %s ir prievadas %d...\n", Strings[1], PORT);
memset(&(dest.sin_zero), '\0', 8);

//Jungiamasi prie serverio
if ( connect(sockfd, (struct sockaddr *)&dest, sizeof(struct sockaddr)) < 0 )
{
printf("Prisijungimas nepavyko\n");
}
else
printf("Prisijungimas pavyko\n");
sprintf(buffer, "GET / HTTP/1.0 \nHost: %s\n\n",Strings[1]);
send(sockfd, buffer, strlen(buffer), 0);

//vykdoma jei pavyko prisijungimas, nuskaitoma ir irasoma
do
{
bzero(buffer, MAXBUF);
bytes_read = recv(sockfd, buffer, MAXBUF, 0);
if ( bytes_read > 0 )

fprintf(tinklapis,"%s",buffer); //irasoma
}
while ( bytes_read > 0 );
printf("Tinklapis issaugotas tinklapis.html faile\n");
fclose(tinklapis);

// isvaloma-isjungiama
close(sockfd);
return 0;
}
