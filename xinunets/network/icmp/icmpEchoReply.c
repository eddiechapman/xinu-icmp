/**
 * @file icmpEchoReply.c
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>
#include "icmp.h"

/**
 * Send an ICMP echo reply message.
 *
 * @param dev       Device descriptor used for sending/recieving packet.
 * @param packet    ICMP Echo Request packet.
 * @return          OK or SYSERR if there is an exception.
 */
int icmpEchoReply(int dev, uchar *packet)
{
  printf("\tReached inside icmpEchoReply.\n");

  struct ethergram *ether = (struct ethergram *)packet;
  struct ipgram    *ip    = (struct ipgram    *)ether->data;
  struct icmpgram  *icmp  = (struct icmpgram  *)ip->opts;
  struct icmpEcho  *echo  = (struct icmpEcho  *)icmp->data;
  uchar enet[ETH_ADDR_LEN];
  int i;

  if (SYSERR == getmac(dev, enet))
  {
    buffree(packet);
    return SYSERR;
  }

  if (SYSERR == getip(dev, ip->src))
  {
    buffree(packet);
    return SYSERR;
  }

  memcpy(ether->dst, ether->src, ETH_ADDR_LEN);
	memcpy(ether->src, enet, ETH_ADDR_LEN);
  icmp->code = 0;
  icmp->type = ICMP_ECHOREPLY;
  icmp->chksum = 0;

  printf("Writing the icmpgram.\n");

  write(dev, (uchar *)packet,
    sizeof(struct ethergram) + sizeof(struct ipgram) + sizeof(struct icmpgram) + sizeof(struct icmpEcho));

  buffree(packet);
  
  return OK;
}