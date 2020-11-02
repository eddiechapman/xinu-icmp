/**
 * @file icmp.h
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#define ICMP_HEADER_LEN		  4

/* Message Types for ICMP */
#define ICMP_ECHOREPLY      0
#define ICMP_UNREACH        3
#define ICMP_SRCQNCH        4
#define ICMP_REDIRECT       5
#define ICMP_ECHO           8
#define ICMP_TIMEEXCD       11
#define ICMP_PARAMPROB      12
#define ICMP_TMSTMP         13
#define ICMP_TMSTMPREPLY    14
#define ICMP_INFORQST       15
#define ICMP_INFOREPLY      16
#define ICMP_TRACEROUTE     30

/*
 * ICMP HEADER
 *
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | Link-Level Header                                             |
 * | ...                                                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | IP Header                                                     |
 * | ...                                                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | Type          | Code          | Checksum                      |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | Data (Variable octets                                         |
 * | ...                                                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct icmpgram                 /**< ICMP Packet          */
{
  uchar   type;                 /**< ICMP type            */
  uchar   code;                 /**< ICMP code            */
  ushort  chksum;               /**< ICMP checksum        */
  uchar   data[1];              /**< ICMP data            */
};

/*
 * ICMP ECHO REQUEST / REPLY
 *
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | Link-Level Header                                             |
 * | ...                                                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | IP Header                                                     |
 * | ...                                                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | ICMP Header                                                   |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | Identifier                    | Sequence number               |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | Data (Variable octets)                                        |
 * | ...                                                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct icmpEcho                 /**< ICMP Echo Packet     */
{
  ushort  id;                   /**< Echo identifier      */
  ushort  seq;                  /**< Echo sequence        */
  uchar   data[1];              /**< Echo optional data   */
};

int icmpRecv(int dev, uchar *packet);
int icmpEchoRequest(int dev, ushort seq, ushort id, uchar *ipaddr);
int icmpEchoReply(int dev, uchar *packet);