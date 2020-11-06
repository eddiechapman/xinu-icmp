#include <xinu.h>
#include "../network/icmp/icmp.h"

/**
 * Send a series of ICMP echo requests at one second intervals.
 * 
 * @param dev     Device descriptor used for sending the packets.
 * @param n       Number of echo requests to send
 * @param ipaddr  Destination network address for echo requests
 * @param id      Process ID of ping command. Used in echo identifier.
 * @returns       non-zero value on error
 */
process sendEchoRequests(int dev, int n, uchar *ipaddr, ushort id)
{
  int i;

  /* printf("sendEchoRequest: entering function\n"); */

  for(i = 0; i < n; i++)
  {
    icmpEchoRequest(dev, i, id, ipaddr);
    sleep(500);
  }

  /* printf("sendEchoRequest: exiting function\n"); */

  return OK;
}

/** 
 * Shell command (ping) tests connection to remote host via ICMP messages.
 * 
 * @param nargs   number of arguments in args array
 * @param args    array of arguments
 * @return        non-zero value on error
 */
command xsh_ping(int nargs, char *args[])
{
  uchar ipaddr[IP_ADDR_LEN];
  uchar *packet = NULL;
  struct ethergram *egram;
  struct ipgram    *ip;
  struct icmpgram  *icmp;
  struct icmpEcho  *echo;
  int mesg = 0;
  int received = 0;
  int n = 10;
  int i;

  /* printf("xsh_ping: entering function\n"); */

  /* Output help, if '--help' argument was supplied */
  if (nargs == 2 && strncmp(args[1], "--help", 6) == 0)
  {
    fprintf(stdout, "Sends an ICMP Echo Request to a remote host.\n");
    fprintf(stdout, "\t--help\t display this help and exit\n");
    fprintf(stdout, "\t<IP>\t resolve requested IP address\n");
    return 0;
  }

  if (nargs > 2)
  {
    fprintf(stderr, "ping: too many arguments\n");
    fprintf(stderr, "Try 'ping --help' for more information\n");
    return 1;
  }

  if (OK != dot2ip(args[1], ipaddr))
	{
		fprintf(stderr, "Could not parse IP address \"%s\"\n", args[1]);
		return 1;
	}

  printf("\n");

  ready(create((void *)sendEchoRequests, INITSTK, proctab[currpid].priority + 1, 
          "send echo requests", 4, ETH0, n, ipaddr, currpid), RESCHED_NO);

  while (1)
  {
    /* printf("xsh_ping: top of receive loop. Message count: %d\n", received); */
    
    mesg = receive();

    /* printf("xsh_ping: message received: %d\n", mesg); */

    packet = (uchar *)mesg;
    egram = (struct ethergram *)packet;
    ip = (struct ipgram *)egram->data;
    icmp = (struct icmpgram *)ip->opts;
    echo = (struct icmpEcho *)icmp->data;

    if(ntohs(echo->id) != currpid)
    {
      printf("Echo reply has incorrect ID: %d (correct: %d)\n", ntohs(echo->id), currpid);
      continue;
    }

    printf("%d bytes from %d.%d.%d.%d: icmp_seq=%d ttl=%d\n", 
            sizeof(packet), ip->src[0], ip->src[1],  ip->src[2], 
          ip->src[3],  ntohs(echo->seq), ip->ttl);
      
    received++;
      
    if (ntohs(echo->seq) == n - 1)  // this is final packet, but some were lost
    {
      break;
    }
    
    /* printf("xsh_ping: bottom of receive loop. Message count: %d\n", received); */
  }

  printf("\n--- ping statistics ---\n");
  printf("%d packets transmitted, %d packets received, ", n, received);
  if (n - received == 0)
  {
    printf("0%% packet loss.\n");
  }
  else
  {
    printf("d%% packet loss.\n", ((n - received) / n) * 100);
  }

  return 0;
}