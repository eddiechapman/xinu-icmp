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
  ushort temp = 0;
  int id;

  temp = icmp->chksum;
  icmp->chksum = 0;
  icmp->chksum = checksum((uchar *)icmp, ICMP_HEADER_LEN);
  if (icmp->chksum != temp)
  {
    printf("Incorrect ICMP checksum: 0x%04X. (correct: 0x%04X))\n", 
            ntohs(temp), ntohs(icmp->chksum));
    buffree(packet);
    return OK; 
  }

  switch (icmp->type)
  {
    case ICMP_ECHOREPLY:
      send(ntohs(echo->id), (ulong)packet);
      break;
    case ICMP_ECHO:
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
