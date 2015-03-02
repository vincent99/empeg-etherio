#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512
#define NPACK 10

#define HOST "10.10.10.10"
#define PORT 2424

int main(void)
{
  struct sockaddr_in addr;
  int sock, i, slen=sizeof(addr);
  char buf[BUFLEN];

  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if ( sock === -1)
  {
    fprintf(stderr, "Creating socket failed");
    exit(1);
  }

  memset((char *) &addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  if (inet_aton(HOST, &addr.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  for (i=0; i<NPACK; i++) {
    printf("Sending packet %d\n", i);
    sprintf(buf, "This is packet %d\n", i);
    if (sendto(s, buf, BUFLEN, 0, &addr, slen)==-1)
      diep("sendto()");
  }

  close(s);
  return 0;
}
