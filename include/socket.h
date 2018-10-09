#ifndef SOCKET_STREAM_LIB_SOCKET_H
#define SOCKET_STREAM_LIB_SOCKET_H

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "filedesc.h"

_FD_NETWORK_NAMESPACE_BEGIN

using flag_t  = int;
using buf_t  = void *;


class socket_base : public Ifd_base
{
public: 
     
    // Type    
    using sock_type_t = uint8_t;

    static constexpr sock_type_t sock_stream    = SOCK_STREAM;
    static constexpr sock_type_t sock_dgram     = SOCK_DGRAM;
    static constexpr sock_type_t sock_raw       = SOCK_RAW;
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr sock_type_t sock_rdm       = SOCK_RDM;
#endif 
    static constexpr sock_type_t sock_seqpacket = SOCK_SEQPACKET;
   
    // Domain type aka address family defines in <sys/socket.h>
    
    // Protocol type aka protocol family defines which is equial to address families

    // Options
    using sock_opt_t = uint16_t;
    static constexpr sock_opt_t so_debug               = SO_DEBUG;                 /* turn on debugging info recording */
    static constexpr sock_opt_t so_acceptconn          = SO_ACCEPTCONN;            /* socket has had listen() */
    static constexpr sock_opt_t so_reusaddr            = SO_REUSEADDR;             /* allow local address reuse */
    static constexpr sock_opt_t so_keepalive           = SO_KEEPALIVE;             /* keep connections alive */
    static constexpr sock_opt_t so_dontroute           = SO_DONTROUTE;             /* just use interface addresses */
    static constexpr sock_opt_t so_broadcast           = SO_BROADCAST;             /* permit sending of broadcast msgs */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr sock_opt_t so_useloopback         = SO_USELOOPBACK;           /* bypass hardware when possible */
    static constexpr sock_opt_t so_linger              = SO_LINGER;                /* linger on close if data present (in ticks) */
#else
    static constexpr sock_opt_t so_linger              = SO_LINGER;                /* linger on close if data present (in seconds) */
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
    static constexpr sock_opt_t so_oobinline           = SO_OOBINLINE;             /* leave received OOB data in line */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr sock_opt_t so_reuseport           = SO_REUSEPORT;             /* allow local address & port reuse */
    static constexpr sock_opt_t so_timestamp           = SO_TIMESTAMP;             /* timestamp received dgram traffic */
    static constexpr sock_opt_t so_timestamp_monotonic = SO_TIMESTAMP_MONOTONIC;   /* Monotonically increasing timestamp on rcvd dgram */
#ifndef __APPLE__
    static constexpr sock_opt_t so_acceptfilter        = SO_ACCEPTFILTER;          /* there is an accept filter */
#else
    static constexpr sock_opt_t so_donttrunc           = SO_DONTTRUNC;              /* APPLE: Retain unread data */
    /*  (ATOMIC proto) */
    static constexpr sock_opt_t so_wantmore            = SO_WANTMORE;	            /* APPLE: Give hint when more data ready */
    static constexpr sock_opt_t so_wantoobflag         = SO_WANTOOBFLAG;            /* APPLE: Want OOB in MSG_FLAG on receive */

#endif  /* (!__APPLE__) */
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */

/*
 * Additional options, not kept in so_options.
 */
    static constexpr sock_opt_t so_sndbuf           = SO_SNDBUF; /* send buffer size */
    static constexpr sock_opt_t so_rcvbuf           = SO_RCVBUF; /* receive buffer size */
    static constexpr sock_opt_t so_sndlowat         = SO_SNDLOWAT; /* send low-water mark */
    static constexpr sock_opt_t so_rcvlowat         = SO_RCVLOWAT; /* receive low-water mark */
    static constexpr sock_opt_t so_sndtimeo         = SO_SNDTIMEO;/* send timeout */
    static constexpr sock_opt_t so_sorcvtimeo       = SO_RCVTIMEO;/* receive timeout */
    static constexpr sock_opt_t so_error            = SO_ERROR; /* get error status and clear */
    static constexpr sock_opt_t so_type             = SO_TYPE;/* get socket type */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr sock_opt_t so_level            = SO_LABEL;/* socket's MAC label */
    static constexpr sock_opt_t so_peerlabel        = SO_PEERLABEL;/* socket's peer MAC label */
#ifdef __APPLE__
    static constexpr sock_opt_t so_nread            = SO_NREAD;/* APPLE: get 1st-packet byte count */
    static constexpr sock_opt_t so_nke              = SO_NKE; /* APPLE: Install socket-level NKE */
    static constexpr sock_opt_t so_nosigpipe        = SO_NOSIGPIPE;/* APPLE: No SIGPIPE on EPIPE */
    static constexpr sock_opt_t so_noaddrerr        = SO_NOADDRERR;/* APPLE: Returns EADDRNOTAVAIL when src is not available anymore */
    static constexpr sock_opt_t so_nwrie            = SO_NWRITE;/* APPLE: Get number of bytes currently in send socket buffer */
    static constexpr sock_opt_t so_reuseshareuid    = SO_REUSESHAREUID;/* APPLE: Allow reuse of port/socket by different userids */
#ifdef __APPLE_API_PRIVATE
    static constexpr sock_opt_t so_notifyconflict   = SO_NOTIFYCONFLICT;/* APPLE: send notification if there is a bind on a port which is already in use */
    static constexpr sock_opt_t so_upcallcloseait   = SO_UPCALLCLOSEWAIT;/* APPLE: block on close until an upcall returns */
#endif
    static constexpr sock_opt_t	so_linger_sec       = SO_LINGER_SEC;/* linger on close if data present (in seconds) */
    static constexpr sock_opt_t so_randomport       = SO_RANDOMPORT;/* APPLE: request local port randomization */
    static constexpr sock_opt_t so_np_extensions    = SO_NP_EXTENSIONS;/* To turn off some POSIX behavior */
#endif

    static constexpr sock_opt_t so_numrcvpkt        = SO_NUMRCVPKT;/* number of datagrams in receive socket buffer */

    static constexpr sock_opt_t so_net_service_type = SO_NET_SERVICE_TYPE;/* Network service type */

    using net_sevice_type = uint16_t;
    static constexpr net_sevice_type best_efort            = NET_SERVICE_TYPE_BE;
    static constexpr net_sevice_type backgrount            = NET_SERVICE_TYPE_BK;
    static constexpr net_sevice_type signaling             = NET_SERVICE_TYPE_SIG;
    static constexpr net_sevice_type inter_video           = NET_SERVICE_TYPE_VI;
    static constexpr net_sevice_type inter_voice           = NET_SERVICE_TYPE_VO;
    static constexpr net_sevice_type responsive_multimedia = NET_SERVICE_TYPE_RV;
    static constexpr net_sevice_type multimedia_streaming  = NET_SERVICE_TYPE_AV;
    static constexpr net_sevice_type oper_admin_manag      = NET_SERVICE_TYPE_OAM;
    static constexpr net_sevice_type responsive_data       = NET_SERVICE_TYPE_RD;

#define	SO_NETSVC_MARKING_LEVEL	0x1119	/* Get QoS marking in effect for socket */

#define	NETSVC_MRKNG_UNKNOWN		0	/* The outgoing network interface is not known */
#define	NETSVC_MRKNG_LVL_L2		1	/* Default marking at layer 2 (for example Wi-Fi WMM) */
#define	NETSVC_MRKNG_LVL_L3L2_ALL	2	/* Layer 3 DSCP marking and layer 2 marking for all Network Service Types */
#define	NETSVC_MRKNG_LVL_L3L2_BK	3	/* The system policy limits layer 3 DSCP marking and layer 2 marking
						 * to background Network Service Types */

typedef __uint32_t sae_associd_t;
#define	SAE_ASSOCID_ANY	0
#define	SAE_ASSOCID_ALL	((sae_associd_t)(-1ULL))

typedef __uint32_t sae_connid_t;
#define	SAE_CONNID_ANY	0
#define	SAE_CONNID_ALL	((sae_connid_t)(-1ULL))

/* connectx() flag parameters */
#define	CONNECT_RESUME_ON_READ_WRITE	0x1 /* resume connect() on read/write */
#define	CONNECT_DATA_IDEMPOTENT		0x2 /* data is idempotent */
#define	CONNECT_DATA_AUTHENTICATED	0x4 /* data includes security that replaces the TFO-cookie */

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define	SOL_SOCKET	0xffff		/* options for socket level */


/*
 * Address families.
 */
#define	AF_UNSPEC	0		/* unspecified */
#define	AF_UNIX		1		/* local to host (pipes) */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
#define	AF_LOCAL	AF_UNIX		/* backward compatibility */
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
#define	AF_INET		2		/* internetwork: UDP, TCP, etc. */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
#define	AF_IMPLINK	3		/* arpanet imp addresses */
#define	AF_PUP		4		/* pup protocols: e.g. BSP */
#define	AF_CHAOS	5		/* mit CHAOS protocols */
#define	AF_NS		6		/* XEROX NS protocols */
#define	AF_ISO		7		/* ISO protocols */
#define	AF_OSI		AF_ISO
#define	AF_ECMA		8		/* European computer manufacturers */
#define	AF_DATAKIT	9		/* datakit protocols */
#define	AF_CCITT	10		/* CCITT protocols, X.25 etc */
#define	AF_SNA		11		/* IBM SNA */
#define	AF_DECnet	12		/* DECnet */
#define	AF_DLI		13		/* DEC Direct data link interface */
#define	AF_LAT		14		/* LAT */
#define	AF_HYLINK	15		/* NSC Hyperchannel */
#define	AF_APPLETALK	16		/* Apple Talk */
#define	AF_ROUTE	17		/* Internal Routing Protocol */
#define	AF_LINK		18		/* Link layer interface */
#define	pseudo_AF_XTP	19		/* eXpress Transfer Protocol (no AF) */
#define	AF_COIP		20		/* connection-oriented IP, aka ST II */
#define	AF_CNT		21		/* Computer Network Technology */
#define	pseudo_AF_RTIP	22		/* Help Identify RTIP packets */
#define	AF_IPX		23		/* Novell Internet Protocol */
#define	AF_SIP		24		/* Simple Internet Protocol */
#define	pseudo_AF_PIP	25		/* Help Identify PIP packets */
#define	AF_NDRV		27		/* Network Driver 'raw' access */
#define	AF_ISDN		28		/* Integrated Services Digital Network */
#define	AF_E164		AF_ISDN		/* CCITT E.164 recommendation */
#define	pseudo_AF_KEY	29		/* Internal key-management function */
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
#define	AF_INET6	30		/* IPv6 */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
#define	AF_NATM		31		/* native ATM access */
#define	AF_SYSTEM	32		/* Kernel event messages */
#define	AF_NETBIOS	33		/* NetBIOS */
#define	AF_PPP		34		/* PPP communication protocol */
#define	pseudo_AF_HDRCMPLT 35		/* Used by BPF to not rewrite headers
					in interface output routine */
#define	AF_RESERVED_36	36		/* Reserved for internal usage */
#define	AF_IEEE80211	37		/* IEEE 802.11 protocol */
#define	AF_UTUN		38
#define	AF_MAX		40
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */

#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
#define	SOCK_MAXADDRLEN	255		/* longest possible addresses */
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
/*
 * Protocol families, same as address families for now.
 */
#define	PF_UNSPEC	AF_UNSPEC
#define	PF_LOCAL	AF_LOCAL
#define	PF_UNIX		PF_LOCAL	/* backward compatibility */
#define	PF_INET		AF_INET
#define	PF_IMPLINK	AF_IMPLINK
#define	PF_PUP		AF_PUP
#define	PF_CHAOS	AF_CHAOS
#define	PF_NS		AF_NS
#define	PF_ISO		AF_ISO
#define	PF_OSI		AF_ISO
#define	PF_ECMA		AF_ECMA
#define	PF_DATAKIT	AF_DATAKIT
#define	PF_CCITT	AF_CCITT
#define	PF_SNA		AF_SNA
#define	PF_DECnet	AF_DECnet
#define	PF_DLI		AF_DLI
#define	PF_LAT		AF_LAT
#define	PF_HYLINK	AF_HYLINK
#define	PF_APPLETALK	AF_APPLETALK
#define	PF_ROUTE	AF_ROUTE
#define	PF_LINK		AF_LINK
#define	PF_XTP		pseudo_AF_XTP	/* really just proto family, no AF */
#define	PF_COIP		AF_COIP
#define	PF_CNT		AF_CNT
#define	PF_SIP		AF_SIP
#define	PF_IPX		AF_IPX		/* same format as AF_NS */
#define	PF_RTIP		pseudo_AF_RTIP	/* same format as AF_INET */
#define	PF_PIP		pseudo_AF_PIP
#define	PF_NDRV		AF_NDRV
#define	PF_ISDN		AF_ISDN
#define	PF_KEY		pseudo_AF_KEY
#define	PF_INET6	AF_INET6
#define	PF_NATM		AF_NATM
#define	PF_SYSTEM	AF_SYSTEM
#define	PF_NETBIOS	AF_NETBIOS
#define	PF_PPP		AF_PPP
#define	PF_RESERVED_36  AF_RESERVED_36
#define	PF_UTUN		AF_UTUN
#define	PF_MAX		AF_MAX

/*
 * These do not have socket-layer support:
 */
#define	PF_VLAN		((uint32_t)0x766c616e)	/* 'vlan' */
#define	PF_BOND		((uint32_t)0x626f6e64)	/* 'bond' */


/*
 * Maximum queue length specifiable by listen.
 */
#define	SOMAXCONN	128
    // Options 
   // States
    
    int send() override;
    int read() override;
    socket_base open() override;
    int close() override;
     ~socket_base() override;
    socket_base();
    void set_option();
    sock_opt_t get_option();
    // 
    //
    // local buffer if there is no opportunety to get date from system buffer
    // c_sock(); -- ? suppose no to keep incapsulation 
    //
    // derived classes:
    //  copy constructor
    // for stream and seq sockets 
    //  is_connected 
private:
    
};

_FD_NETWORK_NAMESPACE_END

#endif //SOCKET_STREAM_LIB_SOCKET_H
