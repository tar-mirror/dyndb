#ifndef DYNDB_H
#define DYNDB_H

#include "int32.h" /* does support autoconf.h */
#include "uint32.h" /* does support autoconf.h */

#define DYNDB_ISTABLE(ui32) (ui32 & 0x80000000UL)
#define DYNDB_TOTABLE(ui32) (ui32 | 0x80000000UL)
#define DYNDB_TABLEPOS(ui32) (ui32 & (~0x80000000UL))
#define DYNDB_MAXCOLLISIONS 4
/* keep SLOTMODS and MAXLLTABSIZE in sync */
#define DYNDB_SLOTMODS {4096,64,32,16,0}
#define DYNDB_MAXLLTABSIZE 64
#define DYNDB_LEVELS 4

#define DYNDB_OFF_NEXT 0
#define DYNDB_OFF_HASH 4
#define DYNDB_OFF_KEYLEN 8
#define DYNDB_OFF_DATALEN 12
#define DYNDB_HEAD_SIZE 16

extern unsigned int dyndb_slotmod[];

struct dyndb {
	int fd;
	int lookflag;
	uint32 lookpos;
	uint32 lookhash;
	uint32 datalen;
	uint32 datapos;
	uint32 keylen;
	uint32 keypos;
	uint32 wlevel;
	uint32 tpos[DYNDB_LEVELS];
	uint32 wi[DYNDB_LEVELS];
	uint32 wchainpos;
	uint32 epos;
	uint32 ehash;
	uint32 ekeylen;
	uint32 edatalen;
};

int32 dyndb_seek(int, uint32 pos);
int32 dyndb_seekend(int);
int32 dyndb_write(int, unsigned const char *vbuf, uint32 l);
int32 dyndb_read(int,unsigned char *vbuf,uint32 len);
#define dyndb_datalen(x) ((x)->datalen)
#define dyndb_datapos(x) ((x)->datapos)
#define dyndb_keylen(x) ((x)->keylen)
#define dyndb_keypos(x) ((x)->keypos)

int dyndb_create_name(struct dyndb *, const char *fname, int mode);
int dyndb_create_fd(struct dyndb *, int fd);
int dyndb_init(struct dyndb *, int fd);

int dyndb_enter(struct dyndb *, const unsigned char *key, uint32 keylen,
    const unsigned char *data, uint32 datalen);
int dyndb_enterstart(struct dyndb *);
int dyndb_enteraddkey(struct dyndb *,const unsigned char *k,uint32 kl);
int dyndb_enteradddata(struct dyndb *,const unsigned char *d,uint32 dl);
int dyndb_enterfinish(struct dyndb *);

void dyndb_lookupstart(struct dyndb *);
int dyndb_lookupnext(struct dyndb *, const unsigned char *key, uint32 keylen);
int dyndb_lookup(struct dyndb *, const unsigned char *key, uint32 keylen);

int dyndb_walkstart(struct dyndb *);
int dyndb_walknext(struct dyndb *);
int dyndb_fwalk(struct dyndb *dy, int (*callback)(uint32 pos,
    void *key, uint32 keylen,
    void *data, uint32 datalen));

uint32 dyndb_hash(const unsigned char *buf,uint32 len);
uint32 dyndb_hashstart(void);
uint32 dyndb_hashadd(uint32 h, const unsigned char *buf,uint32 len);

uint32 dyndb_fromdisk (unsigned char *buf);
void dyndb_todisk (unsigned char *buf, uint32 num);
int dyndb_delete(struct dyndb *);

#endif
