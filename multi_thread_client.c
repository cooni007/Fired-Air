 #include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <assert.h>
 
static void
print_usage (void)
{
  fprintf (stderr,
      "'-s srv_ip'------Select ip for this server, default ip is 0.0.0.0.\n"
      "'-p srv_port'----Select server port, default port_clt is 43567.\n");
}
 
static void
bail (const char *on_what)
{
  fputs (strerror (errno), stderr);
  fputs (": ", stderr);
  fputs (on_what, stderr);
  fputc ('\n', stderr);
  exit (-1);
}
 
  int
main (int argc, char **argv)
{
  int err;
  int i;
  int port = 43567;
  char *srv_ip = "0.0.0.0";
  struct sockaddr_in adr_srvr;  /* AF_INET */
  int len_inet;                 /* length  */
  int sockfd;                   /* Socket */
  char msg[128];
 
  for (i = 1; i < argc;)
  {
    if (argv[i][0] != '-')
    {
      fprintf (stderr, "Usage:%s -s srv_ip "
          "-p srv_port " "-l layer" "-S" "-d\n", argv[0]);
      print_usage ();
      exit (1);
    }
    else
    {
      switch (argv[i][1])
      {
        case ('s'):
          srv_ip = argv[i + 1];
          i += 2;
          break;
        case ('p'):
          port = atoi (argv[i + 1]);
          i += 2;
          break;
        default:
          fprintf (stderr, "Usage:%s -s srv_ip -p srv_port\n", argv[0]);
          print_usage ();
          exit (1);
      }
    }
  }
 
  /*
   * Create a server socket address:
   */
  len_inet = sizeof adr_srvr;
  memset (&adr_srvr, 0, len_inet);
  adr_srvr.sin_family = AF_INET;
  adr_srvr.sin_port = htons (port);
  adr_srvr.sin_addr.s_addr = inet_addr (srv_ip);
 
  if (adr_srvr.sin_addr.s_addr == INADDR_NONE)
    bail ("bad address.");
 
  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
    bail ("socket()");
 
  /*
   * Connect to the server:
   */
  printf ("server ip  : %s\n", srv_ip);
  printf ("server port: %d\n", port);
  err = connect (sockfd, (struct sockaddr *) &adr_srvr, len_inet);
  if (err == -1)
    bail ("client: connect(2)");
 
  printf ("CONNECT SERVER SUCCESSFULLY!\n");
 
  while (1)
  {
    printf ("INPUT YOUR MESSAGE: ");
    fflush(stdout);
    memset(msg, 0, sizeof(msg));
    fgets (msg, sizeof(msg), stdin);
    if ((err = send (sockfd, msg, 1+strlen(msg), MSG_NOSIGNAL)) < 0)
      bail ("client: write()");
  }
}
