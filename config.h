#ifndef _CONFIG_H
#define _CONFIG_H

#if defined(USE_SPIRAM)
#define RAM_SIZE	0x3000
#define SPIRAM_BASE	0x3000
#define SPIRAM_EXTENT	(20 * 1024 / 256)
#else
#define RAM_SIZE	0x8000
#endif

#define CPU_INSTRUCTIONS	1000

#define TFT_FG	GREEN
#define TFT_BG	BLACK

#if defined(USE_SD)
#define PROGRAMS	"/apple1/"
#else
#define PROGRAMS	"/"
#endif

#define KRUSADER

#endif
