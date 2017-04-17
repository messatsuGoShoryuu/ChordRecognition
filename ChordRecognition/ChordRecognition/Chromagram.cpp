#include "Chromagram.h"
#include "DFT.h"
#include <iostream>

void	chromagram(f64*	in, f64* out, ui32 count,f64 refFreq)
{
	for (ui32 i = 0; i < count; i++)
	{
		//Store frequency obtained from DFT bin
		f64 freq = getDFTBinFrequency(i);
		//Divide frequency by reference frequency (out[0] freq)

		/*Formula for equal temperament is:
		*frequency = 2^(n/12) * refFreq
		*where n is the difference of halfsteps between 
		*desired frequency and reference frequency.
		*/
		i32 index = ftom(freq) % 12;

		//Add amplitude to chromagram.
		out[index] += in[i] * in[i];
	}
}

int			ftom(f64 frequency)
{
	frequency = frequency / 440.0;
	int midiNote = log(frequency) / log(2) * 12 + 69;
	return midiNote;
}

void		sortChromagram(ui32* indexes, f64* chromagram)
{
	
}