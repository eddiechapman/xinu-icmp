/* 
  icmpEchoReply.c
 */

#include <xinu.h>
#include "icmp.h"

int icmpEchoReply(int dev, uchar *packet)
{
/*
  struct ethergram *ether = (struct ethergram *)packet;
  struct icmpgram *icmp = (struct icmpgram *)ether->data;
  uchar enet[ETH_ADDR_LEN];
  uchar ip[IP_ADDR_LEN];


  if (SYSERR == getmac(dev, enet))
  {
    buffree(packet);
    return SYSERR;
  }

  if (SYSERR == getip(dev, ip))
  {
    buffree(packet);
    return SYSERR;
  } */

  ICMP_TRACE("Sending ICMP_ECHOREPLY");

  return OK;
}