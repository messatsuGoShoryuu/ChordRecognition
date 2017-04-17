#ifndef _DFT_H_
#define	_DFT_H_

#include <vector>
#include "types.h"

class Complex;

extern void DFT(const std::vector<Complex>& data, std::vector<Complex>& result);
extern void lowPass(std::vector<Complex>& input);
extern f64 lowPass(double input);
extern f64	getDFTBinFrequency(ui32 index);
extern void	initializeDFT();

#endif // !_DFT_H_
