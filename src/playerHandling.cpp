#include "playerHandling.h"

PlayersHandling::PlayersHandling() {
	this->ARR = 0.03;
	this->DAS = 0.1;
	this->SDARR = 0.1;
}

PlayersHandling::PlayersHandling(double das, double arr, double sdarr)
{
	this->ARR = arr;
	this->DAS = das;
	this->SDARR = sdarr;
}

double PlayersHandling::getARR()
{
	return ARR;
}

double PlayersHandling::getSDARR()
{
	return SDARR;
}

bool PlayersHandling::reachDAS(double currentDASTimer)
{
	if (currentDASTimer < DAS) {
		return false;
	}
	return true;
}

bool PlayersHandling::reachARR(double currentARRTimer)
{
	if (currentARRTimer < ARR) {
		return false;
	}

	return true;
}

bool PlayersHandling::reachSDARR(double currentSDARRTimer)
{
	if (currentSDARRTimer < SDARR) {
		return false;
	}

	return true;
}