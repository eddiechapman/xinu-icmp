#include <xinu.h>
#include "../network/icmp/icmp.h"

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
  int i;

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

  for(i = 0; i < 10; i++)
  {
    icmpEchoRequest(ETH0, i, currpid, ipaddr);
  }

  /* 
    TODO: print out statistics about the echo replies that come back
    
    example:
      --- ping statistics ---
      10 packets transmitted, 10 packets recieved, 0.0% packet loss
      
   */

  return 0;
}