#include "RandomValueUniform.h"

RandomValueUniform* RandomValueUniform::Istanza;

RandomValueUniform* RandomValueUniform::Istance()
{
	if (Istanza == nullptr)
		Istanza = new RandomValueUniform();

	return Istanza;
}

RandomValueUniform::RandomValueUniform()
{}

/*
	Questo metodo prende in input un intervallo (start, end)
	e restituisce un numero casuale scelto in maniera uniformemente
	distribuita da questo intervallo.
*/
int RandomValueUniform::getRandomValue(int start, int end)
{
	mt19937 rng(dev());
	uniform_int_distribution<std::mt19937::result_type> dist(start,end);
	return dist(rng);
}