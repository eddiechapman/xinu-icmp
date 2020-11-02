/**
 * @file icmpRecv.c
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>
#include "icmp.h"

/** 
 * Process incoming ICMP packets.
 * 
 * @param dev     Descriptor for network interface that recieved the packet
 * @param packet  Pointer to incoming ICMP packet buffer.
 * @return OK
 */
int icmpRecv(int dev, uchar *packet) 
{
  struct ethergram *egram = (struct ethergram *)packet;
  struct ipgram    *ip    = (struct ipgram    *)egram->data;
  struct icmpgram  *icmp  = (struct icmpgram  *)ip->opts;
  struct icmpEcho  *echo  = (struct icmpEcho  *)icmp->data;
  int id;

  switch (icmp->type)
  {
    case ICMP_ECHOREPLY:
      printf("Recieved ICMP Echo Reply ID: %d, seq: %d\n", 
          ntohs(echo->id), ntohs(echo->seq));
      break;
    case ICMP_ECHO:
      printf("Recieved ICMP Echo Request\n");
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
      printf("ICMP message type %d not handled\n", icmp->type);
      break;
    default:
      printf("ICMP message type %d unknown\n", icmp->type);
      break;
  }

  buffree(packet);
  return OK;
}
