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
  struct icmpgram  *icmp  = (struct icmpgram  *)egram->data;
  int id;

  switch (icmp->type)
  {
    case ICMP_ECHOREPLY:
      printf("Recieved ICMP Echo Reply");
      break;
    case ICMP_ECHO:
      printf("Recieved ICMP Echo Request");
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
      printf("ICMP message type %d not handled", icmp->type);
      break;
    default:
      printf("ICMP message type %d unknown", icmp->type);
      break;
  }

  buffree(packet);
  return OK;
}
