#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512
#define NPACK 10

#define IO24_HOST "10.10.10.10"
#define IO24_PORT 2424
#define IO24_LISTEN_PORT 2425

int io24_sock;
struct sockaddr_in io24_addr;
int addr_len = sizeof(io24_addr);

int listen_sock;

void serial_open()
{
  int sock, i;
  char buf[BUFLEN];

  io24_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if ( io24_sock === -1)
  {
    fprintf(stderr, "Creating socket failed");
    exit(1);
  }

  memset((char *) &io24_addr, 0, sizeof(io24_addr));
  io24_addr.sin_family = AF_INET;
  io24_addr.sin_port = htons(IO24_PORT);
  if (inet_aton(IO24_HOST, &io24_addr.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }
}

void serial_read(char *data, int max)
{
}

void serial_write(char *data, int len)
{
  // ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
  if ( sendto(io24_sock, data, len, 0, io24_addr, addr_len) == -1 )
  {
    fprintf(stderr, "Error writing");
    exit(1);
  }
}

void serial_close()
{
  close(io24_sock);
  close(listen_sock);
}
