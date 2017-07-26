#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

#define FORT_TYPE 0
#define SELECT_TYPE 1

void sig_handler(int signal) {
  while (waitpid (-1, NULL, 0) > 0)
    ; 
}

int setnonblock(int fd, int type) {
  int flags = fcntl(fd,F_GETFL,0);
  if (flags < 0) {
    perror ("get socket flags failed!");
    return -1;
  }
  if (type == 1) {
    flags |= O_NONBLOCK;
  } else {
    flags &= ~O_NONBLOCK;
  }
  if (fcntl(fd,F_SETFL, flags) < 0) {
    printf ("set socket O_NONBLOCK failed!");
    return -1;
  }
  return 0;
}

int main (void) {
  int listen_fd = socket (AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0) {
    printf ("create listen socket failed!\n");
    return -1;
  }
  struct sockaddr_in serveraddr;
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(9876);
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  int ret = 0;
  ret = bind (listen_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
  if (ret < 0) {
    perror ("bind socket failed!");
    return -1;
  }
  // 设置keepalive
  int keepAlive = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive));
  #if SELECT_TYPE
  /* 设置非阻塞模式  原因：
     1.如果不阻塞accept可能会出问题,客户端连接了之后马上发送RST,通过设置SO_LINGER;
       源自berkeley的实现不会报错,这样调用accept就会阻塞;svr4实现accept则会报ECONNABORTED
     2.后面程序中一般都是循环读取数据，设置成非阻塞才可以起到作用
  */
  if (setnonblock(listen_fd, 1) != 0) {
    return -1;
  }
  #endif
  ret = listen(listen_fd, 10);
  if (ret < 0) {
    perror ("listen failed!");
    return -1;
  }
  #if FORT_TYPE
  signal(SIGCHLD, sig_handler);
  #endif
  #if SELECT_TYPE
  int maxfd = listen_fd;
  fd_set rset;
  int client_fd[FD_SETSIZE] = {0};
  client_fd[listen_fd] = 1;
  #endif
  while (1) {
    #if FORT_TYPE // fork 子进程方式
    int client_fd = accept (listen_fd, NULL, NULL);
    if (client_fd < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        perror ("accept failed!");
        return -1;
      }
    }
    pid_t pid = fork();
    if (pid < 0) {
      perror ("fork failed!");
      return -1;
    }
    if (pid == 0) {
      close (listen_fd);
      char line[1024];
child_read:
      while ((ret = read (client_fd, line, 1024)) > 0) {
        int left = ret, pos = 0;
        while (left > 0) {
          int n = write (client_fd, line + pos, left);
          if (n < 0) {
            if (errno != EINTR) {
              perror ("write failed!");
            }
            break;
          }
          left -= n;
          pos += n;
        } 
      }
      if (ret < 0) {
       if (errno == EINTR) {
         goto child_read;
       } else {
         perror ("read failed!");
         exit(-1);
       }
      }
      return 0; 
    } else {
      close (client_fd);
    }
    #endif
    #if SELECT_TYPE // select IO复用方式
    FD_ZERO(&rset);
    int i = 0;
    for (; i < maxfd + 1; ++i) {
      if (client_fd[i] == 1) {
        FD_SET(i, &rset);
      }
    }
    struct timeval tm = {2,0};
    int ret = select (maxfd+1, &rset, NULL, NULL, &tm);
    if (ret < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        perror ("select failed!");
        return -1;
      }
    }
    if (ret == 0) {
      //printf ("doesnot have read fd\n");
      continue;
    }
    if (FD_ISSET(listen_fd, &rset)) {
      int fd = accept (listen_fd, NULL, NULL);
      if (fd < 0) {
        if (errno == EINTR || errno == EWOULDBLOCK || errno == ECONNABORTED || errno == EPROTO) {
          continue;
        } else {
          perror ("accept failed!");
          return -1;
        }
      }
      if (setnonblock(fd,1)) {
        printf ("setnonblock fd: %d failed!\n", fd);
      }
      client_fd[fd] = 1;
      if (fd > maxfd) {
        maxfd = fd;
      }
    }
    for (i = 0; i < maxfd + 1; ++i) {
      if (client_fd[i] != 1 || i == listen_fd)
        continue;
      if (FD_ISSET(i, &rset)) {
        char line[1024];
        while (1) {
          ret = read (i, line, 1024);
          if (ret < 0) {
            if (errno == EINTR) {
              continue;
            } else if (errno == EWOULDBLOCK) {
              printf ("wouldblock\n");
              break;
            } else {
              perror ("read client failed!");
              close (i);
              client_fd[i] = 0;
            }
          }
          if (ret == 0) {
            printf ("client exits, so close the socket\n");
            close (i);
            client_fd[i] = 0;
          }
          if (client_fd[i] == 0) {
            break;
          }
          int left = ret, pos = 0;
          while (left > 0) {
            int n = write (i, line + pos, left);
            if (n <= 0) {
              if (errno != EINTR) {
                perror ("write failed!");
              }
              break;
            }
            left -= n;
            pos += n;
          }
        }
      }
    }
    #endif
  }
  return 0;
}

