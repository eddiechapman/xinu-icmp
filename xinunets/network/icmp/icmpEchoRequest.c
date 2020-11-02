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
  struct icmpgram  *icmp  = (struct icmpgram  *)(ip + 1); // ?
  struct icmpEcho  *echo  = (struct icmpEcho  *)icmp->data;
  int i;

  /* Fill in ethernet header */
  getmac(dev, ether->src);
  printf("\tOur MAC     = ");
	for (i = 0; i < ETH_ADDR_LEN; i++)
	{ 
		printf("%02X", ether->src[i]); 
		if (i < ETH_ADDR_LEN - 1) { printf(":"); } else { printf("\n"); }
	}

  arpResolve(ipaddr, ether->dst);
  printf("\tTheir MAC   = ");
	for (i = 0; i < ETH_ADDR_LEN; i++)
	{ 
		printf("%02X", ether->dst[i]); 
		if (i < ETH_ADDR_LEN - 1) { printf(":"); } else { printf("\n"); }
	}

  ether->type = htons(ETYPE_IPv4);

  /* Fill in IPv4 header */
  printf("\tFilling in the IPv4 header.\n");
  ip->ver_ihl = (IPv4_VERSION << 4) | (IPv4_HDR_LEN >> 2);
	ip->tos = htons(IPv4_TOS_ROUTINE);
	ip->len = 0;
	ip->id = htons(currpid);
	ip->flags_froff = 0;
	ip->ttl = 63;
	ip->proto = IPv4_PROTO_ICMP;
	ip->chksum = 0;
  getip(dev, ip->src);

  printf("Source ip is :%s\n",ip->src);
  printf("Value of variable ipaddr :%s\n",ipaddr);
  printf("Value of data field in ethergram is :%s\n",ether->data);

  memcpy(ip->dst, ipaddr, IP_ADDR_LEN);

  printf("Destination ip is :%s\n",ip->dst);

  /* Fill in ICMP header */
  printf("\tFilling in the ICMP header.\n");
  icmp->code = htons(0);
  icmp->type = ICMP_ECHO;
  icmp->chksum = htons(0);

  printf("\tFilling in the Echo header.\n");
  echo->id = htons(id);
  echo->seq = htons(seq);

  printf("\tReady to send the packet.\n");

  write(dev, (uchar *)packet, 
      sizeof(struct ethergram) + sizeof(struct ipgram) + sizeof(struct icmpgram));
  
  printf("\tOK, it sent.\n");

  printf("packet value is :%s\n", packet);
  printf("ethergram value is :%s\n", ether);
  printf("icmpgram value is :%s\n", icmp);
  printf("ip structure value is : %s\n", ip);
  printf("icmpEcho structure value is :%s\n", echo);
  
  return icmpEchoReply(dev, (uchar *)packet);
}