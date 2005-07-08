#ifndef __FAR_PLUGIN_CONNECT_CONNECTIONS
#define __FAR_PLUGIN_CONNECT_CONNECTIONS

PRESTRUCT( Connection );

struct comvars {
  int          connect;
  char         name[ MAXHOSTNAMELEN ];
  sockaddr_in  mctl;
  sockaddr_in  hctl;
  FILE        *in;
  FILE        *out;
  ftTypes      tpe;
  int          cpnd;
  int          sunqe;
  int          runqe;
};

/*
 * Format of command table.
 */
struct cmd {
  char *c_name;         /* name of command */
  BYTE  c_conn;         /* must be connected to use command */
  BYTE  c_proxy;        /* proxy server may execute */
  BYTE  c_args;         /* minimal parameters number */
};

//[ftp_Connect.cpp]
extern cmd cmdtabdata[];

struct FFtpCacheItem {
  char  DirName[1024];
  char *Listing;
  int   ListingSize;
};

STRUCT( ConnectionState )
  BOOL      Inited;
  int       Blocked;
  int       RetryCount;
  int       TableNum;
  int       Passive;
  HANDLE    Object;

  ConnectionState( void ) { Inited = FALSE; }
};

STRUCT( Connection )
    void       ExecCmdTab(struct cmd *c,int argc,char *argv[]);
    void       Gcat(register char *s1, register char *s2);
    void       Gmatch(register char *s, register char *p);
    void       abortpt();
    void       account(int argc,char **argv);
    void       acollect(register char *as);
    void       addpath(char c);
    int        amatch(register char *s, register char *p);
    int        any(register int c, register char *s);
    char**     blkcpy(char **oav, register char **bv);
    int        blklen(register char **av);
    void       cd(int argc, char *argv[]);
    void       cdup();
    void       collect(register char *as);
    char**     copyblk(register char **v);
    SOCKET     dataconn( void );
    void       deleteFile(int argc, char *argv[]);
    int        digit(register char c);
    void       disconnect();
    void       do_chmod(int argc, char *argv[]);
    void       do_umask(int argc, char *argv[]);
    void       doproxy(int argc, char *argv[]);
    int        empty(struct fd_set *mask, int sec);
    int        execbrc(char *p, char *s);
    void       expand(char *as);
    void       get(int argc, char *argv[]);
    cmd       *getcmd(register char *name);
    int        gethdir(char *home);
    int        getit(int argc, char *argv[], int restartit, char *mode);
    int        getreply( BOOL expecteof, DWORD tm = MAX_DWORD );
    void       ginit(char **agargv);
    BOOL       hookup( char *host, int port );
    void       idle(int argc, char *argv[]);
    BOOL       initconn();
    int        letter(register char c);
    int        login( void );
    void       lostpeer();
    void       ls(int argc, char *argv[]);
    void       makeargv();
    void       makedir(int argc, char *argv[]);
    int        match(char *s, char *p);
    void       matchdir(char *pattern);
    void       modtime(int argc, char *argv[]);
    void       newer(int argc, char *argv[]);
    void       proxtrans(char *cmd, char *local, char *remote);
    void       pswitch(int flag);
    void       put(int argc, char *argv[]);
    void       pwd();
    void       quit();
    void       quote(int argc, char *argv[]);
    void       recvrequestINT(char *cmd, char *local, char *remote, char *mode );
    void       recvrequest(char *cmd, char *local, char *remote, char *mode );
    void       reget(int argc, char *argv[]);
    void       removedir(int argc, char *argv[]);
    void       renamefile(int argc, char *argv[]);
    void       reset();
    void       restart(int argc, char *argv[]);
    void       rmthelp(int argc, char *argv[]);
    void       rmtstatus(int argc, char *argv[]);
    void       rscan(register char **t, int (*f)());
    void       sendrequestINT(char *cmd, char *local, char *remote );
    void       sendrequest(char *cmd, char *local, char *remote );
    void       setcase();
    int        setpeer(int argc, char *argv[]);
    void       setport();
    void       setrunique();
    void       setsunique();

    BOOL       setascii();
    BOOL       setbinary();
    BOOL       setebcdic();
    BOOL       settenex();
    BOOL       settype( ftTypes Mode,CONSTSTR Arg );

    void       site(int argc, char *argv[]);
    void       sizecmd(int argc, char *argv[]);
    char      *slurpstring();
    char*      strend(register char *cp);
    char*      strspl(register char *cp, register char *dp);
    void       syst();
    int        user(int argc, char **argv);

    int        nb_waitstate( SOCKET *peer, int state,DWORD tm = MAX_DWORD );
    BOOL       nb_connect( SOCKET *peer, struct sockaddr FAR* addr, int addrlen);
    int        nb_recv( SOCKET *peer, LPVOID buf, int len, int flags,DWORD tm = MAX_DWORD );
    int        nb_send( SOCKET *peer, LPCVOID buf, int len, int flags);
    int        fgetcSocket( SOCKET s,DWORD tm = MAX_DWORD );
    BOOL       fprintfSocket( SOCKET s, CONSTSTR format, ...);
    BOOL       fputsSocket( CONSTSTR format, SOCKET s);

    BOOL       SetType( int type );
    void       ResetOutput();
    void       AddOutput(BYTE *Data,int Size);

    comvars       proxstruct, tmpstruct;
    sockaddr_in   hisctladdr;
    sockaddr_in   data_addr;
    BOOL          brk_flag;
    sockaddr_in   myctladdr;
    __int64       restart_point;
    SOCKET        cin, cout;
    char          hostname[ FAR_MAX_PATHSIZE ];
    int           slrflag;
    int           portnum;

    /* Lot's of options... */
    /*
     * Options and other state info.
     */
    int           sendport;   /* use PORT cmd for each data connection */
    int           proxy;      /* proxy server connection active */
    int           proxflag;   /* proxy connection exists */
    int           sunique;    /* store files on server with unique name */
    int           runique;    /* store local files with unique name */
    int           code;       /* return/reply code for ftp command */
    char          pasv[64];   /* passive port for proxy data connection */
    char         *altarg;     /* argv[1] with no shell-like preprocessing  */

    ftTypes       type;          /* file transfer type */
    int           stru;          /* file transfer structure */
    char          bytename[32];  /* local byte size in ascii */
    int           bytesize;      /* local byte size in binary */

    String        line;          /* input line buffer */
    char         *stringbase;    /* current scan point in line buffer */
    String        argbuf;        /* argument storage buffer */
    char         *argbase;       /* current storage point in arg buffer */
    int           margc;         /* count of arguments on input line */
    char         *margv[20];     /* args parsed from input line */
    int           cpend;         /* flag: if != 0, then pending server reply */
    int           mflag;         /* flag: if != 0, then active multi command */

    FFtpCacheItem ListCache[16];
    int           ListCachePos;
  public:
    char          UserName[FAR_MAX_NAME];
    char          UserPassword[FAR_MAX_NAME];
    BYTE         *Output;
    int           OutputSize;
    int           OutputPos;
    int           LastUsedTableNum;
    int           connected;  /* connected to server */
    SOCKET        cmd_peer,
                  data_peer;
    int           SocketError;
    String        CurDir;
    char          SystemInfo[512];
    int           SystemInfoFilled;
    int           TableNum;
    int           ErrorCode;
    BOOL          Breakable;
  public:
//
// Host options
//
    FTPHostPlugin Host;
  protected:
//
//JM added
//
    int            cmdLineSize;
    int            cmdSize;
    String         StartReply;
    String         reply_string;
    FCmdLine      *CmdBuff;               //[cmdSize+1][cmdLineSize+1]
    FCmdLine      *RplBuff;               //[cmdSize+1][1024+1]
    FMenuLine     *CmdMsg;
    int            cmdCount;
    char           LastHost[FAR_MAX_PATHSIZE];
    char           LastMsg[FAR_MAX_PATHSIZE];
    char          *IOBuff;
    HANDLE         hIdle;
  public:
    FTPCurrentStates CurrentState;
    char           DirFile[ FAR_MAX_PATHSIZE ];
    int            RetryCount;
    PFTPProgress   TrafficInfo;
    BOOL           CmdVisible;
    BOOL           ResumeSupport;
    BOOL           IOCallback;
    //Completitions
    BOOL           LoginComplete;
//
//JM added END
//
  protected:
    void           InternalError( void );

  protected:
    void           CloseCmdBuff( void );
    void           CloseIOBuff( void );
    void           ResetCmdBuff( void );
    void           SetCmdLine( char *dest,CONSTSTR src,int sz,int out );
    BOOL           SendAbort( SOCKET din );
  public:
    Connection();
    ~Connection();

    void           InitData( PFTPHost p,int blocked /*=TRUE,FALSE,-1*/ );
    void           InitCmdBuff( void );
    void           InitIOBuff( void );
    BOOL           Init( CONSTSTR Host,CONSTSTR Port,CONSTSTR User,CONSTSTR Password );

    int            command(char *fmt, ...);
    int            ProcessCommand( CONSTSTR LineToProcess);
    int            ProcessCommand( String& s ) { return ProcessCommand( s.c_str() ); }
    void           CheckResume( void );
    void           AbortAllRequest(int brkFlag);

    void           GetOutput( String& s );
    void           GetReply( BYTE *Line,int MaxLength)             { StrCpy( (char*)Line, reply_string.c_str(), MaxLength ); }
    void           GetReply( String& s )                           { s = reply_string; }
    CONSTSTR       GetStartReply( void )                           { return StartReply.c_str(); }

    void           CacheReset();
    int            CacheGet();
    void           CacheAdd();

    void           SetTable( int Table )          { TableNum = Table; }
    int            GetTable( void )               { return TableNum; }

    void           FromOEM( BYTE *Line,int sz = -1 );
    void           FromOEM( char *Line )              { FromOEM( (LPBYTE)Line,-1 ); }
    void           FromOEM( String& s )               { FromOEM( (LPBYTE)s.c_str(),-1 ); }

    void           ToOEM( BYTE *Line,int sz = -1 );
    void           ToOEM( char *Line )                { ToOEM( (LPBYTE)Line,-1 ); }
    void           ToOEM( String& s )                 { ToOEM( (LPBYTE)s.c_str(),-1 ); }

    char          *FromOEMDup( CONSTSTR str,int num = 0 );
    char          *ToOEMDup( CONSTSTR str,int num = 0 );
    String         SFromOEM( CONSTSTR str );
    String         SToOEM( CONSTSTR str );
    String         SFromOEM( const String& str ) { return FromOEMDup(str.c_str()); }
    String         SToOEM( const String& str )   { return ToOEMDup(str.c_str()); }

    void           GetState( PConnectionState p );
    void           SetState( PConnectionState p );

    BOOL           GetExitCode();
    int            GetResultCode( void )          { return code; }
    int            GetErrorCode( void )           { return ErrorCode; }

    void           AddCmdLine( CONSTSTR str );
    int            ConnectMessage( int Msg = MNone__,CONSTSTR HostName = NULL,int BtnMsg = MNone__,int btn1 = MNone__, int btn2 = MNone__ );
    BOOL           ConnectMessageTimeout( int Msg = MNone__,CONSTSTR HostName = NULL,int BtnMsg = MNone__ );
};


#endif
