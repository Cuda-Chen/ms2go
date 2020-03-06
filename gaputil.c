#include "libmseed.h"

void
printGapAndOverlap (MS3TraceList *mstl, ms_timeformat_t timeformat,
                    double *minGap, double *maxGap)
{
  MS3TraceID *tid  = NULL;
  MS3TraceSeg *seg = NULL;

  char time1[30], time2[30], gapStr[30];
  double gap, delta, numSamples;
  int8_t printFlag;
  int gapCount = 0, overlapCount = 0;

  if (!mstl)
  {
    return;
  }
  else if (!mstl->traces)
  {
    return;
  }

  tid = mstl->traces;
  while (tid)
  {
    seg = tid->first;
    while (seg->next)
    {
      /* Skip segments with 0 sample rate, usually from SOH records */
      if (seg->samprate == 0.0)
      {
        seg = seg->next;
        continue;
      }

      gap = (double)(seg->next->starttime - seg->endtime) / NSTMODULUS;

      /* Check that any overlap is not larger than the trace coverage */
      if (gap < 0.0)
      {
        delta = (seg->next->samprate) ? (1.0 / seg->next->samprate) : 0.0;

        if ((gap * -1.0) > (((double)(seg->next->endtime - seg->next->starttime) / NSTMODULUS) + delta))
        {
          gap = -(((double)(seg->next->endtime - seg->next->starttime) / NSTMODULUS) + delta);
        }
      }

      printFlag = 1;

      /* Check gap/overlap criteria */
      if (minGap)
      {
        if (gap < *minGap)
        {
          printFlag = 0;
        }
      }

      if (maxGap)
      {
        if (gap > *minGap)
        {
          printFlag = 0;
        }
      }

      if (printFlag)
      {
        numSamples = ms_dabs (gap) * seg->samprate;

        if (gap > 0.0)
        {
          numSamples -= 1.0;
          gapCount += 1;
        }
        else
        {
          numSamples += 1.0;
          overlapCount += 1;
        }

        /* Fix up gap display */
        if (gap >= 86400.0 || gap <= -86400.0)
        {
          snprintf (gapStr, sizeof (gapStr), "%-3.1fd", (gap / 86400));
        }
        else if (gap >= 3600.0 || gap <= -3600.0)
        {
          snprintf (gapStr, sizeof (gapStr), "%-3.1fh", (gap / 3600));
        }
        else if (gap == 0.0)
        {
          snprintf (gapStr, sizeof (gapStr), "-0  ");
        }
        else
        {
          snprintf (gapStr, sizeof (gapStr), "%-4.4g", gap);
        }

        /* Create formatted time strings */
        if (ms_nstime2timestr (seg->endtime, time1, timeformat, NANO_MICRO) == NULL)
        {
          ms_log (2, "Cannot convert trace start time for %s\n", tid->sid);
        }
        if (ms_nstime2timestr (seg->next->starttime, time2, timeformat, NANO_MICRO) == NULL)
        {
          ms_log (2, "Cannot convert trace end time for %s\n", tid->sid);
        }

        ms_log (0, "%-17s %-24s %-24s %-4s %-.8g\n",
                tid->sid, time1, time2, gapStr, numSamples);
      }

      seg = seg->next;
    }

    tid = tid->next;
  }

  ms_log (0, "Gap count: %d, overlap count: %d\n", gapCount, overlapCount);
}
