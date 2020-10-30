#include <xinu.h>
#include "../network/icmp/icmp.h"

command xsh_ping(int nargs, char *args[])
{
  uchar ipaddr[IP_ADDR_LEN];
  dot2ip(args[1], ipaddr);
  printf("You called ping.\n");
  icmpEchoRequest(ETH0, ipaddr);
  return 0;
}