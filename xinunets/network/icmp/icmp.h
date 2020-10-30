/* 
  icmp.h
 */
#ifndef _ICMP_H_
#define _ICMP_H_

/* Tracing macro for ICMP */
#ifdef TRACE_ICMP
#include <stdio.h>
#define ICMP_TRACE(...)     { \
		fprintf(TRACE_ICMP, "%s:%d (%d) ", __FILE__, __LINE__, gettid()); \
		fprintf(TRACE_ICMP, __VA_ARGS__); \
		fprintf(TRACE_ICMP, "\n"); }
#else
#define ICMP_TRACE(...)
#endif

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
struct icmpgram               /**< ICMP Packet          */
{
  uchar type;                 /**< ICMP type            */
  uchar code;                 /**< ICMP code            */
  ushort chksum;              /**< ICMP checksum        */
  uchar data[1];              /**< ICMP data            */
};

// struct icmpEcho
// {
//   ushort id;
//   ushort seq;
//   ulong timesec;                /**< Clock time in seconds, */
//   ulong timetic;                /**< Clock time in tics,    */
//   ulong timecyc;                /**< Clock time in cycles   */
//   ulong arrivsec;
//   ulong arrivtic;
//   ulong arrivcyc;
// };

// #define NPINGQUEUE 5
// #define NPINGHOLD  10

// struct icmpEchoQueue
// {
//   int pid;              /**< ID of process that send the ICMP Echo Request. */
//   int head;             /**< Position to store next ICMP Echo Reply.        */
//   int tail;             /**< Position of next ICMP Echo Reply to get.       */
//   struct packet *pkts[NPINGHOLD];   /*< Stored ICMP Echo Replies.           */
// };

// extern struct icmpEchoQueue echotab[NPINGQUEUE];

// int icmpDaemon(void);
// void icmpInit(void);
int icmpRecv(int dev, uchar *packet);
int icmpEchoRequest(int dev, uchar *dest);
int icmpEchoReply(int dev, uchar *packet);
// void icmpSend(uchar *packet, uchar type, uchar code,
 //                uint datalen, struct netaddr *src, struct netaddr *dst);
#endif                          /* _NET_H_ */