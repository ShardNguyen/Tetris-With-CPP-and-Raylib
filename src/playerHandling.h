#ifndef _PLAYER_HANDLING_
#define _PLAYER_HANDLING_

class PlayersHandling {
private:
	double DAS;
	double ARR;
	double SDARR;
public:
	PlayersHandling();
	PlayersHandling(double das, double arr, double sdarr);
	double getARR();
	double getSDARR();
	bool reachDAS(double currentDASTimer);
	bool reachARR(double currentARRTimer);
	bool reachSDARR(double currentSDARRTimer);
};

#endif