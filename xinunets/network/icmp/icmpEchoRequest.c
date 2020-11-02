/**
 * @file icmpEchoRequest.c
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>
#include "icmp.h"
#include "../arp/arp.h"

/** 
 * Send an ICMP echo request message.
 * 
 * @param dev       Device descriptor used for sending/recieving packet
 * @param seq       Identifies this ICMP request message
 * @param id        Identifies the ping process
 * @param ipaddr    Network address of destination
 * @return OK       There is currently no error checking
 */
int icmpEchoRequest(int dev, ushort seq, ushort id, uchar *ipaddr)
{
  uchar packet[PKTSZ];
  struct ethergram *ether = (struct ethergram *)packet;
  struct ipgram    *ip    = (struct ipgram    *)ether->data;
  struct icmpgram  *icmp  = (struct icmpgram  *)ip->opts;
  struct icmpEcho  *echo  = (struct icmpEcho  *)icmp->data;
  int i;

  /* Fill in ethernet header */
  getmac(dev, ether->src);

  arpResolve(ipaddr, ether->dst);

  ether->type = htons(ETYPE_IPv4);

  /* Fill in IPv4 header */
  ip->ver_ihl = (IPv4_VERSION << 4) | (IPv4_HDR_LEN >> 2);
	ip->tos = htons(IPv4_TOS_ROUTINE);
	ip->len = 0;
	ip->id = htons(currpid);
	ip->flags_froff = 0;
	ip->ttl = 63;
	ip->proto = IPv4_PROTO_ICMP;
	ip->chksum = 0;  // TODO: calculate checksum
  getip(dev, ip->src);

  memcpy(ip->dst, ipaddr, IP_ADDR_LEN);

  icmp->code = 0;
  icmp->type = ICMP_ECHO;
  icmp->chksum = 0; // TODO: calculate checksum

  echo->id = htons(id);
  echo->seq = htons(seq);

  write(dev, (uchar *)packet, 
      sizeof(struct ethergram) + sizeof(struct ipgram) + sizeof(struct icmpgram) + sizeof(struct icmpEcho));
  
  buffree(packet);

  return OK;
}