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
  struct icmpgram  *gram  = (struct icmpgram  *)(ip + 1);
  struct icmpEcho  *echo  = (struct icmpEcho  *)gram->data;
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
  gram->code = htons(ICMP_ECHOREPLY);
  memcpy(gram->tha, gram->sha, ETH_ADDR_LEN);
  memcpy(gram->tpa, gram->spa, IP_ADDR_LEN);
  memcpy(gram->sha, enet, ETH_ADDR_LEN);
  memcpy(gram->spa, ip, IP_ADDR_LEN);

  printf("Writing the icmpgram.\n");

  write(dev, (uchar *)packet,
    sizeof(struct ethergram) + sizeof(struct icmpgram) + sizeof(struct icmpEcho));

  buffree(packet);
  
  return OK;
}