#ifndef INCluasubDRVOH
#define INCluasubDRVOH
typedef enum {
    luasubDRVO_NEVER,
    luasubDRVO_ON_CHANGE,
    luasubDRVO_ALWAYS,
    luasubDRVO_ON_VALID
}luasubDRVO;
#endif

#ifndef INCluasubOVVLH
#define INCluasubOVVLH
typedef enum {
    luasubOVVL_INVALID,
    luasubOVVL_VALID
}luasubOVVL;
#endif

#ifndef INCluasubACTPH
#define INCluasubACTPH
typedef enum {
    luasubACTP_FUNORCHK,
    luasubACTP_FUNCTION,
    luasubACTP_CHUNK,
}luasubACTP;
#endif

#ifndef INCluasubPCTPH
#define INCluasubPCTPH
typedef enum {
    luasubPCTP_UNKNOWN,
    luasubPCTP_ERROR,
    luasubPCTP_FUNCTION,
    luasubPCTP_CHUNK
}luasubPCTP;
#endif

#ifndef INCluasubH
#define INCluasubH

#include <dbBase.h>
#include <epicsTypes.h>
#include <link.h>
#include <epicsMutex.h>
#include <ellLib.h>
#include <epicsTime.h>

struct luasubRecord;
typedef struct luasubRecord {
    char        name[61];   /* Record Name */
    char        desc[41];   /* Descriptor */
    char        asg[29];    /* Access Security Group */
    epicsEnum16 scan;   /* Scan Mechanism */
    epicsEnum16 pini;   /* Process at iocInit */
    epicsInt16  phas;   /* Scan Phase */
    epicsInt16  evnt;   /* Event Number */
    epicsInt16  tse;    /* Time Stamp Event */
    DBLINK      tsel;   /* Time Stamp Link */
    epicsEnum16 dtyp;   /* Device Type */
    epicsInt16  disv;   /* Disable Value */
    epicsInt16  disa;   /* Disable */
    DBLINK      sdis;   /* Scanning Disable */
    epicsMutexId    mlok;   /* Monitor lock */
    ELLLIST     mlis;   /* Monitor List */
    epicsUInt8  disp;   /* Disable putField */
    epicsUInt8  proc;   /* Force Processing */
    epicsEnum16 stat;   /* Alarm Status */
    epicsEnum16 sevr;   /* Alarm Severity */
    epicsEnum16 nsta;   /* New Alarm Status */
    epicsEnum16 nsev;   /* New Alarm Severity */
    epicsEnum16 acks;   /* Alarm Ack Severity */
    epicsEnum16 ackt;   /* Alarm Ack Transient */
    epicsEnum16 diss;   /* Disable Alarm Sevrty */
    epicsUInt8  lcnt;   /* Lock Count */
    epicsUInt8  pact;   /* Record active */
    epicsUInt8  putf;   /* dbPutField process */
    epicsUInt8  rpro;   /* Reprocess  */
    struct asgMember *asp;  /* Access Security Pvt */
    struct putNotify *ppn;  /* addr of PUTNOTIFY */
    struct putNotifyRecord *ppnr;   /* pputNotifyRecord */
    struct scan_element *spvt;  /* Scan Private */
    struct rset *rset;  /* Address of RSET */
    struct dset *dset;  /* DSET address */
    void        *dpvt;  /* Device Private */
    struct dbRecordType *rdes;  /* Address of dbRecordType */
    struct lockRecord *lset;    /* Lock Set */
    epicsEnum16 prio;   /* Scheduling Priority */
    epicsUInt8  tpro;   /* Trace Processing */
    char bkpt;  /* Break Point */
    epicsUInt8  udf;    /* Undefined */
    epicsTimeStamp  time;   /* Time */
    DBLINK      flnk;   /* Forward Process Link */
    epicsInt32  val;    /* Subr. return value */
    epicsInt32  oval;   /* Old return value */
    DBLINK      inp;    /* Input Specification */
    char        icod[41];   /* Initialization Code */
    epicsInt16  actp;   /* Allowed Code Type */
    epicsUInt8  rst;    /* Reset Execution Stack */
    char        pcod[41];   /* Processing Code */
    char        ocod[41];   /* Old Processing Code */
    epicsUInt16 yldd;   /* Is Yielded? */
    epicsEnum16 pctp;   /* Processing Code Type */
    epicsInt16  prec;   /* Display Precision */
    epicsEnum16 drvo;   /* Drive Outputs */
    DBLINK      inpa;   /* Input Link A */
    DBLINK      inpb;   /* Input Link B */
    DBLINK      inpc;   /* Input Link C */
    DBLINK      inpd;   /* Input Link D */
    DBLINK      inpe;   /* Input Link E */
    DBLINK      inpf;   /* Input Link F */
    DBLINK      inpg;   /* Input Link G */
    DBLINK      inph;   /* Input Link H */
    DBLINK      inpi;   /* Input Link I */
    DBLINK      inpj;   /* Input Link J */
    DBLINK      inpk;   /* Input Link K */
    DBLINK      inpl;   /* Input Link L */
    DBLINK      inpm;   /* Input Link M */
    DBLINK      inpn;   /* Input Link N */
    DBLINK      inpo;   /* Input Link O */
    DBLINK      inpp;   /* Input Link P */
    DBLINK      inpq;   /* Input Link Q */
    DBLINK      inpr;   /* Input Link R */
    DBLINK      inps;   /* Input Link S */
    DBLINK      inpt;   /* Input Link T */
    DBLINK      inpu;   /* Input Link U */
    DBLINK      inpv;   /* Input Link V */
    DBLINK      inpw;   /* Input Link W */
    DBLINK      inpx;   /* Input Link X */
    DBLINK      inpy;   /* Input Link Y */
    DBLINK      inpz;   /* Input Link Z */
    void *a;    /* Input value A */
    void *b;    /* Input value B */
    void *c;    /* Input value C */
    void *d;    /* Input value D */
    void *e;    /* Input value E */
    void *f;    /* Input value F */
    void *g;    /* Input value G */
    void *h;    /* Input value H */
    void *i;    /* Input value I */
    void *j;    /* Input value J */
    void *k;    /* Input value K */
    void *l;    /* Input value L */
    void *m;    /* Input value M */
    void *n;    /* Input value N */
    void *o;    /* Input value O */
    void *p;    /* Input value P */
    void *q;    /* Input value Q */
    void *r;    /* Input value R */
    void *s;    /* Input value S */
    void *t;    /* Input value T */
    void *u;    /* Input value U */
    void *v;    /* Input value V */
    void *w;    /* Input value W */
    void *x;    /* Input value X */
    void *y;    /* Input value Y */
    void *z;    /* Input value Z */
    epicsEnum16 fta;    /* Type of A */
    epicsEnum16 ftb;    /* Type of B */
    epicsEnum16 ftc;    /* Type of C */
    epicsEnum16 ftd;    /* Type of D */
    epicsEnum16 fte;    /* Type of E */
    epicsEnum16 ftf;    /* Type of F */
    epicsEnum16 ftg;    /* Type of G */
    epicsEnum16 fth;    /* Type of H */
    epicsEnum16 fti;    /* Type of I */
    epicsEnum16 ftj;    /* Type of J */
    epicsEnum16 ftk;    /* Type of K */
    epicsEnum16 ftl;    /* Type of L */
    epicsEnum16 ftm;    /* Type of M */
    epicsEnum16 ftn;    /* Type of N */
    epicsEnum16 fto;    /* Type of O */
    epicsEnum16 ftp;    /* Type of P */
    epicsEnum16 ftq;    /* Type of Q */
    epicsEnum16 ftr;    /* Type of R */
    epicsEnum16 fts;    /* Type of S */
    epicsEnum16 ftt;    /* Type of T */
    epicsEnum16 ftu;    /* Type of U */
    epicsEnum16 ftv;    /* Type of V */
    epicsEnum16 ftw;    /* Type of W */
    epicsEnum16 ftx;    /* Type of X */
    epicsEnum16 fty;    /* Type of Y */
    epicsEnum16 ftz;    /* Type of Z */
    epicsUInt32 noa;    /* Max. elements in A */
    epicsUInt32 nob;    /* Max. elements in B */
    epicsUInt32 noc;    /* Max. elements in C */
    epicsUInt32 nod;    /* Max. elements in D */
    epicsUInt32 noe;    /* Max. elements in E */
    epicsUInt32 nof;    /* Max. elements in F */
    epicsUInt32 nog;    /* Max. elements in G */
    epicsUInt32 noh;    /* Max. elements in H */
    epicsUInt32 noi;    /* Max. elements in I */
    epicsUInt32 noj;    /* Max. elements in J */
    epicsUInt32 nok;    /* Max. elements in K */
    epicsUInt32 nol;    /* Max. elements in L */
    epicsUInt32 nom;    /* Max. elements in M */
    epicsUInt32 non;    /* Max. elements in N */
    epicsUInt32 noo;    /* Max. elements in O */
    epicsUInt32 nop;    /* Max. elements in P */
    epicsUInt32 noq;    /* Max. elements in Q */
    epicsUInt32 nor;    /* Max. elements in R */
    epicsUInt32 nos;    /* Max. elements in S */
    epicsUInt32 not;    /* Max. elements in T */
    epicsUInt32 nou;    /* Max. elements in U */
    epicsUInt32 nov;    /* Max. elements in V */
    epicsUInt32 now;    /* Max. elements in W */
    epicsUInt32 nox;    /* Max. elements in X */
    epicsUInt32 noy;    /* Max. elements in Y */
    epicsUInt32 noz;    /* Max. elements in Z */
    epicsUInt32 nea;    /* Num. elements in A */
    epicsUInt32 neb;    /* Num. elements in B */
    epicsUInt32 nec;    /* Num. elements in C */
    epicsUInt32 ned;    /* Num. elements in D */
    epicsUInt32 nee;    /* Num. elements in E */
    epicsUInt32 nef;    /* Num. elements in F */
    epicsUInt32 neg;    /* Num. elements in G */
    epicsUInt32 neh;    /* Num. elements in H */
    epicsUInt32 nei;    /* Num. elements in I */
    epicsUInt32 nej;    /* Num. elements in J */
    epicsUInt32 nek;    /* Num. elements in K */
    epicsUInt32 nel;    /* Num. elements in L */
    epicsUInt32 nem;    /* Num. elements in M */
    epicsUInt32 nen;    /* Num. elements in N */
    epicsUInt32 neo;    /* Num. elements in O */
    epicsUInt32 nep;    /* Num. elements in P */
    epicsUInt32 neq;    /* Num. elements in Q */
    epicsUInt32 ner;    /* Num. elements in R */
    epicsUInt32 nes;    /* Num. elements in S */
    epicsUInt32 net;    /* Num. elements in T */
    epicsUInt32 neu;    /* Num. elements in U */
    epicsUInt32 nev;    /* Num. elements in V */
    epicsUInt32 new;    /* Num. elements in W */
    epicsUInt32 nex;    /* Num. elements in X */
    epicsUInt32 ney;    /* Num. elements in Y */
    epicsUInt32 nez;    /* Num. elements in Z */
    DBLINK      outa;   /* Output Link A */
    DBLINK      outb;   /* Output Link B */
    DBLINK      outc;   /* Output Link C */
    DBLINK      outd;   /* Output Link D */
    DBLINK      oute;   /* Output Link E */
    DBLINK      outf;   /* Output Link F */
    DBLINK      outg;   /* Output Link G */
    DBLINK      outh;   /* Output Link H */
    DBLINK      outi;   /* Output Link I */
    DBLINK      outj;   /* Output Link J */
    DBLINK      outk;   /* Output Link K */
    DBLINK      outl;   /* Output Link L */
    DBLINK      outm;   /* Output Link M */
    DBLINK      outn;   /* Output Link N */
    DBLINK      outo;   /* Output Link O */
    DBLINK      outp;   /* Output Link P */
    DBLINK      outq;   /* Output Link Q */
    DBLINK      outr;   /* Output Link R */
    DBLINK      outs;   /* Output Link S */
    DBLINK      outt;   /* Output Link T */
    DBLINK      outu;   /* Output Link U */
    DBLINK      outv;   /* Output Link V */
    DBLINK      outw;   /* Output Link W */
    DBLINK      outx;   /* Output Link X */
    DBLINK      outy;   /* Output Link Y */
    DBLINK      outz;   /* Output Link Z */
    void *vala; /* Output value A */
    void *valb; /* Output value B */
    void *valc; /* Output value C */
    void *vald; /* Output value D */
    void *vale; /* Output value E */
    void *valf; /* Output value F */
    void *valg; /* Output value G */
    void *valh; /* Output value H */
    void *vali; /* Output value I */
    void *valj; /* Output value J */
    void *valk; /* Output value K */
    void *vall; /* Output value L */
    void *valm; /* Output value M */
    void *valn; /* Output value N */
    void *valo; /* Output value O */
    void *valp; /* Output value P */
    void *valq; /* Output value Q */
    void *valr; /* Output value R */
    void *vals; /* Output value S */
    void *valt; /* Output value T */
    void *valu; /* Output value U */
    void *valv; /* Output value V */
    void *valw; /* Output value W */
    void *valx; /* Output value X */
    void *valy; /* Output value Y */
    void *valz; /* Output value Z */
    void *ovla; /* Old Output A */
    void *ovlb; /* Old Output B */
    void *ovlc; /* Old Output C */
    void *ovld; /* Old Output D */
    void *ovle; /* Old Output E */
    void *ovlf; /* Old Output F */
    void *ovlg; /* Old Output G */
    void *ovlh; /* Old Output H */
    void *ovli; /* Old Output I */
    void *ovlj; /* Old Output J */
    void *ovlk; /* Old Output K */
    void *ovll; /* Old Output L */
    void *ovlm; /* Old Output M */
    void *ovln; /* Old Output N */
    void *ovlo; /* Old Output O */
    void *ovlp; /* Old Output P */
    void *ovlq; /* Old Output Q */
    void *ovlr; /* Old Output R */
    void *ovls; /* Old Output S */
    void *ovlt; /* Old Output T */
    void *ovlu; /* Old Output U */
    void *ovlv; /* Old Output V */
    void *ovlw; /* Old Output W */
    void *ovlx; /* Old Output X */
    void *ovly; /* Old Output Y */
    void *ovlz; /* Old Output Z */
    epicsEnum16 ftva;   /* Type of VALA */
    epicsEnum16 ftvb;   /* Type of VALB */
    epicsEnum16 ftvc;   /* Type of VALC */
    epicsEnum16 ftvd;   /* Type of VALD */
    epicsEnum16 ftve;   /* Type of VALE */
    epicsEnum16 ftvf;   /* Type of VALF */
    epicsEnum16 ftvg;   /* Type of VALG */
    epicsEnum16 ftvh;   /* Type of VALH */
    epicsEnum16 ftvi;   /* Type of VALI */
    epicsEnum16 ftvj;   /* Type of VALJ */
    epicsEnum16 ftvk;   /* Type of VALK */
    epicsEnum16 ftvl;   /* Type of VALL */
    epicsEnum16 ftvm;   /* Type of VALM */
    epicsEnum16 ftvn;   /* Type of VALN */
    epicsEnum16 ftvo;   /* Type of VALO */
    epicsEnum16 ftvp;   /* Type of VALP */
    epicsEnum16 ftvq;   /* Type of VALQ */
    epicsEnum16 ftvr;   /* Type of VALR */
    epicsEnum16 ftvs;   /* Type of VALS */
    epicsEnum16 ftvt;   /* Type of VALT */
    epicsEnum16 ftvu;   /* Type of VALU */
    epicsEnum16 ftvv;   /* Type of VALV */
    epicsEnum16 ftvw;   /* Type of VALW */
    epicsEnum16 ftvx;   /* Type of VALX */
    epicsEnum16 ftvy;   /* Type of VALY */
    epicsEnum16 ftvz;   /* Type of VALZ */
    epicsUInt32 nova;   /* Max. elements in VALA */
    epicsUInt32 novb;   /* Max. elements in VALB */
    epicsUInt32 novc;   /* Max. elements in VALC */
    epicsUInt32 novd;   /* Max. elements in VALD */
    epicsUInt32 nove;   /* Max. elements in VALE */
    epicsUInt32 novf;   /* Max. elements in VALF */
    epicsUInt32 novg;   /* Max. elements in VALG */
    epicsUInt32 novh;   /* Max. elements in VAlH */
    epicsUInt32 novi;   /* Max. elements in VALI */
    epicsUInt32 novj;   /* Max. elements in VALJ */
    epicsUInt32 novk;   /* Max. elements in VALK */
    epicsUInt32 novl;   /* Max. elements in VALL */
    epicsUInt32 novm;   /* Max. elements in VALM */
    epicsUInt32 novn;   /* Max. elements in VALN */
    epicsUInt32 novo;   /* Max. elements in VALO */
    epicsUInt32 novp;   /* Max. elements in VALP */
    epicsUInt32 novq;   /* Max. elements in VALQ */
    epicsUInt32 novr;   /* Max. elements in VALR */
    epicsUInt32 novs;   /* Max. elements in VALS */
    epicsUInt32 novt;   /* Max. elements in VALT */
    epicsUInt32 novu;   /* Max. elements in VALU */
    epicsUInt32 novv;   /* Max. elements in VALV */
    epicsUInt32 novw;   /* Max. elements in VALW */
    epicsUInt32 novx;   /* Max. elements in VALX */
    epicsUInt32 novy;   /* Max. elements in VALY */
    epicsUInt32 novz;   /* Max. elements in VALZ */
    epicsUInt32 neva;   /* Num. elements in VALA */
    epicsUInt32 nevb;   /* Num. elements in VALB */
    epicsUInt32 nevc;   /* Num. elements in VALC */
    epicsUInt32 nevd;   /* Num. elements in VALD */
    epicsUInt32 neve;   /* Num. elements in VALE */
    epicsUInt32 nevf;   /* Num. elements in VALF */
    epicsUInt32 nevg;   /* Num. elements in VALG */
    epicsUInt32 nevh;   /* Num. elements in VAlH */
    epicsUInt32 nevi;   /* Num. elements in VALI */
    epicsUInt32 nevj;   /* Num. elements in VALJ */
    epicsUInt32 nevk;   /* Num. elements in VALK */
    epicsUInt32 nevl;   /* Num. elements in VALL */
    epicsUInt32 nevm;   /* Num. elements in VALM */
    epicsUInt32 nevn;   /* Num. elements in VALN */
    epicsUInt32 nevo;   /* Num. elements in VALO */
    epicsUInt32 nevp;   /* Num. elements in VALP */
    epicsUInt32 nevq;   /* Num. elements in VALQ */
    epicsUInt32 nevr;   /* Num. elements in VALR */
    epicsUInt32 nevs;   /* Num. elements in VALS */
    epicsUInt32 nevt;   /* Num. elements in VALT */
    epicsUInt32 nevu;   /* Num. elements in VALU */
    epicsUInt32 nevv;   /* Num. elements in VALV */
    epicsUInt32 nevw;   /* Num. elements in VALW */
    epicsUInt32 nevx;   /* Num. elements in VALX */
    epicsUInt32 nevy;   /* Num. elements in VALY */
    epicsUInt32 nevz;   /* Num. elements in VALZ */
    epicsUInt32 onva;   /* Num. elements in OVLA */
    epicsUInt32 onvb;   /* Num. elements in OVLB */
    epicsUInt32 onvc;   /* Num. elements in OVLC */
    epicsUInt32 onvd;   /* Num. elements in OVLD */
    epicsUInt32 onve;   /* Num. elements in OVLE */
    epicsUInt32 onvf;   /* Num. elements in OVLF */
    epicsUInt32 onvg;   /* Num. elements in OVLG */
    epicsUInt32 onvh;   /* Num. elements in VAlH */
    epicsUInt32 onvi;   /* Num. elements in OVLI */
    epicsUInt32 onvj;   /* Num. elements in OVLJ */
    epicsUInt32 onvk;   /* Num. elements in OVLK */
    epicsUInt32 onvl;   /* Num. elements in OVLL */
    epicsUInt32 onvm;   /* Num. elements in OVLM */
    epicsUInt32 onvn;   /* Num. elements in OVLN */
    epicsUInt32 onvo;   /* Num. elements in OVLO */
    epicsUInt32 onvp;   /* Num. elements in OVLP */
    epicsUInt32 onvq;   /* Num. elements in OVLQ */
    epicsUInt32 onvr;   /* Num. elements in OVLR */
    epicsUInt32 onvs;   /* Num. elements in OVLS */
    epicsUInt32 onvt;   /* Num. elements in OVLT */
    epicsUInt32 onvu;   /* Num. elements in OVLU */
    epicsUInt32 onvv;   /* Num. elements in OVLV */
    epicsUInt32 onvw;   /* Num. elements in OVLW */
    epicsUInt32 onvx;   /* Num. elements in OVLX */
    epicsUInt32 onvy;   /* Num. elements in OVLY */
    epicsUInt32 onvz;   /* Num. elements in OVLZ */
    epicsEnum16 vlda;   /* Output Valid VALA */
    epicsEnum16 vldb;   /* Output Valid VALB */
    epicsEnum16 vldc;   /* Output Valid VALC */
    epicsEnum16 vldd;   /* Output Valid VALD */
    epicsEnum16 vlde;   /* Output Valid VALE */
    epicsEnum16 vldf;   /* Output Valid VALF */
    epicsEnum16 vldg;   /* Output Valid VALG */
    epicsEnum16 vldh;   /* Output Valid VALH */
    epicsEnum16 vldi;   /* Output Valid VALI */
    epicsEnum16 vldj;   /* Output Valid VALJ */
    epicsEnum16 vldk;   /* Output Valid VALK */
    epicsEnum16 vldl;   /* Output Valid VALL */
    epicsEnum16 vldm;   /* Output Valid VALM */
    epicsEnum16 vldn;   /* Output Valid VALN */
    epicsEnum16 vldo;   /* Output Valid VALO */
    epicsEnum16 vldp;   /* Output Valid VALP */
    epicsEnum16 vldq;   /* Output Valid VALQ */
    epicsEnum16 vldr;   /* Output Valid VALR */
    epicsEnum16 vlds;   /* Output Valid VALS */
    epicsEnum16 vldt;   /* Output Valid VALT */
    epicsEnum16 vldu;   /* Output Valid VALU */ 
    epicsEnum16 vldv;   /* Output Valid VALV */
    epicsEnum16 vldw;   /* Output Valid VALW */
    epicsEnum16 vldx;   /* Output Valid VALX */
    epicsEnum16 vldy;   /* Output Valid VALY */
    epicsEnum16 vldz;   /* Output Valid VALZ */ 
} luasubRecord;

#define luasubRecordNAME    0
#define luasubRecordDESC    1
#define luasubRecordASG     2
#define luasubRecordSCAN    3
#define luasubRecordPINI    4
#define luasubRecordPHAS    5
#define luasubRecordEVNT    6
#define luasubRecordTSE     7
#define luasubRecordTSEL    8
#define luasubRecordDTYP    9
#define luasubRecordDISV    10
#define luasubRecordDISA    11
#define luasubRecordSDIS    12
#define luasubRecordMLOK    13
#define luasubRecordMLIS    14
#define luasubRecordDISP    15
#define luasubRecordPROC    16
#define luasubRecordSTAT    17
#define luasubRecordSEVR    18
#define luasubRecordNSTA    19
#define luasubRecordNSEV    20
#define luasubRecordACKS    21
#define luasubRecordACKT    22
#define luasubRecordDISS    23
#define luasubRecordLCNT    24
#define luasubRecordPACT    25
#define luasubRecordPUTF    26
#define luasubRecordRPRO    27
#define luasubRecordASP     28
#define luasubRecordPPN     29
#define luasubRecordPPNR    30
#define luasubRecordSPVT    31
#define luasubRecordRSET    32
#define luasubRecordDSET    33
#define luasubRecordDPVT    34
#define luasubRecordRDES    35
#define luasubRecordLSET    36
#define luasubRecordPRIO    37
#define luasubRecordTPRO    38
#define luasubRecordBKPT    39
#define luasubRecordUDF     40
#define luasubRecordTIME    41
#define luasubRecordFLNK    42
#define luasubRecordVAL     43
#define luasubRecordOVAL    44
#define luasubRecordINP     45
#define luasubRecordICOD    46
#define luasubRecordACTP    47
#define luasubRecordRST     48
#define luasubRecordPCOD    49
#define luasubRecordOCOD    50
#define luasubRecordYLDD    51
#define luasubRecordPCTP    52
#define luasubRecordPREC    53
#define luasubRecordDRVO    54
#define luasubRecordINPA    55
#define luasubRecordINPB    56
#define luasubRecordINPC    57
#define luasubRecordINPD    58
#define luasubRecordINPE    59
#define luasubRecordINPF    60
#define luasubRecordINPG    61
#define luasubRecordINPH    62
#define luasubRecordINPI    63
#define luasubRecordINPJ    64
#define luasubRecordINPK    65
#define luasubRecordINPL    66
#define luasubRecordINPM    67
#define luasubRecordINPN    68
#define luasubRecordINPO    69
#define luasubRecordINPP    70
#define luasubRecordINPQ    71
#define luasubRecordINPR    72
#define luasubRecordINPS    73
#define luasubRecordINPT    74
#define luasubRecordINPU    75
#define luasubRecordINPV    76
#define luasubRecordINPW    77
#define luasubRecordINPX    78
#define luasubRecordINPY    79
#define luasubRecordINPZ    80
#define luasubRecordA       81
#define luasubRecordB       82
#define luasubRecordC       83
#define luasubRecordD       84
#define luasubRecordE       85
#define luasubRecordF       86
#define luasubRecordG       87
#define luasubRecordH       88
#define luasubRecordI       89
#define luasubRecordJ       90
#define luasubRecordK       91
#define luasubRecordL       92
#define luasubRecordM       93
#define luasubRecordN       94
#define luasubRecordO       95
#define luasubRecordP       96
#define luasubRecordQ       97
#define luasubRecordR       98
#define luasubRecordS       99
#define luasubRecordT       100
#define luasubRecordU       101
#define luasubRecordV       102
#define luasubRecordW       103
#define luasubRecordX       104
#define luasubRecordY       105
#define luasubRecordZ       106
#define luasubRecordFTA     107
#define luasubRecordFTB     108
#define luasubRecordFTC     109
#define luasubRecordFTD     110
#define luasubRecordFTE     111
#define luasubRecordFTF     112
#define luasubRecordFTG     113
#define luasubRecordFTH     114
#define luasubRecordFTI     115
#define luasubRecordFTJ     116
#define luasubRecordFTK     117
#define luasubRecordFTL     118
#define luasubRecordFTM     119
#define luasubRecordFTN     120
#define luasubRecordFTO     121
#define luasubRecordFTP     122
#define luasubRecordFTQ     123
#define luasubRecordFTR     124
#define luasubRecordFTS     125
#define luasubRecordFTT     126
#define luasubRecordFTU     127
#define luasubRecordFTV     128
#define luasubRecordFTW     129
#define luasubRecordFTX     130
#define luasubRecordFTY     131
#define luasubRecordFTZ     132
#define luasubRecordNOA     133
#define luasubRecordNOB     134
#define luasubRecordNOC     135
#define luasubRecordNOD     136
#define luasubRecordNOE     137
#define luasubRecordNOF     138
#define luasubRecordNOG     139
#define luasubRecordNOH     140
#define luasubRecordNOI     141
#define luasubRecordNOJ     142
#define luasubRecordNOK     143
#define luasubRecordNOL     144
#define luasubRecordNOM     145
#define luasubRecordNON     146
#define luasubRecordNOO     147
#define luasubRecordNOP     148
#define luasubRecordNOQ     149
#define luasubRecordNOR     150
#define luasubRecordNOS     151
#define luasubRecordNOT     152
#define luasubRecordNOU     153
#define luasubRecordNOV     154
#define luasubRecordNOW     155
#define luasubRecordNOX     156
#define luasubRecordNOY     157
#define luasubRecordNOZ     158
#define luasubRecordNEA     159
#define luasubRecordNEB     160
#define luasubRecordNEC     161
#define luasubRecordNED     162
#define luasubRecordNEE     163
#define luasubRecordNEF     164
#define luasubRecordNEG     165
#define luasubRecordNEH     166
#define luasubRecordNEI     167
#define luasubRecordNEJ     168
#define luasubRecordNEK     169
#define luasubRecordNEL     170
#define luasubRecordNEM     171
#define luasubRecordNEN     172
#define luasubRecordNEO     173
#define luasubRecordNEP     174
#define luasubRecordNEQ     175
#define luasubRecordNER     176
#define luasubRecordNES     177
#define luasubRecordNET     178
#define luasubRecordNEU     179
#define luasubRecordNEV     180
#define luasubRecordNEW     181
#define luasubRecordNEX     182
#define luasubRecordNEY     183
#define luasubRecordNEZ     184
#define luasubRecordOUTA    185
#define luasubRecordOUTB    186
#define luasubRecordOUTC    187
#define luasubRecordOUTD    188
#define luasubRecordOUTE    189
#define luasubRecordOUTF    190
#define luasubRecordOUTG    191
#define luasubRecordOUTH    192
#define luasubRecordOUTI    193
#define luasubRecordOUTJ    194
#define luasubRecordOUTK    195
#define luasubRecordOUTL    196
#define luasubRecordOUTM    197
#define luasubRecordOUTN    198
#define luasubRecordOUTO    199
#define luasubRecordOUTP    200
#define luasubRecordOUTQ    201
#define luasubRecordOUTR    202
#define luasubRecordOUTS    203
#define luasubRecordOUTT    204
#define luasubRecordOUTU    205
#define luasubRecordOUTV    206
#define luasubRecordOUTW    207
#define luasubRecordOUTX    208
#define luasubRecordOUTY    209
#define luasubRecordOUTZ    210
#define luasubRecordVALA    211
#define luasubRecordVALB    212
#define luasubRecordVALC    213
#define luasubRecordVALD    214
#define luasubRecordVALE    215
#define luasubRecordVALF    216
#define luasubRecordVALG    217
#define luasubRecordVALH    218
#define luasubRecordVALI    219
#define luasubRecordVALJ    220
#define luasubRecordVALK    221
#define luasubRecordVALL    222
#define luasubRecordVALM    223
#define luasubRecordVALN    224
#define luasubRecordVALO    225
#define luasubRecordVALP    226
#define luasubRecordVALQ    227
#define luasubRecordVALR    228
#define luasubRecordVALS    229
#define luasubRecordVALT    230
#define luasubRecordVALU    231
#define luasubRecordVALV    232
#define luasubRecordVALW    233
#define luasubRecordVALX    234
#define luasubRecordVALY    235
#define luasubRecordVALZ    236
#define luasubRecordOVLA    237
#define luasubRecordOVLB    238
#define luasubRecordOVLC    239
#define luasubRecordOVLD    240
#define luasubRecordOVLE    241
#define luasubRecordOVLF    242
#define luasubRecordOVLG    243
#define luasubRecordOVLH    244
#define luasubRecordOVLI    245
#define luasubRecordOVLJ    246
#define luasubRecordOVLK    247
#define luasubRecordOVLL    248
#define luasubRecordOVLM    249
#define luasubRecordOVLN    250
#define luasubRecordOVLO    251
#define luasubRecordOVLP    252
#define luasubRecordOVLQ    253
#define luasubRecordOVLR    254
#define luasubRecordOVLS    255
#define luasubRecordOVLT    256
#define luasubRecordOVLU    257
#define luasubRecordOVLV    258
#define luasubRecordOVLW    259
#define luasubRecordOVLX    260
#define luasubRecordOVLY    261
#define luasubRecordOVLZ    262
#define luasubRecordFTVA    263
#define luasubRecordFTVB    264
#define luasubRecordFTVC    265
#define luasubRecordFTVD    266
#define luasubRecordFTVE    267
#define luasubRecordFTVF    268
#define luasubRecordFTVG    269
#define luasubRecordFTVH    270
#define luasubRecordFTVI    271
#define luasubRecordFTVJ    272
#define luasubRecordFTVK    273
#define luasubRecordFTVL    274
#define luasubRecordFTVM    275
#define luasubRecordFTVN    276
#define luasubRecordFTVO    277
#define luasubRecordFTVP    278
#define luasubRecordFTVQ    279
#define luasubRecordFTVR    280
#define luasubRecordFTVS    281
#define luasubRecordFTVT    282
#define luasubRecordFTVU    283
#define luasubRecordFTVV    284
#define luasubRecordFTVW    285
#define luasubRecordFTVX    286
#define luasubRecordFTVY    287
#define luasubRecordFTVZ    288
#define luasubRecordNOVA    289
#define luasubRecordNOVB    290
#define luasubRecordNOVC    291
#define luasubRecordNOVD    292
#define luasubRecordNOVE    293
#define luasubRecordNOVF    294
#define luasubRecordNOVG    295
#define luasubRecordNOVH    296
#define luasubRecordNOVI    297
#define luasubRecordNOVJ    298
#define luasubRecordNOVK    299
#define luasubRecordNOVL    300
#define luasubRecordNOVM    301
#define luasubRecordNOVN    302
#define luasubRecordNOVO    303
#define luasubRecordNOVP    304
#define luasubRecordNOVQ    305
#define luasubRecordNOVR    306
#define luasubRecordNOVS    307
#define luasubRecordNOVT    308
#define luasubRecordNOVU    309
#define luasubRecordNOVV    310
#define luasubRecordNOVW    311
#define luasubRecordNOVX    312
#define luasubRecordNOVY    313
#define luasubRecordNOVZ    314
#define luasubRecordNEVA    315
#define luasubRecordNEVB    316
#define luasubRecordNEVC    317
#define luasubRecordNEVD    318
#define luasubRecordNEVE    319
#define luasubRecordNEVF    320
#define luasubRecordNEVG    321
#define luasubRecordNEVH    322
#define luasubRecordNEVI    323
#define luasubRecordNEVJ    324
#define luasubRecordNEVK    325
#define luasubRecordNEVL    326
#define luasubRecordNEVM    327
#define luasubRecordNEVN    328
#define luasubRecordNEVO    329
#define luasubRecordNEVP    330
#define luasubRecordNEVQ    331
#define luasubRecordNEVR    332
#define luasubRecordNEVS    333
#define luasubRecordNEVT    334
#define luasubRecordNEVU    335
#define luasubRecordNEVV    336
#define luasubRecordNEVW    337
#define luasubRecordNEVX    338
#define luasubRecordNEVY    339
#define luasubRecordNEVZ    340
#define luasubRecordONVA    341
#define luasubRecordONVB    342
#define luasubRecordONVC    343
#define luasubRecordONVD    344
#define luasubRecordONVE    345
#define luasubRecordONVF    346
#define luasubRecordONVG    347
#define luasubRecordONVH    348
#define luasubRecordONVI    349
#define luasubRecordONVJ    350
#define luasubRecordONVK    351
#define luasubRecordONVL    352
#define luasubRecordONVM    353
#define luasubRecordONVN    354
#define luasubRecordONVO    355
#define luasubRecordONVP    356
#define luasubRecordONVQ    357
#define luasubRecordONVR    358
#define luasubRecordONVS    359
#define luasubRecordONVT    360
#define luasubRecordONVU    361
#define luasubRecordONVV    362
#define luasubRecordONVW    363
#define luasubRecordONVX    364
#define luasubRecordONVY    365
#define luasubRecordONVZ    366
#define luasubRecordVLDA    367
#define luasubRecordVLDB    368
#define luasubRecordVLDC    369
#define luasubRecordVLDD    370
#define luasubRecordVLDE    371
#define luasubRecordVLDF    372
#define luasubRecordVLDG    373
#define luasubRecordVLDH    374
#define luasubRecordVLDI    375
#define luasubRecordVLDJ    376
#define luasubRecordVLDK    377
#define luasubRecordVLDL    378
#define luasubRecordVLDM    379
#define luasubRecordVLDN    380
#define luasubRecordVLDO    381
#define luasubRecordVLDP    382
#define luasubRecordVLDQ    383
#define luasubRecordVLDR    384
#define luasubRecordVLDS    385
#define luasubRecordVLDT    386
#define luasubRecordVLDU    387
#define luasubRecordVLDV    388
#define luasubRecordVLDW    389
#define luasubRecordVLDX    390
#define luasubRecordVLDY    391
#define luasubRecordVLDZ    392
#endif /* INCluasubH */

#ifdef GEN_SIZE_OFFSET
#ifdef __cplusplus
extern "C" {
#endif

#include <epicsAssert.h>
#include <epicsExport.h>

static int luasubRecordSizeOffset(dbRecordType *pdbRecordType) {
    luasubRecord *prec=0;
    assert(pdbRecordType->no_fields==393);
    pdbRecordType->papFldDes[0]->size=sizeof(prec->name);
    pdbRecordType->papFldDes[0]->offset=(short)((char *)&prec->name - (char *)prec);
    pdbRecordType->papFldDes[1]->size=sizeof(prec->desc);
    pdbRecordType->papFldDes[1]->offset=(short)((char *)&prec->desc - (char *)prec);
    pdbRecordType->papFldDes[2]->size=sizeof(prec->asg);
    pdbRecordType->papFldDes[2]->offset=(short)((char *)&prec->asg - (char *)prec);
    pdbRecordType->papFldDes[3]->size=sizeof(prec->scan);
    pdbRecordType->papFldDes[3]->offset=(short)((char *)&prec->scan - (char *)prec);
    pdbRecordType->papFldDes[4]->size=sizeof(prec->pini);
    pdbRecordType->papFldDes[4]->offset=(short)((char *)&prec->pini - (char *)prec);
    pdbRecordType->papFldDes[5]->size=sizeof(prec->phas);
    pdbRecordType->papFldDes[5]->offset=(short)((char *)&prec->phas - (char *)prec);
    pdbRecordType->papFldDes[6]->size=sizeof(prec->evnt);
    pdbRecordType->papFldDes[6]->offset=(short)((char *)&prec->evnt - (char *)prec);
    pdbRecordType->papFldDes[7]->size=sizeof(prec->tse);
    pdbRecordType->papFldDes[7]->offset=(short)((char *)&prec->tse - (char *)prec);
    pdbRecordType->papFldDes[8]->size=sizeof(prec->tsel);
    pdbRecordType->papFldDes[8]->offset=(short)((char *)&prec->tsel - (char *)prec);
    pdbRecordType->papFldDes[9]->size=sizeof(prec->dtyp);
    pdbRecordType->papFldDes[9]->offset=(short)((char *)&prec->dtyp - (char *)prec);
    pdbRecordType->papFldDes[10]->size=sizeof(prec->disv);
    pdbRecordType->papFldDes[10]->offset=(short)((char *)&prec->disv - (char *)prec);
    pdbRecordType->papFldDes[11]->size=sizeof(prec->disa);
    pdbRecordType->papFldDes[11]->offset=(short)((char *)&prec->disa - (char *)prec);
    pdbRecordType->papFldDes[12]->size=sizeof(prec->sdis);
    pdbRecordType->papFldDes[12]->offset=(short)((char *)&prec->sdis - (char *)prec);
    pdbRecordType->papFldDes[13]->size=sizeof(prec->mlok);
    pdbRecordType->papFldDes[13]->offset=(short)((char *)&prec->mlok - (char *)prec);
    pdbRecordType->papFldDes[14]->size=sizeof(prec->mlis);
    pdbRecordType->papFldDes[14]->offset=(short)((char *)&prec->mlis - (char *)prec);
    pdbRecordType->papFldDes[15]->size=sizeof(prec->disp);
    pdbRecordType->papFldDes[15]->offset=(short)((char *)&prec->disp - (char *)prec);
    pdbRecordType->papFldDes[16]->size=sizeof(prec->proc);
    pdbRecordType->papFldDes[16]->offset=(short)((char *)&prec->proc - (char *)prec);
    pdbRecordType->papFldDes[17]->size=sizeof(prec->stat);
    pdbRecordType->papFldDes[17]->offset=(short)((char *)&prec->stat - (char *)prec);
    pdbRecordType->papFldDes[18]->size=sizeof(prec->sevr);
    pdbRecordType->papFldDes[18]->offset=(short)((char *)&prec->sevr - (char *)prec);
    pdbRecordType->papFldDes[19]->size=sizeof(prec->nsta);
    pdbRecordType->papFldDes[19]->offset=(short)((char *)&prec->nsta - (char *)prec);
    pdbRecordType->papFldDes[20]->size=sizeof(prec->nsev);
    pdbRecordType->papFldDes[20]->offset=(short)((char *)&prec->nsev - (char *)prec);
    pdbRecordType->papFldDes[21]->size=sizeof(prec->acks);
    pdbRecordType->papFldDes[21]->offset=(short)((char *)&prec->acks - (char *)prec);
    pdbRecordType->papFldDes[22]->size=sizeof(prec->ackt);
    pdbRecordType->papFldDes[22]->offset=(short)((char *)&prec->ackt - (char *)prec);
    pdbRecordType->papFldDes[23]->size=sizeof(prec->diss);
    pdbRecordType->papFldDes[23]->offset=(short)((char *)&prec->diss - (char *)prec);
    pdbRecordType->papFldDes[24]->size=sizeof(prec->lcnt);
    pdbRecordType->papFldDes[24]->offset=(short)((char *)&prec->lcnt - (char *)prec);
    pdbRecordType->papFldDes[25]->size=sizeof(prec->pact);
    pdbRecordType->papFldDes[25]->offset=(short)((char *)&prec->pact - (char *)prec);
    pdbRecordType->papFldDes[26]->size=sizeof(prec->putf);
    pdbRecordType->papFldDes[26]->offset=(short)((char *)&prec->putf - (char *)prec);
    pdbRecordType->papFldDes[27]->size=sizeof(prec->rpro);
    pdbRecordType->papFldDes[27]->offset=(short)((char *)&prec->rpro - (char *)prec);
    pdbRecordType->papFldDes[28]->size=sizeof(prec->asp);
    pdbRecordType->papFldDes[28]->offset=(short)((char *)&prec->asp - (char *)prec);
    pdbRecordType->papFldDes[29]->size=sizeof(prec->ppn);
    pdbRecordType->papFldDes[29]->offset=(short)((char *)&prec->ppn - (char *)prec);
    pdbRecordType->papFldDes[30]->size=sizeof(prec->ppnr);
    pdbRecordType->papFldDes[30]->offset=(short)((char *)&prec->ppnr - (char *)prec);
    pdbRecordType->papFldDes[31]->size=sizeof(prec->spvt);
    pdbRecordType->papFldDes[31]->offset=(short)((char *)&prec->spvt - (char *)prec);
    pdbRecordType->papFldDes[32]->size=sizeof(prec->rset);
    pdbRecordType->papFldDes[32]->offset=(short)((char *)&prec->rset - (char *)prec);
    pdbRecordType->papFldDes[33]->size=sizeof(prec->dset);
    pdbRecordType->papFldDes[33]->offset=(short)((char *)&prec->dset - (char *)prec);
    pdbRecordType->papFldDes[34]->size=sizeof(prec->dpvt);
    pdbRecordType->papFldDes[34]->offset=(short)((char *)&prec->dpvt - (char *)prec);
    pdbRecordType->papFldDes[35]->size=sizeof(prec->rdes);
    pdbRecordType->papFldDes[35]->offset=(short)((char *)&prec->rdes - (char *)prec);
    pdbRecordType->papFldDes[36]->size=sizeof(prec->lset);
    pdbRecordType->papFldDes[36]->offset=(short)((char *)&prec->lset - (char *)prec);
    pdbRecordType->papFldDes[37]->size=sizeof(prec->prio);
    pdbRecordType->papFldDes[37]->offset=(short)((char *)&prec->prio - (char *)prec);
    pdbRecordType->papFldDes[38]->size=sizeof(prec->tpro);
    pdbRecordType->papFldDes[38]->offset=(short)((char *)&prec->tpro - (char *)prec);
    pdbRecordType->papFldDes[39]->size=sizeof(prec->bkpt);
    pdbRecordType->papFldDes[39]->offset=(short)((char *)&prec->bkpt - (char *)prec);
    pdbRecordType->papFldDes[40]->size=sizeof(prec->udf);
    pdbRecordType->papFldDes[40]->offset=(short)((char *)&prec->udf - (char *)prec);
    pdbRecordType->papFldDes[41]->size=sizeof(prec->time);
    pdbRecordType->papFldDes[41]->offset=(short)((char *)&prec->time - (char *)prec);
    pdbRecordType->papFldDes[42]->size=sizeof(prec->flnk);
    pdbRecordType->papFldDes[42]->offset=(short)((char *)&prec->flnk - (char *)prec);
    pdbRecordType->papFldDes[43]->size=sizeof(prec->val);
    pdbRecordType->papFldDes[43]->offset=(short)((char *)&prec->val - (char *)prec);
    pdbRecordType->papFldDes[44]->size=sizeof(prec->oval);
    pdbRecordType->papFldDes[44]->offset=(short)((char *)&prec->oval - (char *)prec);
    pdbRecordType->papFldDes[45]->size=sizeof(prec->inp);
    pdbRecordType->papFldDes[45]->offset=(short)((char *)&prec->inp - (char *)prec);
    pdbRecordType->papFldDes[46]->size=sizeof(prec->icod);
    pdbRecordType->papFldDes[46]->offset=(short)((char *)&prec->icod - (char *)prec);
    pdbRecordType->papFldDes[47]->size=sizeof(prec->actp);
    pdbRecordType->papFldDes[47]->offset=(short)((char *)&prec->actp - (char *)prec);
    pdbRecordType->papFldDes[48]->size=sizeof(prec->rst);
    pdbRecordType->papFldDes[48]->offset=(short)((char *)&prec->rst - (char *)prec);
    pdbRecordType->papFldDes[49]->size=sizeof(prec->pcod);
    pdbRecordType->papFldDes[49]->offset=(short)((char *)&prec->pcod - (char *)prec);
    pdbRecordType->papFldDes[50]->size=sizeof(prec->ocod);
    pdbRecordType->papFldDes[50]->offset=(short)((char *)&prec->ocod - (char *)prec);
    pdbRecordType->papFldDes[51]->size=sizeof(prec->yldd);
    pdbRecordType->papFldDes[51]->offset=(short)((char *)&prec->yldd - (char *)prec);
    pdbRecordType->papFldDes[52]->size=sizeof(prec->pctp);
    pdbRecordType->papFldDes[52]->offset=(short)((char *)&prec->pctp - (char *)prec);
    pdbRecordType->papFldDes[53]->size=sizeof(prec->prec);
    pdbRecordType->papFldDes[53]->offset=(short)((char *)&prec->prec - (char *)prec);
    pdbRecordType->papFldDes[54]->size=sizeof(prec->drvo);
    pdbRecordType->papFldDes[54]->offset=(short)((char *)&prec->drvo - (char *)prec);
    pdbRecordType->papFldDes[55]->size=sizeof(prec->inpa);
    pdbRecordType->papFldDes[55]->offset=(short)((char *)&prec->inpa - (char *)prec);
    pdbRecordType->papFldDes[56]->size=sizeof(prec->inpb);
    pdbRecordType->papFldDes[56]->offset=(short)((char *)&prec->inpb - (char *)prec);
    pdbRecordType->papFldDes[57]->size=sizeof(prec->inpc);
    pdbRecordType->papFldDes[57]->offset=(short)((char *)&prec->inpc - (char *)prec);
    pdbRecordType->papFldDes[58]->size=sizeof(prec->inpd);
    pdbRecordType->papFldDes[58]->offset=(short)((char *)&prec->inpd - (char *)prec);
    pdbRecordType->papFldDes[59]->size=sizeof(prec->inpe);
    pdbRecordType->papFldDes[59]->offset=(short)((char *)&prec->inpe - (char *)prec);
    pdbRecordType->papFldDes[60]->size=sizeof(prec->inpf);
    pdbRecordType->papFldDes[60]->offset=(short)((char *)&prec->inpf - (char *)prec);
    pdbRecordType->papFldDes[61]->size=sizeof(prec->inpg);
    pdbRecordType->papFldDes[61]->offset=(short)((char *)&prec->inpg - (char *)prec);
    pdbRecordType->papFldDes[62]->size=sizeof(prec->inph);
    pdbRecordType->papFldDes[62]->offset=(short)((char *)&prec->inph - (char *)prec);
    pdbRecordType->papFldDes[63]->size=sizeof(prec->inpi);
    pdbRecordType->papFldDes[63]->offset=(short)((char *)&prec->inpi - (char *)prec);
    pdbRecordType->papFldDes[64]->size=sizeof(prec->inpj);
    pdbRecordType->papFldDes[64]->offset=(short)((char *)&prec->inpj - (char *)prec);
    pdbRecordType->papFldDes[65]->size=sizeof(prec->inpk);
    pdbRecordType->papFldDes[65]->offset=(short)((char *)&prec->inpk - (char *)prec);
    pdbRecordType->papFldDes[66]->size=sizeof(prec->inpl);
    pdbRecordType->papFldDes[66]->offset=(short)((char *)&prec->inpl - (char *)prec);
    pdbRecordType->papFldDes[67]->size=sizeof(prec->inpm);
    pdbRecordType->papFldDes[67]->offset=(short)((char *)&prec->inpm - (char *)prec);
    pdbRecordType->papFldDes[68]->size=sizeof(prec->inpn);
    pdbRecordType->papFldDes[68]->offset=(short)((char *)&prec->inpn - (char *)prec);
    pdbRecordType->papFldDes[69]->size=sizeof(prec->inpo);
    pdbRecordType->papFldDes[69]->offset=(short)((char *)&prec->inpo - (char *)prec);
    pdbRecordType->papFldDes[70]->size=sizeof(prec->inpp);
    pdbRecordType->papFldDes[70]->offset=(short)((char *)&prec->inpp - (char *)prec);
    pdbRecordType->papFldDes[71]->size=sizeof(prec->inpq);
    pdbRecordType->papFldDes[71]->offset=(short)((char *)&prec->inpq - (char *)prec);
    pdbRecordType->papFldDes[72]->size=sizeof(prec->inpr);
    pdbRecordType->papFldDes[72]->offset=(short)((char *)&prec->inpr - (char *)prec);
    pdbRecordType->papFldDes[73]->size=sizeof(prec->inps);
    pdbRecordType->papFldDes[73]->offset=(short)((char *)&prec->inps - (char *)prec);
    pdbRecordType->papFldDes[74]->size=sizeof(prec->inpt);
    pdbRecordType->papFldDes[74]->offset=(short)((char *)&prec->inpt - (char *)prec);
    pdbRecordType->papFldDes[75]->size=sizeof(prec->inpu);
    pdbRecordType->papFldDes[75]->offset=(short)((char *)&prec->inpu - (char *)prec);
    pdbRecordType->papFldDes[76]->size=sizeof(prec->inpv);
    pdbRecordType->papFldDes[76]->offset=(short)((char *)&prec->inpv - (char *)prec);
    pdbRecordType->papFldDes[77]->size=sizeof(prec->inpw);
    pdbRecordType->papFldDes[77]->offset=(short)((char *)&prec->inpw - (char *)prec);
    pdbRecordType->papFldDes[78]->size=sizeof(prec->inpx);
    pdbRecordType->papFldDes[78]->offset=(short)((char *)&prec->inpx - (char *)prec);
    pdbRecordType->papFldDes[79]->size=sizeof(prec->inpy);
    pdbRecordType->papFldDes[79]->offset=(short)((char *)&prec->inpy - (char *)prec);
    pdbRecordType->papFldDes[80]->size=sizeof(prec->inpz);
    pdbRecordType->papFldDes[80]->offset=(short)((char *)&prec->inpz - (char *)prec);
    pdbRecordType->papFldDes[81]->size=sizeof(prec->a);
    pdbRecordType->papFldDes[81]->offset=(short)((char *)&prec->a - (char *)prec);
    pdbRecordType->papFldDes[82]->size=sizeof(prec->b);
    pdbRecordType->papFldDes[82]->offset=(short)((char *)&prec->b - (char *)prec);
    pdbRecordType->papFldDes[83]->size=sizeof(prec->c);
    pdbRecordType->papFldDes[83]->offset=(short)((char *)&prec->c - (char *)prec);
    pdbRecordType->papFldDes[84]->size=sizeof(prec->d);
    pdbRecordType->papFldDes[84]->offset=(short)((char *)&prec->d - (char *)prec);
    pdbRecordType->papFldDes[85]->size=sizeof(prec->e);
    pdbRecordType->papFldDes[85]->offset=(short)((char *)&prec->e - (char *)prec);
    pdbRecordType->papFldDes[86]->size=sizeof(prec->f);
    pdbRecordType->papFldDes[86]->offset=(short)((char *)&prec->f - (char *)prec);
    pdbRecordType->papFldDes[87]->size=sizeof(prec->g);
    pdbRecordType->papFldDes[87]->offset=(short)((char *)&prec->g - (char *)prec);
    pdbRecordType->papFldDes[88]->size=sizeof(prec->h);
    pdbRecordType->papFldDes[88]->offset=(short)((char *)&prec->h - (char *)prec);
    pdbRecordType->papFldDes[89]->size=sizeof(prec->i);
    pdbRecordType->papFldDes[89]->offset=(short)((char *)&prec->i - (char *)prec);
    pdbRecordType->papFldDes[90]->size=sizeof(prec->j);
    pdbRecordType->papFldDes[90]->offset=(short)((char *)&prec->j - (char *)prec);
    pdbRecordType->papFldDes[91]->size=sizeof(prec->k);
    pdbRecordType->papFldDes[91]->offset=(short)((char *)&prec->k - (char *)prec);
    pdbRecordType->papFldDes[92]->size=sizeof(prec->l);
    pdbRecordType->papFldDes[92]->offset=(short)((char *)&prec->l - (char *)prec);
    pdbRecordType->papFldDes[93]->size=sizeof(prec->m);
    pdbRecordType->papFldDes[93]->offset=(short)((char *)&prec->m - (char *)prec);
    pdbRecordType->papFldDes[94]->size=sizeof(prec->n);
    pdbRecordType->papFldDes[94]->offset=(short)((char *)&prec->n - (char *)prec);
    pdbRecordType->papFldDes[95]->size=sizeof(prec->o);
    pdbRecordType->papFldDes[95]->offset=(short)((char *)&prec->o - (char *)prec);
    pdbRecordType->papFldDes[96]->size=sizeof(prec->p);
    pdbRecordType->papFldDes[96]->offset=(short)((char *)&prec->p - (char *)prec);
    pdbRecordType->papFldDes[97]->size=sizeof(prec->q);
    pdbRecordType->papFldDes[97]->offset=(short)((char *)&prec->q - (char *)prec);
    pdbRecordType->papFldDes[98]->size=sizeof(prec->r);
    pdbRecordType->papFldDes[98]->offset=(short)((char *)&prec->r - (char *)prec);
    pdbRecordType->papFldDes[99]->size=sizeof(prec->s);
    pdbRecordType->papFldDes[99]->offset=(short)((char *)&prec->s - (char *)prec);
    pdbRecordType->papFldDes[100]->size=sizeof(prec->t);
    pdbRecordType->papFldDes[100]->offset=(short)((char *)&prec->t - (char *)prec);
    pdbRecordType->papFldDes[101]->size=sizeof(prec->u);
    pdbRecordType->papFldDes[101]->offset=(short)((char *)&prec->u - (char *)prec);
    pdbRecordType->papFldDes[102]->size=sizeof(prec->v);
    pdbRecordType->papFldDes[102]->offset=(short)((char *)&prec->v - (char *)prec);
    pdbRecordType->papFldDes[103]->size=sizeof(prec->w);
    pdbRecordType->papFldDes[103]->offset=(short)((char *)&prec->w - (char *)prec);
    pdbRecordType->papFldDes[104]->size=sizeof(prec->x);
    pdbRecordType->papFldDes[104]->offset=(short)((char *)&prec->x - (char *)prec);
    pdbRecordType->papFldDes[105]->size=sizeof(prec->y);
    pdbRecordType->papFldDes[105]->offset=(short)((char *)&prec->y - (char *)prec);
    pdbRecordType->papFldDes[106]->size=sizeof(prec->z);
    pdbRecordType->papFldDes[106]->offset=(short)((char *)&prec->z - (char *)prec);
    pdbRecordType->papFldDes[107]->size=sizeof(prec->fta);
    pdbRecordType->papFldDes[107]->offset=(short)((char *)&prec->fta - (char *)prec);
    pdbRecordType->papFldDes[108]->size=sizeof(prec->ftb);
    pdbRecordType->papFldDes[108]->offset=(short)((char *)&prec->ftb - (char *)prec);
    pdbRecordType->papFldDes[109]->size=sizeof(prec->ftc);
    pdbRecordType->papFldDes[109]->offset=(short)((char *)&prec->ftc - (char *)prec);
    pdbRecordType->papFldDes[110]->size=sizeof(prec->ftd);
    pdbRecordType->papFldDes[110]->offset=(short)((char *)&prec->ftd - (char *)prec);
    pdbRecordType->papFldDes[111]->size=sizeof(prec->fte);
    pdbRecordType->papFldDes[111]->offset=(short)((char *)&prec->fte - (char *)prec);
    pdbRecordType->papFldDes[112]->size=sizeof(prec->ftf);
    pdbRecordType->papFldDes[112]->offset=(short)((char *)&prec->ftf - (char *)prec);
    pdbRecordType->papFldDes[113]->size=sizeof(prec->ftg);
    pdbRecordType->papFldDes[113]->offset=(short)((char *)&prec->ftg - (char *)prec);
    pdbRecordType->papFldDes[114]->size=sizeof(prec->fth);
    pdbRecordType->papFldDes[114]->offset=(short)((char *)&prec->fth - (char *)prec);
    pdbRecordType->papFldDes[115]->size=sizeof(prec->fti);
    pdbRecordType->papFldDes[115]->offset=(short)((char *)&prec->fti - (char *)prec);
    pdbRecordType->papFldDes[116]->size=sizeof(prec->ftj);
    pdbRecordType->papFldDes[116]->offset=(short)((char *)&prec->ftj - (char *)prec);
    pdbRecordType->papFldDes[117]->size=sizeof(prec->ftk);
    pdbRecordType->papFldDes[117]->offset=(short)((char *)&prec->ftk - (char *)prec);
    pdbRecordType->papFldDes[118]->size=sizeof(prec->ftl);
    pdbRecordType->papFldDes[118]->offset=(short)((char *)&prec->ftl - (char *)prec);
    pdbRecordType->papFldDes[119]->size=sizeof(prec->ftm);
    pdbRecordType->papFldDes[119]->offset=(short)((char *)&prec->ftm - (char *)prec);
    pdbRecordType->papFldDes[120]->size=sizeof(prec->ftn);
    pdbRecordType->papFldDes[120]->offset=(short)((char *)&prec->ftn - (char *)prec);
    pdbRecordType->papFldDes[121]->size=sizeof(prec->fto);
    pdbRecordType->papFldDes[121]->offset=(short)((char *)&prec->fto - (char *)prec);
    pdbRecordType->papFldDes[122]->size=sizeof(prec->ftp);
    pdbRecordType->papFldDes[122]->offset=(short)((char *)&prec->ftp - (char *)prec);
    pdbRecordType->papFldDes[123]->size=sizeof(prec->ftq);
    pdbRecordType->papFldDes[123]->offset=(short)((char *)&prec->ftq - (char *)prec);
    pdbRecordType->papFldDes[124]->size=sizeof(prec->ftr);
    pdbRecordType->papFldDes[124]->offset=(short)((char *)&prec->ftr - (char *)prec);
    pdbRecordType->papFldDes[125]->size=sizeof(prec->fts);
    pdbRecordType->papFldDes[125]->offset=(short)((char *)&prec->fts - (char *)prec);
    pdbRecordType->papFldDes[126]->size=sizeof(prec->ftt);
    pdbRecordType->papFldDes[126]->offset=(short)((char *)&prec->ftt - (char *)prec);
    pdbRecordType->papFldDes[127]->size=sizeof(prec->ftu);
    pdbRecordType->papFldDes[127]->offset=(short)((char *)&prec->ftu - (char *)prec);
    pdbRecordType->papFldDes[128]->size=sizeof(prec->ftv);
    pdbRecordType->papFldDes[128]->offset=(short)((char *)&prec->ftv - (char *)prec);
    pdbRecordType->papFldDes[129]->size=sizeof(prec->ftw);
    pdbRecordType->papFldDes[129]->offset=(short)((char *)&prec->ftw - (char *)prec);
    pdbRecordType->papFldDes[130]->size=sizeof(prec->ftx);
    pdbRecordType->papFldDes[130]->offset=(short)((char *)&prec->ftx - (char *)prec);
    pdbRecordType->papFldDes[131]->size=sizeof(prec->fty);
    pdbRecordType->papFldDes[131]->offset=(short)((char *)&prec->fty - (char *)prec);
    pdbRecordType->papFldDes[132]->size=sizeof(prec->ftz);
    pdbRecordType->papFldDes[132]->offset=(short)((char *)&prec->ftz - (char *)prec);
    pdbRecordType->papFldDes[133]->size=sizeof(prec->noa);
    pdbRecordType->papFldDes[133]->offset=(short)((char *)&prec->noa - (char *)prec);
    pdbRecordType->papFldDes[134]->size=sizeof(prec->nob);
    pdbRecordType->papFldDes[134]->offset=(short)((char *)&prec->nob - (char *)prec);
    pdbRecordType->papFldDes[135]->size=sizeof(prec->noc);
    pdbRecordType->papFldDes[135]->offset=(short)((char *)&prec->noc - (char *)prec);
    pdbRecordType->papFldDes[136]->size=sizeof(prec->nod);
    pdbRecordType->papFldDes[136]->offset=(short)((char *)&prec->nod - (char *)prec);
    pdbRecordType->papFldDes[137]->size=sizeof(prec->noe);
    pdbRecordType->papFldDes[137]->offset=(short)((char *)&prec->noe - (char *)prec);
    pdbRecordType->papFldDes[138]->size=sizeof(prec->nof);
    pdbRecordType->papFldDes[138]->offset=(short)((char *)&prec->nof - (char *)prec);
    pdbRecordType->papFldDes[139]->size=sizeof(prec->nog);
    pdbRecordType->papFldDes[139]->offset=(short)((char *)&prec->nog - (char *)prec);
    pdbRecordType->papFldDes[140]->size=sizeof(prec->noh);
    pdbRecordType->papFldDes[140]->offset=(short)((char *)&prec->noh - (char *)prec);
    pdbRecordType->papFldDes[141]->size=sizeof(prec->noi);
    pdbRecordType->papFldDes[141]->offset=(short)((char *)&prec->noi - (char *)prec);
    pdbRecordType->papFldDes[142]->size=sizeof(prec->noj);
    pdbRecordType->papFldDes[142]->offset=(short)((char *)&prec->noj - (char *)prec);
    pdbRecordType->papFldDes[143]->size=sizeof(prec->nok);
    pdbRecordType->papFldDes[143]->offset=(short)((char *)&prec->nok - (char *)prec);
    pdbRecordType->papFldDes[144]->size=sizeof(prec->nol);
    pdbRecordType->papFldDes[144]->offset=(short)((char *)&prec->nol - (char *)prec);
    pdbRecordType->papFldDes[145]->size=sizeof(prec->nom);
    pdbRecordType->papFldDes[145]->offset=(short)((char *)&prec->nom - (char *)prec);
    pdbRecordType->papFldDes[146]->size=sizeof(prec->non);
    pdbRecordType->papFldDes[146]->offset=(short)((char *)&prec->non - (char *)prec);
    pdbRecordType->papFldDes[147]->size=sizeof(prec->noo);
    pdbRecordType->papFldDes[147]->offset=(short)((char *)&prec->noo - (char *)prec);
    pdbRecordType->papFldDes[148]->size=sizeof(prec->nop);
    pdbRecordType->papFldDes[148]->offset=(short)((char *)&prec->nop - (char *)prec);
    pdbRecordType->papFldDes[149]->size=sizeof(prec->noq);
    pdbRecordType->papFldDes[149]->offset=(short)((char *)&prec->noq - (char *)prec);
    pdbRecordType->papFldDes[150]->size=sizeof(prec->nor);
    pdbRecordType->papFldDes[150]->offset=(short)((char *)&prec->nor - (char *)prec);
    pdbRecordType->papFldDes[151]->size=sizeof(prec->nos);
    pdbRecordType->papFldDes[151]->offset=(short)((char *)&prec->nos - (char *)prec);
    pdbRecordType->papFldDes[152]->size=sizeof(prec->not);
    pdbRecordType->papFldDes[152]->offset=(short)((char *)&prec->not - (char *)prec);
    pdbRecordType->papFldDes[153]->size=sizeof(prec->nou);
    pdbRecordType->papFldDes[153]->offset=(short)((char *)&prec->nou - (char *)prec);
    pdbRecordType->papFldDes[154]->size=sizeof(prec->nov);
    pdbRecordType->papFldDes[154]->offset=(short)((char *)&prec->nov - (char *)prec);
    pdbRecordType->papFldDes[155]->size=sizeof(prec->now);
    pdbRecordType->papFldDes[155]->offset=(short)((char *)&prec->now - (char *)prec);
    pdbRecordType->papFldDes[156]->size=sizeof(prec->nox);
    pdbRecordType->papFldDes[156]->offset=(short)((char *)&prec->nox - (char *)prec);
    pdbRecordType->papFldDes[157]->size=sizeof(prec->noy);
    pdbRecordType->papFldDes[157]->offset=(short)((char *)&prec->noy - (char *)prec);
    pdbRecordType->papFldDes[158]->size=sizeof(prec->noz);
    pdbRecordType->papFldDes[158]->offset=(short)((char *)&prec->noz - (char *)prec);
    pdbRecordType->papFldDes[159]->size=sizeof(prec->nea);
    pdbRecordType->papFldDes[159]->offset=(short)((char *)&prec->nea - (char *)prec);
    pdbRecordType->papFldDes[160]->size=sizeof(prec->neb);
    pdbRecordType->papFldDes[160]->offset=(short)((char *)&prec->neb - (char *)prec);
    pdbRecordType->papFldDes[161]->size=sizeof(prec->nec);
    pdbRecordType->papFldDes[161]->offset=(short)((char *)&prec->nec - (char *)prec);
    pdbRecordType->papFldDes[162]->size=sizeof(prec->ned);
    pdbRecordType->papFldDes[162]->offset=(short)((char *)&prec->ned - (char *)prec);
    pdbRecordType->papFldDes[163]->size=sizeof(prec->nee);
    pdbRecordType->papFldDes[163]->offset=(short)((char *)&prec->nee - (char *)prec);
    pdbRecordType->papFldDes[164]->size=sizeof(prec->nef);
    pdbRecordType->papFldDes[164]->offset=(short)((char *)&prec->nef - (char *)prec);
    pdbRecordType->papFldDes[165]->size=sizeof(prec->neg);
    pdbRecordType->papFldDes[165]->offset=(short)((char *)&prec->neg - (char *)prec);
    pdbRecordType->papFldDes[166]->size=sizeof(prec->neh);
    pdbRecordType->papFldDes[166]->offset=(short)((char *)&prec->neh - (char *)prec);
    pdbRecordType->papFldDes[167]->size=sizeof(prec->nei);
    pdbRecordType->papFldDes[167]->offset=(short)((char *)&prec->nei - (char *)prec);
    pdbRecordType->papFldDes[168]->size=sizeof(prec->nej);
    pdbRecordType->papFldDes[168]->offset=(short)((char *)&prec->nej - (char *)prec);
    pdbRecordType->papFldDes[169]->size=sizeof(prec->nek);
    pdbRecordType->papFldDes[169]->offset=(short)((char *)&prec->nek - (char *)prec);
    pdbRecordType->papFldDes[170]->size=sizeof(prec->nel);
    pdbRecordType->papFldDes[170]->offset=(short)((char *)&prec->nel - (char *)prec);
    pdbRecordType->papFldDes[171]->size=sizeof(prec->nem);
    pdbRecordType->papFldDes[171]->offset=(short)((char *)&prec->nem - (char *)prec);
    pdbRecordType->papFldDes[172]->size=sizeof(prec->nen);
    pdbRecordType->papFldDes[172]->offset=(short)((char *)&prec->nen - (char *)prec);
    pdbRecordType->papFldDes[173]->size=sizeof(prec->neo);
    pdbRecordType->papFldDes[173]->offset=(short)((char *)&prec->neo - (char *)prec);
    pdbRecordType->papFldDes[174]->size=sizeof(prec->nep);
    pdbRecordType->papFldDes[174]->offset=(short)((char *)&prec->nep - (char *)prec);
    pdbRecordType->papFldDes[175]->size=sizeof(prec->neq);
    pdbRecordType->papFldDes[175]->offset=(short)((char *)&prec->neq - (char *)prec);
    pdbRecordType->papFldDes[176]->size=sizeof(prec->ner);
    pdbRecordType->papFldDes[176]->offset=(short)((char *)&prec->ner - (char *)prec);
    pdbRecordType->papFldDes[177]->size=sizeof(prec->nes);
    pdbRecordType->papFldDes[177]->offset=(short)((char *)&prec->nes - (char *)prec);
    pdbRecordType->papFldDes[178]->size=sizeof(prec->net);
    pdbRecordType->papFldDes[178]->offset=(short)((char *)&prec->net - (char *)prec);
    pdbRecordType->papFldDes[179]->size=sizeof(prec->neu);
    pdbRecordType->papFldDes[179]->offset=(short)((char *)&prec->neu - (char *)prec);
    pdbRecordType->papFldDes[180]->size=sizeof(prec->nev);
    pdbRecordType->papFldDes[180]->offset=(short)((char *)&prec->nev - (char *)prec);
    pdbRecordType->papFldDes[181]->size=sizeof(prec->new);
    pdbRecordType->papFldDes[181]->offset=(short)((char *)&prec->new - (char *)prec);
    pdbRecordType->papFldDes[182]->size=sizeof(prec->nex);
    pdbRecordType->papFldDes[182]->offset=(short)((char *)&prec->nex - (char *)prec);
    pdbRecordType->papFldDes[183]->size=sizeof(prec->ney);
    pdbRecordType->papFldDes[183]->offset=(short)((char *)&prec->ney - (char *)prec);
    pdbRecordType->papFldDes[184]->size=sizeof(prec->nez);
    pdbRecordType->papFldDes[184]->offset=(short)((char *)&prec->nez - (char *)prec);
    pdbRecordType->papFldDes[185]->size=sizeof(prec->outa);
    pdbRecordType->papFldDes[185]->offset=(short)((char *)&prec->outa - (char *)prec);
    pdbRecordType->papFldDes[186]->size=sizeof(prec->outb);
    pdbRecordType->papFldDes[186]->offset=(short)((char *)&prec->outb - (char *)prec);
    pdbRecordType->papFldDes[187]->size=sizeof(prec->outc);
    pdbRecordType->papFldDes[187]->offset=(short)((char *)&prec->outc - (char *)prec);
    pdbRecordType->papFldDes[188]->size=sizeof(prec->outd);
    pdbRecordType->papFldDes[188]->offset=(short)((char *)&prec->outd - (char *)prec);
    pdbRecordType->papFldDes[189]->size=sizeof(prec->oute);
    pdbRecordType->papFldDes[189]->offset=(short)((char *)&prec->oute - (char *)prec);
    pdbRecordType->papFldDes[190]->size=sizeof(prec->outf);
    pdbRecordType->papFldDes[190]->offset=(short)((char *)&prec->outf - (char *)prec);
    pdbRecordType->papFldDes[191]->size=sizeof(prec->outg);
    pdbRecordType->papFldDes[191]->offset=(short)((char *)&prec->outg - (char *)prec);
    pdbRecordType->papFldDes[192]->size=sizeof(prec->outh);
    pdbRecordType->papFldDes[192]->offset=(short)((char *)&prec->outh - (char *)prec);
    pdbRecordType->papFldDes[193]->size=sizeof(prec->outi);
    pdbRecordType->papFldDes[193]->offset=(short)((char *)&prec->outi - (char *)prec);
    pdbRecordType->papFldDes[194]->size=sizeof(prec->outj);
    pdbRecordType->papFldDes[194]->offset=(short)((char *)&prec->outj - (char *)prec);
    pdbRecordType->papFldDes[195]->size=sizeof(prec->outk);
    pdbRecordType->papFldDes[195]->offset=(short)((char *)&prec->outk - (char *)prec);
    pdbRecordType->papFldDes[196]->size=sizeof(prec->outl);
    pdbRecordType->papFldDes[196]->offset=(short)((char *)&prec->outl - (char *)prec);
    pdbRecordType->papFldDes[197]->size=sizeof(prec->outm);
    pdbRecordType->papFldDes[197]->offset=(short)((char *)&prec->outm - (char *)prec);
    pdbRecordType->papFldDes[198]->size=sizeof(prec->outn);
    pdbRecordType->papFldDes[198]->offset=(short)((char *)&prec->outn - (char *)prec);
    pdbRecordType->papFldDes[199]->size=sizeof(prec->outo);
    pdbRecordType->papFldDes[199]->offset=(short)((char *)&prec->outo - (char *)prec);
    pdbRecordType->papFldDes[200]->size=sizeof(prec->outp);
    pdbRecordType->papFldDes[200]->offset=(short)((char *)&prec->outp - (char *)prec);
    pdbRecordType->papFldDes[201]->size=sizeof(prec->outq);
    pdbRecordType->papFldDes[201]->offset=(short)((char *)&prec->outq - (char *)prec);
    pdbRecordType->papFldDes[202]->size=sizeof(prec->outr);
    pdbRecordType->papFldDes[202]->offset=(short)((char *)&prec->outr - (char *)prec);
    pdbRecordType->papFldDes[203]->size=sizeof(prec->outs);
    pdbRecordType->papFldDes[203]->offset=(short)((char *)&prec->outs - (char *)prec);
    pdbRecordType->papFldDes[204]->size=sizeof(prec->outt);
    pdbRecordType->papFldDes[204]->offset=(short)((char *)&prec->outt - (char *)prec);
    pdbRecordType->papFldDes[205]->size=sizeof(prec->outu);
    pdbRecordType->papFldDes[205]->offset=(short)((char *)&prec->outu - (char *)prec);
    pdbRecordType->papFldDes[206]->size=sizeof(prec->outv);
    pdbRecordType->papFldDes[206]->offset=(short)((char *)&prec->outv - (char *)prec);
    pdbRecordType->papFldDes[207]->size=sizeof(prec->outw);
    pdbRecordType->papFldDes[207]->offset=(short)((char *)&prec->outw - (char *)prec);
    pdbRecordType->papFldDes[208]->size=sizeof(prec->outx);
    pdbRecordType->papFldDes[208]->offset=(short)((char *)&prec->outx - (char *)prec);
    pdbRecordType->papFldDes[209]->size=sizeof(prec->outy);
    pdbRecordType->papFldDes[209]->offset=(short)((char *)&prec->outy - (char *)prec);
    pdbRecordType->papFldDes[210]->size=sizeof(prec->outz);
    pdbRecordType->papFldDes[210]->offset=(short)((char *)&prec->outz - (char *)prec);
    pdbRecordType->papFldDes[211]->size=sizeof(prec->vala);
    pdbRecordType->papFldDes[211]->offset=(short)((char *)&prec->vala - (char *)prec);
    pdbRecordType->papFldDes[212]->size=sizeof(prec->valb);
    pdbRecordType->papFldDes[212]->offset=(short)((char *)&prec->valb - (char *)prec);
    pdbRecordType->papFldDes[213]->size=sizeof(prec->valc);
    pdbRecordType->papFldDes[213]->offset=(short)((char *)&prec->valc - (char *)prec);
    pdbRecordType->papFldDes[214]->size=sizeof(prec->vald);
    pdbRecordType->papFldDes[214]->offset=(short)((char *)&prec->vald - (char *)prec);
    pdbRecordType->papFldDes[215]->size=sizeof(prec->vale);
    pdbRecordType->papFldDes[215]->offset=(short)((char *)&prec->vale - (char *)prec);
    pdbRecordType->papFldDes[216]->size=sizeof(prec->valf);
    pdbRecordType->papFldDes[216]->offset=(short)((char *)&prec->valf - (char *)prec);
    pdbRecordType->papFldDes[217]->size=sizeof(prec->valg);
    pdbRecordType->papFldDes[217]->offset=(short)((char *)&prec->valg - (char *)prec);
    pdbRecordType->papFldDes[218]->size=sizeof(prec->valh);
    pdbRecordType->papFldDes[218]->offset=(short)((char *)&prec->valh - (char *)prec);
    pdbRecordType->papFldDes[219]->size=sizeof(prec->vali);
    pdbRecordType->papFldDes[219]->offset=(short)((char *)&prec->vali - (char *)prec);
    pdbRecordType->papFldDes[220]->size=sizeof(prec->valj);
    pdbRecordType->papFldDes[220]->offset=(short)((char *)&prec->valj - (char *)prec);
    pdbRecordType->papFldDes[221]->size=sizeof(prec->valk);
    pdbRecordType->papFldDes[221]->offset=(short)((char *)&prec->valk - (char *)prec);
    pdbRecordType->papFldDes[222]->size=sizeof(prec->vall);
    pdbRecordType->papFldDes[222]->offset=(short)((char *)&prec->vall - (char *)prec);
    pdbRecordType->papFldDes[223]->size=sizeof(prec->valm);
    pdbRecordType->papFldDes[223]->offset=(short)((char *)&prec->valm - (char *)prec);
    pdbRecordType->papFldDes[224]->size=sizeof(prec->valn);
    pdbRecordType->papFldDes[224]->offset=(short)((char *)&prec->valn - (char *)prec);
    pdbRecordType->papFldDes[225]->size=sizeof(prec->valo);
    pdbRecordType->papFldDes[225]->offset=(short)((char *)&prec->valo - (char *)prec);
    pdbRecordType->papFldDes[226]->size=sizeof(prec->valp);
    pdbRecordType->papFldDes[226]->offset=(short)((char *)&prec->valp - (char *)prec);
    pdbRecordType->papFldDes[227]->size=sizeof(prec->valq);
    pdbRecordType->papFldDes[227]->offset=(short)((char *)&prec->valq - (char *)prec);
    pdbRecordType->papFldDes[228]->size=sizeof(prec->valr);
    pdbRecordType->papFldDes[228]->offset=(short)((char *)&prec->valr - (char *)prec);
    pdbRecordType->papFldDes[229]->size=sizeof(prec->vals);
    pdbRecordType->papFldDes[229]->offset=(short)((char *)&prec->vals - (char *)prec);
    pdbRecordType->papFldDes[230]->size=sizeof(prec->valt);
    pdbRecordType->papFldDes[230]->offset=(short)((char *)&prec->valt - (char *)prec);
    pdbRecordType->papFldDes[231]->size=sizeof(prec->valu);
    pdbRecordType->papFldDes[231]->offset=(short)((char *)&prec->valu - (char *)prec);
    pdbRecordType->papFldDes[232]->size=sizeof(prec->valv);
    pdbRecordType->papFldDes[232]->offset=(short)((char *)&prec->valv - (char *)prec);
    pdbRecordType->papFldDes[233]->size=sizeof(prec->valw);
    pdbRecordType->papFldDes[233]->offset=(short)((char *)&prec->valw - (char *)prec);
    pdbRecordType->papFldDes[234]->size=sizeof(prec->valx);
    pdbRecordType->papFldDes[234]->offset=(short)((char *)&prec->valx - (char *)prec);
    pdbRecordType->papFldDes[235]->size=sizeof(prec->valy);
    pdbRecordType->papFldDes[235]->offset=(short)((char *)&prec->valy - (char *)prec);
    pdbRecordType->papFldDes[236]->size=sizeof(prec->valz);
    pdbRecordType->papFldDes[236]->offset=(short)((char *)&prec->valz - (char *)prec);
    pdbRecordType->papFldDes[237]->size=sizeof(prec->ovla);
    pdbRecordType->papFldDes[237]->offset=(short)((char *)&prec->ovla - (char *)prec);
    pdbRecordType->papFldDes[238]->size=sizeof(prec->ovlb);
    pdbRecordType->papFldDes[238]->offset=(short)((char *)&prec->ovlb - (char *)prec);
    pdbRecordType->papFldDes[239]->size=sizeof(prec->ovlc);
    pdbRecordType->papFldDes[239]->offset=(short)((char *)&prec->ovlc - (char *)prec);
    pdbRecordType->papFldDes[240]->size=sizeof(prec->ovld);
    pdbRecordType->papFldDes[240]->offset=(short)((char *)&prec->ovld - (char *)prec);
    pdbRecordType->papFldDes[241]->size=sizeof(prec->ovle);
    pdbRecordType->papFldDes[241]->offset=(short)((char *)&prec->ovle - (char *)prec);
    pdbRecordType->papFldDes[242]->size=sizeof(prec->ovlf);
    pdbRecordType->papFldDes[242]->offset=(short)((char *)&prec->ovlf - (char *)prec);
    pdbRecordType->papFldDes[243]->size=sizeof(prec->ovlg);
    pdbRecordType->papFldDes[243]->offset=(short)((char *)&prec->ovlg - (char *)prec);
    pdbRecordType->papFldDes[244]->size=sizeof(prec->ovlh);
    pdbRecordType->papFldDes[244]->offset=(short)((char *)&prec->ovlh - (char *)prec);
    pdbRecordType->papFldDes[245]->size=sizeof(prec->ovli);
    pdbRecordType->papFldDes[245]->offset=(short)((char *)&prec->ovli - (char *)prec);
    pdbRecordType->papFldDes[246]->size=sizeof(prec->ovlj);
    pdbRecordType->papFldDes[246]->offset=(short)((char *)&prec->ovlj - (char *)prec);
    pdbRecordType->papFldDes[247]->size=sizeof(prec->ovlk);
    pdbRecordType->papFldDes[247]->offset=(short)((char *)&prec->ovlk - (char *)prec);
    pdbRecordType->papFldDes[248]->size=sizeof(prec->ovll);
    pdbRecordType->papFldDes[248]->offset=(short)((char *)&prec->ovll - (char *)prec);
    pdbRecordType->papFldDes[249]->size=sizeof(prec->ovlm);
    pdbRecordType->papFldDes[249]->offset=(short)((char *)&prec->ovlm - (char *)prec);
    pdbRecordType->papFldDes[250]->size=sizeof(prec->ovln);
    pdbRecordType->papFldDes[250]->offset=(short)((char *)&prec->ovln - (char *)prec);
    pdbRecordType->papFldDes[251]->size=sizeof(prec->ovlo);
    pdbRecordType->papFldDes[251]->offset=(short)((char *)&prec->ovlo - (char *)prec);
    pdbRecordType->papFldDes[252]->size=sizeof(prec->ovlp);
    pdbRecordType->papFldDes[252]->offset=(short)((char *)&prec->ovlp - (char *)prec);
    pdbRecordType->papFldDes[253]->size=sizeof(prec->ovlq);
    pdbRecordType->papFldDes[253]->offset=(short)((char *)&prec->ovlq - (char *)prec);
    pdbRecordType->papFldDes[254]->size=sizeof(prec->ovlr);
    pdbRecordType->papFldDes[254]->offset=(short)((char *)&prec->ovlr - (char *)prec);
    pdbRecordType->papFldDes[255]->size=sizeof(prec->ovls);
    pdbRecordType->papFldDes[255]->offset=(short)((char *)&prec->ovls - (char *)prec);
    pdbRecordType->papFldDes[256]->size=sizeof(prec->ovlt);
    pdbRecordType->papFldDes[256]->offset=(short)((char *)&prec->ovlt - (char *)prec);
    pdbRecordType->papFldDes[257]->size=sizeof(prec->ovlu);
    pdbRecordType->papFldDes[257]->offset=(short)((char *)&prec->ovlu - (char *)prec);
    pdbRecordType->papFldDes[258]->size=sizeof(prec->ovlv);
    pdbRecordType->papFldDes[258]->offset=(short)((char *)&prec->ovlv - (char *)prec);
    pdbRecordType->papFldDes[259]->size=sizeof(prec->ovlw);
    pdbRecordType->papFldDes[259]->offset=(short)((char *)&prec->ovlw - (char *)prec);
    pdbRecordType->papFldDes[260]->size=sizeof(prec->ovlx);
    pdbRecordType->papFldDes[260]->offset=(short)((char *)&prec->ovlx - (char *)prec);
    pdbRecordType->papFldDes[261]->size=sizeof(prec->ovly);
    pdbRecordType->papFldDes[261]->offset=(short)((char *)&prec->ovly - (char *)prec);
    pdbRecordType->papFldDes[262]->size=sizeof(prec->ovlz);
    pdbRecordType->papFldDes[262]->offset=(short)((char *)&prec->ovlz - (char *)prec);
    pdbRecordType->papFldDes[263]->size=sizeof(prec->ftva);
    pdbRecordType->papFldDes[263]->offset=(short)((char *)&prec->ftva - (char *)prec);
    pdbRecordType->papFldDes[264]->size=sizeof(prec->ftvb);
    pdbRecordType->papFldDes[264]->offset=(short)((char *)&prec->ftvb - (char *)prec);
    pdbRecordType->papFldDes[265]->size=sizeof(prec->ftvc);
    pdbRecordType->papFldDes[265]->offset=(short)((char *)&prec->ftvc - (char *)prec);
    pdbRecordType->papFldDes[266]->size=sizeof(prec->ftvd);
    pdbRecordType->papFldDes[266]->offset=(short)((char *)&prec->ftvd - (char *)prec);
    pdbRecordType->papFldDes[267]->size=sizeof(prec->ftve);
    pdbRecordType->papFldDes[267]->offset=(short)((char *)&prec->ftve - (char *)prec);
    pdbRecordType->papFldDes[268]->size=sizeof(prec->ftvf);
    pdbRecordType->papFldDes[268]->offset=(short)((char *)&prec->ftvf - (char *)prec);
    pdbRecordType->papFldDes[269]->size=sizeof(prec->ftvg);
    pdbRecordType->papFldDes[269]->offset=(short)((char *)&prec->ftvg - (char *)prec);
    pdbRecordType->papFldDes[270]->size=sizeof(prec->ftvh);
    pdbRecordType->papFldDes[270]->offset=(short)((char *)&prec->ftvh - (char *)prec);
    pdbRecordType->papFldDes[271]->size=sizeof(prec->ftvi);
    pdbRecordType->papFldDes[271]->offset=(short)((char *)&prec->ftvi - (char *)prec);
    pdbRecordType->papFldDes[272]->size=sizeof(prec->ftvj);
    pdbRecordType->papFldDes[272]->offset=(short)((char *)&prec->ftvj - (char *)prec);
    pdbRecordType->papFldDes[273]->size=sizeof(prec->ftvk);
    pdbRecordType->papFldDes[273]->offset=(short)((char *)&prec->ftvk - (char *)prec);
    pdbRecordType->papFldDes[274]->size=sizeof(prec->ftvl);
    pdbRecordType->papFldDes[274]->offset=(short)((char *)&prec->ftvl - (char *)prec);
    pdbRecordType->papFldDes[275]->size=sizeof(prec->ftvm);
    pdbRecordType->papFldDes[275]->offset=(short)((char *)&prec->ftvm - (char *)prec);
    pdbRecordType->papFldDes[276]->size=sizeof(prec->ftvn);
    pdbRecordType->papFldDes[276]->offset=(short)((char *)&prec->ftvn - (char *)prec);
    pdbRecordType->papFldDes[277]->size=sizeof(prec->ftvo);
    pdbRecordType->papFldDes[277]->offset=(short)((char *)&prec->ftvo - (char *)prec);
    pdbRecordType->papFldDes[278]->size=sizeof(prec->ftvp);
    pdbRecordType->papFldDes[278]->offset=(short)((char *)&prec->ftvp - (char *)prec);
    pdbRecordType->papFldDes[279]->size=sizeof(prec->ftvq);
    pdbRecordType->papFldDes[279]->offset=(short)((char *)&prec->ftvq - (char *)prec);
    pdbRecordType->papFldDes[280]->size=sizeof(prec->ftvr);
    pdbRecordType->papFldDes[280]->offset=(short)((char *)&prec->ftvr - (char *)prec);
    pdbRecordType->papFldDes[281]->size=sizeof(prec->ftvs);
    pdbRecordType->papFldDes[281]->offset=(short)((char *)&prec->ftvs - (char *)prec);
    pdbRecordType->papFldDes[282]->size=sizeof(prec->ftvt);
    pdbRecordType->papFldDes[282]->offset=(short)((char *)&prec->ftvt - (char *)prec);
    pdbRecordType->papFldDes[283]->size=sizeof(prec->ftvu);
    pdbRecordType->papFldDes[283]->offset=(short)((char *)&prec->ftvu - (char *)prec);
    pdbRecordType->papFldDes[284]->size=sizeof(prec->ftvv);
    pdbRecordType->papFldDes[284]->offset=(short)((char *)&prec->ftvv - (char *)prec);
    pdbRecordType->papFldDes[285]->size=sizeof(prec->ftvw);
    pdbRecordType->papFldDes[285]->offset=(short)((char *)&prec->ftvw - (char *)prec);
    pdbRecordType->papFldDes[286]->size=sizeof(prec->ftvx);
    pdbRecordType->papFldDes[286]->offset=(short)((char *)&prec->ftvx - (char *)prec);
    pdbRecordType->papFldDes[287]->size=sizeof(prec->ftvy);
    pdbRecordType->papFldDes[287]->offset=(short)((char *)&prec->ftvy - (char *)prec);
    pdbRecordType->papFldDes[288]->size=sizeof(prec->ftvz);
    pdbRecordType->papFldDes[288]->offset=(short)((char *)&prec->ftvz - (char *)prec);
    pdbRecordType->papFldDes[289]->size=sizeof(prec->nova);
    pdbRecordType->papFldDes[289]->offset=(short)((char *)&prec->nova - (char *)prec);
    pdbRecordType->papFldDes[290]->size=sizeof(prec->novb);
    pdbRecordType->papFldDes[290]->offset=(short)((char *)&prec->novb - (char *)prec);
    pdbRecordType->papFldDes[291]->size=sizeof(prec->novc);
    pdbRecordType->papFldDes[291]->offset=(short)((char *)&prec->novc - (char *)prec);
    pdbRecordType->papFldDes[292]->size=sizeof(prec->novd);
    pdbRecordType->papFldDes[292]->offset=(short)((char *)&prec->novd - (char *)prec);
    pdbRecordType->papFldDes[293]->size=sizeof(prec->nove);
    pdbRecordType->papFldDes[293]->offset=(short)((char *)&prec->nove - (char *)prec);
    pdbRecordType->papFldDes[294]->size=sizeof(prec->novf);
    pdbRecordType->papFldDes[294]->offset=(short)((char *)&prec->novf - (char *)prec);
    pdbRecordType->papFldDes[295]->size=sizeof(prec->novg);
    pdbRecordType->papFldDes[295]->offset=(short)((char *)&prec->novg - (char *)prec);
    pdbRecordType->papFldDes[296]->size=sizeof(prec->novh);
    pdbRecordType->papFldDes[296]->offset=(short)((char *)&prec->novh - (char *)prec);
    pdbRecordType->papFldDes[297]->size=sizeof(prec->novi);
    pdbRecordType->papFldDes[297]->offset=(short)((char *)&prec->novi - (char *)prec);
    pdbRecordType->papFldDes[298]->size=sizeof(prec->novj);
    pdbRecordType->papFldDes[298]->offset=(short)((char *)&prec->novj - (char *)prec);
    pdbRecordType->papFldDes[299]->size=sizeof(prec->novk);
    pdbRecordType->papFldDes[299]->offset=(short)((char *)&prec->novk - (char *)prec);
    pdbRecordType->papFldDes[300]->size=sizeof(prec->novl);
    pdbRecordType->papFldDes[300]->offset=(short)((char *)&prec->novl - (char *)prec);
    pdbRecordType->papFldDes[301]->size=sizeof(prec->novm);
    pdbRecordType->papFldDes[301]->offset=(short)((char *)&prec->novm - (char *)prec);
    pdbRecordType->papFldDes[302]->size=sizeof(prec->novn);
    pdbRecordType->papFldDes[302]->offset=(short)((char *)&prec->novn - (char *)prec);
    pdbRecordType->papFldDes[303]->size=sizeof(prec->novo);
    pdbRecordType->papFldDes[303]->offset=(short)((char *)&prec->novo - (char *)prec);
    pdbRecordType->papFldDes[304]->size=sizeof(prec->novp);
    pdbRecordType->papFldDes[304]->offset=(short)((char *)&prec->novp - (char *)prec);
    pdbRecordType->papFldDes[305]->size=sizeof(prec->novq);
    pdbRecordType->papFldDes[305]->offset=(short)((char *)&prec->novq - (char *)prec);
    pdbRecordType->papFldDes[306]->size=sizeof(prec->novr);
    pdbRecordType->papFldDes[306]->offset=(short)((char *)&prec->novr - (char *)prec);
    pdbRecordType->papFldDes[307]->size=sizeof(prec->novs);
    pdbRecordType->papFldDes[307]->offset=(short)((char *)&prec->novs - (char *)prec);
    pdbRecordType->papFldDes[308]->size=sizeof(prec->novt);
    pdbRecordType->papFldDes[308]->offset=(short)((char *)&prec->novt - (char *)prec);
    pdbRecordType->papFldDes[309]->size=sizeof(prec->novu);
    pdbRecordType->papFldDes[309]->offset=(short)((char *)&prec->novu - (char *)prec);
    pdbRecordType->papFldDes[310]->size=sizeof(prec->novv);
    pdbRecordType->papFldDes[310]->offset=(short)((char *)&prec->novv - (char *)prec);
    pdbRecordType->papFldDes[311]->size=sizeof(prec->novw);
    pdbRecordType->papFldDes[311]->offset=(short)((char *)&prec->novw - (char *)prec);
    pdbRecordType->papFldDes[312]->size=sizeof(prec->novx);
    pdbRecordType->papFldDes[312]->offset=(short)((char *)&prec->novx - (char *)prec);
    pdbRecordType->papFldDes[313]->size=sizeof(prec->novy);
    pdbRecordType->papFldDes[313]->offset=(short)((char *)&prec->novy - (char *)prec);
    pdbRecordType->papFldDes[314]->size=sizeof(prec->novz);
    pdbRecordType->papFldDes[314]->offset=(short)((char *)&prec->novz - (char *)prec);
    pdbRecordType->papFldDes[315]->size=sizeof(prec->neva);
    pdbRecordType->papFldDes[315]->offset=(short)((char *)&prec->neva - (char *)prec);
    pdbRecordType->papFldDes[316]->size=sizeof(prec->nevb);
    pdbRecordType->papFldDes[316]->offset=(short)((char *)&prec->nevb - (char *)prec);
    pdbRecordType->papFldDes[317]->size=sizeof(prec->nevc);
    pdbRecordType->papFldDes[317]->offset=(short)((char *)&prec->nevc - (char *)prec);
    pdbRecordType->papFldDes[318]->size=sizeof(prec->nevd);
    pdbRecordType->papFldDes[318]->offset=(short)((char *)&prec->nevd - (char *)prec);
    pdbRecordType->papFldDes[319]->size=sizeof(prec->neve);
    pdbRecordType->papFldDes[319]->offset=(short)((char *)&prec->neve - (char *)prec);
    pdbRecordType->papFldDes[320]->size=sizeof(prec->nevf);
    pdbRecordType->papFldDes[320]->offset=(short)((char *)&prec->nevf - (char *)prec);
    pdbRecordType->papFldDes[321]->size=sizeof(prec->nevg);
    pdbRecordType->papFldDes[321]->offset=(short)((char *)&prec->nevg - (char *)prec);
    pdbRecordType->papFldDes[322]->size=sizeof(prec->nevh);
    pdbRecordType->papFldDes[322]->offset=(short)((char *)&prec->nevh - (char *)prec);
    pdbRecordType->papFldDes[323]->size=sizeof(prec->nevi);
    pdbRecordType->papFldDes[323]->offset=(short)((char *)&prec->nevi - (char *)prec);
    pdbRecordType->papFldDes[324]->size=sizeof(prec->nevj);
    pdbRecordType->papFldDes[324]->offset=(short)((char *)&prec->nevj - (char *)prec);
    pdbRecordType->papFldDes[325]->size=sizeof(prec->nevk);
    pdbRecordType->papFldDes[325]->offset=(short)((char *)&prec->nevk - (char *)prec);
    pdbRecordType->papFldDes[326]->size=sizeof(prec->nevl);
    pdbRecordType->papFldDes[326]->offset=(short)((char *)&prec->nevl - (char *)prec);
    pdbRecordType->papFldDes[327]->size=sizeof(prec->nevm);
    pdbRecordType->papFldDes[327]->offset=(short)((char *)&prec->nevm - (char *)prec);
    pdbRecordType->papFldDes[328]->size=sizeof(prec->nevn);
    pdbRecordType->papFldDes[328]->offset=(short)((char *)&prec->nevn - (char *)prec);
    pdbRecordType->papFldDes[329]->size=sizeof(prec->nevo);
    pdbRecordType->papFldDes[329]->offset=(short)((char *)&prec->nevo - (char *)prec);
    pdbRecordType->papFldDes[330]->size=sizeof(prec->nevp);
    pdbRecordType->papFldDes[330]->offset=(short)((char *)&prec->nevp - (char *)prec);
    pdbRecordType->papFldDes[331]->size=sizeof(prec->nevq);
    pdbRecordType->papFldDes[331]->offset=(short)((char *)&prec->nevq - (char *)prec);
    pdbRecordType->papFldDes[332]->size=sizeof(prec->nevr);
    pdbRecordType->papFldDes[332]->offset=(short)((char *)&prec->nevr - (char *)prec);
    pdbRecordType->papFldDes[333]->size=sizeof(prec->nevs);
    pdbRecordType->papFldDes[333]->offset=(short)((char *)&prec->nevs - (char *)prec);
    pdbRecordType->papFldDes[334]->size=sizeof(prec->nevt);
    pdbRecordType->papFldDes[334]->offset=(short)((char *)&prec->nevt - (char *)prec);
    pdbRecordType->papFldDes[335]->size=sizeof(prec->nevu);
    pdbRecordType->papFldDes[335]->offset=(short)((char *)&prec->nevu - (char *)prec);
    pdbRecordType->papFldDes[336]->size=sizeof(prec->nevv);
    pdbRecordType->papFldDes[336]->offset=(short)((char *)&prec->nevv - (char *)prec);
    pdbRecordType->papFldDes[337]->size=sizeof(prec->nevw);
    pdbRecordType->papFldDes[337]->offset=(short)((char *)&prec->nevw - (char *)prec);
    pdbRecordType->papFldDes[338]->size=sizeof(prec->nevx);
    pdbRecordType->papFldDes[338]->offset=(short)((char *)&prec->nevx - (char *)prec);
    pdbRecordType->papFldDes[339]->size=sizeof(prec->nevy);
    pdbRecordType->papFldDes[339]->offset=(short)((char *)&prec->nevy - (char *)prec);
    pdbRecordType->papFldDes[340]->size=sizeof(prec->nevz);
    pdbRecordType->papFldDes[340]->offset=(short)((char *)&prec->nevz - (char *)prec);
    pdbRecordType->papFldDes[341]->size=sizeof(prec->onva);
    pdbRecordType->papFldDes[341]->offset=(short)((char *)&prec->onva - (char *)prec);
    pdbRecordType->papFldDes[342]->size=sizeof(prec->onvb);
    pdbRecordType->papFldDes[342]->offset=(short)((char *)&prec->onvb - (char *)prec);
    pdbRecordType->papFldDes[343]->size=sizeof(prec->onvc);
    pdbRecordType->papFldDes[343]->offset=(short)((char *)&prec->onvc - (char *)prec);
    pdbRecordType->papFldDes[344]->size=sizeof(prec->onvd);
    pdbRecordType->papFldDes[344]->offset=(short)((char *)&prec->onvd - (char *)prec);
    pdbRecordType->papFldDes[345]->size=sizeof(prec->onve);
    pdbRecordType->papFldDes[345]->offset=(short)((char *)&prec->onve - (char *)prec);
    pdbRecordType->papFldDes[346]->size=sizeof(prec->onvf);
    pdbRecordType->papFldDes[346]->offset=(short)((char *)&prec->onvf - (char *)prec);
    pdbRecordType->papFldDes[347]->size=sizeof(prec->onvg);
    pdbRecordType->papFldDes[347]->offset=(short)((char *)&prec->onvg - (char *)prec);
    pdbRecordType->papFldDes[348]->size=sizeof(prec->onvh);
    pdbRecordType->papFldDes[348]->offset=(short)((char *)&prec->onvh - (char *)prec);
    pdbRecordType->papFldDes[349]->size=sizeof(prec->onvi);
    pdbRecordType->papFldDes[349]->offset=(short)((char *)&prec->onvi - (char *)prec);
    pdbRecordType->papFldDes[350]->size=sizeof(prec->onvj);
    pdbRecordType->papFldDes[350]->offset=(short)((char *)&prec->onvj - (char *)prec);
    pdbRecordType->papFldDes[351]->size=sizeof(prec->onvk);
    pdbRecordType->papFldDes[351]->offset=(short)((char *)&prec->onvk - (char *)prec);
    pdbRecordType->papFldDes[352]->size=sizeof(prec->onvl);
    pdbRecordType->papFldDes[352]->offset=(short)((char *)&prec->onvl - (char *)prec);
    pdbRecordType->papFldDes[353]->size=sizeof(prec->onvm);
    pdbRecordType->papFldDes[353]->offset=(short)((char *)&prec->onvm - (char *)prec);
    pdbRecordType->papFldDes[354]->size=sizeof(prec->onvn);
    pdbRecordType->papFldDes[354]->offset=(short)((char *)&prec->onvn - (char *)prec);
    pdbRecordType->papFldDes[355]->size=sizeof(prec->onvo);
    pdbRecordType->papFldDes[355]->offset=(short)((char *)&prec->onvo - (char *)prec);
    pdbRecordType->papFldDes[356]->size=sizeof(prec->onvp);
    pdbRecordType->papFldDes[356]->offset=(short)((char *)&prec->onvp - (char *)prec);
    pdbRecordType->papFldDes[357]->size=sizeof(prec->onvq);
    pdbRecordType->papFldDes[357]->offset=(short)((char *)&prec->onvq - (char *)prec);
    pdbRecordType->papFldDes[358]->size=sizeof(prec->onvr);
    pdbRecordType->papFldDes[358]->offset=(short)((char *)&prec->onvr - (char *)prec);
    pdbRecordType->papFldDes[359]->size=sizeof(prec->onvs);
    pdbRecordType->papFldDes[359]->offset=(short)((char *)&prec->onvs - (char *)prec);
    pdbRecordType->papFldDes[360]->size=sizeof(prec->onvt);
    pdbRecordType->papFldDes[360]->offset=(short)((char *)&prec->onvt - (char *)prec);
    pdbRecordType->papFldDes[361]->size=sizeof(prec->onvu);
    pdbRecordType->papFldDes[361]->offset=(short)((char *)&prec->onvu - (char *)prec);
    pdbRecordType->papFldDes[362]->size=sizeof(prec->onvv);
    pdbRecordType->papFldDes[362]->offset=(short)((char *)&prec->onvv - (char *)prec);
    pdbRecordType->papFldDes[363]->size=sizeof(prec->onvw);
    pdbRecordType->papFldDes[363]->offset=(short)((char *)&prec->onvw - (char *)prec);
    pdbRecordType->papFldDes[364]->size=sizeof(prec->onvx);
    pdbRecordType->papFldDes[364]->offset=(short)((char *)&prec->onvx - (char *)prec);
    pdbRecordType->papFldDes[365]->size=sizeof(prec->onvy);
    pdbRecordType->papFldDes[365]->offset=(short)((char *)&prec->onvy - (char *)prec);
    pdbRecordType->papFldDes[366]->size=sizeof(prec->onvz);
    pdbRecordType->papFldDes[366]->offset=(short)((char *)&prec->onvz - (char *)prec);
    pdbRecordType->papFldDes[367]->size=sizeof(prec->vlda);
    pdbRecordType->papFldDes[367]->offset=(short)((char *)&prec->vlda - (char *)prec);
    pdbRecordType->papFldDes[368]->size=sizeof(prec->vldb);
    pdbRecordType->papFldDes[368]->offset=(short)((char *)&prec->vldb - (char *)prec);
    pdbRecordType->papFldDes[369]->size=sizeof(prec->vldc);
    pdbRecordType->papFldDes[369]->offset=(short)((char *)&prec->vldc - (char *)prec);
    pdbRecordType->papFldDes[370]->size=sizeof(prec->vldd);
    pdbRecordType->papFldDes[370]->offset=(short)((char *)&prec->vldd - (char *)prec);
    pdbRecordType->papFldDes[371]->size=sizeof(prec->vlde);
    pdbRecordType->papFldDes[371]->offset=(short)((char *)&prec->vlde - (char *)prec);
    pdbRecordType->papFldDes[372]->size=sizeof(prec->vldf);
    pdbRecordType->papFldDes[372]->offset=(short)((char *)&prec->vldf - (char *)prec);
    pdbRecordType->papFldDes[373]->size=sizeof(prec->vldg);
    pdbRecordType->papFldDes[373]->offset=(short)((char *)&prec->vldg - (char *)prec);
    pdbRecordType->papFldDes[374]->size=sizeof(prec->vldh);
    pdbRecordType->papFldDes[374]->offset=(short)((char *)&prec->vldh - (char *)prec);
    pdbRecordType->papFldDes[375]->size=sizeof(prec->vldi);
    pdbRecordType->papFldDes[375]->offset=(short)((char *)&prec->vldi - (char *)prec);
    pdbRecordType->papFldDes[376]->size=sizeof(prec->vldj);
    pdbRecordType->papFldDes[376]->offset=(short)((char *)&prec->vldj - (char *)prec);
    pdbRecordType->papFldDes[377]->size=sizeof(prec->vldk);
    pdbRecordType->papFldDes[377]->offset=(short)((char *)&prec->vldk - (char *)prec);
    pdbRecordType->papFldDes[378]->size=sizeof(prec->vldl);
    pdbRecordType->papFldDes[378]->offset=(short)((char *)&prec->vldl - (char *)prec);
    pdbRecordType->papFldDes[379]->size=sizeof(prec->vldm);
    pdbRecordType->papFldDes[379]->offset=(short)((char *)&prec->vldm - (char *)prec);
    pdbRecordType->papFldDes[380]->size=sizeof(prec->vldn);
    pdbRecordType->papFldDes[380]->offset=(short)((char *)&prec->vldn - (char *)prec);
    pdbRecordType->papFldDes[381]->size=sizeof(prec->vldo);
    pdbRecordType->papFldDes[381]->offset=(short)((char *)&prec->vldo - (char *)prec);
    pdbRecordType->papFldDes[382]->size=sizeof(prec->vldp);
    pdbRecordType->papFldDes[382]->offset=(short)((char *)&prec->vldp - (char *)prec);
    pdbRecordType->papFldDes[383]->size=sizeof(prec->vldq);
    pdbRecordType->papFldDes[383]->offset=(short)((char *)&prec->vldq - (char *)prec);
    pdbRecordType->papFldDes[384]->size=sizeof(prec->vldr);
    pdbRecordType->papFldDes[384]->offset=(short)((char *)&prec->vldr - (char *)prec);
    pdbRecordType->papFldDes[385]->size=sizeof(prec->vlds);
    pdbRecordType->papFldDes[385]->offset=(short)((char *)&prec->vlds - (char *)prec);
    pdbRecordType->papFldDes[386]->size=sizeof(prec->vldt);
    pdbRecordType->papFldDes[386]->offset=(short)((char *)&prec->vldt - (char *)prec);
    pdbRecordType->papFldDes[387]->size=sizeof(prec->vldu);
    pdbRecordType->papFldDes[387]->offset=(short)((char *)&prec->vldu - (char *)prec);
    pdbRecordType->papFldDes[388]->size=sizeof(prec->vldv);
    pdbRecordType->papFldDes[388]->offset=(short)((char *)&prec->vldv - (char *)prec);
    pdbRecordType->papFldDes[389]->size=sizeof(prec->vldw);
    pdbRecordType->papFldDes[389]->offset=(short)((char *)&prec->vldw - (char *)prec);
    pdbRecordType->papFldDes[390]->size=sizeof(prec->vldx);
    pdbRecordType->papFldDes[390]->offset=(short)((char *)&prec->vldx - (char *)prec);
    pdbRecordType->papFldDes[391]->size=sizeof(prec->vldy);
    pdbRecordType->papFldDes[391]->offset=(short)((char *)&prec->vldy - (char *)prec);
    pdbRecordType->papFldDes[392]->size=sizeof(prec->vldz);
    pdbRecordType->papFldDes[392]->offset=(short)((char *)&prec->vldz - (char *)prec);
    pdbRecordType->rec_size=sizeof(*prec);
    return(0);
}
epicsExportRegistrar(luasubRecordSizeOffset);

#ifdef __cplusplus
}
#endif

#endif /* GEN_SIZE_OFFSET */
