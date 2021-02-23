#ifndef _H_STORAGE
#define _H_STORAGE

#if defined(RETROFW) || defined(PC)
#define STORAGE_HOMEDIR
#endif

#if defined(POCKETGO) || defined(GP2X)
#undef STORAGE_HOMEDIR
#endif

char *getSavecardPath(void);
char *getConfigPath(void);

#endif
