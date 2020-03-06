#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>

#include "libmseed.h"

#include "gaputil.h"

int
main (int argc, char **argv)
{
  MS3TraceList *mstl      = NULL;
  char *mseedfile         = NULL;
  MS3Tolerance *tolerance = NULL;
  int8_t splitver         = 0;
  uint32_t flags          = 0;
  int8_t verbose          = 0;
  int rv;
  double mingap     = 0.0;
  double maxgap     = 10.0;
  double *ptrmingap = NULL;
  double *ptrmaxgap = NULL;

  if (argc < 2)
  {
    ms_log (1, "Usage: %s <mseedfile>\n", argv[0]);
    return -1;
  }
  mseedfile = argv[1];

  /* Set bit flag to validate CRC */
  flags |= MSF_VALIDATECRC;

  /* Set bit flag to build a record list */
  flags |= MSF_RECORDLIST;

  /* Read all miniSEED data into a trace list */
  rv = ms3_readtracelist (&mstl, mseedfile, tolerance, splitver, flags, verbose);
  if (rv != MS_NOERROR)
  {
    ms_log (2, "Cannot read miniSEED from file: %s\n", ms_errorstr (rv));
    return -1;
  }

  /* Print the numbers of gap and overlap */
  //mstl3_printgaplist (mstl, ISOMONTHDAY, ptrmingap, ptrmaxgap);
  printGapAndOverlap (mstl, ISOMONTHDAY, ptrmingap, ptrmaxgap);

  return 0;
}
