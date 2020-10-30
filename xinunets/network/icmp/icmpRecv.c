/* 
 * icmpRecv.c
 */

#include <xinu.h>
#include "icmp.h"

/* 
 * Process incoming ICMP packets.
 */
int icmpRecv(int dev, uchar *packet) 
{
  struct ethergram *egram = (struct ethergram *)packet;
  struct icmpgram  *icmp  = (struct icmpgram  *)egram->data;
  struct icmpEcho *echo;
  int id;

  switch (icmp->type)
  {
  case ICMP_ECHOREPLY:
    ICMP_TRACE("Received Echo Reply");
    break;
    // echo = (struct icmpEcho *)icmp->data;
    //  = ntohs(echo->id);  // ping process ID
    // validate that id is more than 0 and less than our max ID constant
    // calculate the RTT time etc and store in echo struct
    // loop through the echo queue looking for corresponding ping process
    // add the packet to the echo queue
    // return SYSERR and free packet buffer if queue is full
    // notify the ping process that an echo reply has arrived
    // return OK if successful
    // return SYSERR and free packet buffer if ping process cannot be found
  case ICMP_ECHO:
    ICMP_TRACE("Recieved Echo Request");
    icmpEchoReply(dev, packet);
    break;
  case ICMP_UNREACH:
  case ICMP_SRCQNCH:
  case ICMP_REDIRECT:
  case ICMP_TIMEEXCD:
  case ICMP_PARAMPROB:
  case ICMP_TMSTMP:
  case ICMP_TMSTMPREPLY:
  case ICMP_INFORQST:
  case ICMP_INFOREPLY:
  case ICMP_TRACEROUTE:
    ICMP_TRACE("ICMP message type %d not handled", icmp->type);
    break;
  default:
    ICMP_TRACE("ICMP message type %d unknown", icmp->type);
    break;
  }

  buffree(packet);
  return OK;
}
