#ifndef SOCKET_STREAM_LIB_SOCKET_H
#define SOCKET_STREAM_LIB_SOCKET_H

#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include "filedesc.h"

_FD_NETWORK_NAMESPACE_BEGIN

namespace 
{
    const fd_buf_sz_t c_bufsz_1    = 1;
    const fd_buf_sz_t c_bufsz_2    = 2;
    const fd_buf_sz_t c_bufsz_4    = 4;  
    const fd_buf_sz_t c_bufsz_8    = 8; 
    const fd_buf_sz_t c_bufsz_16   = 16;
    const fd_buf_sz_t c_bufsz_32   = 32;
    const fd_buf_sz_t c_bufsz_64   = 64;  
    const fd_buf_sz_t c_bufsz_128  = 128;
    const fd_buf_sz_t c_bufsz_256  = 256;
    const fd_buf_sz_t c_bufsz_512  = 512;
    const fd_buf_sz_t c_bufsz_1024 = 1024;  
    const fd_buf_sz_t c_bufsz_2048 = 2048;
    const fd_buf_sz_t c_bufsz_4096 = 4096;
}

class socket_exception : public std::exception
{
public:
    socket_exception()
    {
    }
    socket_exception(const socket_exception& socket_exception) : 
                     m_socket_exception_message(socket_exception.m_socket_exception_message)
    {
    }
    socket_exception(const std::string& exception_message) : 
                     m_socket_exception_message(exception_message)
    {
    }
    socket_exception(const char* exception_message) : 
                     m_socket_exception_message(exception_message)
    {
    }

    const char* what() const noexcept override
    {
        return m_socket_exception_message.c_str();
    }

    ~socket_exception()
    {
    }
private:
    std::string m_socket_exception_message;

};

class socket_base : public Ifd_base
{
public: 
    using c_socket_t = int;
    // Type    
    using sock_type_t = uint16_t;
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

    static constexpr sock_opt_t so_netsvc_marking_level    = SO_NETSVC_MARKING_LEVEL; /* Get QoS marking in effect for socket */
#endif

#ifdef __APPLE__
    static constexpr uint16_t netsvc_mrkng_unknown = NETSVC_MRKNG_UNKNOWN; /* The outgoing network interface is not known */
    static constexpr uint16_t netsvc_mrkng_lvl_l2 = NETSVC_MRKNG_LVL_L2; /* Default marking at layer 2 (for example Wi-Fi WMM) */
    static constexpr uint16_t netsvc_mrkng_lvl_l3l2_all = NETSVC_MRKNG_LVL_L3L2_ALL; /* Layer 3 DSCP marking and layer 2 marking for all Network Service Types */
    static constexpr uint16_t netsvc_mrkng_lvl_l3l2_bk = NETSVC_MRKNG_LVL_L3L2_BK; /* The system policy limits layer 3 DSCP marking and layer 2 marking
						 * to background Network Service Types */

typedef __uint32_t sae_associd_t;
    static constexpr sae_associd_t sae_associd_any = SAE_ASSOCID_ANY;
    static constexpr sae_associd_t sae_associd_all = SAE_ASSOCID_ALL;

typedef __uint32_t sae_connid_t;
    static constexpr sae_connid_t sae_connid_any = SAE_CONNID_ANY;
    static constexpr sae_connid_t sae_connid_all = SAE_CONNID_ALL;
 
/* connectx() flag parameters */
    static constexpr uint16_t connect_resume_on_read_write = CONNECT_RESUME_ON_READ_WRITE; /* resume connect() on read/write */
    static constexpr uint16_t connect_data_idempotent      = CONNECT_DATA_IDEMPOTENT; /* data is idempotent */
    static constexpr uint16_t connect_data_authenticated   = CONNECT_DATA_AUTHENTICATED; /* data includes security that replaces the TFO-cookie */
#endif


/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
    static constexpr auto sol_socket = SOL_SOCKET; /* options for socket level */



/*
 * Address families.
 */
    using address_family_t = uint16_t;
    static constexpr address_family_t af_unspec             = AF_UNSPEC;    /* unspecified */
    static constexpr address_family_t af_unix               = AF_UNIX;      /* local to host (pipes) */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr address_family_t af_local              = AF_LOCAL;     /* backward compatibility */
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
    static constexpr address_family_t af_inet               = AF_INET;      /* internetwork: UDP, TCP, etc. */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr address_family_t af_implink            = AF_IMPLINK;   /* arpanet imp addresses */
    static constexpr address_family_t af_pup                = AF_PUP;       /* pup protocols: e.g. BSP */
    static constexpr address_family_t af_chaos              = AF_CHAOS;     /* mit CHAOS protocols */ 
    static constexpr address_family_t af_ns                 = AF_NS;        /* XEROX NS protocols */
    static constexpr address_family_t af_iso                = AF_ISO;       /* ISO protocols */
    static constexpr address_family_t af_osi                = AF_OSI;
    static constexpr address_family_t af_ecma               = AF_ECMA;      /* European computer manufacturers */
    static constexpr address_family_t af_datakit            = AF_DATAKIT;   /* datakit protocols */
    static constexpr address_family_t af_ccitt              = AF_CCITT;     /* CCITT protocols, X.25 etc */
    static constexpr address_family_t af_sna                = AF_SNA;       /* IBM SNA */
    static constexpr address_family_t af_dec_net            = AF_DECnet;    /* DECnet */
    static constexpr address_family_t af_dli                = AF_DLI;       /* DEC Direct data link interface */
    static constexpr address_family_t af_lat                = AF_LAT;       /* LAT */
    static constexpr address_family_t af_hylink             = AF_HYLINK;    /* NSC Hyperchannel */
    static constexpr address_family_t af_appletalk          = AF_APPLETALK; /* Apple Talk */
    static constexpr address_family_t af_route              = AF_ROUTE;     /* Internal Routing Protocol */
    static constexpr address_family_t af_link               = AF_LINK;      /* Link layer interface */
    static constexpr address_family_t pseudo_af_xtp         = pseudo_AF_XTP;/* eXpress Transfer Protocol (no AF) */
    static constexpr address_family_t af_coip               = AF_COIP;      /* connection-oriented ip, aka st ii */
    static constexpr address_family_t af_cnt                = AF_CNT;       /* Computer Network Technology */
    static constexpr address_family_t pseudo_af_rtip        = pseudo_AF_RTIP;/* Help Identify RTIP packets */
    static constexpr address_family_t af_ipx                = AF_IPX;       /* novell internet protocol */
    static constexpr address_family_t af_sip                = AF_SIP;
    static constexpr address_family_t pseudo_af_pip         = pseudo_AF_PIP;/* Help Identify PIP packets */
    static constexpr address_family_t af_ndrv               = AF_NDRV;      /* Network Driver 'raw' access */
    static constexpr address_family_t af_isdn               = AF_ISDN;      /* Integrated Services Digital Network */
    static constexpr address_family_t af_e164               = AF_E164;      /* CCITT E.164 recommendation */
    static constexpr address_family_t pseudo_af_key         = pseudo_AF_KEY;/* Internal key-management function */
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
    static constexpr address_family_t af_inet6              = AF_INET6;     /* IPv6 */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr address_family_t af_natm               = AF_NATM;      /* native ATM access */
    static constexpr address_family_t af_system             = AF_SYSTEM;    /* Kernel event messages */
    static constexpr address_family_t af_netbios            = AF_NETBIOS;   /* NetBIOS */
    static constexpr address_family_t af_ppp                = AF_PPP;
    static constexpr address_family_t pseudo_af_hdrcmplt    = pseudo_AF_HDRCMPLT;/* Used by BPF to not rewrite headers
					                                                             in interface output routine */
    static constexpr address_family_t af_reserved_36        = AF_RESERVED_36;/* Reserved for internal usage */
    static constexpr address_family_t af_ieee80211          = AF_IEEE80211;/* IEEE 802.11 protocol */
    static constexpr address_family_t af_utun               = AF_UTUN;
    static constexpr address_family_t af_max                = AF_MAX;
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr uint16_t sock_maxaddrlen = SOCK_MAXADDRLEN; 
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */

/*
 * Protocol families, same as address families for now.
 */
    using protocol_family_t = uint16_t;
    static constexpr protocol_family_t pf_unspec      = af_unspec;
    static constexpr protocol_family_t pf_unix        = af_unix;
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr protocol_family_t pf_local       = pf_unix;
#endif
    static constexpr protocol_family_t pf_inet        = af_inet;
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr protocol_family_t pf_implink     = af_implink;
    static constexpr protocol_family_t pf_pup         = af_pup;
    static constexpr protocol_family_t pf_chaos       = af_chaos;
    static constexpr protocol_family_t pf_ns          = af_ns;
    static constexpr protocol_family_t pf_iso         = af_iso;
    static constexpr protocol_family_t pf_osi         = pf_iso;
    static constexpr protocol_family_t pf_ecma        = af_ecma;
    static constexpr protocol_family_t pf_datakit     = af_datakit;
    static constexpr protocol_family_t pf_ccitt       = af_ccitt;
    static constexpr protocol_family_t pf_sna         = af_sna;
    static constexpr protocol_family_t pf_dec_net     = af_dec_net;
    static constexpr protocol_family_t pf_dli         = af_dli;
    static constexpr protocol_family_t pf_lat         = af_lat;
    static constexpr protocol_family_t pf_hylink      = af_hylink;
    static constexpr protocol_family_t pf_appletalk   = af_appletalk;
    static constexpr protocol_family_t pf_route       = af_route;
    static constexpr protocol_family_t pf_link        = af_link;
    static constexpr protocol_family_t pf_xtp         = pseudo_af_xtp;
    static constexpr protocol_family_t pf_coip        = af_coip;
    static constexpr protocol_family_t pf_cnt         = af_cnt;
    static constexpr protocol_family_t pf_sip         = af_sip;
    static constexpr protocol_family_t pf_ipx         = af_ipx;
    static constexpr protocol_family_t pf_rtip        = af_rtip;
    static constexpr protocol_family_t pf_pip         = pseudo_af_pip;
    static constexpr protocol_family_t pf_ndrv        = af_ndrv;
    static constexpr protocol_family_t pf_isdn        = af_isdn;
    static constexpr protocol_family_t pf_key         = pseudo_af_key;
#endif	/* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
    static constexpr protocol_family_t pf_inet6       = af_inet6;
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr protocol_family_t pf_natm        = af_natm;
    static constexpr protocol_family_t pf_system      = af_system;
    static constexpr protocol_family_t pf_netbios     = af_netbios;
    static constexpr protocol_family_t pf_ppp         = af_ppp;
    static constexpr protocol_family_t pf_reserved_36 = af_reserved_36;
    static constexpr protocol_family_t pf_utun        = af_utun;
#endif
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    static constexpr protocol_family_t pf_max         = af_max; 

/*
 * These do not have socket-layer support:
 */
    static constexpr protocol_family_t pf_vlan        = PF_VLAN; /* 'vlan' */
    static constexpr protocol_family_t pf_bond        = PF_BOND; /* 'bond' */
#endif

/*
 * Maximum queue length specifiable by listen.
 */
    static constexpr uint16_t somaconn = SOMAXCONN; 


    // States
    using sock_state = uint16_t;
    static constexpr sock_state valid   = 0x1;
    static constexpr sock_state invalid = 0x2;
    static constexpr sock_state used    = 0x4;
    static constexpr sock_state unused  = 0x8;

    socket_base() : m_state(invalid) 
    {
    }
    socket_base(address_family_t domain    = af_inet, 
                sock_type_t type           = sock_raw, 
                protocol_family_t protocol = 0) : m_domain(domain), 
                                                  m_type(sock_raw), 
                                                  m_protocol(protocol),
                                                  m_state(unused) 
    {
        m_c_socket = socket(m_domain, m_type, m_protocol);

        if (m_c_socket != 0)
        {
            m_state = invalid;
            throw socket_exception("Socket can't be created.");
        }

        m_state |= valid;
    } 

    virtual sock_state open(address_family_t domain, sock_type_t type, protocol_family_t protocol);

    // Возможно стоит выставлять состояние сокета на время чтения/записи в состояние used
    ssize_t write(base_buf_t buffer, fd_buf_sz_t size) override;
    ssize_t read(base_buf_t buffer, fd_buf_sz_t size) override;

    void close() override;
    
    
    void set_option(int socket,
                    int level, 
                    int option_name, 
                    const void *option_value,
                    socklen_t option_len);
    sock_opt_t get_option();

    bool is_valid();

    ~socket_base() noexcept override
    {
        try 
        {
            ::close(m_c_socket);
        }
        catch(socket_exception& e)
        {
            std::cerr << e.what() + '\n';
            m_state = invalid;
        }
        catch(...)
        {

        }
    }
    // local buffer if there is no opportunety to get date from system buffer
    // c_sock(); -- ? suppose no to keep incapsulation 
    //
    // derived classes:
    // copy constructor
    // for stream and seq sockets 
private:
    c_socket_t m_c_socket;
    
    address_family_t m_domain;
    sock_type_t m_type;
    protocol_family_t m_protocol;    
    
    sock_state m_state;
    // Deleted for sockets 
    using Ifd_base::open;
};

_FD_NETWORK_NAMESPACE_END

#endif //SOCKET_STREAM_LIB_SOCKET_H
