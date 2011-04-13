/**
 * \file config.h
 */

#ifndef CONFIG_DSP
#define CONFIG_DSP

#ifndef RESTRICT
# ifdef USE_RESTRICT
#  define RESTRICT USE_RESTRICT
# else
#  define RESTRICT
# endif
#endif

#endif
