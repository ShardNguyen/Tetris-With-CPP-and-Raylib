#include "kickTable.h"

void SRSTable::getKickOffset(std::vector<std::vector<int>> &kickOffset, int id, int rotationState)
{
	switch (id) {
		case 3:
			kickOffset = ITable[rotationState];
			break;
		case 4:
			kickOffset = {};
			break;
		default:
			kickOffset = JLSTZTable[rotationState];
			break;
	}
}

void AkiraSRSTable::getKickOffset(std::vector<std::vector<int>> &kickOffset, int id, int rotationState)
{
	switch (id) {
		case 3:
			kickOffset = ITable[rotationState];
			break;
		case 4:
			kickOffset = {};
			break;
		default:
			kickOffset = JLSTZTable[rotationState];
			break;
	}
}
