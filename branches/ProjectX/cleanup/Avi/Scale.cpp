// scale.cpp
//

// This code and information is provided "as is" without warranty of
// any kind, either expressed or implied, including but not limited to
// the implied warranties of merchantability and/or fitness for a
// particular purpose.

// Copyright (C) 1995 Intel Corporation.  All rights reserved.
//

#include <windows.h>

#include "scale.h"

#include <stdio.h>

#ifdef _DEBUG
#include <assert.h>
#define ASSERT assert
#else
#define ASSERT / ## /
#endif

int giActualProcessor = -1; // Index of native platform processor
                            //  closest to description.

//////////////////////////////////////////////////////////////////////////////
// Scalability

// 5=Pentium(r), 4=486, 3=386, 0=other or unknown or non-intel
int getIntelProcessorLevel ()
{
	SYSTEM_INFO si;

	GetSystemInfo (&si);
	
	if (si.dwProcessorType == PROCESSOR_INTEL_PENTIUM)
		return 5;
	else if (si.dwProcessorType == PROCESSOR_INTEL_486)
		return 4;
	else if (si.dwProcessorType == PROCESSOR_INTEL_386)
		return 3;
	else
		return 0;
}

// Compute MHz rating using RDTSC (Pentium(R) or higher only)
//
int getFreqMHz_RDTSC ()
{
	int Time_Processor_Pentium ();

	long consistent = 0;
	long sane = 1000;

	int freq1, freq2;

	while (!consistent && --sane)
	{
		freq1 = Time_Processor_Pentium ();
		freq2 = Time_Processor_Pentium ();

		if (ABS_(freq1-freq2) < 1)
			consistent++;
	}
	return (freq1);
}


#include <limits.h>

#define INT64(x) ((__int64 &)(x))

int Time_Processor_Pentium ()
{
        double freq;
        ULARGE_INTEGER t0, t1;
        LARGE_INTEGER li, q0, q1;
        __asm
        {
                _emit 0x0F
                _emit 0x31
                MOV t0.LowPart, EAX
                MOV t0.HighPart, EDX
        }
        QueryPerformanceCounter(&q0);
        if (QueryPerformanceFrequency(&li)) {
        	freq = double(INT64(li));
        } else {
			return 0;
		}

	// Need to consume some cycles here to get accurate count
        int i = 333;
        while (i--)
        	QueryPerformanceCounter(&q1);

        __asm
        {
                _emit 0x0F
                _emit 0x31
                MOV t1.LowPart, EAX
                MOV t1.HighPart, EDX
        }
        unsigned __int64 *i0, *i1;
        i0 = (unsigned __int64*)&t0;
        i1 = (unsigned __int64*)&t1;
        
		double mhz = (freq*(INT64(t1)-INT64(t0)))/(INT64(q1)-INT64(q0));
		mhz /= 1000000;

        return (int)mhz;
}

#define NITER 20

/* Added support for Pentium(r) and future generation processors.*/
static unsigned short processor_cycles[] = {
        165, 165, 25, 103, 42, 38,      
        38,  38,  38, 38,  38, 38,
        38,  38,  38, 38,  38, 38,
        38,  38,  38
};
     
// Compute MHz another way 
//
int getFreqMHz_BSF ()
{
	unsigned short Time_Processor_80386 ();

	unsigned long tmp, cycles, ticks, cps;
	
	double sum = 0.0;
	double frac;

	cycles = NITER * 42; // 42 is magic number for 486
	
	// Take the average of 100 run times.
	for (int i = 0; i < 100; i++)
	{
		tmp = Time_Processor_80386();
		sum += (double)tmp;

	}
	sum  /= 100.0;
	frac = sum - (double)((long)(sum));

	if (frac >= 0.5)
	{
		sum++;
	}

	ticks = (unsigned long)sum;

	cps = cycles * 1193180 / ticks; // clock freq = 119318 MHz
	cps /= 1000000;

	return (int)cps;
}

unsigned short Time_Processor_80386 ()
{
	unsigned short time;
	_asm {
		cli               ; disable interrupts

		mov al,0b8h  ;channel 2, LSB+MSB, mode 4, binary 10111000b
		out 43h,al

		in al,61h
		or al,1
		out 61h,al

		xor al,al         ; zero out timer
		out 42h,al
		out 42h,al

		mov eax,80000000h

		;; Perform 20 iterations of BSF (remember to set NITER)
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax

		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax
		bsf ecx,eax

		in al,42h         ;MSB timer      ; Read timer ticks.
		mov ah,al
		in al,42h         ;LSB timer
		xchg al,ah
		neg ax

		sti               ; reenable interrupts.

		mov time, ax
	}
	return time; // redundant but don't want compiler warning
}


BOOL setProcessor (PROCESSOR processor_list [], int nlen)
{
	int i, mhz, processor, level;

	// Get default processor
	if (giActualProcessor < 0)
	{
		if ((level = getIntelProcessorLevel ()) >= 5)
		{
			mhz = getFreqMHz_RDTSC ();
		}
		else if (level == 4)
		{
			mhz = getFreqMHz_BSF ();
			mhz = (int)((double(mhz)/3.0)*2.0);
#define LIST_P4 3 
			nlen = LIST_P4+1;
		}
		else
		{
			return FALSE;
		}

		processor = 0;
		for (i = 1; i < nlen; i++)
		{
			if (ABS_(mhz - processor_list[i].mhz) <
			    ABS_(mhz - processor_list[processor].mhz))
				processor = i;
		}
		giActualProcessor = processor;
	
		// append default signifier
		if (processor)
			strcat (processor_list [processor].text, " [Host]");
	}
	
	return TRUE;
}

