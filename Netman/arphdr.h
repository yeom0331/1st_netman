#pragma once

#include <cstdint>
#include "mac.h"
#include "ip.h"
#include "winsock2.h"

#pragma pack(push, 1)
struct ArpHdr final {
    uint16_t hrd_;
    uint16_t pro_;
    uint8_t  hln_;
    uint8_t  pln_;
    uint16_t op_;
    Mac      smac_;
    Ip       sip_;
    Mac      tmac_;
    Ip       tip_;

    typedef enum {
        NETROM = 0,  // from KA9Q: NET/ROM pseudo
        ETHER = 1,  // Ethernet 10Mbps
        EETHER = 2,  // Experimental Ethernet
        AX25 = 3,  // AX.25 Level 2
        PRONET = 4,  // PROnet token ring
        CHAOS = 5,  // Chaosnet
        IEEE802 = 6,  // IEEE 802.2 Ethernet/TR/TB
        ARCNET = 7,  // ARCnet
        APPLETLK = 8,  // APPLEtalk
        LANSTAR = 9,  // Lanstar
        DLCI = 15, // Frame Relay DLCI
        ATM = 19, // ATM
        METRICOM = 23, // Metricom STRIP (new IANA id)
        IPSEC = 31  // IPsec tunnel
    } HardwareType;

    typedef enum {
        Request = 1, // req to resolve address
        Reply = 2, // resp to previous request
        RevRequest = 3, // req protocol address given hardware
        RevReply = 4, // resp giving protocol address
        InvRequest = 8, // req to identify peer
        InvReply = 9  // resp identifying peer
    } Operation;
};
typedef ArpHdr* PArpHdr;
#pragma pack(pop)

#pragma pack(push, 1)
struct EthArpPacket {
    EthHdr eth_;
    ArpHdr arp_;
};
#pragma pack(pop)
