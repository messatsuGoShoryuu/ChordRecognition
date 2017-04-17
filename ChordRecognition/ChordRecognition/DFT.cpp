#include "DFT.h"

#include "Complex.h"
#include <iostream>


#define MIN_FREQ 60
#define MAX_FREQ 22500
#include "define.h"
#include "Filter.h"


Biquad LPF;
Biquad HPF;


f64	DFT_binConstant = 0;
void DFT(const std::vector<Complex>& in, std::vector<Complex>& result)
{
	/*
	*	Discete Fourier transform formula (https://en.wikipedia.org/wiki/Discrete_Fourier_transform)
	*		   n-1
	*	F(k) =	Σ	x(t) = e^((-2 * PI * k * t) / n
	*		  t = 0
	*	
	*	where:	
	*		F is the frequency function
	*		k is the frequency 
	*		x is the time function
	*		t is time
	*		n is the bin size
	*
	*	We are using Euler's formula that dictates:
	*	For any real x
	*	e^(i*x) = cos(x) + i * sin(x).
	*
	*	We will calculate this sum for each frequency bin.
	*/

	//each F(k)
	ui32 n = in.size();

	//Both std::vectors in the arguments should have been resized to n before. 
	//They are to be used as static arrays.

	//Calculate the constant part before
	f64 c = - 2.0 * PI / n;

	//calculate min freq index
	f64 Fs = (f64)DFT_SAMPLE_RATE / (f64)INPUT_BUFFER_SIZE;
	ui32 minIndex = MIN_FREQ / Fs;
	ui32 maxIndex = MAX_FREQ / Fs;

	for (ui32 k = 0; k < n; k++)
	{
		if (k<minIndex || k>maxIndex)
		{
			result[k] = Complex(0, 0);
			continue;
		}
		//This loop is the Sigma sum
		Complex sum(0, 0);					//Temp local variable
		for (ui32 t = 0; t < n; t++)
		{
			f64 x = k * t * c;
			Complex euler(cos(x), sin(x));
			sum += in[t] * euler;
		}

		result[k] = sum;					//add the bin to the result.
	}
}

void lowPass(std::vector<Complex>& input)
{
	ui32 count = input.size();
	for (ui32 i = 0; i < count; i++)
	{
		input[i].setReal(LPF.process(input[i].real()));
	}
}

f64 lowPass(f64 input)
{
	return LPF.process(input);
}

f64	getDFTBinFrequency(ui32 index)
{
	return index * DFT_binConstant;
}

void initializeDFT()
{
	DFT_binConstant = (f64)DFT_SAMPLE_RATE / (f64)INPUT_BUFFER_SIZE;
	LPF = Biquad(bq_type_lowpass, 20000.0 / SAMPLE_RATE, 0.707, 6);
}