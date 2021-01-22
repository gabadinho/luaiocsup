/* luasubRecord.h generated from luasubRecord.dbd */

#ifndef INC_luasubRecord_H
#define INC_luasubRecord_H

#include "dbBase.h"

#include "epicsTypes.h"
#include "link.h"
#include "epicsMutex.h"
#include "ellLib.h"
#include "devSup.h"
#include "epicsTime.h"
struct luasubRecord;

#ifndef luasubACTP_NUM_CHOICES
typedef enum {
    luasubACTP_FUNORCHK             /* FUNORCHK */,
    luasubACTP_FUNCTION             /* FUNCTION */,
    luasubACTP_CHUNK                /* CHUNK */
} luasubACTP;
#define luasubACTP_NUM_CHOICES 3
#endif

#ifndef luasubDRVO_NUM_CHOICES
typedef enum {
    luasubDRVO_NEVER                /* NEVER */,
    luasubDRVO_ON_CHANGE            /* ON CHANGE */,
    luasubDRVO_ALWAYS               /* ALWAYS */,
    luasubDRVO_ON_VALID             /* ON VALID */
} luasubDRVO;
#define luasubDRVO_NUM_CHOICES 4
#endif

#ifndef luasubOVVL_NUM_CHOICES
typedef enum {
    luasubOVVL_INVALID              /* INVALID */,
    luasubOVVL_VALID                /* VALID */
} luasubOVVL;
#define luasubOVVL_NUM_CHOICES 2
#endif

#ifndef luasubPCTP_NUM_CHOICES
typedef enum {
    luasubPCTP_UNKNOWN              /* UNKNOWN */,
    luasubPCTP_ERROR                /* ERROR */,
    luasubPCTP_FUNCTION             /* FUNCTION */,
    luasubPCTP_CHUNK                /* CHUNK */
} luasubPCTP;
#define luasubPCTP_NUM_CHOICES 4
#endif

typedef struct luasubRecord {
    char                name[61];   /* Record Name */
    char                desc[41];   /* Descriptor */
    char                asg[29];    /* Access Security Group */
    epicsEnum16         scan;       /* Scan Mechanism */
    epicsEnum16         pini;       /* Process at iocInit */
    epicsInt16          phas;       /* Scan Phase */
    char                evnt[40];   /* Event Name */
    epicsInt16          tse;        /* Time Stamp Event */
    DBLINK              tsel;       /* Time Stamp Link */
    epicsEnum16         dtyp;       /* Device Type */
    epicsInt16          disv;       /* Disable Value */
    epicsInt16          disa;       /* Disable */
    DBLINK              sdis;       /* Scanning Disable */
    epicsMutexId        mlok;       /* Monitor lock */
    ELLLIST             mlis;       /* Monitor List */
    ELLLIST             bklnk;      /* Backwards link tracking */
    epicsUInt8          disp;       /* Disable putField */
    epicsUInt8          proc;       /* Force Processing */
    epicsEnum16         stat;       /* Alarm Status */
    epicsEnum16         sevr;       /* Alarm Severity */
    epicsEnum16         nsta;       /* New Alarm Status */
    epicsEnum16         nsev;       /* New Alarm Severity */
    epicsEnum16         acks;       /* Alarm Ack Severity */
    epicsEnum16         ackt;       /* Alarm Ack Transient */
    epicsEnum16         diss;       /* Disable Alarm Sevrty */
    epicsUInt8          lcnt;       /* Lock Count */
    epicsUInt8          pact;       /* Record active */
    epicsUInt8          putf;       /* dbPutField process */
    epicsUInt8          rpro;       /* Reprocess  */
    struct asgMember    *asp;       /* Access Security Pvt */
    struct processNotify *ppn;      /* pprocessNotify */
    struct processNotifyRecord *ppnr; /* pprocessNotifyRecord */
    struct scan_element *spvt;      /* Scan Private */
    struct typed_rset   *rset;      /* Address of RSET */
    unambiguous_dset    *dset;      /* DSET address */
    void                *dpvt;      /* Device Private */
    struct dbRecordType *rdes;      /* Address of dbRecordType */
    struct lockRecord   *lset;      /* Lock Set */
    epicsEnum16         prio;       /* Scheduling Priority */
    epicsUInt8          tpro;       /* Trace Processing */
    epicsUInt8          bkpt;       /* Break Point */
    epicsUInt8          udf;        /* Undefined */
    epicsEnum16         udfs;       /* Undefined Alarm Sevrty */
    epicsTimeStamp      time;       /* Time */
    DBLINK              flnk;       /* Forward Process Link */
    epicsInt32          val;        /* Subr. return value */
    epicsInt32          oval;       /* Old return value */
    DBLINK              inp;        /* Input Specification */
    char                icod[40];   /* Initialization Code */
    epicsEnum16         actp;       /* Name Type */
    epicsUInt8          rst;        /* Reset Execution Stack */
    char                pcod[40];   /* Processing Code */
    char                ocod[40];   /* Old Processing Code */
    epicsInt16          yldd;       /* Is Yielded? */
    epicsEnum16         pctp;       /* Code Type */
    epicsInt16          prec;       /* Display Precision */
    epicsEnum16         drvo;       /* Drive Outputs */
    DBLINK              inpa;       /* Input Link A */
    DBLINK              inpb;       /* Input Link B */
    DBLINK              inpc;       /* Input Link C */
    DBLINK              inpd;       /* Input Link D */
    DBLINK              inpe;       /* Input Link E */
    DBLINK              inpf;       /* Input Link F */
    DBLINK              inpg;       /* Input Link G */
    DBLINK              inph;       /* Input Link H */
    DBLINK              inpi;       /* Input Link I */
    DBLINK              inpj;       /* Input Link J */
    DBLINK              inpk;       /* Input Link K */
    DBLINK              inpl;       /* Input Link L */
    DBLINK              inpm;       /* Input Link M */
    DBLINK              inpn;       /* Input Link N */
    DBLINK              inpo;       /* Input Link O */
    DBLINK              inpp;       /* Input Link P */
    DBLINK              inpq;       /* Input Link Q */
    DBLINK              inpr;       /* Input Link R */
    DBLINK              inps;       /* Input Link S */
    DBLINK              inpt;       /* Input Link T */
    DBLINK              inpu;       /* Input Link U */
    DBLINK              inpv;       /* Input Link V */
    DBLINK              inpw;       /* Input Link W */
    DBLINK              inpx;       /* Input Link X */
    DBLINK              inpy;       /* Input Link Y */
    DBLINK              inpz;       /* Input Link Z */
    void *a;                        /* Input value A */
    void *b;                        /* Input value B */
    void *c;                        /* Input value C */
    void *d;                        /* Input value D */
    void *e;                        /* Input value E */
    void *f;                        /* Input value F */
    void *g;                        /* Input value G */
    void *h;                        /* Input value H */
    void *i;                        /* Input value I */
    void *j;                        /* Input value J */
    void *k;                        /* Input value K */
    void *l;                        /* Input value L */
    void *m;                        /* Input value M */
    void *n;                        /* Input value N */
    void *o;                        /* Input value O */
    void *p;                        /* Input value P */
    void *q;                        /* Input value Q */
    void *r;                        /* Input value R */
    void *s;                        /* Input value S */
    void *t;                        /* Input value T */
    void *u;                        /* Input value U */
    void *v;                        /* Input value V */
    void *w;                        /* Input value W */
    void *x;                        /* Input value X */
    void *y;                        /* Input value Y */
    void *z;                        /* Input value Z */
    epicsEnum16         fta;        /* Type of A */
    epicsEnum16         ftb;        /* Type of B */
    epicsEnum16         ftc;        /* Type of C */
    epicsEnum16         ftd;        /* Type of D */
    epicsEnum16         fte;        /* Type of E */
    epicsEnum16         ftf;        /* Type of F */
    epicsEnum16         ftg;        /* Type of G */
    epicsEnum16         fth;        /* Type of H */
    epicsEnum16         fti;        /* Type of I */
    epicsEnum16         ftj;        /* Type of J */
    epicsEnum16         ftk;        /* Type of K */
    epicsEnum16         ftl;        /* Type of L */
    epicsEnum16         ftm;        /* Type of M */
    epicsEnum16         ftn;        /* Type of N */
    epicsEnum16         fto;        /* Type of O */
    epicsEnum16         ftp;        /* Type of P */
    epicsEnum16         ftq;        /* Type of Q */
    epicsEnum16         ftr;        /* Type of R */
    epicsEnum16         fts;        /* Type of S */
    epicsEnum16         ftt;        /* Type of T */
    epicsEnum16         ftu;        /* Type of U */
    epicsEnum16         ftv;        /* Type of V */
    epicsEnum16         ftw;        /* Type of W */
    epicsEnum16         ftx;        /* Type of X */
    epicsEnum16         fty;        /* Type of Y */
    epicsEnum16         ftz;        /* Type of Z */
    epicsUInt32         noa;        /* Max. elements in A */
    epicsUInt32         nob;        /* Max. elements in B */
    epicsUInt32         noc;        /* Max. elements in C */
    epicsUInt32         nod;        /* Max. elements in D */
    epicsUInt32         noe;        /* Max. elements in E */
    epicsUInt32         nof;        /* Max. elements in F */
    epicsUInt32         nog;        /* Max. elements in G */
    epicsUInt32         noh;        /* Max. elements in H */
    epicsUInt32         noi;        /* Max. elements in I */
    epicsUInt32         noj;        /* Max. elements in J */
    epicsUInt32         nok;        /* Max. elements in K */
    epicsUInt32         nol;        /* Max. elements in L */
    epicsUInt32         nom;        /* Max. elements in M */
    epicsUInt32         non;        /* Max. elements in N */
    epicsUInt32         noo;        /* Max. elements in O */
    epicsUInt32         nop;        /* Max. elements in P */
    epicsUInt32         noq;        /* Max. elements in Q */
    epicsUInt32         nor;        /* Max. elements in R */
    epicsUInt32         nos;        /* Max. elements in S */
    epicsUInt32         NOT;        /* Max. elements in T */
    epicsUInt32         nou;        /* Max. elements in U */
    epicsUInt32         nov;        /* Max. elements in V */
    epicsUInt32         now;        /* Max. elements in W */
    epicsUInt32         nox;        /* Max. elements in X */
    epicsUInt32         noy;        /* Max. elements in Y */
    epicsUInt32         noz;        /* Max. elements in Z */
    epicsUInt32         nea;        /* Num. elements in A */
    epicsUInt32         neb;        /* Num. elements in B */
    epicsUInt32         nec;        /* Num. elements in C */
    epicsUInt32         ned;        /* Num. elements in D */
    epicsUInt32         nee;        /* Num. elements in E */
    epicsUInt32         nef;        /* Num. elements in F */
    epicsUInt32         neg;        /* Num. elements in G */
    epicsUInt32         neh;        /* Num. elements in H */
    epicsUInt32         nei;        /* Num. elements in I */
    epicsUInt32         nej;        /* Num. elements in J */
    epicsUInt32         nek;        /* Num. elements in K */
    epicsUInt32         nel;        /* Num. elements in L */
    epicsUInt32         nem;        /* Num. elements in M */
    epicsUInt32         nen;        /* Num. elements in N */
    epicsUInt32         neo;        /* Num. elements in O */
    epicsUInt32         nep;        /* Num. elements in P */
    epicsUInt32         neq;        /* Num. elements in Q */
    epicsUInt32         ner;        /* Num. elements in R */
    epicsUInt32         nes;        /* Num. elements in S */
    epicsUInt32         net;        /* Num. elements in T */
    epicsUInt32         neu;        /* Num. elements in U */
    epicsUInt32         nev;        /* Num. elements in V */
    epicsUInt32         NEW;        /* Num. elements in W */
    epicsUInt32         nex;        /* Num. elements in X */
    epicsUInt32         ney;        /* Num. elements in Y */
    epicsUInt32         nez;        /* Num. elements in Z */
    DBLINK              outa;       /* Output Link A */
    DBLINK              outb;       /* Output Link B */
    DBLINK              outc;       /* Output Link C */
    DBLINK              outd;       /* Output Link D */
    DBLINK              oute;       /* Output Link E */
    DBLINK              outf;       /* Output Link F */
    DBLINK              outg;       /* Output Link G */
    DBLINK              outh;       /* Output Link H */
    DBLINK              outi;       /* Output Link I */
    DBLINK              outj;       /* Output Link J */
    DBLINK              outk;       /* Output Link K */
    DBLINK              outl;       /* Output Link L */
    DBLINK              outm;       /* Output Link M */
    DBLINK              outn;       /* Output Link N */
    DBLINK              outo;       /* Output Link O */
    DBLINK              outp;       /* Output Link P */
    DBLINK              outq;       /* Output Link Q */
    DBLINK              outr;       /* Output Link R */
    DBLINK              outs;       /* Output Link S */
    DBLINK              outt;       /* Output Link T */
    DBLINK              outu;       /* Output Link U */
    DBLINK              outv;       /* Output Link V */
    DBLINK              outw;       /* Output Link W */
    DBLINK              outx;       /* Output Link X */
    DBLINK              outy;       /* Output Link Y */
    DBLINK              outz;       /* Output Link Z */
    void *vala;                     /* Output value A */
    void *valb;                     /* Output value B */
    void *valc;                     /* Output value C */
    void *vald;                     /* Output value D */
    void *vale;                     /* Output value E */
    void *valf;                     /* Output value F */
    void *valg;                     /* Output value G */
    void *valh;                     /* Output value H */
    void *vali;                     /* Output value I */
    void *valj;                     /* Output value J */
    void *valk;                     /* Output value K */
    void *vall;                     /* Output value L */
    void *valm;                     /* Output value M */
    void *valn;                     /* Output value N */
    void *valo;                     /* Output value O */
    void *valp;                     /* Output value P */
    void *valq;                     /* Output value Q */
    void *valr;                     /* Output value R */
    void *vals;                     /* Output value S */
    void *valt;                     /* Output value T */
    void *valu;                     /* Output value U */
    void *valv;                     /* Output value V */
    void *valw;                     /* Output value W */
    void *valx;                     /* Output value X */
    void *valy;                     /* Output value Y */
    void *valz;                     /* Output value Z */
    void *ovla;                     /* Old Output A */
    void *ovlb;                     /* Old Output B */
    void *ovlc;                     /* Old Output C */
    void *ovld;                     /* Old Output D */
    void *ovle;                     /* Old Output E */
    void *ovlf;                     /* Old Output F */
    void *ovlg;                     /* Old Output G */
    void *ovlh;                     /* Old Output H */
    void *ovli;                     /* Old Output I */
    void *ovlj;                     /* Old Output J */
    void *ovlk;                     /* Old Output K */
    void *ovll;                     /* Old Output L */
    void *ovlm;                     /* Old Output M */
    void *ovln;                     /* Old Output N */
    void *ovlo;                     /* Old Output O */
    void *ovlp;                     /* Old Output P */
    void *ovlq;                     /* Old Output Q */
    void *ovlr;                     /* Old Output R */
    void *ovls;                     /* Old Output S */
    void *ovlt;                     /* Old Output T */
    void *ovlu;                     /* Old Output U */
    void *ovlv;                     /* Old Output V */
    void *ovlw;                     /* Old Output W */
    void *ovlx;                     /* Old Output X */
    void *ovly;                     /* Old Output Y */
    void *ovlz;                     /* Old Output Z */
    epicsEnum16         ftva;       /* Type of VALA */
    epicsEnum16         ftvb;       /* Type of VALB */
    epicsEnum16         ftvc;       /* Type of VALC */
    epicsEnum16         ftvd;       /* Type of VALD */
    epicsEnum16         ftve;       /* Type of VALE */
    epicsEnum16         ftvf;       /* Type of VALF */
    epicsEnum16         ftvg;       /* Type of VALG */
    epicsEnum16         ftvh;       /* Type of VALH */
    epicsEnum16         ftvi;       /* Type of VALI */
    epicsEnum16         ftvj;       /* Type of VALJ */
    epicsEnum16         ftvk;       /* Type of VALK */
    epicsEnum16         ftvl;       /* Type of VALL */
    epicsEnum16         ftvm;       /* Type of VALM */
    epicsEnum16         ftvn;       /* Type of VALN */
    epicsEnum16         ftvo;       /* Type of VALO */
    epicsEnum16         ftvp;       /* Type of VALP */
    epicsEnum16         ftvq;       /* Type of VALQ */
    epicsEnum16         ftvr;       /* Type of VALR */
    epicsEnum16         ftvs;       /* Type of VALS */
    epicsEnum16         ftvt;       /* Type of VALT */
    epicsEnum16         ftvu;       /* Type of VALU */
    epicsEnum16         ftvv;       /* Type of VALV */
    epicsEnum16         ftvw;       /* Type of VALW */
    epicsEnum16         ftvx;       /* Type of VALX */
    epicsEnum16         ftvy;       /* Type of VALY */
    epicsEnum16         ftvz;       /* Type of VALZ */
    epicsUInt32         nova;       /* Max. elements in VALA */
    epicsUInt32         novb;       /* Max. elements in VALB */
    epicsUInt32         novc;       /* Max. elements in VALC */
    epicsUInt32         novd;       /* Max. elements in VALD */
    epicsUInt32         nove;       /* Max. elements in VALE */
    epicsUInt32         novf;       /* Max. elements in VALF */
    epicsUInt32         novg;       /* Max. elements in VALG */
    epicsUInt32         novh;       /* Max. elements in VAlH */
    epicsUInt32         novi;       /* Max. elements in VALI */
    epicsUInt32         novj;       /* Max. elements in VALJ */
    epicsUInt32         novk;       /* Max. elements in VALK */
    epicsUInt32         novl;       /* Max. elements in VALL */
    epicsUInt32         novm;       /* Max. elements in VALM */
    epicsUInt32         novn;       /* Max. elements in VALN */
    epicsUInt32         novo;       /* Max. elements in VALO */
    epicsUInt32         novp;       /* Max. elements in VALP */
    epicsUInt32         novq;       /* Max. elements in VALQ */
    epicsUInt32         novr;       /* Max. elements in VALR */
    epicsUInt32         novs;       /* Max. elements in VALS */
    epicsUInt32         novt;       /* Max. elements in VALT */
    epicsUInt32         novu;       /* Max. elements in VALU */
    epicsUInt32         novv;       /* Max. elements in VALV */
    epicsUInt32         novw;       /* Max. elements in VALW */
    epicsUInt32         novx;       /* Max. elements in VALX */
    epicsUInt32         novy;       /* Max. elements in VALY */
    epicsUInt32         novz;       /* Max. elements in VALZ */
    epicsUInt32         neva;       /* Num. elements in VALA */
    epicsUInt32         nevb;       /* Num. elements in VALB */
    epicsUInt32         nevc;       /* Num. elements in VALC */
    epicsUInt32         nevd;       /* Num. elements in VALD */
    epicsUInt32         neve;       /* Num. elements in VALE */
    epicsUInt32         nevf;       /* Num. elements in VALF */
    epicsUInt32         nevg;       /* Num. elements in VALG */
    epicsUInt32         nevh;       /* Num. elements in VAlH */
    epicsUInt32         nevi;       /* Num. elements in VALI */
    epicsUInt32         nevj;       /* Num. elements in VALJ */
    epicsUInt32         nevk;       /* Num. elements in VALK */
    epicsUInt32         nevl;       /* Num. elements in VALL */
    epicsUInt32         nevm;       /* Num. elements in VALM */
    epicsUInt32         nevn;       /* Num. elements in VALN */
    epicsUInt32         nevo;       /* Num. elements in VALO */
    epicsUInt32         nevp;       /* Num. elements in VALP */
    epicsUInt32         nevq;       /* Num. elements in VALQ */
    epicsUInt32         nevr;       /* Num. elements in VALR */
    epicsUInt32         nevs;       /* Num. elements in VALS */
    epicsUInt32         nevt;       /* Num. elements in VALT */
    epicsUInt32         nevu;       /* Num. elements in VALU */
    epicsUInt32         nevv;       /* Num. elements in VALV */
    epicsUInt32         nevw;       /* Num. elements in VALW */
    epicsUInt32         nevx;       /* Num. elements in VALX */
    epicsUInt32         nevy;       /* Num. elements in VALY */
    epicsUInt32         nevz;       /* Num. elements in VALZ */
    epicsUInt32         onva;       /* Num. elements in OVLA */
    epicsUInt32         onvb;       /* Num. elements in OVLB */
    epicsUInt32         onvc;       /* Num. elements in OVLC */
    epicsUInt32         onvd;       /* Num. elements in OVLD */
    epicsUInt32         onve;       /* Num. elements in OVLE */
    epicsUInt32         onvf;       /* Num. elements in OVLF */
    epicsUInt32         onvg;       /* Num. elements in OVLG */
    epicsUInt32         onvh;       /* Num. elements in OVLH */
    epicsUInt32         onvi;       /* Num. elements in OVLI */
    epicsUInt32         onvj;       /* Num. elements in OVLJ */
    epicsUInt32         onvk;       /* Num. elements in OVLK */
    epicsUInt32         onvl;       /* Num. elements in OVLL */
    epicsUInt32         onvm;       /* Num. elements in OVLM */
    epicsUInt32         onvn;       /* Num. elements in OVLN */
    epicsUInt32         onvo;       /* Num. elements in OVLO */
    epicsUInt32         onvp;       /* Num. elements in OVLP */
    epicsUInt32         onvq;       /* Num. elements in OVLQ */
    epicsUInt32         onvr;       /* Num. elements in OVLR */
    epicsUInt32         onvs;       /* Num. elements in OVLS */
    epicsUInt32         onvt;       /* Num. elements in OVLT */
    epicsUInt32         onvu;       /* Num. elements in OVLU */
    epicsUInt32         onvv;       /* Num. elements in OVLV */
    epicsUInt32         onvw;       /* Num. elements in OVLW */
    epicsUInt32         onvx;       /* Num. elements in OVLX */
    epicsUInt32         onvy;       /* Num. elements in OVLY */
    epicsUInt32         onvz;       /* Num. elements in OVLZ */
    epicsEnum16         vlda;       /* Output Valid VALA */
    epicsEnum16         vldb;       /* Output Valid VALB */
    epicsEnum16         vldc;       /* Output Valid VALC */
    epicsEnum16         vldd;       /* Output Valid VALD */
    epicsEnum16         vlde;       /* Output Valid VALE */
    epicsEnum16         vldf;       /* Output Valid VALF */
    epicsEnum16         vldg;       /* Output Valid VALG */
    epicsEnum16         vldh;       /* Output Valid VALH */
    epicsEnum16         vldi;       /* Output Valid VALI */
    epicsEnum16         vldj;       /* Output Valid VALJ */
    epicsEnum16         vldk;       /* Output Valid VALK */
    epicsEnum16         vldl;       /* Output Valid VALL */
    epicsEnum16         vldm;       /* Output Valid VALM */
    epicsEnum16         vldn;       /* Output Valid VALN */
    epicsEnum16         vldo;       /* Output Valid VALO */
    epicsEnum16         vldp;       /* Output Valid VALP */
    epicsEnum16         vldq;       /* Output Valid VALQ */
    epicsEnum16         vldr;       /* Output Valid VALR */
    epicsEnum16         vlds;       /* Output Valid VALS */
    epicsEnum16         vldt;       /* Output Valid VALT */
    epicsEnum16         vldu;       /* Output Valid VALU */
    epicsEnum16         vldv;       /* Output Valid VALV */
    epicsEnum16         vldw;       /* Output Valid VALW */
    epicsEnum16         vldx;       /* Output Valid VALX */
    epicsEnum16         vldy;       /* Output Valid VALY */
    epicsEnum16         vldz;       /* Output Valid VALZ */
} luasubRecord;

typedef enum {
	luasubRecordNAME = 0,
	luasubRecordDESC = 1,
	luasubRecordASG = 2,
	luasubRecordSCAN = 3,
	luasubRecordPINI = 4,
	luasubRecordPHAS = 5,
	luasubRecordEVNT = 6,
	luasubRecordTSE = 7,
	luasubRecordTSEL = 8,
	luasubRecordDTYP = 9,
	luasubRecordDISV = 10,
	luasubRecordDISA = 11,
	luasubRecordSDIS = 12,
	luasubRecordMLOK = 13,
	luasubRecordMLIS = 14,
	luasubRecordBKLNK = 15,
	luasubRecordDISP = 16,
	luasubRecordPROC = 17,
	luasubRecordSTAT = 18,
	luasubRecordSEVR = 19,
	luasubRecordNSTA = 20,
	luasubRecordNSEV = 21,
	luasubRecordACKS = 22,
	luasubRecordACKT = 23,
	luasubRecordDISS = 24,
	luasubRecordLCNT = 25,
	luasubRecordPACT = 26,
	luasubRecordPUTF = 27,
	luasubRecordRPRO = 28,
	luasubRecordASP = 29,
	luasubRecordPPN = 30,
	luasubRecordPPNR = 31,
	luasubRecordSPVT = 32,
	luasubRecordRSET = 33,
	luasubRecordDSET = 34,
	luasubRecordDPVT = 35,
	luasubRecordRDES = 36,
	luasubRecordLSET = 37,
	luasubRecordPRIO = 38,
	luasubRecordTPRO = 39,
	luasubRecordBKPT = 40,
	luasubRecordUDF = 41,
	luasubRecordUDFS = 42,
	luasubRecordTIME = 43,
	luasubRecordFLNK = 44,
	luasubRecordVAL = 45,
	luasubRecordOVAL = 46,
	luasubRecordINP = 47,
	luasubRecordICOD = 48,
	luasubRecordACTP = 49,
	luasubRecordRST = 50,
	luasubRecordPCOD = 51,
	luasubRecordOCOD = 52,
	luasubRecordYLDD = 53,
	luasubRecordPCTP = 54,
	luasubRecordPREC = 55,
	luasubRecordDRVO = 56,
	luasubRecordINPA = 57,
	luasubRecordINPB = 58,
	luasubRecordINPC = 59,
	luasubRecordINPD = 60,
	luasubRecordINPE = 61,
	luasubRecordINPF = 62,
	luasubRecordINPG = 63,
	luasubRecordINPH = 64,
	luasubRecordINPI = 65,
	luasubRecordINPJ = 66,
	luasubRecordINPK = 67,
	luasubRecordINPL = 68,
	luasubRecordINPM = 69,
	luasubRecordINPN = 70,
	luasubRecordINPO = 71,
	luasubRecordINPP = 72,
	luasubRecordINPQ = 73,
	luasubRecordINPR = 74,
	luasubRecordINPS = 75,
	luasubRecordINPT = 76,
	luasubRecordINPU = 77,
	luasubRecordINPV = 78,
	luasubRecordINPW = 79,
	luasubRecordINPX = 80,
	luasubRecordINPY = 81,
	luasubRecordINPZ = 82,
	luasubRecordA = 83,
	luasubRecordB = 84,
	luasubRecordC = 85,
	luasubRecordD = 86,
	luasubRecordE = 87,
	luasubRecordF = 88,
	luasubRecordG = 89,
	luasubRecordH = 90,
	luasubRecordI = 91,
	luasubRecordJ = 92,
	luasubRecordK = 93,
	luasubRecordL = 94,
	luasubRecordM = 95,
	luasubRecordN = 96,
	luasubRecordO = 97,
	luasubRecordP = 98,
	luasubRecordQ = 99,
	luasubRecordR = 100,
	luasubRecordS = 101,
	luasubRecordT = 102,
	luasubRecordU = 103,
	luasubRecordV = 104,
	luasubRecordW = 105,
	luasubRecordX = 106,
	luasubRecordY = 107,
	luasubRecordZ = 108,
	luasubRecordFTA = 109,
	luasubRecordFTB = 110,
	luasubRecordFTC = 111,
	luasubRecordFTD = 112,
	luasubRecordFTE = 113,
	luasubRecordFTF = 114,
	luasubRecordFTG = 115,
	luasubRecordFTH = 116,
	luasubRecordFTI = 117,
	luasubRecordFTJ = 118,
	luasubRecordFTK = 119,
	luasubRecordFTL = 120,
	luasubRecordFTM = 121,
	luasubRecordFTN = 122,
	luasubRecordFTO = 123,
	luasubRecordFTP = 124,
	luasubRecordFTQ = 125,
	luasubRecordFTR = 126,
	luasubRecordFTS = 127,
	luasubRecordFTT = 128,
	luasubRecordFTU = 129,
	luasubRecordFTV = 130,
	luasubRecordFTW = 131,
	luasubRecordFTX = 132,
	luasubRecordFTY = 133,
	luasubRecordFTZ = 134,
	luasubRecordNOA = 135,
	luasubRecordNOB = 136,
	luasubRecordNOC = 137,
	luasubRecordNOD = 138,
	luasubRecordNOE = 139,
	luasubRecordNOF = 140,
	luasubRecordNOG = 141,
	luasubRecordNOH = 142,
	luasubRecordNOI = 143,
	luasubRecordNOJ = 144,
	luasubRecordNOK = 145,
	luasubRecordNOL = 146,
	luasubRecordNOM = 147,
	luasubRecordNON = 148,
	luasubRecordNOO = 149,
	luasubRecordNOP = 150,
	luasubRecordNOQ = 151,
	luasubRecordNOR = 152,
	luasubRecordNOS = 153,
	luasubRecordNOT = 154,
	luasubRecordNOU = 155,
	luasubRecordNOV = 156,
	luasubRecordNOW = 157,
	luasubRecordNOX = 158,
	luasubRecordNOY = 159,
	luasubRecordNOZ = 160,
	luasubRecordNEA = 161,
	luasubRecordNEB = 162,
	luasubRecordNEC = 163,
	luasubRecordNED = 164,
	luasubRecordNEE = 165,
	luasubRecordNEF = 166,
	luasubRecordNEG = 167,
	luasubRecordNEH = 168,
	luasubRecordNEI = 169,
	luasubRecordNEJ = 170,
	luasubRecordNEK = 171,
	luasubRecordNEL = 172,
	luasubRecordNEM = 173,
	luasubRecordNEN = 174,
	luasubRecordNEO = 175,
	luasubRecordNEP = 176,
	luasubRecordNEQ = 177,
	luasubRecordNER = 178,
	luasubRecordNES = 179,
	luasubRecordNET = 180,
	luasubRecordNEU = 181,
	luasubRecordNEV = 182,
	luasubRecordNEW = 183,
	luasubRecordNEX = 184,
	luasubRecordNEY = 185,
	luasubRecordNEZ = 186,
	luasubRecordOUTA = 187,
	luasubRecordOUTB = 188,
	luasubRecordOUTC = 189,
	luasubRecordOUTD = 190,
	luasubRecordOUTE = 191,
	luasubRecordOUTF = 192,
	luasubRecordOUTG = 193,
	luasubRecordOUTH = 194,
	luasubRecordOUTI = 195,
	luasubRecordOUTJ = 196,
	luasubRecordOUTK = 197,
	luasubRecordOUTL = 198,
	luasubRecordOUTM = 199,
	luasubRecordOUTN = 200,
	luasubRecordOUTO = 201,
	luasubRecordOUTP = 202,
	luasubRecordOUTQ = 203,
	luasubRecordOUTR = 204,
	luasubRecordOUTS = 205,
	luasubRecordOUTT = 206,
	luasubRecordOUTU = 207,
	luasubRecordOUTV = 208,
	luasubRecordOUTW = 209,
	luasubRecordOUTX = 210,
	luasubRecordOUTY = 211,
	luasubRecordOUTZ = 212,
	luasubRecordVALA = 213,
	luasubRecordVALB = 214,
	luasubRecordVALC = 215,
	luasubRecordVALD = 216,
	luasubRecordVALE = 217,
	luasubRecordVALF = 218,
	luasubRecordVALG = 219,
	luasubRecordVALH = 220,
	luasubRecordVALI = 221,
	luasubRecordVALJ = 222,
	luasubRecordVALK = 223,
	luasubRecordVALL = 224,
	luasubRecordVALM = 225,
	luasubRecordVALN = 226,
	luasubRecordVALO = 227,
	luasubRecordVALP = 228,
	luasubRecordVALQ = 229,
	luasubRecordVALR = 230,
	luasubRecordVALS = 231,
	luasubRecordVALT = 232,
	luasubRecordVALU = 233,
	luasubRecordVALV = 234,
	luasubRecordVALW = 235,
	luasubRecordVALX = 236,
	luasubRecordVALY = 237,
	luasubRecordVALZ = 238,
	luasubRecordOVLA = 239,
	luasubRecordOVLB = 240,
	luasubRecordOVLC = 241,
	luasubRecordOVLD = 242,
	luasubRecordOVLE = 243,
	luasubRecordOVLF = 244,
	luasubRecordOVLG = 245,
	luasubRecordOVLH = 246,
	luasubRecordOVLI = 247,
	luasubRecordOVLJ = 248,
	luasubRecordOVLK = 249,
	luasubRecordOVLL = 250,
	luasubRecordOVLM = 251,
	luasubRecordOVLN = 252,
	luasubRecordOVLO = 253,
	luasubRecordOVLP = 254,
	luasubRecordOVLQ = 255,
	luasubRecordOVLR = 256,
	luasubRecordOVLS = 257,
	luasubRecordOVLT = 258,
	luasubRecordOVLU = 259,
	luasubRecordOVLV = 260,
	luasubRecordOVLW = 261,
	luasubRecordOVLX = 262,
	luasubRecordOVLY = 263,
	luasubRecordOVLZ = 264,
	luasubRecordFTVA = 265,
	luasubRecordFTVB = 266,
	luasubRecordFTVC = 267,
	luasubRecordFTVD = 268,
	luasubRecordFTVE = 269,
	luasubRecordFTVF = 270,
	luasubRecordFTVG = 271,
	luasubRecordFTVH = 272,
	luasubRecordFTVI = 273,
	luasubRecordFTVJ = 274,
	luasubRecordFTVK = 275,
	luasubRecordFTVL = 276,
	luasubRecordFTVM = 277,
	luasubRecordFTVN = 278,
	luasubRecordFTVO = 279,
	luasubRecordFTVP = 280,
	luasubRecordFTVQ = 281,
	luasubRecordFTVR = 282,
	luasubRecordFTVS = 283,
	luasubRecordFTVT = 284,
	luasubRecordFTVU = 285,
	luasubRecordFTVV = 286,
	luasubRecordFTVW = 287,
	luasubRecordFTVX = 288,
	luasubRecordFTVY = 289,
	luasubRecordFTVZ = 290,
	luasubRecordNOVA = 291,
	luasubRecordNOVB = 292,
	luasubRecordNOVC = 293,
	luasubRecordNOVD = 294,
	luasubRecordNOVE = 295,
	luasubRecordNOVF = 296,
	luasubRecordNOVG = 297,
	luasubRecordNOVH = 298,
	luasubRecordNOVI = 299,
	luasubRecordNOVJ = 300,
	luasubRecordNOVK = 301,
	luasubRecordNOVL = 302,
	luasubRecordNOVM = 303,
	luasubRecordNOVN = 304,
	luasubRecordNOVO = 305,
	luasubRecordNOVP = 306,
	luasubRecordNOVQ = 307,
	luasubRecordNOVR = 308,
	luasubRecordNOVS = 309,
	luasubRecordNOVT = 310,
	luasubRecordNOVU = 311,
	luasubRecordNOVV = 312,
	luasubRecordNOVW = 313,
	luasubRecordNOVX = 314,
	luasubRecordNOVY = 315,
	luasubRecordNOVZ = 316,
	luasubRecordNEVA = 317,
	luasubRecordNEVB = 318,
	luasubRecordNEVC = 319,
	luasubRecordNEVD = 320,
	luasubRecordNEVE = 321,
	luasubRecordNEVF = 322,
	luasubRecordNEVG = 323,
	luasubRecordNEVH = 324,
	luasubRecordNEVI = 325,
	luasubRecordNEVJ = 326,
	luasubRecordNEVK = 327,
	luasubRecordNEVL = 328,
	luasubRecordNEVM = 329,
	luasubRecordNEVN = 330,
	luasubRecordNEVO = 331,
	luasubRecordNEVP = 332,
	luasubRecordNEVQ = 333,
	luasubRecordNEVR = 334,
	luasubRecordNEVS = 335,
	luasubRecordNEVT = 336,
	luasubRecordNEVU = 337,
	luasubRecordNEVV = 338,
	luasubRecordNEVW = 339,
	luasubRecordNEVX = 340,
	luasubRecordNEVY = 341,
	luasubRecordNEVZ = 342,
	luasubRecordONVA = 343,
	luasubRecordONVB = 344,
	luasubRecordONVC = 345,
	luasubRecordONVD = 346,
	luasubRecordONVE = 347,
	luasubRecordONVF = 348,
	luasubRecordONVG = 349,
	luasubRecordONVH = 350,
	luasubRecordONVI = 351,
	luasubRecordONVJ = 352,
	luasubRecordONVK = 353,
	luasubRecordONVL = 354,
	luasubRecordONVM = 355,
	luasubRecordONVN = 356,
	luasubRecordONVO = 357,
	luasubRecordONVP = 358,
	luasubRecordONVQ = 359,
	luasubRecordONVR = 360,
	luasubRecordONVS = 361,
	luasubRecordONVT = 362,
	luasubRecordONVU = 363,
	luasubRecordONVV = 364,
	luasubRecordONVW = 365,
	luasubRecordONVX = 366,
	luasubRecordONVY = 367,
	luasubRecordONVZ = 368,
	luasubRecordVLDA = 369,
	luasubRecordVLDB = 370,
	luasubRecordVLDC = 371,
	luasubRecordVLDD = 372,
	luasubRecordVLDE = 373,
	luasubRecordVLDF = 374,
	luasubRecordVLDG = 375,
	luasubRecordVLDH = 376,
	luasubRecordVLDI = 377,
	luasubRecordVLDJ = 378,
	luasubRecordVLDK = 379,
	luasubRecordVLDL = 380,
	luasubRecordVLDM = 381,
	luasubRecordVLDN = 382,
	luasubRecordVLDO = 383,
	luasubRecordVLDP = 384,
	luasubRecordVLDQ = 385,
	luasubRecordVLDR = 386,
	luasubRecordVLDS = 387,
	luasubRecordVLDT = 388,
	luasubRecordVLDU = 389,
	luasubRecordVLDV = 390,
	luasubRecordVLDW = 391,
	luasubRecordVLDX = 392,
	luasubRecordVLDY = 393,
	luasubRecordVLDZ = 394
} luasubFieldIndex;

#ifdef GEN_SIZE_OFFSET

#include <epicsAssert.h>
#include <epicsExport.h>
#ifdef __cplusplus
extern "C" {
#endif
static int luasubRecordSizeOffset(dbRecordType *prt)
{
    luasubRecord *prec = 0;

    assert(prt->no_fields == 395);
    prt->papFldDes[luasubRecordNAME]->size = sizeof(prec->name);
    prt->papFldDes[luasubRecordDESC]->size = sizeof(prec->desc);
    prt->papFldDes[luasubRecordASG]->size = sizeof(prec->asg);
    prt->papFldDes[luasubRecordSCAN]->size = sizeof(prec->scan);
    prt->papFldDes[luasubRecordPINI]->size = sizeof(prec->pini);
    prt->papFldDes[luasubRecordPHAS]->size = sizeof(prec->phas);
    prt->papFldDes[luasubRecordEVNT]->size = sizeof(prec->evnt);
    prt->papFldDes[luasubRecordTSE]->size = sizeof(prec->tse);
    prt->papFldDes[luasubRecordTSEL]->size = sizeof(prec->tsel);
    prt->papFldDes[luasubRecordDTYP]->size = sizeof(prec->dtyp);
    prt->papFldDes[luasubRecordDISV]->size = sizeof(prec->disv);
    prt->papFldDes[luasubRecordDISA]->size = sizeof(prec->disa);
    prt->papFldDes[luasubRecordSDIS]->size = sizeof(prec->sdis);
    prt->papFldDes[luasubRecordMLOK]->size = sizeof(prec->mlok);
    prt->papFldDes[luasubRecordMLIS]->size = sizeof(prec->mlis);
    prt->papFldDes[luasubRecordBKLNK]->size = sizeof(prec->bklnk);
    prt->papFldDes[luasubRecordDISP]->size = sizeof(prec->disp);
    prt->papFldDes[luasubRecordPROC]->size = sizeof(prec->proc);
    prt->papFldDes[luasubRecordSTAT]->size = sizeof(prec->stat);
    prt->papFldDes[luasubRecordSEVR]->size = sizeof(prec->sevr);
    prt->papFldDes[luasubRecordNSTA]->size = sizeof(prec->nsta);
    prt->papFldDes[luasubRecordNSEV]->size = sizeof(prec->nsev);
    prt->papFldDes[luasubRecordACKS]->size = sizeof(prec->acks);
    prt->papFldDes[luasubRecordACKT]->size = sizeof(prec->ackt);
    prt->papFldDes[luasubRecordDISS]->size = sizeof(prec->diss);
    prt->papFldDes[luasubRecordLCNT]->size = sizeof(prec->lcnt);
    prt->papFldDes[luasubRecordPACT]->size = sizeof(prec->pact);
    prt->papFldDes[luasubRecordPUTF]->size = sizeof(prec->putf);
    prt->papFldDes[luasubRecordRPRO]->size = sizeof(prec->rpro);
    prt->papFldDes[luasubRecordASP]->size = sizeof(prec->asp);
    prt->papFldDes[luasubRecordPPN]->size = sizeof(prec->ppn);
    prt->papFldDes[luasubRecordPPNR]->size = sizeof(prec->ppnr);
    prt->papFldDes[luasubRecordSPVT]->size = sizeof(prec->spvt);
    prt->papFldDes[luasubRecordRSET]->size = sizeof(prec->rset);
    prt->papFldDes[luasubRecordDSET]->size = sizeof(prec->dset);
    prt->papFldDes[luasubRecordDPVT]->size = sizeof(prec->dpvt);
    prt->papFldDes[luasubRecordRDES]->size = sizeof(prec->rdes);
    prt->papFldDes[luasubRecordLSET]->size = sizeof(prec->lset);
    prt->papFldDes[luasubRecordPRIO]->size = sizeof(prec->prio);
    prt->papFldDes[luasubRecordTPRO]->size = sizeof(prec->tpro);
    prt->papFldDes[luasubRecordBKPT]->size = sizeof(prec->bkpt);
    prt->papFldDes[luasubRecordUDF]->size = sizeof(prec->udf);
    prt->papFldDes[luasubRecordUDFS]->size = sizeof(prec->udfs);
    prt->papFldDes[luasubRecordTIME]->size = sizeof(prec->time);
    prt->papFldDes[luasubRecordFLNK]->size = sizeof(prec->flnk);
    prt->papFldDes[luasubRecordVAL]->size = sizeof(prec->val);
    prt->papFldDes[luasubRecordOVAL]->size = sizeof(prec->oval);
    prt->papFldDes[luasubRecordINP]->size = sizeof(prec->inp);
    prt->papFldDes[luasubRecordICOD]->size = sizeof(prec->icod);
    prt->papFldDes[luasubRecordACTP]->size = sizeof(prec->actp);
    prt->papFldDes[luasubRecordRST]->size = sizeof(prec->rst);
    prt->papFldDes[luasubRecordPCOD]->size = sizeof(prec->pcod);
    prt->papFldDes[luasubRecordOCOD]->size = sizeof(prec->ocod);
    prt->papFldDes[luasubRecordYLDD]->size = sizeof(prec->yldd);
    prt->papFldDes[luasubRecordPCTP]->size = sizeof(prec->pctp);
    prt->papFldDes[luasubRecordPREC]->size = sizeof(prec->prec);
    prt->papFldDes[luasubRecordDRVO]->size = sizeof(prec->drvo);
    prt->papFldDes[luasubRecordINPA]->size = sizeof(prec->inpa);
    prt->papFldDes[luasubRecordINPB]->size = sizeof(prec->inpb);
    prt->papFldDes[luasubRecordINPC]->size = sizeof(prec->inpc);
    prt->papFldDes[luasubRecordINPD]->size = sizeof(prec->inpd);
    prt->papFldDes[luasubRecordINPE]->size = sizeof(prec->inpe);
    prt->papFldDes[luasubRecordINPF]->size = sizeof(prec->inpf);
    prt->papFldDes[luasubRecordINPG]->size = sizeof(prec->inpg);
    prt->papFldDes[luasubRecordINPH]->size = sizeof(prec->inph);
    prt->papFldDes[luasubRecordINPI]->size = sizeof(prec->inpi);
    prt->papFldDes[luasubRecordINPJ]->size = sizeof(prec->inpj);
    prt->papFldDes[luasubRecordINPK]->size = sizeof(prec->inpk);
    prt->papFldDes[luasubRecordINPL]->size = sizeof(prec->inpl);
    prt->papFldDes[luasubRecordINPM]->size = sizeof(prec->inpm);
    prt->papFldDes[luasubRecordINPN]->size = sizeof(prec->inpn);
    prt->papFldDes[luasubRecordINPO]->size = sizeof(prec->inpo);
    prt->papFldDes[luasubRecordINPP]->size = sizeof(prec->inpp);
    prt->papFldDes[luasubRecordINPQ]->size = sizeof(prec->inpq);
    prt->papFldDes[luasubRecordINPR]->size = sizeof(prec->inpr);
    prt->papFldDes[luasubRecordINPS]->size = sizeof(prec->inps);
    prt->papFldDes[luasubRecordINPT]->size = sizeof(prec->inpt);
    prt->papFldDes[luasubRecordINPU]->size = sizeof(prec->inpu);
    prt->papFldDes[luasubRecordINPV]->size = sizeof(prec->inpv);
    prt->papFldDes[luasubRecordINPW]->size = sizeof(prec->inpw);
    prt->papFldDes[luasubRecordINPX]->size = sizeof(prec->inpx);
    prt->papFldDes[luasubRecordINPY]->size = sizeof(prec->inpy);
    prt->papFldDes[luasubRecordINPZ]->size = sizeof(prec->inpz);
    prt->papFldDes[luasubRecordA]->size = sizeof(prec->a);
    prt->papFldDes[luasubRecordB]->size = sizeof(prec->b);
    prt->papFldDes[luasubRecordC]->size = sizeof(prec->c);
    prt->papFldDes[luasubRecordD]->size = sizeof(prec->d);
    prt->papFldDes[luasubRecordE]->size = sizeof(prec->e);
    prt->papFldDes[luasubRecordF]->size = sizeof(prec->f);
    prt->papFldDes[luasubRecordG]->size = sizeof(prec->g);
    prt->papFldDes[luasubRecordH]->size = sizeof(prec->h);
    prt->papFldDes[luasubRecordI]->size = sizeof(prec->i);
    prt->papFldDes[luasubRecordJ]->size = sizeof(prec->j);
    prt->papFldDes[luasubRecordK]->size = sizeof(prec->k);
    prt->papFldDes[luasubRecordL]->size = sizeof(prec->l);
    prt->papFldDes[luasubRecordM]->size = sizeof(prec->m);
    prt->papFldDes[luasubRecordN]->size = sizeof(prec->n);
    prt->papFldDes[luasubRecordO]->size = sizeof(prec->o);
    prt->papFldDes[luasubRecordP]->size = sizeof(prec->p);
    prt->papFldDes[luasubRecordQ]->size = sizeof(prec->q);
    prt->papFldDes[luasubRecordR]->size = sizeof(prec->r);
    prt->papFldDes[luasubRecordS]->size = sizeof(prec->s);
    prt->papFldDes[luasubRecordT]->size = sizeof(prec->t);
    prt->papFldDes[luasubRecordU]->size = sizeof(prec->u);
    prt->papFldDes[luasubRecordV]->size = sizeof(prec->v);
    prt->papFldDes[luasubRecordW]->size = sizeof(prec->w);
    prt->papFldDes[luasubRecordX]->size = sizeof(prec->x);
    prt->papFldDes[luasubRecordY]->size = sizeof(prec->y);
    prt->papFldDes[luasubRecordZ]->size = sizeof(prec->z);
    prt->papFldDes[luasubRecordFTA]->size = sizeof(prec->fta);
    prt->papFldDes[luasubRecordFTB]->size = sizeof(prec->ftb);
    prt->papFldDes[luasubRecordFTC]->size = sizeof(prec->ftc);
    prt->papFldDes[luasubRecordFTD]->size = sizeof(prec->ftd);
    prt->papFldDes[luasubRecordFTE]->size = sizeof(prec->fte);
    prt->papFldDes[luasubRecordFTF]->size = sizeof(prec->ftf);
    prt->papFldDes[luasubRecordFTG]->size = sizeof(prec->ftg);
    prt->papFldDes[luasubRecordFTH]->size = sizeof(prec->fth);
    prt->papFldDes[luasubRecordFTI]->size = sizeof(prec->fti);
    prt->papFldDes[luasubRecordFTJ]->size = sizeof(prec->ftj);
    prt->papFldDes[luasubRecordFTK]->size = sizeof(prec->ftk);
    prt->papFldDes[luasubRecordFTL]->size = sizeof(prec->ftl);
    prt->papFldDes[luasubRecordFTM]->size = sizeof(prec->ftm);
    prt->papFldDes[luasubRecordFTN]->size = sizeof(prec->ftn);
    prt->papFldDes[luasubRecordFTO]->size = sizeof(prec->fto);
    prt->papFldDes[luasubRecordFTP]->size = sizeof(prec->ftp);
    prt->papFldDes[luasubRecordFTQ]->size = sizeof(prec->ftq);
    prt->papFldDes[luasubRecordFTR]->size = sizeof(prec->ftr);
    prt->papFldDes[luasubRecordFTS]->size = sizeof(prec->fts);
    prt->papFldDes[luasubRecordFTT]->size = sizeof(prec->ftt);
    prt->papFldDes[luasubRecordFTU]->size = sizeof(prec->ftu);
    prt->papFldDes[luasubRecordFTV]->size = sizeof(prec->ftv);
    prt->papFldDes[luasubRecordFTW]->size = sizeof(prec->ftw);
    prt->papFldDes[luasubRecordFTX]->size = sizeof(prec->ftx);
    prt->papFldDes[luasubRecordFTY]->size = sizeof(prec->fty);
    prt->papFldDes[luasubRecordFTZ]->size = sizeof(prec->ftz);
    prt->papFldDes[luasubRecordNOA]->size = sizeof(prec->noa);
    prt->papFldDes[luasubRecordNOB]->size = sizeof(prec->nob);
    prt->papFldDes[luasubRecordNOC]->size = sizeof(prec->noc);
    prt->papFldDes[luasubRecordNOD]->size = sizeof(prec->nod);
    prt->papFldDes[luasubRecordNOE]->size = sizeof(prec->noe);
    prt->papFldDes[luasubRecordNOF]->size = sizeof(prec->nof);
    prt->papFldDes[luasubRecordNOG]->size = sizeof(prec->nog);
    prt->papFldDes[luasubRecordNOH]->size = sizeof(prec->noh);
    prt->papFldDes[luasubRecordNOI]->size = sizeof(prec->noi);
    prt->papFldDes[luasubRecordNOJ]->size = sizeof(prec->noj);
    prt->papFldDes[luasubRecordNOK]->size = sizeof(prec->nok);
    prt->papFldDes[luasubRecordNOL]->size = sizeof(prec->nol);
    prt->papFldDes[luasubRecordNOM]->size = sizeof(prec->nom);
    prt->papFldDes[luasubRecordNON]->size = sizeof(prec->non);
    prt->papFldDes[luasubRecordNOO]->size = sizeof(prec->noo);
    prt->papFldDes[luasubRecordNOP]->size = sizeof(prec->nop);
    prt->papFldDes[luasubRecordNOQ]->size = sizeof(prec->noq);
    prt->papFldDes[luasubRecordNOR]->size = sizeof(prec->nor);
    prt->papFldDes[luasubRecordNOS]->size = sizeof(prec->nos);
    prt->papFldDes[luasubRecordNOT]->size = sizeof(prec->NOT);
    prt->papFldDes[luasubRecordNOU]->size = sizeof(prec->nou);
    prt->papFldDes[luasubRecordNOV]->size = sizeof(prec->nov);
    prt->papFldDes[luasubRecordNOW]->size = sizeof(prec->now);
    prt->papFldDes[luasubRecordNOX]->size = sizeof(prec->nox);
    prt->papFldDes[luasubRecordNOY]->size = sizeof(prec->noy);
    prt->papFldDes[luasubRecordNOZ]->size = sizeof(prec->noz);
    prt->papFldDes[luasubRecordNEA]->size = sizeof(prec->nea);
    prt->papFldDes[luasubRecordNEB]->size = sizeof(prec->neb);
    prt->papFldDes[luasubRecordNEC]->size = sizeof(prec->nec);
    prt->papFldDes[luasubRecordNED]->size = sizeof(prec->ned);
    prt->papFldDes[luasubRecordNEE]->size = sizeof(prec->nee);
    prt->papFldDes[luasubRecordNEF]->size = sizeof(prec->nef);
    prt->papFldDes[luasubRecordNEG]->size = sizeof(prec->neg);
    prt->papFldDes[luasubRecordNEH]->size = sizeof(prec->neh);
    prt->papFldDes[luasubRecordNEI]->size = sizeof(prec->nei);
    prt->papFldDes[luasubRecordNEJ]->size = sizeof(prec->nej);
    prt->papFldDes[luasubRecordNEK]->size = sizeof(prec->nek);
    prt->papFldDes[luasubRecordNEL]->size = sizeof(prec->nel);
    prt->papFldDes[luasubRecordNEM]->size = sizeof(prec->nem);
    prt->papFldDes[luasubRecordNEN]->size = sizeof(prec->nen);
    prt->papFldDes[luasubRecordNEO]->size = sizeof(prec->neo);
    prt->papFldDes[luasubRecordNEP]->size = sizeof(prec->nep);
    prt->papFldDes[luasubRecordNEQ]->size = sizeof(prec->neq);
    prt->papFldDes[luasubRecordNER]->size = sizeof(prec->ner);
    prt->papFldDes[luasubRecordNES]->size = sizeof(prec->nes);
    prt->papFldDes[luasubRecordNET]->size = sizeof(prec->net);
    prt->papFldDes[luasubRecordNEU]->size = sizeof(prec->neu);
    prt->papFldDes[luasubRecordNEV]->size = sizeof(prec->nev);
    prt->papFldDes[luasubRecordNEW]->size = sizeof(prec->NEW);
    prt->papFldDes[luasubRecordNEX]->size = sizeof(prec->nex);
    prt->papFldDes[luasubRecordNEY]->size = sizeof(prec->ney);
    prt->papFldDes[luasubRecordNEZ]->size = sizeof(prec->nez);
    prt->papFldDes[luasubRecordOUTA]->size = sizeof(prec->outa);
    prt->papFldDes[luasubRecordOUTB]->size = sizeof(prec->outb);
    prt->papFldDes[luasubRecordOUTC]->size = sizeof(prec->outc);
    prt->papFldDes[luasubRecordOUTD]->size = sizeof(prec->outd);
    prt->papFldDes[luasubRecordOUTE]->size = sizeof(prec->oute);
    prt->papFldDes[luasubRecordOUTF]->size = sizeof(prec->outf);
    prt->papFldDes[luasubRecordOUTG]->size = sizeof(prec->outg);
    prt->papFldDes[luasubRecordOUTH]->size = sizeof(prec->outh);
    prt->papFldDes[luasubRecordOUTI]->size = sizeof(prec->outi);
    prt->papFldDes[luasubRecordOUTJ]->size = sizeof(prec->outj);
    prt->papFldDes[luasubRecordOUTK]->size = sizeof(prec->outk);
    prt->papFldDes[luasubRecordOUTL]->size = sizeof(prec->outl);
    prt->papFldDes[luasubRecordOUTM]->size = sizeof(prec->outm);
    prt->papFldDes[luasubRecordOUTN]->size = sizeof(prec->outn);
    prt->papFldDes[luasubRecordOUTO]->size = sizeof(prec->outo);
    prt->papFldDes[luasubRecordOUTP]->size = sizeof(prec->outp);
    prt->papFldDes[luasubRecordOUTQ]->size = sizeof(prec->outq);
    prt->papFldDes[luasubRecordOUTR]->size = sizeof(prec->outr);
    prt->papFldDes[luasubRecordOUTS]->size = sizeof(prec->outs);
    prt->papFldDes[luasubRecordOUTT]->size = sizeof(prec->outt);
    prt->papFldDes[luasubRecordOUTU]->size = sizeof(prec->outu);
    prt->papFldDes[luasubRecordOUTV]->size = sizeof(prec->outv);
    prt->papFldDes[luasubRecordOUTW]->size = sizeof(prec->outw);
    prt->papFldDes[luasubRecordOUTX]->size = sizeof(prec->outx);
    prt->papFldDes[luasubRecordOUTY]->size = sizeof(prec->outy);
    prt->papFldDes[luasubRecordOUTZ]->size = sizeof(prec->outz);
    prt->papFldDes[luasubRecordVALA]->size = sizeof(prec->vala);
    prt->papFldDes[luasubRecordVALB]->size = sizeof(prec->valb);
    prt->papFldDes[luasubRecordVALC]->size = sizeof(prec->valc);
    prt->papFldDes[luasubRecordVALD]->size = sizeof(prec->vald);
    prt->papFldDes[luasubRecordVALE]->size = sizeof(prec->vale);
    prt->papFldDes[luasubRecordVALF]->size = sizeof(prec->valf);
    prt->papFldDes[luasubRecordVALG]->size = sizeof(prec->valg);
    prt->papFldDes[luasubRecordVALH]->size = sizeof(prec->valh);
    prt->papFldDes[luasubRecordVALI]->size = sizeof(prec->vali);
    prt->papFldDes[luasubRecordVALJ]->size = sizeof(prec->valj);
    prt->papFldDes[luasubRecordVALK]->size = sizeof(prec->valk);
    prt->papFldDes[luasubRecordVALL]->size = sizeof(prec->vall);
    prt->papFldDes[luasubRecordVALM]->size = sizeof(prec->valm);
    prt->papFldDes[luasubRecordVALN]->size = sizeof(prec->valn);
    prt->papFldDes[luasubRecordVALO]->size = sizeof(prec->valo);
    prt->papFldDes[luasubRecordVALP]->size = sizeof(prec->valp);
    prt->papFldDes[luasubRecordVALQ]->size = sizeof(prec->valq);
    prt->papFldDes[luasubRecordVALR]->size = sizeof(prec->valr);
    prt->papFldDes[luasubRecordVALS]->size = sizeof(prec->vals);
    prt->papFldDes[luasubRecordVALT]->size = sizeof(prec->valt);
    prt->papFldDes[luasubRecordVALU]->size = sizeof(prec->valu);
    prt->papFldDes[luasubRecordVALV]->size = sizeof(prec->valv);
    prt->papFldDes[luasubRecordVALW]->size = sizeof(prec->valw);
    prt->papFldDes[luasubRecordVALX]->size = sizeof(prec->valx);
    prt->papFldDes[luasubRecordVALY]->size = sizeof(prec->valy);
    prt->papFldDes[luasubRecordVALZ]->size = sizeof(prec->valz);
    prt->papFldDes[luasubRecordOVLA]->size = sizeof(prec->ovla);
    prt->papFldDes[luasubRecordOVLB]->size = sizeof(prec->ovlb);
    prt->papFldDes[luasubRecordOVLC]->size = sizeof(prec->ovlc);
    prt->papFldDes[luasubRecordOVLD]->size = sizeof(prec->ovld);
    prt->papFldDes[luasubRecordOVLE]->size = sizeof(prec->ovle);
    prt->papFldDes[luasubRecordOVLF]->size = sizeof(prec->ovlf);
    prt->papFldDes[luasubRecordOVLG]->size = sizeof(prec->ovlg);
    prt->papFldDes[luasubRecordOVLH]->size = sizeof(prec->ovlh);
    prt->papFldDes[luasubRecordOVLI]->size = sizeof(prec->ovli);
    prt->papFldDes[luasubRecordOVLJ]->size = sizeof(prec->ovlj);
    prt->papFldDes[luasubRecordOVLK]->size = sizeof(prec->ovlk);
    prt->papFldDes[luasubRecordOVLL]->size = sizeof(prec->ovll);
    prt->papFldDes[luasubRecordOVLM]->size = sizeof(prec->ovlm);
    prt->papFldDes[luasubRecordOVLN]->size = sizeof(prec->ovln);
    prt->papFldDes[luasubRecordOVLO]->size = sizeof(prec->ovlo);
    prt->papFldDes[luasubRecordOVLP]->size = sizeof(prec->ovlp);
    prt->papFldDes[luasubRecordOVLQ]->size = sizeof(prec->ovlq);
    prt->papFldDes[luasubRecordOVLR]->size = sizeof(prec->ovlr);
    prt->papFldDes[luasubRecordOVLS]->size = sizeof(prec->ovls);
    prt->papFldDes[luasubRecordOVLT]->size = sizeof(prec->ovlt);
    prt->papFldDes[luasubRecordOVLU]->size = sizeof(prec->ovlu);
    prt->papFldDes[luasubRecordOVLV]->size = sizeof(prec->ovlv);
    prt->papFldDes[luasubRecordOVLW]->size = sizeof(prec->ovlw);
    prt->papFldDes[luasubRecordOVLX]->size = sizeof(prec->ovlx);
    prt->papFldDes[luasubRecordOVLY]->size = sizeof(prec->ovly);
    prt->papFldDes[luasubRecordOVLZ]->size = sizeof(prec->ovlz);
    prt->papFldDes[luasubRecordFTVA]->size = sizeof(prec->ftva);
    prt->papFldDes[luasubRecordFTVB]->size = sizeof(prec->ftvb);
    prt->papFldDes[luasubRecordFTVC]->size = sizeof(prec->ftvc);
    prt->papFldDes[luasubRecordFTVD]->size = sizeof(prec->ftvd);
    prt->papFldDes[luasubRecordFTVE]->size = sizeof(prec->ftve);
    prt->papFldDes[luasubRecordFTVF]->size = sizeof(prec->ftvf);
    prt->papFldDes[luasubRecordFTVG]->size = sizeof(prec->ftvg);
    prt->papFldDes[luasubRecordFTVH]->size = sizeof(prec->ftvh);
    prt->papFldDes[luasubRecordFTVI]->size = sizeof(prec->ftvi);
    prt->papFldDes[luasubRecordFTVJ]->size = sizeof(prec->ftvj);
    prt->papFldDes[luasubRecordFTVK]->size = sizeof(prec->ftvk);
    prt->papFldDes[luasubRecordFTVL]->size = sizeof(prec->ftvl);
    prt->papFldDes[luasubRecordFTVM]->size = sizeof(prec->ftvm);
    prt->papFldDes[luasubRecordFTVN]->size = sizeof(prec->ftvn);
    prt->papFldDes[luasubRecordFTVO]->size = sizeof(prec->ftvo);
    prt->papFldDes[luasubRecordFTVP]->size = sizeof(prec->ftvp);
    prt->papFldDes[luasubRecordFTVQ]->size = sizeof(prec->ftvq);
    prt->papFldDes[luasubRecordFTVR]->size = sizeof(prec->ftvr);
    prt->papFldDes[luasubRecordFTVS]->size = sizeof(prec->ftvs);
    prt->papFldDes[luasubRecordFTVT]->size = sizeof(prec->ftvt);
    prt->papFldDes[luasubRecordFTVU]->size = sizeof(prec->ftvu);
    prt->papFldDes[luasubRecordFTVV]->size = sizeof(prec->ftvv);
    prt->papFldDes[luasubRecordFTVW]->size = sizeof(prec->ftvw);
    prt->papFldDes[luasubRecordFTVX]->size = sizeof(prec->ftvx);
    prt->papFldDes[luasubRecordFTVY]->size = sizeof(prec->ftvy);
    prt->papFldDes[luasubRecordFTVZ]->size = sizeof(prec->ftvz);
    prt->papFldDes[luasubRecordNOVA]->size = sizeof(prec->nova);
    prt->papFldDes[luasubRecordNOVB]->size = sizeof(prec->novb);
    prt->papFldDes[luasubRecordNOVC]->size = sizeof(prec->novc);
    prt->papFldDes[luasubRecordNOVD]->size = sizeof(prec->novd);
    prt->papFldDes[luasubRecordNOVE]->size = sizeof(prec->nove);
    prt->papFldDes[luasubRecordNOVF]->size = sizeof(prec->novf);
    prt->papFldDes[luasubRecordNOVG]->size = sizeof(prec->novg);
    prt->papFldDes[luasubRecordNOVH]->size = sizeof(prec->novh);
    prt->papFldDes[luasubRecordNOVI]->size = sizeof(prec->novi);
    prt->papFldDes[luasubRecordNOVJ]->size = sizeof(prec->novj);
    prt->papFldDes[luasubRecordNOVK]->size = sizeof(prec->novk);
    prt->papFldDes[luasubRecordNOVL]->size = sizeof(prec->novl);
    prt->papFldDes[luasubRecordNOVM]->size = sizeof(prec->novm);
    prt->papFldDes[luasubRecordNOVN]->size = sizeof(prec->novn);
    prt->papFldDes[luasubRecordNOVO]->size = sizeof(prec->novo);
    prt->papFldDes[luasubRecordNOVP]->size = sizeof(prec->novp);
    prt->papFldDes[luasubRecordNOVQ]->size = sizeof(prec->novq);
    prt->papFldDes[luasubRecordNOVR]->size = sizeof(prec->novr);
    prt->papFldDes[luasubRecordNOVS]->size = sizeof(prec->novs);
    prt->papFldDes[luasubRecordNOVT]->size = sizeof(prec->novt);
    prt->papFldDes[luasubRecordNOVU]->size = sizeof(prec->novu);
    prt->papFldDes[luasubRecordNOVV]->size = sizeof(prec->novv);
    prt->papFldDes[luasubRecordNOVW]->size = sizeof(prec->novw);
    prt->papFldDes[luasubRecordNOVX]->size = sizeof(prec->novx);
    prt->papFldDes[luasubRecordNOVY]->size = sizeof(prec->novy);
    prt->papFldDes[luasubRecordNOVZ]->size = sizeof(prec->novz);
    prt->papFldDes[luasubRecordNEVA]->size = sizeof(prec->neva);
    prt->papFldDes[luasubRecordNEVB]->size = sizeof(prec->nevb);
    prt->papFldDes[luasubRecordNEVC]->size = sizeof(prec->nevc);
    prt->papFldDes[luasubRecordNEVD]->size = sizeof(prec->nevd);
    prt->papFldDes[luasubRecordNEVE]->size = sizeof(prec->neve);
    prt->papFldDes[luasubRecordNEVF]->size = sizeof(prec->nevf);
    prt->papFldDes[luasubRecordNEVG]->size = sizeof(prec->nevg);
    prt->papFldDes[luasubRecordNEVH]->size = sizeof(prec->nevh);
    prt->papFldDes[luasubRecordNEVI]->size = sizeof(prec->nevi);
    prt->papFldDes[luasubRecordNEVJ]->size = sizeof(prec->nevj);
    prt->papFldDes[luasubRecordNEVK]->size = sizeof(prec->nevk);
    prt->papFldDes[luasubRecordNEVL]->size = sizeof(prec->nevl);
    prt->papFldDes[luasubRecordNEVM]->size = sizeof(prec->nevm);
    prt->papFldDes[luasubRecordNEVN]->size = sizeof(prec->nevn);
    prt->papFldDes[luasubRecordNEVO]->size = sizeof(prec->nevo);
    prt->papFldDes[luasubRecordNEVP]->size = sizeof(prec->nevp);
    prt->papFldDes[luasubRecordNEVQ]->size = sizeof(prec->nevq);
    prt->papFldDes[luasubRecordNEVR]->size = sizeof(prec->nevr);
    prt->papFldDes[luasubRecordNEVS]->size = sizeof(prec->nevs);
    prt->papFldDes[luasubRecordNEVT]->size = sizeof(prec->nevt);
    prt->papFldDes[luasubRecordNEVU]->size = sizeof(prec->nevu);
    prt->papFldDes[luasubRecordNEVV]->size = sizeof(prec->nevv);
    prt->papFldDes[luasubRecordNEVW]->size = sizeof(prec->nevw);
    prt->papFldDes[luasubRecordNEVX]->size = sizeof(prec->nevx);
    prt->papFldDes[luasubRecordNEVY]->size = sizeof(prec->nevy);
    prt->papFldDes[luasubRecordNEVZ]->size = sizeof(prec->nevz);
    prt->papFldDes[luasubRecordONVA]->size = sizeof(prec->onva);
    prt->papFldDes[luasubRecordONVB]->size = sizeof(prec->onvb);
    prt->papFldDes[luasubRecordONVC]->size = sizeof(prec->onvc);
    prt->papFldDes[luasubRecordONVD]->size = sizeof(prec->onvd);
    prt->papFldDes[luasubRecordONVE]->size = sizeof(prec->onve);
    prt->papFldDes[luasubRecordONVF]->size = sizeof(prec->onvf);
    prt->papFldDes[luasubRecordONVG]->size = sizeof(prec->onvg);
    prt->papFldDes[luasubRecordONVH]->size = sizeof(prec->onvh);
    prt->papFldDes[luasubRecordONVI]->size = sizeof(prec->onvi);
    prt->papFldDes[luasubRecordONVJ]->size = sizeof(prec->onvj);
    prt->papFldDes[luasubRecordONVK]->size = sizeof(prec->onvk);
    prt->papFldDes[luasubRecordONVL]->size = sizeof(prec->onvl);
    prt->papFldDes[luasubRecordONVM]->size = sizeof(prec->onvm);
    prt->papFldDes[luasubRecordONVN]->size = sizeof(prec->onvn);
    prt->papFldDes[luasubRecordONVO]->size = sizeof(prec->onvo);
    prt->papFldDes[luasubRecordONVP]->size = sizeof(prec->onvp);
    prt->papFldDes[luasubRecordONVQ]->size = sizeof(prec->onvq);
    prt->papFldDes[luasubRecordONVR]->size = sizeof(prec->onvr);
    prt->papFldDes[luasubRecordONVS]->size = sizeof(prec->onvs);
    prt->papFldDes[luasubRecordONVT]->size = sizeof(prec->onvt);
    prt->papFldDes[luasubRecordONVU]->size = sizeof(prec->onvu);
    prt->papFldDes[luasubRecordONVV]->size = sizeof(prec->onvv);
    prt->papFldDes[luasubRecordONVW]->size = sizeof(prec->onvw);
    prt->papFldDes[luasubRecordONVX]->size = sizeof(prec->onvx);
    prt->papFldDes[luasubRecordONVY]->size = sizeof(prec->onvy);
    prt->papFldDes[luasubRecordONVZ]->size = sizeof(prec->onvz);
    prt->papFldDes[luasubRecordVLDA]->size = sizeof(prec->vlda);
    prt->papFldDes[luasubRecordVLDB]->size = sizeof(prec->vldb);
    prt->papFldDes[luasubRecordVLDC]->size = sizeof(prec->vldc);
    prt->papFldDes[luasubRecordVLDD]->size = sizeof(prec->vldd);
    prt->papFldDes[luasubRecordVLDE]->size = sizeof(prec->vlde);
    prt->papFldDes[luasubRecordVLDF]->size = sizeof(prec->vldf);
    prt->papFldDes[luasubRecordVLDG]->size = sizeof(prec->vldg);
    prt->papFldDes[luasubRecordVLDH]->size = sizeof(prec->vldh);
    prt->papFldDes[luasubRecordVLDI]->size = sizeof(prec->vldi);
    prt->papFldDes[luasubRecordVLDJ]->size = sizeof(prec->vldj);
    prt->papFldDes[luasubRecordVLDK]->size = sizeof(prec->vldk);
    prt->papFldDes[luasubRecordVLDL]->size = sizeof(prec->vldl);
    prt->papFldDes[luasubRecordVLDM]->size = sizeof(prec->vldm);
    prt->papFldDes[luasubRecordVLDN]->size = sizeof(prec->vldn);
    prt->papFldDes[luasubRecordVLDO]->size = sizeof(prec->vldo);
    prt->papFldDes[luasubRecordVLDP]->size = sizeof(prec->vldp);
    prt->papFldDes[luasubRecordVLDQ]->size = sizeof(prec->vldq);
    prt->papFldDes[luasubRecordVLDR]->size = sizeof(prec->vldr);
    prt->papFldDes[luasubRecordVLDS]->size = sizeof(prec->vlds);
    prt->papFldDes[luasubRecordVLDT]->size = sizeof(prec->vldt);
    prt->papFldDes[luasubRecordVLDU]->size = sizeof(prec->vldu);
    prt->papFldDes[luasubRecordVLDV]->size = sizeof(prec->vldv);
    prt->papFldDes[luasubRecordVLDW]->size = sizeof(prec->vldw);
    prt->papFldDes[luasubRecordVLDX]->size = sizeof(prec->vldx);
    prt->papFldDes[luasubRecordVLDY]->size = sizeof(prec->vldy);
    prt->papFldDes[luasubRecordVLDZ]->size = sizeof(prec->vldz);
    prt->papFldDes[luasubRecordNAME]->offset = (unsigned short)((char *)&prec->name - (char *)prec);
    prt->papFldDes[luasubRecordDESC]->offset = (unsigned short)((char *)&prec->desc - (char *)prec);
    prt->papFldDes[luasubRecordASG]->offset = (unsigned short)((char *)&prec->asg - (char *)prec);
    prt->papFldDes[luasubRecordSCAN]->offset = (unsigned short)((char *)&prec->scan - (char *)prec);
    prt->papFldDes[luasubRecordPINI]->offset = (unsigned short)((char *)&prec->pini - (char *)prec);
    prt->papFldDes[luasubRecordPHAS]->offset = (unsigned short)((char *)&prec->phas - (char *)prec);
    prt->papFldDes[luasubRecordEVNT]->offset = (unsigned short)((char *)&prec->evnt - (char *)prec);
    prt->papFldDes[luasubRecordTSE]->offset = (unsigned short)((char *)&prec->tse - (char *)prec);
    prt->papFldDes[luasubRecordTSEL]->offset = (unsigned short)((char *)&prec->tsel - (char *)prec);
    prt->papFldDes[luasubRecordDTYP]->offset = (unsigned short)((char *)&prec->dtyp - (char *)prec);
    prt->papFldDes[luasubRecordDISV]->offset = (unsigned short)((char *)&prec->disv - (char *)prec);
    prt->papFldDes[luasubRecordDISA]->offset = (unsigned short)((char *)&prec->disa - (char *)prec);
    prt->papFldDes[luasubRecordSDIS]->offset = (unsigned short)((char *)&prec->sdis - (char *)prec);
    prt->papFldDes[luasubRecordMLOK]->offset = (unsigned short)((char *)&prec->mlok - (char *)prec);
    prt->papFldDes[luasubRecordMLIS]->offset = (unsigned short)((char *)&prec->mlis - (char *)prec);
    prt->papFldDes[luasubRecordBKLNK]->offset = (unsigned short)((char *)&prec->bklnk - (char *)prec);
    prt->papFldDes[luasubRecordDISP]->offset = (unsigned short)((char *)&prec->disp - (char *)prec);
    prt->papFldDes[luasubRecordPROC]->offset = (unsigned short)((char *)&prec->proc - (char *)prec);
    prt->papFldDes[luasubRecordSTAT]->offset = (unsigned short)((char *)&prec->stat - (char *)prec);
    prt->papFldDes[luasubRecordSEVR]->offset = (unsigned short)((char *)&prec->sevr - (char *)prec);
    prt->papFldDes[luasubRecordNSTA]->offset = (unsigned short)((char *)&prec->nsta - (char *)prec);
    prt->papFldDes[luasubRecordNSEV]->offset = (unsigned short)((char *)&prec->nsev - (char *)prec);
    prt->papFldDes[luasubRecordACKS]->offset = (unsigned short)((char *)&prec->acks - (char *)prec);
    prt->papFldDes[luasubRecordACKT]->offset = (unsigned short)((char *)&prec->ackt - (char *)prec);
    prt->papFldDes[luasubRecordDISS]->offset = (unsigned short)((char *)&prec->diss - (char *)prec);
    prt->papFldDes[luasubRecordLCNT]->offset = (unsigned short)((char *)&prec->lcnt - (char *)prec);
    prt->papFldDes[luasubRecordPACT]->offset = (unsigned short)((char *)&prec->pact - (char *)prec);
    prt->papFldDes[luasubRecordPUTF]->offset = (unsigned short)((char *)&prec->putf - (char *)prec);
    prt->papFldDes[luasubRecordRPRO]->offset = (unsigned short)((char *)&prec->rpro - (char *)prec);
    prt->papFldDes[luasubRecordASP]->offset = (unsigned short)((char *)&prec->asp - (char *)prec);
    prt->papFldDes[luasubRecordPPN]->offset = (unsigned short)((char *)&prec->ppn - (char *)prec);
    prt->papFldDes[luasubRecordPPNR]->offset = (unsigned short)((char *)&prec->ppnr - (char *)prec);
    prt->papFldDes[luasubRecordSPVT]->offset = (unsigned short)((char *)&prec->spvt - (char *)prec);
    prt->papFldDes[luasubRecordRSET]->offset = (unsigned short)((char *)&prec->rset - (char *)prec);
    prt->papFldDes[luasubRecordDSET]->offset = (unsigned short)((char *)&prec->dset - (char *)prec);
    prt->papFldDes[luasubRecordDPVT]->offset = (unsigned short)((char *)&prec->dpvt - (char *)prec);
    prt->papFldDes[luasubRecordRDES]->offset = (unsigned short)((char *)&prec->rdes - (char *)prec);
    prt->papFldDes[luasubRecordLSET]->offset = (unsigned short)((char *)&prec->lset - (char *)prec);
    prt->papFldDes[luasubRecordPRIO]->offset = (unsigned short)((char *)&prec->prio - (char *)prec);
    prt->papFldDes[luasubRecordTPRO]->offset = (unsigned short)((char *)&prec->tpro - (char *)prec);
    prt->papFldDes[luasubRecordBKPT]->offset = (unsigned short)((char *)&prec->bkpt - (char *)prec);
    prt->papFldDes[luasubRecordUDF]->offset = (unsigned short)((char *)&prec->udf - (char *)prec);
    prt->papFldDes[luasubRecordUDFS]->offset = (unsigned short)((char *)&prec->udfs - (char *)prec);
    prt->papFldDes[luasubRecordTIME]->offset = (unsigned short)((char *)&prec->time - (char *)prec);
    prt->papFldDes[luasubRecordFLNK]->offset = (unsigned short)((char *)&prec->flnk - (char *)prec);
    prt->papFldDes[luasubRecordVAL]->offset = (unsigned short)((char *)&prec->val - (char *)prec);
    prt->papFldDes[luasubRecordOVAL]->offset = (unsigned short)((char *)&prec->oval - (char *)prec);
    prt->papFldDes[luasubRecordINP]->offset = (unsigned short)((char *)&prec->inp - (char *)prec);
    prt->papFldDes[luasubRecordICOD]->offset = (unsigned short)((char *)&prec->icod - (char *)prec);
    prt->papFldDes[luasubRecordACTP]->offset = (unsigned short)((char *)&prec->actp - (char *)prec);
    prt->papFldDes[luasubRecordRST]->offset = (unsigned short)((char *)&prec->rst - (char *)prec);
    prt->papFldDes[luasubRecordPCOD]->offset = (unsigned short)((char *)&prec->pcod - (char *)prec);
    prt->papFldDes[luasubRecordOCOD]->offset = (unsigned short)((char *)&prec->ocod - (char *)prec);
    prt->papFldDes[luasubRecordYLDD]->offset = (unsigned short)((char *)&prec->yldd - (char *)prec);
    prt->papFldDes[luasubRecordPCTP]->offset = (unsigned short)((char *)&prec->pctp - (char *)prec);
    prt->papFldDes[luasubRecordPREC]->offset = (unsigned short)((char *)&prec->prec - (char *)prec);
    prt->papFldDes[luasubRecordDRVO]->offset = (unsigned short)((char *)&prec->drvo - (char *)prec);
    prt->papFldDes[luasubRecordINPA]->offset = (unsigned short)((char *)&prec->inpa - (char *)prec);
    prt->papFldDes[luasubRecordINPB]->offset = (unsigned short)((char *)&prec->inpb - (char *)prec);
    prt->papFldDes[luasubRecordINPC]->offset = (unsigned short)((char *)&prec->inpc - (char *)prec);
    prt->papFldDes[luasubRecordINPD]->offset = (unsigned short)((char *)&prec->inpd - (char *)prec);
    prt->papFldDes[luasubRecordINPE]->offset = (unsigned short)((char *)&prec->inpe - (char *)prec);
    prt->papFldDes[luasubRecordINPF]->offset = (unsigned short)((char *)&prec->inpf - (char *)prec);
    prt->papFldDes[luasubRecordINPG]->offset = (unsigned short)((char *)&prec->inpg - (char *)prec);
    prt->papFldDes[luasubRecordINPH]->offset = (unsigned short)((char *)&prec->inph - (char *)prec);
    prt->papFldDes[luasubRecordINPI]->offset = (unsigned short)((char *)&prec->inpi - (char *)prec);
    prt->papFldDes[luasubRecordINPJ]->offset = (unsigned short)((char *)&prec->inpj - (char *)prec);
    prt->papFldDes[luasubRecordINPK]->offset = (unsigned short)((char *)&prec->inpk - (char *)prec);
    prt->papFldDes[luasubRecordINPL]->offset = (unsigned short)((char *)&prec->inpl - (char *)prec);
    prt->papFldDes[luasubRecordINPM]->offset = (unsigned short)((char *)&prec->inpm - (char *)prec);
    prt->papFldDes[luasubRecordINPN]->offset = (unsigned short)((char *)&prec->inpn - (char *)prec);
    prt->papFldDes[luasubRecordINPO]->offset = (unsigned short)((char *)&prec->inpo - (char *)prec);
    prt->papFldDes[luasubRecordINPP]->offset = (unsigned short)((char *)&prec->inpp - (char *)prec);
    prt->papFldDes[luasubRecordINPQ]->offset = (unsigned short)((char *)&prec->inpq - (char *)prec);
    prt->papFldDes[luasubRecordINPR]->offset = (unsigned short)((char *)&prec->inpr - (char *)prec);
    prt->papFldDes[luasubRecordINPS]->offset = (unsigned short)((char *)&prec->inps - (char *)prec);
    prt->papFldDes[luasubRecordINPT]->offset = (unsigned short)((char *)&prec->inpt - (char *)prec);
    prt->papFldDes[luasubRecordINPU]->offset = (unsigned short)((char *)&prec->inpu - (char *)prec);
    prt->papFldDes[luasubRecordINPV]->offset = (unsigned short)((char *)&prec->inpv - (char *)prec);
    prt->papFldDes[luasubRecordINPW]->offset = (unsigned short)((char *)&prec->inpw - (char *)prec);
    prt->papFldDes[luasubRecordINPX]->offset = (unsigned short)((char *)&prec->inpx - (char *)prec);
    prt->papFldDes[luasubRecordINPY]->offset = (unsigned short)((char *)&prec->inpy - (char *)prec);
    prt->papFldDes[luasubRecordINPZ]->offset = (unsigned short)((char *)&prec->inpz - (char *)prec);
    prt->papFldDes[luasubRecordA]->offset = (unsigned short)((char *)&prec->a - (char *)prec);
    prt->papFldDes[luasubRecordB]->offset = (unsigned short)((char *)&prec->b - (char *)prec);
    prt->papFldDes[luasubRecordC]->offset = (unsigned short)((char *)&prec->c - (char *)prec);
    prt->papFldDes[luasubRecordD]->offset = (unsigned short)((char *)&prec->d - (char *)prec);
    prt->papFldDes[luasubRecordE]->offset = (unsigned short)((char *)&prec->e - (char *)prec);
    prt->papFldDes[luasubRecordF]->offset = (unsigned short)((char *)&prec->f - (char *)prec);
    prt->papFldDes[luasubRecordG]->offset = (unsigned short)((char *)&prec->g - (char *)prec);
    prt->papFldDes[luasubRecordH]->offset = (unsigned short)((char *)&prec->h - (char *)prec);
    prt->papFldDes[luasubRecordI]->offset = (unsigned short)((char *)&prec->i - (char *)prec);
    prt->papFldDes[luasubRecordJ]->offset = (unsigned short)((char *)&prec->j - (char *)prec);
    prt->papFldDes[luasubRecordK]->offset = (unsigned short)((char *)&prec->k - (char *)prec);
    prt->papFldDes[luasubRecordL]->offset = (unsigned short)((char *)&prec->l - (char *)prec);
    prt->papFldDes[luasubRecordM]->offset = (unsigned short)((char *)&prec->m - (char *)prec);
    prt->papFldDes[luasubRecordN]->offset = (unsigned short)((char *)&prec->n - (char *)prec);
    prt->papFldDes[luasubRecordO]->offset = (unsigned short)((char *)&prec->o - (char *)prec);
    prt->papFldDes[luasubRecordP]->offset = (unsigned short)((char *)&prec->p - (char *)prec);
    prt->papFldDes[luasubRecordQ]->offset = (unsigned short)((char *)&prec->q - (char *)prec);
    prt->papFldDes[luasubRecordR]->offset = (unsigned short)((char *)&prec->r - (char *)prec);
    prt->papFldDes[luasubRecordS]->offset = (unsigned short)((char *)&prec->s - (char *)prec);
    prt->papFldDes[luasubRecordT]->offset = (unsigned short)((char *)&prec->t - (char *)prec);
    prt->papFldDes[luasubRecordU]->offset = (unsigned short)((char *)&prec->u - (char *)prec);
    prt->papFldDes[luasubRecordV]->offset = (unsigned short)((char *)&prec->v - (char *)prec);
    prt->papFldDes[luasubRecordW]->offset = (unsigned short)((char *)&prec->w - (char *)prec);
    prt->papFldDes[luasubRecordX]->offset = (unsigned short)((char *)&prec->x - (char *)prec);
    prt->papFldDes[luasubRecordY]->offset = (unsigned short)((char *)&prec->y - (char *)prec);
    prt->papFldDes[luasubRecordZ]->offset = (unsigned short)((char *)&prec->z - (char *)prec);
    prt->papFldDes[luasubRecordFTA]->offset = (unsigned short)((char *)&prec->fta - (char *)prec);
    prt->papFldDes[luasubRecordFTB]->offset = (unsigned short)((char *)&prec->ftb - (char *)prec);
    prt->papFldDes[luasubRecordFTC]->offset = (unsigned short)((char *)&prec->ftc - (char *)prec);
    prt->papFldDes[luasubRecordFTD]->offset = (unsigned short)((char *)&prec->ftd - (char *)prec);
    prt->papFldDes[luasubRecordFTE]->offset = (unsigned short)((char *)&prec->fte - (char *)prec);
    prt->papFldDes[luasubRecordFTF]->offset = (unsigned short)((char *)&prec->ftf - (char *)prec);
    prt->papFldDes[luasubRecordFTG]->offset = (unsigned short)((char *)&prec->ftg - (char *)prec);
    prt->papFldDes[luasubRecordFTH]->offset = (unsigned short)((char *)&prec->fth - (char *)prec);
    prt->papFldDes[luasubRecordFTI]->offset = (unsigned short)((char *)&prec->fti - (char *)prec);
    prt->papFldDes[luasubRecordFTJ]->offset = (unsigned short)((char *)&prec->ftj - (char *)prec);
    prt->papFldDes[luasubRecordFTK]->offset = (unsigned short)((char *)&prec->ftk - (char *)prec);
    prt->papFldDes[luasubRecordFTL]->offset = (unsigned short)((char *)&prec->ftl - (char *)prec);
    prt->papFldDes[luasubRecordFTM]->offset = (unsigned short)((char *)&prec->ftm - (char *)prec);
    prt->papFldDes[luasubRecordFTN]->offset = (unsigned short)((char *)&prec->ftn - (char *)prec);
    prt->papFldDes[luasubRecordFTO]->offset = (unsigned short)((char *)&prec->fto - (char *)prec);
    prt->papFldDes[luasubRecordFTP]->offset = (unsigned short)((char *)&prec->ftp - (char *)prec);
    prt->papFldDes[luasubRecordFTQ]->offset = (unsigned short)((char *)&prec->ftq - (char *)prec);
    prt->papFldDes[luasubRecordFTR]->offset = (unsigned short)((char *)&prec->ftr - (char *)prec);
    prt->papFldDes[luasubRecordFTS]->offset = (unsigned short)((char *)&prec->fts - (char *)prec);
    prt->papFldDes[luasubRecordFTT]->offset = (unsigned short)((char *)&prec->ftt - (char *)prec);
    prt->papFldDes[luasubRecordFTU]->offset = (unsigned short)((char *)&prec->ftu - (char *)prec);
    prt->papFldDes[luasubRecordFTV]->offset = (unsigned short)((char *)&prec->ftv - (char *)prec);
    prt->papFldDes[luasubRecordFTW]->offset = (unsigned short)((char *)&prec->ftw - (char *)prec);
    prt->papFldDes[luasubRecordFTX]->offset = (unsigned short)((char *)&prec->ftx - (char *)prec);
    prt->papFldDes[luasubRecordFTY]->offset = (unsigned short)((char *)&prec->fty - (char *)prec);
    prt->papFldDes[luasubRecordFTZ]->offset = (unsigned short)((char *)&prec->ftz - (char *)prec);
    prt->papFldDes[luasubRecordNOA]->offset = (unsigned short)((char *)&prec->noa - (char *)prec);
    prt->papFldDes[luasubRecordNOB]->offset = (unsigned short)((char *)&prec->nob - (char *)prec);
    prt->papFldDes[luasubRecordNOC]->offset = (unsigned short)((char *)&prec->noc - (char *)prec);
    prt->papFldDes[luasubRecordNOD]->offset = (unsigned short)((char *)&prec->nod - (char *)prec);
    prt->papFldDes[luasubRecordNOE]->offset = (unsigned short)((char *)&prec->noe - (char *)prec);
    prt->papFldDes[luasubRecordNOF]->offset = (unsigned short)((char *)&prec->nof - (char *)prec);
    prt->papFldDes[luasubRecordNOG]->offset = (unsigned short)((char *)&prec->nog - (char *)prec);
    prt->papFldDes[luasubRecordNOH]->offset = (unsigned short)((char *)&prec->noh - (char *)prec);
    prt->papFldDes[luasubRecordNOI]->offset = (unsigned short)((char *)&prec->noi - (char *)prec);
    prt->papFldDes[luasubRecordNOJ]->offset = (unsigned short)((char *)&prec->noj - (char *)prec);
    prt->papFldDes[luasubRecordNOK]->offset = (unsigned short)((char *)&prec->nok - (char *)prec);
    prt->papFldDes[luasubRecordNOL]->offset = (unsigned short)((char *)&prec->nol - (char *)prec);
    prt->papFldDes[luasubRecordNOM]->offset = (unsigned short)((char *)&prec->nom - (char *)prec);
    prt->papFldDes[luasubRecordNON]->offset = (unsigned short)((char *)&prec->non - (char *)prec);
    prt->papFldDes[luasubRecordNOO]->offset = (unsigned short)((char *)&prec->noo - (char *)prec);
    prt->papFldDes[luasubRecordNOP]->offset = (unsigned short)((char *)&prec->nop - (char *)prec);
    prt->papFldDes[luasubRecordNOQ]->offset = (unsigned short)((char *)&prec->noq - (char *)prec);
    prt->papFldDes[luasubRecordNOR]->offset = (unsigned short)((char *)&prec->nor - (char *)prec);
    prt->papFldDes[luasubRecordNOS]->offset = (unsigned short)((char *)&prec->nos - (char *)prec);
    prt->papFldDes[luasubRecordNOT]->offset = (unsigned short)((char *)&prec->NOT - (char *)prec);
    prt->papFldDes[luasubRecordNOU]->offset = (unsigned short)((char *)&prec->nou - (char *)prec);
    prt->papFldDes[luasubRecordNOV]->offset = (unsigned short)((char *)&prec->nov - (char *)prec);
    prt->papFldDes[luasubRecordNOW]->offset = (unsigned short)((char *)&prec->now - (char *)prec);
    prt->papFldDes[luasubRecordNOX]->offset = (unsigned short)((char *)&prec->nox - (char *)prec);
    prt->papFldDes[luasubRecordNOY]->offset = (unsigned short)((char *)&prec->noy - (char *)prec);
    prt->papFldDes[luasubRecordNOZ]->offset = (unsigned short)((char *)&prec->noz - (char *)prec);
    prt->papFldDes[luasubRecordNEA]->offset = (unsigned short)((char *)&prec->nea - (char *)prec);
    prt->papFldDes[luasubRecordNEB]->offset = (unsigned short)((char *)&prec->neb - (char *)prec);
    prt->papFldDes[luasubRecordNEC]->offset = (unsigned short)((char *)&prec->nec - (char *)prec);
    prt->papFldDes[luasubRecordNED]->offset = (unsigned short)((char *)&prec->ned - (char *)prec);
    prt->papFldDes[luasubRecordNEE]->offset = (unsigned short)((char *)&prec->nee - (char *)prec);
    prt->papFldDes[luasubRecordNEF]->offset = (unsigned short)((char *)&prec->nef - (char *)prec);
    prt->papFldDes[luasubRecordNEG]->offset = (unsigned short)((char *)&prec->neg - (char *)prec);
    prt->papFldDes[luasubRecordNEH]->offset = (unsigned short)((char *)&prec->neh - (char *)prec);
    prt->papFldDes[luasubRecordNEI]->offset = (unsigned short)((char *)&prec->nei - (char *)prec);
    prt->papFldDes[luasubRecordNEJ]->offset = (unsigned short)((char *)&prec->nej - (char *)prec);
    prt->papFldDes[luasubRecordNEK]->offset = (unsigned short)((char *)&prec->nek - (char *)prec);
    prt->papFldDes[luasubRecordNEL]->offset = (unsigned short)((char *)&prec->nel - (char *)prec);
    prt->papFldDes[luasubRecordNEM]->offset = (unsigned short)((char *)&prec->nem - (char *)prec);
    prt->papFldDes[luasubRecordNEN]->offset = (unsigned short)((char *)&prec->nen - (char *)prec);
    prt->papFldDes[luasubRecordNEO]->offset = (unsigned short)((char *)&prec->neo - (char *)prec);
    prt->papFldDes[luasubRecordNEP]->offset = (unsigned short)((char *)&prec->nep - (char *)prec);
    prt->papFldDes[luasubRecordNEQ]->offset = (unsigned short)((char *)&prec->neq - (char *)prec);
    prt->papFldDes[luasubRecordNER]->offset = (unsigned short)((char *)&prec->ner - (char *)prec);
    prt->papFldDes[luasubRecordNES]->offset = (unsigned short)((char *)&prec->nes - (char *)prec);
    prt->papFldDes[luasubRecordNET]->offset = (unsigned short)((char *)&prec->net - (char *)prec);
    prt->papFldDes[luasubRecordNEU]->offset = (unsigned short)((char *)&prec->neu - (char *)prec);
    prt->papFldDes[luasubRecordNEV]->offset = (unsigned short)((char *)&prec->nev - (char *)prec);
    prt->papFldDes[luasubRecordNEW]->offset = (unsigned short)((char *)&prec->NEW - (char *)prec);
    prt->papFldDes[luasubRecordNEX]->offset = (unsigned short)((char *)&prec->nex - (char *)prec);
    prt->papFldDes[luasubRecordNEY]->offset = (unsigned short)((char *)&prec->ney - (char *)prec);
    prt->papFldDes[luasubRecordNEZ]->offset = (unsigned short)((char *)&prec->nez - (char *)prec);
    prt->papFldDes[luasubRecordOUTA]->offset = (unsigned short)((char *)&prec->outa - (char *)prec);
    prt->papFldDes[luasubRecordOUTB]->offset = (unsigned short)((char *)&prec->outb - (char *)prec);
    prt->papFldDes[luasubRecordOUTC]->offset = (unsigned short)((char *)&prec->outc - (char *)prec);
    prt->papFldDes[luasubRecordOUTD]->offset = (unsigned short)((char *)&prec->outd - (char *)prec);
    prt->papFldDes[luasubRecordOUTE]->offset = (unsigned short)((char *)&prec->oute - (char *)prec);
    prt->papFldDes[luasubRecordOUTF]->offset = (unsigned short)((char *)&prec->outf - (char *)prec);
    prt->papFldDes[luasubRecordOUTG]->offset = (unsigned short)((char *)&prec->outg - (char *)prec);
    prt->papFldDes[luasubRecordOUTH]->offset = (unsigned short)((char *)&prec->outh - (char *)prec);
    prt->papFldDes[luasubRecordOUTI]->offset = (unsigned short)((char *)&prec->outi - (char *)prec);
    prt->papFldDes[luasubRecordOUTJ]->offset = (unsigned short)((char *)&prec->outj - (char *)prec);
    prt->papFldDes[luasubRecordOUTK]->offset = (unsigned short)((char *)&prec->outk - (char *)prec);
    prt->papFldDes[luasubRecordOUTL]->offset = (unsigned short)((char *)&prec->outl - (char *)prec);
    prt->papFldDes[luasubRecordOUTM]->offset = (unsigned short)((char *)&prec->outm - (char *)prec);
    prt->papFldDes[luasubRecordOUTN]->offset = (unsigned short)((char *)&prec->outn - (char *)prec);
    prt->papFldDes[luasubRecordOUTO]->offset = (unsigned short)((char *)&prec->outo - (char *)prec);
    prt->papFldDes[luasubRecordOUTP]->offset = (unsigned short)((char *)&prec->outp - (char *)prec);
    prt->papFldDes[luasubRecordOUTQ]->offset = (unsigned short)((char *)&prec->outq - (char *)prec);
    prt->papFldDes[luasubRecordOUTR]->offset = (unsigned short)((char *)&prec->outr - (char *)prec);
    prt->papFldDes[luasubRecordOUTS]->offset = (unsigned short)((char *)&prec->outs - (char *)prec);
    prt->papFldDes[luasubRecordOUTT]->offset = (unsigned short)((char *)&prec->outt - (char *)prec);
    prt->papFldDes[luasubRecordOUTU]->offset = (unsigned short)((char *)&prec->outu - (char *)prec);
    prt->papFldDes[luasubRecordOUTV]->offset = (unsigned short)((char *)&prec->outv - (char *)prec);
    prt->papFldDes[luasubRecordOUTW]->offset = (unsigned short)((char *)&prec->outw - (char *)prec);
    prt->papFldDes[luasubRecordOUTX]->offset = (unsigned short)((char *)&prec->outx - (char *)prec);
    prt->papFldDes[luasubRecordOUTY]->offset = (unsigned short)((char *)&prec->outy - (char *)prec);
    prt->papFldDes[luasubRecordOUTZ]->offset = (unsigned short)((char *)&prec->outz - (char *)prec);
    prt->papFldDes[luasubRecordVALA]->offset = (unsigned short)((char *)&prec->vala - (char *)prec);
    prt->papFldDes[luasubRecordVALB]->offset = (unsigned short)((char *)&prec->valb - (char *)prec);
    prt->papFldDes[luasubRecordVALC]->offset = (unsigned short)((char *)&prec->valc - (char *)prec);
    prt->papFldDes[luasubRecordVALD]->offset = (unsigned short)((char *)&prec->vald - (char *)prec);
    prt->papFldDes[luasubRecordVALE]->offset = (unsigned short)((char *)&prec->vale - (char *)prec);
    prt->papFldDes[luasubRecordVALF]->offset = (unsigned short)((char *)&prec->valf - (char *)prec);
    prt->papFldDes[luasubRecordVALG]->offset = (unsigned short)((char *)&prec->valg - (char *)prec);
    prt->papFldDes[luasubRecordVALH]->offset = (unsigned short)((char *)&prec->valh - (char *)prec);
    prt->papFldDes[luasubRecordVALI]->offset = (unsigned short)((char *)&prec->vali - (char *)prec);
    prt->papFldDes[luasubRecordVALJ]->offset = (unsigned short)((char *)&prec->valj - (char *)prec);
    prt->papFldDes[luasubRecordVALK]->offset = (unsigned short)((char *)&prec->valk - (char *)prec);
    prt->papFldDes[luasubRecordVALL]->offset = (unsigned short)((char *)&prec->vall - (char *)prec);
    prt->papFldDes[luasubRecordVALM]->offset = (unsigned short)((char *)&prec->valm - (char *)prec);
    prt->papFldDes[luasubRecordVALN]->offset = (unsigned short)((char *)&prec->valn - (char *)prec);
    prt->papFldDes[luasubRecordVALO]->offset = (unsigned short)((char *)&prec->valo - (char *)prec);
    prt->papFldDes[luasubRecordVALP]->offset = (unsigned short)((char *)&prec->valp - (char *)prec);
    prt->papFldDes[luasubRecordVALQ]->offset = (unsigned short)((char *)&prec->valq - (char *)prec);
    prt->papFldDes[luasubRecordVALR]->offset = (unsigned short)((char *)&prec->valr - (char *)prec);
    prt->papFldDes[luasubRecordVALS]->offset = (unsigned short)((char *)&prec->vals - (char *)prec);
    prt->papFldDes[luasubRecordVALT]->offset = (unsigned short)((char *)&prec->valt - (char *)prec);
    prt->papFldDes[luasubRecordVALU]->offset = (unsigned short)((char *)&prec->valu - (char *)prec);
    prt->papFldDes[luasubRecordVALV]->offset = (unsigned short)((char *)&prec->valv - (char *)prec);
    prt->papFldDes[luasubRecordVALW]->offset = (unsigned short)((char *)&prec->valw - (char *)prec);
    prt->papFldDes[luasubRecordVALX]->offset = (unsigned short)((char *)&prec->valx - (char *)prec);
    prt->papFldDes[luasubRecordVALY]->offset = (unsigned short)((char *)&prec->valy - (char *)prec);
    prt->papFldDes[luasubRecordVALZ]->offset = (unsigned short)((char *)&prec->valz - (char *)prec);
    prt->papFldDes[luasubRecordOVLA]->offset = (unsigned short)((char *)&prec->ovla - (char *)prec);
    prt->papFldDes[luasubRecordOVLB]->offset = (unsigned short)((char *)&prec->ovlb - (char *)prec);
    prt->papFldDes[luasubRecordOVLC]->offset = (unsigned short)((char *)&prec->ovlc - (char *)prec);
    prt->papFldDes[luasubRecordOVLD]->offset = (unsigned short)((char *)&prec->ovld - (char *)prec);
    prt->papFldDes[luasubRecordOVLE]->offset = (unsigned short)((char *)&prec->ovle - (char *)prec);
    prt->papFldDes[luasubRecordOVLF]->offset = (unsigned short)((char *)&prec->ovlf - (char *)prec);
    prt->papFldDes[luasubRecordOVLG]->offset = (unsigned short)((char *)&prec->ovlg - (char *)prec);
    prt->papFldDes[luasubRecordOVLH]->offset = (unsigned short)((char *)&prec->ovlh - (char *)prec);
    prt->papFldDes[luasubRecordOVLI]->offset = (unsigned short)((char *)&prec->ovli - (char *)prec);
    prt->papFldDes[luasubRecordOVLJ]->offset = (unsigned short)((char *)&prec->ovlj - (char *)prec);
    prt->papFldDes[luasubRecordOVLK]->offset = (unsigned short)((char *)&prec->ovlk - (char *)prec);
    prt->papFldDes[luasubRecordOVLL]->offset = (unsigned short)((char *)&prec->ovll - (char *)prec);
    prt->papFldDes[luasubRecordOVLM]->offset = (unsigned short)((char *)&prec->ovlm - (char *)prec);
    prt->papFldDes[luasubRecordOVLN]->offset = (unsigned short)((char *)&prec->ovln - (char *)prec);
    prt->papFldDes[luasubRecordOVLO]->offset = (unsigned short)((char *)&prec->ovlo - (char *)prec);
    prt->papFldDes[luasubRecordOVLP]->offset = (unsigned short)((char *)&prec->ovlp - (char *)prec);
    prt->papFldDes[luasubRecordOVLQ]->offset = (unsigned short)((char *)&prec->ovlq - (char *)prec);
    prt->papFldDes[luasubRecordOVLR]->offset = (unsigned short)((char *)&prec->ovlr - (char *)prec);
    prt->papFldDes[luasubRecordOVLS]->offset = (unsigned short)((char *)&prec->ovls - (char *)prec);
    prt->papFldDes[luasubRecordOVLT]->offset = (unsigned short)((char *)&prec->ovlt - (char *)prec);
    prt->papFldDes[luasubRecordOVLU]->offset = (unsigned short)((char *)&prec->ovlu - (char *)prec);
    prt->papFldDes[luasubRecordOVLV]->offset = (unsigned short)((char *)&prec->ovlv - (char *)prec);
    prt->papFldDes[luasubRecordOVLW]->offset = (unsigned short)((char *)&prec->ovlw - (char *)prec);
    prt->papFldDes[luasubRecordOVLX]->offset = (unsigned short)((char *)&prec->ovlx - (char *)prec);
    prt->papFldDes[luasubRecordOVLY]->offset = (unsigned short)((char *)&prec->ovly - (char *)prec);
    prt->papFldDes[luasubRecordOVLZ]->offset = (unsigned short)((char *)&prec->ovlz - (char *)prec);
    prt->papFldDes[luasubRecordFTVA]->offset = (unsigned short)((char *)&prec->ftva - (char *)prec);
    prt->papFldDes[luasubRecordFTVB]->offset = (unsigned short)((char *)&prec->ftvb - (char *)prec);
    prt->papFldDes[luasubRecordFTVC]->offset = (unsigned short)((char *)&prec->ftvc - (char *)prec);
    prt->papFldDes[luasubRecordFTVD]->offset = (unsigned short)((char *)&prec->ftvd - (char *)prec);
    prt->papFldDes[luasubRecordFTVE]->offset = (unsigned short)((char *)&prec->ftve - (char *)prec);
    prt->papFldDes[luasubRecordFTVF]->offset = (unsigned short)((char *)&prec->ftvf - (char *)prec);
    prt->papFldDes[luasubRecordFTVG]->offset = (unsigned short)((char *)&prec->ftvg - (char *)prec);
    prt->papFldDes[luasubRecordFTVH]->offset = (unsigned short)((char *)&prec->ftvh - (char *)prec);
    prt->papFldDes[luasubRecordFTVI]->offset = (unsigned short)((char *)&prec->ftvi - (char *)prec);
    prt->papFldDes[luasubRecordFTVJ]->offset = (unsigned short)((char *)&prec->ftvj - (char *)prec);
    prt->papFldDes[luasubRecordFTVK]->offset = (unsigned short)((char *)&prec->ftvk - (char *)prec);
    prt->papFldDes[luasubRecordFTVL]->offset = (unsigned short)((char *)&prec->ftvl - (char *)prec);
    prt->papFldDes[luasubRecordFTVM]->offset = (unsigned short)((char *)&prec->ftvm - (char *)prec);
    prt->papFldDes[luasubRecordFTVN]->offset = (unsigned short)((char *)&prec->ftvn - (char *)prec);
    prt->papFldDes[luasubRecordFTVO]->offset = (unsigned short)((char *)&prec->ftvo - (char *)prec);
    prt->papFldDes[luasubRecordFTVP]->offset = (unsigned short)((char *)&prec->ftvp - (char *)prec);
    prt->papFldDes[luasubRecordFTVQ]->offset = (unsigned short)((char *)&prec->ftvq - (char *)prec);
    prt->papFldDes[luasubRecordFTVR]->offset = (unsigned short)((char *)&prec->ftvr - (char *)prec);
    prt->papFldDes[luasubRecordFTVS]->offset = (unsigned short)((char *)&prec->ftvs - (char *)prec);
    prt->papFldDes[luasubRecordFTVT]->offset = (unsigned short)((char *)&prec->ftvt - (char *)prec);
    prt->papFldDes[luasubRecordFTVU]->offset = (unsigned short)((char *)&prec->ftvu - (char *)prec);
    prt->papFldDes[luasubRecordFTVV]->offset = (unsigned short)((char *)&prec->ftvv - (char *)prec);
    prt->papFldDes[luasubRecordFTVW]->offset = (unsigned short)((char *)&prec->ftvw - (char *)prec);
    prt->papFldDes[luasubRecordFTVX]->offset = (unsigned short)((char *)&prec->ftvx - (char *)prec);
    prt->papFldDes[luasubRecordFTVY]->offset = (unsigned short)((char *)&prec->ftvy - (char *)prec);
    prt->papFldDes[luasubRecordFTVZ]->offset = (unsigned short)((char *)&prec->ftvz - (char *)prec);
    prt->papFldDes[luasubRecordNOVA]->offset = (unsigned short)((char *)&prec->nova - (char *)prec);
    prt->papFldDes[luasubRecordNOVB]->offset = (unsigned short)((char *)&prec->novb - (char *)prec);
    prt->papFldDes[luasubRecordNOVC]->offset = (unsigned short)((char *)&prec->novc - (char *)prec);
    prt->papFldDes[luasubRecordNOVD]->offset = (unsigned short)((char *)&prec->novd - (char *)prec);
    prt->papFldDes[luasubRecordNOVE]->offset = (unsigned short)((char *)&prec->nove - (char *)prec);
    prt->papFldDes[luasubRecordNOVF]->offset = (unsigned short)((char *)&prec->novf - (char *)prec);
    prt->papFldDes[luasubRecordNOVG]->offset = (unsigned short)((char *)&prec->novg - (char *)prec);
    prt->papFldDes[luasubRecordNOVH]->offset = (unsigned short)((char *)&prec->novh - (char *)prec);
    prt->papFldDes[luasubRecordNOVI]->offset = (unsigned short)((char *)&prec->novi - (char *)prec);
    prt->papFldDes[luasubRecordNOVJ]->offset = (unsigned short)((char *)&prec->novj - (char *)prec);
    prt->papFldDes[luasubRecordNOVK]->offset = (unsigned short)((char *)&prec->novk - (char *)prec);
    prt->papFldDes[luasubRecordNOVL]->offset = (unsigned short)((char *)&prec->novl - (char *)prec);
    prt->papFldDes[luasubRecordNOVM]->offset = (unsigned short)((char *)&prec->novm - (char *)prec);
    prt->papFldDes[luasubRecordNOVN]->offset = (unsigned short)((char *)&prec->novn - (char *)prec);
    prt->papFldDes[luasubRecordNOVO]->offset = (unsigned short)((char *)&prec->novo - (char *)prec);
    prt->papFldDes[luasubRecordNOVP]->offset = (unsigned short)((char *)&prec->novp - (char *)prec);
    prt->papFldDes[luasubRecordNOVQ]->offset = (unsigned short)((char *)&prec->novq - (char *)prec);
    prt->papFldDes[luasubRecordNOVR]->offset = (unsigned short)((char *)&prec->novr - (char *)prec);
    prt->papFldDes[luasubRecordNOVS]->offset = (unsigned short)((char *)&prec->novs - (char *)prec);
    prt->papFldDes[luasubRecordNOVT]->offset = (unsigned short)((char *)&prec->novt - (char *)prec);
    prt->papFldDes[luasubRecordNOVU]->offset = (unsigned short)((char *)&prec->novu - (char *)prec);
    prt->papFldDes[luasubRecordNOVV]->offset = (unsigned short)((char *)&prec->novv - (char *)prec);
    prt->papFldDes[luasubRecordNOVW]->offset = (unsigned short)((char *)&prec->novw - (char *)prec);
    prt->papFldDes[luasubRecordNOVX]->offset = (unsigned short)((char *)&prec->novx - (char *)prec);
    prt->papFldDes[luasubRecordNOVY]->offset = (unsigned short)((char *)&prec->novy - (char *)prec);
    prt->papFldDes[luasubRecordNOVZ]->offset = (unsigned short)((char *)&prec->novz - (char *)prec);
    prt->papFldDes[luasubRecordNEVA]->offset = (unsigned short)((char *)&prec->neva - (char *)prec);
    prt->papFldDes[luasubRecordNEVB]->offset = (unsigned short)((char *)&prec->nevb - (char *)prec);
    prt->papFldDes[luasubRecordNEVC]->offset = (unsigned short)((char *)&prec->nevc - (char *)prec);
    prt->papFldDes[luasubRecordNEVD]->offset = (unsigned short)((char *)&prec->nevd - (char *)prec);
    prt->papFldDes[luasubRecordNEVE]->offset = (unsigned short)((char *)&prec->neve - (char *)prec);
    prt->papFldDes[luasubRecordNEVF]->offset = (unsigned short)((char *)&prec->nevf - (char *)prec);
    prt->papFldDes[luasubRecordNEVG]->offset = (unsigned short)((char *)&prec->nevg - (char *)prec);
    prt->papFldDes[luasubRecordNEVH]->offset = (unsigned short)((char *)&prec->nevh - (char *)prec);
    prt->papFldDes[luasubRecordNEVI]->offset = (unsigned short)((char *)&prec->nevi - (char *)prec);
    prt->papFldDes[luasubRecordNEVJ]->offset = (unsigned short)((char *)&prec->nevj - (char *)prec);
    prt->papFldDes[luasubRecordNEVK]->offset = (unsigned short)((char *)&prec->nevk - (char *)prec);
    prt->papFldDes[luasubRecordNEVL]->offset = (unsigned short)((char *)&prec->nevl - (char *)prec);
    prt->papFldDes[luasubRecordNEVM]->offset = (unsigned short)((char *)&prec->nevm - (char *)prec);
    prt->papFldDes[luasubRecordNEVN]->offset = (unsigned short)((char *)&prec->nevn - (char *)prec);
    prt->papFldDes[luasubRecordNEVO]->offset = (unsigned short)((char *)&prec->nevo - (char *)prec);
    prt->papFldDes[luasubRecordNEVP]->offset = (unsigned short)((char *)&prec->nevp - (char *)prec);
    prt->papFldDes[luasubRecordNEVQ]->offset = (unsigned short)((char *)&prec->nevq - (char *)prec);
    prt->papFldDes[luasubRecordNEVR]->offset = (unsigned short)((char *)&prec->nevr - (char *)prec);
    prt->papFldDes[luasubRecordNEVS]->offset = (unsigned short)((char *)&prec->nevs - (char *)prec);
    prt->papFldDes[luasubRecordNEVT]->offset = (unsigned short)((char *)&prec->nevt - (char *)prec);
    prt->papFldDes[luasubRecordNEVU]->offset = (unsigned short)((char *)&prec->nevu - (char *)prec);
    prt->papFldDes[luasubRecordNEVV]->offset = (unsigned short)((char *)&prec->nevv - (char *)prec);
    prt->papFldDes[luasubRecordNEVW]->offset = (unsigned short)((char *)&prec->nevw - (char *)prec);
    prt->papFldDes[luasubRecordNEVX]->offset = (unsigned short)((char *)&prec->nevx - (char *)prec);
    prt->papFldDes[luasubRecordNEVY]->offset = (unsigned short)((char *)&prec->nevy - (char *)prec);
    prt->papFldDes[luasubRecordNEVZ]->offset = (unsigned short)((char *)&prec->nevz - (char *)prec);
    prt->papFldDes[luasubRecordONVA]->offset = (unsigned short)((char *)&prec->onva - (char *)prec);
    prt->papFldDes[luasubRecordONVB]->offset = (unsigned short)((char *)&prec->onvb - (char *)prec);
    prt->papFldDes[luasubRecordONVC]->offset = (unsigned short)((char *)&prec->onvc - (char *)prec);
    prt->papFldDes[luasubRecordONVD]->offset = (unsigned short)((char *)&prec->onvd - (char *)prec);
    prt->papFldDes[luasubRecordONVE]->offset = (unsigned short)((char *)&prec->onve - (char *)prec);
    prt->papFldDes[luasubRecordONVF]->offset = (unsigned short)((char *)&prec->onvf - (char *)prec);
    prt->papFldDes[luasubRecordONVG]->offset = (unsigned short)((char *)&prec->onvg - (char *)prec);
    prt->papFldDes[luasubRecordONVH]->offset = (unsigned short)((char *)&prec->onvh - (char *)prec);
    prt->papFldDes[luasubRecordONVI]->offset = (unsigned short)((char *)&prec->onvi - (char *)prec);
    prt->papFldDes[luasubRecordONVJ]->offset = (unsigned short)((char *)&prec->onvj - (char *)prec);
    prt->papFldDes[luasubRecordONVK]->offset = (unsigned short)((char *)&prec->onvk - (char *)prec);
    prt->papFldDes[luasubRecordONVL]->offset = (unsigned short)((char *)&prec->onvl - (char *)prec);
    prt->papFldDes[luasubRecordONVM]->offset = (unsigned short)((char *)&prec->onvm - (char *)prec);
    prt->papFldDes[luasubRecordONVN]->offset = (unsigned short)((char *)&prec->onvn - (char *)prec);
    prt->papFldDes[luasubRecordONVO]->offset = (unsigned short)((char *)&prec->onvo - (char *)prec);
    prt->papFldDes[luasubRecordONVP]->offset = (unsigned short)((char *)&prec->onvp - (char *)prec);
    prt->papFldDes[luasubRecordONVQ]->offset = (unsigned short)((char *)&prec->onvq - (char *)prec);
    prt->papFldDes[luasubRecordONVR]->offset = (unsigned short)((char *)&prec->onvr - (char *)prec);
    prt->papFldDes[luasubRecordONVS]->offset = (unsigned short)((char *)&prec->onvs - (char *)prec);
    prt->papFldDes[luasubRecordONVT]->offset = (unsigned short)((char *)&prec->onvt - (char *)prec);
    prt->papFldDes[luasubRecordONVU]->offset = (unsigned short)((char *)&prec->onvu - (char *)prec);
    prt->papFldDes[luasubRecordONVV]->offset = (unsigned short)((char *)&prec->onvv - (char *)prec);
    prt->papFldDes[luasubRecordONVW]->offset = (unsigned short)((char *)&prec->onvw - (char *)prec);
    prt->papFldDes[luasubRecordONVX]->offset = (unsigned short)((char *)&prec->onvx - (char *)prec);
    prt->papFldDes[luasubRecordONVY]->offset = (unsigned short)((char *)&prec->onvy - (char *)prec);
    prt->papFldDes[luasubRecordONVZ]->offset = (unsigned short)((char *)&prec->onvz - (char *)prec);
    prt->papFldDes[luasubRecordVLDA]->offset = (unsigned short)((char *)&prec->vlda - (char *)prec);
    prt->papFldDes[luasubRecordVLDB]->offset = (unsigned short)((char *)&prec->vldb - (char *)prec);
    prt->papFldDes[luasubRecordVLDC]->offset = (unsigned short)((char *)&prec->vldc - (char *)prec);
    prt->papFldDes[luasubRecordVLDD]->offset = (unsigned short)((char *)&prec->vldd - (char *)prec);
    prt->papFldDes[luasubRecordVLDE]->offset = (unsigned short)((char *)&prec->vlde - (char *)prec);
    prt->papFldDes[luasubRecordVLDF]->offset = (unsigned short)((char *)&prec->vldf - (char *)prec);
    prt->papFldDes[luasubRecordVLDG]->offset = (unsigned short)((char *)&prec->vldg - (char *)prec);
    prt->papFldDes[luasubRecordVLDH]->offset = (unsigned short)((char *)&prec->vldh - (char *)prec);
    prt->papFldDes[luasubRecordVLDI]->offset = (unsigned short)((char *)&prec->vldi - (char *)prec);
    prt->papFldDes[luasubRecordVLDJ]->offset = (unsigned short)((char *)&prec->vldj - (char *)prec);
    prt->papFldDes[luasubRecordVLDK]->offset = (unsigned short)((char *)&prec->vldk - (char *)prec);
    prt->papFldDes[luasubRecordVLDL]->offset = (unsigned short)((char *)&prec->vldl - (char *)prec);
    prt->papFldDes[luasubRecordVLDM]->offset = (unsigned short)((char *)&prec->vldm - (char *)prec);
    prt->papFldDes[luasubRecordVLDN]->offset = (unsigned short)((char *)&prec->vldn - (char *)prec);
    prt->papFldDes[luasubRecordVLDO]->offset = (unsigned short)((char *)&prec->vldo - (char *)prec);
    prt->papFldDes[luasubRecordVLDP]->offset = (unsigned short)((char *)&prec->vldp - (char *)prec);
    prt->papFldDes[luasubRecordVLDQ]->offset = (unsigned short)((char *)&prec->vldq - (char *)prec);
    prt->papFldDes[luasubRecordVLDR]->offset = (unsigned short)((char *)&prec->vldr - (char *)prec);
    prt->papFldDes[luasubRecordVLDS]->offset = (unsigned short)((char *)&prec->vlds - (char *)prec);
    prt->papFldDes[luasubRecordVLDT]->offset = (unsigned short)((char *)&prec->vldt - (char *)prec);
    prt->papFldDes[luasubRecordVLDU]->offset = (unsigned short)((char *)&prec->vldu - (char *)prec);
    prt->papFldDes[luasubRecordVLDV]->offset = (unsigned short)((char *)&prec->vldv - (char *)prec);
    prt->papFldDes[luasubRecordVLDW]->offset = (unsigned short)((char *)&prec->vldw - (char *)prec);
    prt->papFldDes[luasubRecordVLDX]->offset = (unsigned short)((char *)&prec->vldx - (char *)prec);
    prt->papFldDes[luasubRecordVLDY]->offset = (unsigned short)((char *)&prec->vldy - (char *)prec);
    prt->papFldDes[luasubRecordVLDZ]->offset = (unsigned short)((char *)&prec->vldz - (char *)prec);
    prt->rec_size = sizeof(*prec);
    return 0;
}
epicsExportRegistrar(luasubRecordSizeOffset);

#ifdef __cplusplus
}
#endif
#endif /* GEN_SIZE_OFFSET */

#endif /* INC_luasubRecord_H */
