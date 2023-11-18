#ifndef __CONFIG_H__
#define __CONFIG_H__

#define RAM_TOP		0xd000u

#if (RAM_SIZE >= RAM_TOP)
#define RAM_PAGES	(RAM_TOP / ram<>::page_size)

#elif defined(USE_SPIRAM)
#define RAM_PAGES	(RAM_SIZE / ram<>::page_size)
#define SPIRAM_BASE	RAM_SIZE
#define SPIRAM_EXTENT	min(RAM_TOP - SPIRAM_BASE, SPIRAM_SIZE) / Memory::page_size

#else
#define RAM_PAGES	(RAM_SIZE / ram<>::page_size)

#endif

#define CPU_INSTRUCTIONS	1000

#define FG_COLOUR	GREEN
#define BG_COLOUR	BLACK

#if defined(USE_UTFT)
#define ORIENT	landscape
#elif defined(USE_ESPI)
#define ORIENT	reverse_landscape
#elif defined(USE_VGA)
#define ORIENT	landscape
#endif

#if defined(USE_SD)
#define PROGRAMS	"/apple1/"
#else
#define PROGRAMS	"/"
#endif

#define KRUSADER

#endif
