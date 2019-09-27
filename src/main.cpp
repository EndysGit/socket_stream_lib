#include <iostream>
#include <ostream>
#include <ios>

#include <socket.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>


class socket
{
};

/* stream prefixes:
 *  - i = input
 *  - o = output
 *  - s = socket
 */

template <typename charT, typename traits, typename socketT>
class basic_isstreami : virtual public std::basic_ios<charT, traits>
{
};

template <typename charT, typename traits, typename socketT>
class basic_osstream : virtual public std::basic_ios<charT, traits> 
{

};

class socket_base
{
public:
// Flags:
// ------
//
// State:
// ------
//
//
};

int foo(int i)
{ return i;}

void print_ethernet_header(const unsigned char *const buffer)
{
    struct ethhdr *ethernet_header = (struct ethhdr *) (buffer);
    printf("\nEthernet header\n");
    printf("\t|~Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n"
            , ethernet_header->h_source[0]
            , ethernet_header->h_source[1]
            , ethernet_header->h_source[2]
            , ethernet_header->h_source[3]
            , ethernet_header->h_source[4]
            , ethernet_header->h_source[5]);
    printf("\t|~Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n"
            , ethernet_header->h_dest[0]
            , ethernet_header->h_dest[1]
            , ethernet_header->h_dest[2]
            , ethernet_header->h_dest[3]
            , ethernet_header->h_dest[4]
            , ethernet_header->h_dest[5]);
    printf("\t|~Protocol : %d\n", ethernet_header->h_proto);
}

void print_ip_header(const unsigned char *const buffer)
{
    size_t iphdrlen;
    struct iphdr *ip = (struct iphdr *) (buffer + sizeof(struct ethhdr));
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip->daddr;

    printf("Ip Header:\n");
    printf("\t|~Version: %d\n", (unsigned int) ip->version);
    printf("\t|~Internet Header Length: %d DWORDS or %d Bytes\n", (unsigned int) ip->ihl,
           (unsigned int) ip->ihl * 4);
    printf("\t|~Type Of Service: %d\n", (unsigned int) ip->tos);
    printf("\t|~Total Length: %d Bytes\n", ntohs(ip->tot_len));
    printf("\t|~Identification: %d\n", ntohs(ip->id));
    printf("\t|~Time To Live: %d\n", (unsigned int) ip->ttl);
    printf("\t|~Protocol: %d\n", (unsigned int) ip->protocol);
    printf("\t|~Header Checksum: %d\n", ntohs(ip->check));
    printf("\t|~Source IP: %s\n", inet_ntoa(source.sin_addr));
    printf("\t|~Destination IP: %s\n", inet_ntoa(dest.sin_addr));
}


int main()
{
    int sock_r;
    sock_r = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock_r < 0)
    {
        printf("Cant create socket\n");
        exit(2);
    }


    size_t buffer_size = 65536;
    unsigned char *buffer = (unsigned char *) malloc(buffer_size);
    memset(buffer, 0, buffer_size);

    struct sockaddr saddr;
    size_t saddr_len = sizeof(saddr);

    if (recvfrom(sock_r, buffer, buffer_size, 0, &saddr, (socklen_t*)&saddr_len) < 0)
    {
        printf("Error receiveing data\n");
        exit(2);
    }

    print_ethernet_header(buffer);
    print_ip_header(buffer);

    struct iphdr *ip = (struct iphdr *) (buffer + sizeof(struct ethhdr));
    size_t  iphdr_len = ip->ihl * 4;

    if (ip->protocol == 6)
    {
        struct tcphdr *tcp = (struct tcphdr*)(buffer + iphdr_len + sizeof(struct ethhdr));
        printf("TCP Header:\n");
        printf("\t|~Source Port: %d\n", ntohs(tcp->source));
        printf("\t|~Destination Port: %d\n", ntohs(tcp->dest));
        printf("\t|~Sequence Number: %d\n", ntohs(tcp->seq));
        printf("\t|~Acknowledgment Number: %d\n", ntohs(tcp->ack_seq));
        printf("\t|~Header Length: %d DWORDS or %d Bytes\n", ntohs(tcp->doff), ntohs(tcp->doff) * 4);
        printf("\t|~-----------Flegs-------------\n");
        printf("\t\t|~Urgent Flag: %d\n", ntohs(tcp->urg));
        printf("\t\t|~Acknowledgement Flag: %d\n", ntohs(tcp->ack));
        printf("\t\t|~Push Flag: %d\n", ntohs(tcp->psh));
        printf("\t\t|~Reset Flag: %d\n", ntohs(tcp->res1));
        printf("\t\t|~Synchronise Flag: %d\n", ntohs(tcp->syn));
        printf("\t\t|~Finish Flag: %d\n", ntohs(tcp->fin));
        printf("\t|~Window Size: %d\n", ntohs(tcp->window));
        printf("\t|~Checksum: %d\n", ntohs(tcp->check));
        printf("\t|~Urgent Pointer: %d\n", ntohs(tcp->urg_ptr));
    }
    else if (ip->protocol == 17)
    {
        struct udphdr *udp = (struct udphdr *) (buffer + iphdr_len + sizeof(struct ethhdr));
        printf("UDP Header:\n");
        printf("\t|~Source Port: %d\n", ntohs(udp->source));
        printf("\t|~Destination Port: %d\n", ntohs(udp->dest));
        printf("\t|~UDP Length: %d\n", ntohs(udp->len));
        printf("\t|~UDP Checksum: %d\n", ntohs(udp->check));
    }
    size_t transport_layer_len = (ip->protocol == 17) ? sizeof(struct udphdr) :
                                  sizeof(struct tcphdr);
            unsigned char *data = (buffer +
                                   iphdr_len +
                                   sizeof(struct ethhdr) +
                                   transport_layer_len);
    long remaining_data = ntohs(ip->tot_len) - (iphdr_len +
                                               sizeof(struct ethhdr) +
                                               transport_layer_len);
    for (ptrdiff_t i{0}; i < ntohs(ip->tot_len); ++i)
    {
        if (i != 0 && i%16 == 0 )
            printf("\n");

        printf(" %.2x ", data[i]);
    }
    memset(buffer, 0, buffer_size);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
