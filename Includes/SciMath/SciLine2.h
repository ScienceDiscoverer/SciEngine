#ifndef SCILINE2_H
#define SCILINE2_H

#include "SciVector2.h"

struct SciLine2
{
	// Constructors
	SciLine2(const SciVector2& orig_p, const SciVector2& dir_v) : orig(orig_p), dir(dir_v) {}

	// Some usefull functions goes here...

	// Data
	SciVector2 orig;
	SciVector2 dir;
};

#endif /* SCILINE2_H */
