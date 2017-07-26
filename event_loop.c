// event loop
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

#define NONE_EVENT 0X00
#define READ_EVENT 0x01
#define WRIT_EVENT 0X02
#define EXCP_EVENT 0X04

typedef struct event event;
struct event{
  int fd;
  int mask; // 注册事件
  int (*rfunc) (int fd);
  int (*wfunc) (int fd, char *data);
  char *wdata; // 保存写数据
  struct event *next;
};

typedef struct event_loop event_loop;
struct event_loop {
  struct event *reg_event;
  int stop;
};

static event_loop *ev_loop = NULL;

event_loop *init_event_loop() {
  event_loop *ev_loop = (event_loop *) malloc (sizeof(event_loop));
  if (ev_loop != NULL) {
    ev_loop->reg_event = NULL;
  }
  ev_loop->stop = 0;
  return ev_loop;
}

int addevent(event_loop *ev_loop, event *e) {
  e->next = ev_loop->reg_event;
  ev_loop->reg_event = e;
  return 0;
}

event* delevent(event_loop *ev_loop, int fd, int mask) {
  event *p, *q;
  p = q = ev_loop->reg_event;
  while (p && p->fd != fd) {
    q = p;
    p = p->next;
  }
  if (p != NULL) {
    p->mask = mask;
    if (p->mask == NONE_EVENT) {
      if (p == ev_loop->reg_event) {
        ev_loop->reg_event = ev_loop->reg_event->next;
      } else {
        q->next = p->next;
      }
      free (p);
    }
    p = q->next;
  }
  return p;
}

void loop_run (event_loop *ev_loop) {
  while (!ev_loop->stop) {
    fd_set rset, wset;
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    int maxfd = 0;
    event *p = ev_loop->reg_event;
    while (p) {
      if (p->fd > maxfd)
        maxfd = p->fd;
      if (p->mask & READ_EVENT)
        FD_SET(p->fd, &rset);
      if (p->mask & WRIT_EVENT)
        FD_SET(p->fd, &wset);
      p = p->next;
    }
    int ret = select (maxfd + 1, &rset, &wset, NULL, NULL);
    if (ret < 0) {
      if (errno == EINTR)
        continue;
      else {
        printf ("select failed!\n");
      }
    }
    if (ret == 0) {
      continue;
    }
    p = ev_loop->reg_event;
    while (p) {
      int ret = 0;
      int mask = p->mask;
      if (FD_ISSET(p->fd, &rset) && p->rfunc != NULL) {
        ret = p->rfunc (p->fd);
        if (ret == 1) {
          mask &= ~READ_EVENT;
          if (p->wdata == NULL)
            mask &= ~WRIT_EVENT;
        }
      }
      if (FD_ISSET(p->fd, &wset) && p->wfunc != NULL &&
        p->wdata != NULL) {
        p->wfunc (p->fd, p->wdata);
        free (p->wdata);
        p->wdata = NULL;
        if ((mask & READ_EVENT) == NONE_EVENT)
          mask &= ~WRIT_EVENT;
      }
      if (mask != p->mask) {
        if (mask == NONE_EVENT)
          close (p->fd);
        p = delevent(ev_loop, p->fd, mask);
      } else {
        p = p->next;
      }
    }
  }
  return;
}

void set_write_data (int fd, char *data) {
  event *p = ev_loop->reg_event;
  while (p) {
    if (p->fd == fd)
      break;
    p = p->next;
  }
  if (p) {
    int len = (p->wdata == NULL ? 0 : strlen(p->wdata));
    int rlen = len + strlen(data) + 1;
    p->wdata = (char *)realloc(p->wdata, rlen * sizeof(char));
    strcpy (p->wdata+len, data);
  }
  return;
}

int sock_read_handler (int fd) {
  printf ("in sock_read_handler fd:%d\n", fd);
  char line[1024];
  while (1) {
    int ret = read (fd, line, 1023);
    if (ret < 0) {
      if (errno != EWOULDBLOCK) {
        printf ("read client failed!\n");
      }
      break;
    }
    if (ret == 0) {
      return 1;
    }
    line[ret] = 0;
    printf ("%s\n", line);
    set_write_data(fd,line);
  }
  return 0;
}

int sock_write_handler (int fd, char *data) {
  printf ("in sock_write_handler fd:%d\n", fd);
  int left = strlen(data);
  int pos = 0;
  while (left > 0) {
    int ret = write (fd, data + pos, left);
    if (ret == 0) {
      break;
    }
    if (ret < 0) {
      //
    }
    left -= ret;
    pos += ret;
  }
  return 0;
}

int accept_handler (int fd) {
  printf ("in accept_handler\n");
  while (1) {
    int ret = accept (fd, NULL, NULL);
    if (ret < 0) {
      if (errno == EWOULDBLOCK || errno == EPROTO || errno == ECONNABORTED || errno == EINTR)
        break;
      else {
        perror ("accept failed!\n");
        return -1;
      }
    }
    setnonblock(ret, 1);
    event *client_event = (event *) malloc (sizeof(event));
    if (client_event == NULL) {
      printf ("accept create client event failed!\n");
      return -1;
    }
    client_event->fd = ret;
    client_event->mask = READ_EVENT | WRIT_EVENT;
    client_event->rfunc = sock_read_handler;
    client_event->wfunc = sock_write_handler;
    client_event->wdata = NULL;
    client_event->next = NULL;
    addevent (ev_loop, client_event);
  }
  return 0;
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
  // 设置keepalive
  int keepAlive = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive));
  int reuseaddr = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&reuseaddr, sizeof(reuseaddr));
  int reuseport = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, (void*)&reuseport, sizeof(reuseport));
  ret = bind (listen_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
  if (ret < 0) {
    perror ("bind socket failed!");
    return -1;
  }
  if (setnonblock(listen_fd, 1) != 0) {
    return -1;
  }
  ret = listen(listen_fd, 10);
  if (ret < 0) {
    perror ("listen failed!");
    return -1;
  }
  ev_loop = init_event_loop();
  event *listen_event = (event *)malloc (sizeof(event));
  if (listen_event == NULL) {
    printf ("create listen event failed!\n");
    return -1;
  }
  listen_event->fd = listen_fd;
  listen_event->mask = READ_EVENT;
  listen_event->rfunc = accept_handler;
  listen_event->wfunc = NULL;
  listen_event->wdata = NULL;
  listen_event->next = NULL;
  addevent (ev_loop, listen_event);
  loop_run(ev_loop);
  return 0;
}
