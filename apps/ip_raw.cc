#include "socket.hh"
#include <iostream>

using namespace std;

/* rfc791 section-3.1
 0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Version|  IHL  |Type of Service|          Total Length         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |         Identification        |Flags|      Fragment Offset    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Time to Live |    Protocol   |         Header Checksum       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                       Source Address                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Destination Address                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Options                    |    Padding    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                    Example Internet Datagram Header

    rfc768
                 0      7 8     15 16    23 24    31
                 +--------+--------+--------+--------+
                 |     Source      |   Destination   |
                 |      Port       |      Port       |
                 +--------+--------+--------+--------+
                 |                 |                 |
                 |     Length      |    Checksum     |
                 +--------+--------+--------+--------+
                 |
                 |          data octets ...
                 +---------------- ...

                      User Datagram Header Format
*/
int main()
{
  // construct an Internet or user datagram here, and send using a RawSocket
  string datagram = {};
  string udpHeader = {};
  unsigned int protocol = 17;
  string data = "test ip raw packet\n";

  datagram += 0b0100'0101; // version and IHL
  datagram += string( 7, 0 ); // rest of first two lines
  datagram += 64;//TTL
  datagram += protocol; // 1	ICMP, 5 ST, 6	TCP, 17	UDP - protocol
  datagram += string( 6, 0 ); // rest of next two lines
  /* destination IP */
  datagram += 127u; // u - unsigned int
  datagram += string( 2, 0 );
  datagram += 1u;

  udpHeader += string(2, 8); // source port 0x0808
  udpHeader += string(2, 6); // destination port 0x0606
  uint16_t udpLen = 8 + data.size();
  uint16_t beLen = htobe16(udpLen); // Host to Big Endian 16-bit
#if 1
  udpHeader +=string(reinterpret_cast<char*>(&beLen), 2); // length
#else
  udpHeader += (char)(udpLen >> 8); // length
  udpHeader += (char)(udpLen & 0xff); // length
#endif
  udpHeader += string( 2, 0 ); // checksum
  
  if (protocol == 17) {
    datagram += udpHeader;
  }
  datagram += data;

  RawSocket {}.send( datagram, Address { "127.0.0.1" } );
  return 0;
}
