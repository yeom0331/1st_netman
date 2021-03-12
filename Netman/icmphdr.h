#pragma pack(push,1)
struct ethernet_header {
    Mac dmac;
    Mac smac;
    u_int16_t type;
};
#pragma pack(pop)
#ifndef ETHERTYPE_PUP
#define ETHERTYPE_PUP           0x0200  /* PUP protocol */
#endif
#ifndef ETHERTYPE_IP
#define ETHERTYPE_IP            0x0800  /* IP protocol */
#endif
#ifndef ETHERTYPE_ARP
#define ETHERTYPE_ARP           0x0806  /* addr. resolution protocol */
#endif
#ifndef ETHERTYPE_REVARP
#define ETHERTYPE_REVARP        0x8035  /* reverse addr. resolution protocol */
#endif
#ifndef ETHERTYPE_VLAN
#define ETHERTYPE_VLAN          0x8100  /* IEEE 802.1Q VLAN tagging */
#endif
#ifndef ETHERTYPE_EAP
#define ETHERTYPE_EAP           0x888e  /* IEEE 802.1X EAP authentication */
#endif
#ifndef ETHERTYPE_MPLS
#define ETHERTYPE_MPLS          0x8847  /* MPLS */
#endif
#ifndef ETHERTYPE_LOOPBACK
#define ETHERTYPE_LOOPBACK      0x9000  /* used to test interfaces */
#endif

#pragma pack(push,1)
struct ip_header {
    u_int8_t ip_hl : 4, ip_v : 4;
    u_int8_t ip_tos;
    u_int16_t ip_total_len;
    u_int16_t ip_id;
    uint16_t ip_off;
    uint8_t ip_ttl;
    uint8_t ip_protocol;
    uint16_t ip_checksum;
    Ip sip;
    Ip dip;
};
#pragma pack(pop)

#pragma pack(push,1)
struct icmp_header {
    uint8_t icmp_type;
    uint8_t icmp_code;
    uint16_t icmp_checksum;
    uint16_t icmp_id;
    uint16_t icmp_seq;
};
#pragma pack(pop)