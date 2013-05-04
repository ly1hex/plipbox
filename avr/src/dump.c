/*
 * dump.c - helper functions for debugging
 *
 * Written by
 *  Christian Vogelgsang <chris@vogelgsang.org>
 *
 * This file is part of plipbox.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "dump.h"
#include "uart.h"
#include "uartutil.h"
#include "net/net.h"
#include "net/arp.h"
#include "net/eth.h"
#include "net/ip.h"
#include "param.h"

void dump_eth_pkt(const u08 *eth_buf, u16 size)
{
  uart_send('[');
  uart_send_hex_word(size);
  uart_send(',');
  uart_send_hex_word(eth_get_pkt_type(eth_buf));
  uart_send(',');
  net_dump_mac(eth_get_src_mac(eth_buf));
  uart_send('>');
  net_dump_mac(eth_get_tgt_mac(eth_buf));
  uart_send(']');
  uart_send(' ');
}

void dump_arp_pkt(const u08 *arp_buf)
{
  uart_send_pstring(PSTR("[ARP:"));

  // ARP op
  u16 op = arp_get_op(arp_buf);
  if(op == ARP_REQUEST) {
    uart_send_pstring(PSTR("REQ "));
  } else if(op == ARP_REPLY) {
    uart_send_pstring(PSTR("REPL"));
  } else {
    uart_send_hex_word(op);
  }
  uart_send(',');
  
  // src pair
  uart_send('(');
  net_dump_mac(arp_get_src_mac(arp_buf));
  uart_send(',');
  net_dump_ip(arp_get_src_ip(arp_buf));
  uart_send(')');
  uart_send('>');

  // tgt pair
  uart_send('(');
  net_dump_mac(arp_get_tgt_mac(arp_buf));
  uart_send(',');
  net_dump_ip(arp_get_tgt_ip(arp_buf));
  uart_send(')');
  
  uart_send(']');
  uart_send(' ');
}

void dump_ip_pkt(const u08 *ip_buf)
{
  uart_send_pstring(PSTR("[IP4:"));

  // size
  uart_send_hex_word(ip_get_total_length(ip_buf));

  // ip proto
  u08 proto = ip_get_protocol(ip_buf);
  if(proto == IP_PROTOCOL_ICMP) {
    uart_send_pstring(PSTR(",ICMP"));
  } else if(proto == IP_PROTOCOL_TCP) {
    uart_send_pstring(PSTR(",TCP "));
  } else if(proto == IP_PROTOCOL_UDP) {
    uart_send_pstring(PSTR(",UDP "));
  } else {
    uart_send(',');
    uart_send_hex_word(proto);
  }

  // src/tgt ip
  uart_send(',');
  net_dump_ip(ip_get_src_ip(ip_buf));
  uart_send('>');
  net_dump_ip(ip_get_tgt_ip(ip_buf)),

  uart_send(']');
  uart_send(' ');
}

extern void dump_line(const u08 *eth_buf, u16 size)
{
  if(param.dump_eth) {
    dump_eth_pkt(eth_buf, size);
  }
  const u08 *ip_buf = eth_buf + ETH_HDR_SIZE;
  u16 type = eth_get_pkt_type(eth_buf);
  if(type == ETH_TYPE_ARP) {
    if(param.dump_arp) {
      dump_arp_pkt(ip_buf);
    }
  } else if(type == ETH_TYPE_IPV4) {
    if(param.dump_ip) {
      dump_ip_pkt(ip_buf);
    }
  }
}
