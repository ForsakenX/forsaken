
//
// scale.h
//

// This code and information is provided "as is" without warranty of
// any kind, either expressed or implied, including but not limited to
// the implied warranties of merchantability and/or fitness for a
// particular purpose.

// Copyright (C) 1995 Intel Corporation.  All rights reserved.
//

typedef struct tagPROCESSOR
{
	char text [128]; // text description
	long mhz;        // mhz in terms of Pentium(R)
}
PROCESSOR;

BOOL setProcessor (PROCESSOR processor_list [], int nlen);
extern int giActualProcessor;

#define ABS_(x) (x<0 ? (-(x)) : (x))

