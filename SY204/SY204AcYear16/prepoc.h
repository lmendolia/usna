/*
	Name: MIDN A.N. Mendolia
	Alpha: 184242
	Course: SY204 - Sys Prgm & OS Fund
	Meeting: Preprocessor Directives
	
	Description: (See Meeting Title)
*/

// DO NOT alter the code in preproc.h to solve the student activities. You may use preprocessor directives in preproc.c and gcc command line options per directions in preproc.c.

#ifndef PREPROC_H
	#define PREPROC_H
	#define CHAFF
	#ifdef SWORRIOR
		#undef SWORRIOR
	#endif
	#define SWORRIOR 1110
	void wheresMyPrototype(void);
	#define PROTOED
	#define HAYSTACK
#else
	#define JAMMING
	void freqHop(int obfus);
	#define FREQUENCY -1
#endif
