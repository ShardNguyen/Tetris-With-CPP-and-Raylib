#ifndef _KICK_TABLE_
#define _KICK_TABLE_
#include "block.h"
#include "grid.h"
#include <vector>

/* Every table will follow this row format:
0 -> R
R -> 0
R -> 2 
2 -> R
2 -> L
L -> 2
L -> 0
0 -> L
*/

class KickTable {
public:
	virtual void getKickOffset(std::vector<std::vector<int>> &kickOffset, int id, int rotationState) = 0;
	virtual ~KickTable() = default;
};

class SRSTable: public KickTable {
private:
	std::vector<std::vector<std::vector<int>>> JLSTZTable = {
		{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
		{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
		{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
		{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
		{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
		{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
		{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
		{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}
	};
	std::vector<std::vector<std::vector<int>>> ITable = {
		{{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}},
		{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}},
		{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}},
		{{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}},
		{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}},
		{{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}},
		{{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}},
		{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}
	};
public:
	void getKickOffset(std::vector<std::vector<int>> &kickOffset, int id, int rotationState);
};

class AkiraSRSTable: public KickTable {
private:
	std::vector<std::vector<std::vector<int>>> JLSTZTable = {
		{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
		{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
		{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
		{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},
		{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
		{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
		{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
		{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}
	};
	std::vector<std::vector<std::vector<int>>> ITable = {
		{{0, 0}, {-2, 0}, {1, 0}, {1, 2}, {-2, -1}},
		{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}},
		{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}},
		{{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -1}},
		{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -1}},
		{{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}},
		{{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}},
		{{0, 0}, {2, 0}, {-1, 0}, {-1, 2}, {2, -1}}
	};
	void getKickOffset(std::vector<std::vector<int>> &kickOffset, int id, int rotationState);
};

#endif