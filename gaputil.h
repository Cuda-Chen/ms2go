#ifndef GAPUTIL_H
#define GAPUTIL_H

#include "libmseed.h"

void printGapAndOverlap (MS3TraceList *mstl, ms_timeformat_t timeformat,
                         double *minGap, double *maxGap);

#endif
