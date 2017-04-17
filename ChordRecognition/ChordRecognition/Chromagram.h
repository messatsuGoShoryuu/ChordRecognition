#ifndef _CHROMAGRAM_H_
#define	_CHROMAGRAM_H_

#include <vector>
#include "Complex.h"
#include <algorithm>

extern	void	chromagram(f64*	in, f64* out, ui32 count, f64 refFreq);
extern void		sortChromagram(ui32* indexes, f64* chromagram);
extern int		ftom(f64 frequency);

#endif // !_CHROMAGRAM_H_
