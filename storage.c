#include "storage.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef STORAGE_HOMEDIR
static char dirpath[256];
static char savecard_path[256];
static char config_path[256];

static void initPaths(void)
{
	static int initialized = 0;
	if (!initialized)
	{
		const char *home = getenv("HOME");
		sprintf(dirpath, "%s/%s", home, ".fleshchasmer");
		mkdir(dirpath, 0744);
		sprintf(savecard_path, "%s/%s", dirpath, "savecard.000");
		sprintf(config_path, "%s/%s", dirpath, "config.cfg");
		initialized = 1;
	}
}
#endif

char *getSavecardPath(void)
{
#ifndef STORAGE_HOMEDIR
	return "data/savecard.000";
#else
	initPaths();
	return savecard_path;
#endif
}

char *getConfigPath(void)
{
#ifndef STORAGE_HOMEDIR
	return "data/config.cfg";
#else
	initPaths();
	return config_path;
#endif
}
