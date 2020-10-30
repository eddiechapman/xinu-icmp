/* 
  icmpEchoRequest.c
 */

#include <xinu.h>
#include "icmp.h"
#include "../arp/arp.h"

/** 
 * Send an ICMP echo request message.
 * 
 * @param dev
 * @param ipaddr    Network address of destination 
 * 
 */
int icmpEchoRequest(int dev, int icmpindex, uchar *ipaddr)
{
  uchar packet[PKTSZ];
  struct ethergram *ether = (struct ethergram *)packet;
  struct ipgram    *ip = (struct ipgram *)ether->data;
  struct icmpgram  *icmp  = (struct icmpgram *)(ip + 1);

  bzero(packet, PKTSZ);

  /* Fill in ethernet header */
  getmac(dev, ether->src);
  arpResolve(ipaddr, ip->dst);
  ip->type = htons(ETYPE_IPv4);

  /* Fill in IPv4 header */
  ip->ver_ihl = (IPv4_VERSION << 4) | (IPv4_HDR_LEN >> 2);
	ip->tos = IPv4_TOS_ROUTINE;
	ip->len = 0; /* Update later */
	ip->id  = htons(currpid);
	ip->flags_froff = 0;
	ip->ttl = 63;
	ip->proto = IPv4_PROTO_ICMP;
	ip->chksum = 0; /* FIXME */
  getip(dev, ip->src);
  memcpy(ip->dst, ipaddr, IP_ADDR_LEN);

  /* Fill in ICMP header */
  icmp->code = 0;
  icmp->type = ICMP_ECHO;
  icmp->chksum = 0;

  for (int i = 0; i < ((ulong)packet); i++)
  {
    printf(" %02X", packet[i]);
    if ((i % 16) == 15) { printf("\n"); }
  }
  printf("\n");

  write(dev, packet, sizeof(struct ethergram) + sizeof(struct ipgram) + sizeof(struct icmpgram));
  return OK;
}