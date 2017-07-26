#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

void sig_handler(int signal) {
  printf ("get signal SIGPIPE!\n");
}

int main (void) {
  //signal (SIGPIPE, sig_handler);
  int sock = socket (AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror ("socket failed");
    return -1;
  }
  struct sockaddr_in serveraddr;
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(9876);
  inet_pton (AF_INET, "127.0.0.1", &serveraddr.sin_addr);
  int ret = connect (sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
  if (ret < 0) {
    perror ("connect failed");
    return -1;
  }
  int keepAlive = 1;
  setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive));
  fd_set rset;
  int std_flag = 1, sock_flag = 1;
  while (1) {
    FD_ZERO(&rset);
    if (std_flag == 1) {
      FD_SET(0,&rset);
    }
    if (sock_flag == 1) {
      FD_SET(sock,&rset);
    }
    if (std_flag + sock_flag == 0) {
      break;
    }
    struct timeval tm = {2,0};
    ret = select (sock+1, &rset, NULL, NULL, &tm);
    if (ret < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        perror ("select failed");
        return -1;
      }
    }
    if (ret == 0) {
      //printf ("select return 0\n");
      continue;
    }
    if (FD_ISSET(0,&rset)) {
      char line[1024];
      int n = read (0, line, 1024);
      if (n <= 0) {
        std_flag = 0;
        if (n < 0) {
          perror ("read user input failed!");
        }
        shutdown(sock,SHUT_WR);// 这里要加才生效
        continue;
      }
      if (sock_flag == 0) {
        printf ("server is gone! go over \n");
        break;
      }
      int left = n, pos = 0;
      while (left > 0) {
        n = write (sock, line+pos, left);
        if (n < 0) {
          perror ("write to sock failed!");
          break;
        }
        pos += n;
        left -= n;
      }
    }
    if (FD_ISSET(sock, &rset)) {
      char line[1024];
      int n = read (sock, line, 1024);
      if (n <= 0) {
        sock_flag = 0;
        if (n < 0) {
          perror ("read net message failed!");
        } else {
          printf ("server is gone!\n");
          close(sock);
        }
        continue;
      }
      write (1, line, n);
    }
  }
  close (sock);
  return 0;
}
