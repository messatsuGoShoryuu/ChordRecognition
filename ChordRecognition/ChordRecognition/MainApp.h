#ifndef _MAINAPP_H_
#define _MAINAPP_H_

#include <vector>
#include "types.h"
#include "Complex.h"


class MainApp
{
public:
	void	initialize();
	void	shutDown();
	bool	DFT();

	inline	f64		fundamental(){ return m_fundamental; }
	void calculateFundamental();
	void	scheduleDFT(f32* input);

	void	calculateChromagram();

	inline	const f64*	chromagram(){ return m_chromagram; }

	inline	const std::vector<Complex>&	inputSamples(){ return m_input; }
	inline  const std::vector<Complex>&	dft(){ return m_DFT; }
	inline  const std::vector<f64>&	inputf() { return m_inputf; }
	inline	bool	requestSamples(){ return m_requestSamples; }

	inline void		resetDFTState()
	{
		m_requestSamples = true;
		m_wantsToDFT = false;
	}

	void update();

	inline	void					run(){ m_isRunning = true; }
	inline	void					stop(){ m_isRunning = false; }
	inline bool						isRunning(){ return m_isRunning; }
private:
	std::vector<Complex>	m_DFT;
	std::vector<Complex>	m_input;
	std::vector<f64>		m_inputf;
	std::vector<Complex>	m_temp;
	f64						m_chromagram[12];
	bool					m_wantsToDFT;
	bool					m_requestSamples;
	bool					m_isRunning = false;
	f64						m_fundamental;
};
#endif // !_MAINAPP_H
