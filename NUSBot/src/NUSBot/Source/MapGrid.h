#pragma once

#include <Common.h>
#include "micromanagement/MicroManager.h"

class GridCell
{
public:

	int					timeLastVisited, timeLastOpponentSeen;
	UnitVector			ourUnits;
	UnitVector			oppUnits;
	BWAPI::Position		center;

	//$Sam-S 2014 May 11 - re-design constructor
//	GridCell() :		timeLastVisited(0), timeLastOpponentSeen(0) {}
	GridCell();
	//$Sam-E 2014 May 11 - re-design constructor
	
	//$Sam-S 2014 Apr 28
	//All Influence Map variables
	int					BIM_Mineral_Amount	;		// New	total Mineral within a Grid
	int					BIM_Gas_Amount		;		// New	total Gas within a Grid

	bool				BIM_Unit_Grid_1st_Time_Flag_E;	// Control flag to differenciate the times of update of a certain Grid cell within a frame. 
	int 				BIM_UA_HP_E		;		// New	total 'HP' for ALL Enemy Unit Attackable (UA) within a Grid
	int 				BIM_UA_DHPC_E	;		// New	total 'Damage * HP / CoolDown' for ALL Enemy Unit Attackable (UA) within a Grid
	int 				BIM_UA_DHPC_G_E	;		// GroundWeapon: New	total 'Damage * HP / CoolDown' for ALL Enemy Unit Attackable (UA) within a Grid
	int 				BIM_UA_DHPC_A_E	;		// AirWeapon: New	total 'Damage * HP / CoolDown' for ALL Enemy Unit Attackable (UA) within a Grid
	int 				BIM_UN_HP_E		;		// New	total 'HP' for ALL Enemy Unit Non-Attackable (UN) within a Grid

	bool				BIM_Unit_Grid_1st_Time_Flag_O;	// Control flag to differenciate the times of update of a certain Grid cell within a frame. 
 	int 				BIM_UA_HP_O		;		// New	total 'HP' for ALL Own Unit Attackable (UA) within a Grid
	int 				BIM_UA_DHPC_O	;		// New	total 'Damage * HP / CoolDown' for ALL Own Unit Attackable (UA) within a Grid
	int 				BIM_UA_DHPC_G_O	;		// GroundWeapon: New	total 'Damage * HP / CoolDown' for ALL Own Unit Attackable (UA) within a Grid
	int 				BIM_UA_DHPC_A_O	;		// AirWeapon: New	total 'Damage * HP / CoolDown' for ALL Own Unit Attackable (UA) within a Grid
 	int 				BIM_UN_HP_O		;		// New	total 'HP' for ALL Own Unit Non-Attackable (UN) within a Grid
										
	bool				BIM_Building_Grid_1st_Time_Flag_E;	// Control flag to differenciate the times of update of a certain Grid cell within a frame. 
	int 				BIM_BA_HP_E		;		// New	total 'HP' for ALL Enemy Building Attackable (BA) within a Grid
	int 				BIM_BA_DHPC_E	;		// New	total 'Damage * HP / CoolDown' for ALL Enemy Building Attackable (BA) within a Grid
	int 				BIM_BA_DHPC_G_E	;		// GroundWeapon: New	total 'Damage * HP / CoolDown' for ALL Enemy Building Attackable (BA) within a Grid
	int 				BIM_BA_DHPC_A_E	;		// AirWeapon: New	total 'Damage * HP / CoolDown' for ALL Enemy Building Attackable (BA) within a Grid
	int 				BIM_BN_HP_E		;		// New	total 'HP' for ALL Enemy Building Non-Attackable (BN) within a Grid

	bool				BIM_Building_Grid_1st_Time_Flag_O;	// Control flag to differenciate the times of update of a certain Grid cell within a frame. 
	int 				BIM_BA_HP_O		;		// New	total 'HP' for ALL Own Building Attackable (BA) within a Grid
	int 				BIM_BA_DHPC_O	;		// New	total 'Damage * HP / CoolDown' for ALL Own Building Attackable (BA) within a Grid
	int 				BIM_BA_DHPC_G_O	;		// GroundWeapon: New	total 'Damage * HP / CoolDown' for ALL Own Building Attackable (BA) within a Grid
	int 				BIM_BA_DHPC_A_O	;		// AirWeapon: New	total 'Damage * HP / CoolDown' for ALL Own Building Attackable (BA) within a Grid
	int 				BIM_BN_HP_O		;		// New	total 'HP' for ALL Own Building Non-Attackable (BN) within a Grid
										
	BWAPI::Position		BIM_Choke_Position	;		// New	center coordination of choke point (x, y)
	double				BIM_Choke_Width		;		// New	choke width (in pixels? refer to BWTA)
	double				BIM_Choke_Range		;		// New	A circle radius/range for the Units to be considered/sum-up around the centre of choke point. This can be a pre-defined fixed value in NUS-Bot Parameter/Option.

	bool				BIM_Choke_Grid_1st_Time_Flag_E;	// Control flag to differenciate the times of update of a certain Grid cell within a frame. 
	int 				LIM_Choke_UA_HP_E		;		// New	total 'HP' for ALL Enemy Unit Attackable (UA) within Choke-Range
	int 				LIM_Choke_UA_DHPC_G_E	;		// GroundWeapon: New	total 'Damage * HP / CoolDown' for ALL Enemy Unit Attackable (UA) within Choke-Range
	int 				LIM_Choke_UA_DHPC_A_E	;		// AirWeapon: New	total 'Damage * HP / CoolDown' for ALL Enemy Unit Attackable (UA) within Choke-Range
	int 				LIM_Choke_UA_DHPC_E		;		// New	total 'Damage * HP / CoolDown' for ALL Enemy Unit Attackable (UA) within Choke-Range
	int 				LIM_Choke_UN_HP_E		;		// New	total 'HP' for ALL Enemy Unit Non-Attackable (UN) within Choke-Range
	int 				LIM_Choke_BA_HP_E		;		// New	total 'HP' for ALL Enemy Building Attackable (BA) within Choke-Range
	int 				LIM_Choke_BA_DHPC_G_E	;		// GroundWeapon: New	total 'Damage * HP / CoolDown' for ALL Enemy Building Attackable (BA) within Choke-Range
	int 				LIM_Choke_BA_DHPC_A_E	;		// AirWeapon: New	total 'Damage * HP / CoolDown' for ALL Enemy Building Attackable (BA) within Choke-Range
	int 				LIM_Choke_BA_DHPC_E		;		// New	total 'Damage * HP / CoolDown' for ALL Enemy Building Attackable (BA) within Choke-Range
	int 				LIM_Choke_BN_HP_E		;		// New	total 'HP' for ALL Enemy Building Non-Attackable (BN) within Choke-Range

	bool				BIM_Choke_Grid_1st_Time_Flag_O;	// Control flag to differenciate the times of update of a certain Grid cell within a frame. 
	int 				LIM_Choke_UA_HP_O		;		// New	total 'HP' for ALL Own Unit Attackable (UA) within Choke-Range
	int 				LIM_Choke_UA_DHPC_G_O	;		// GroundWeapon: New	total 'Damage * HP / CoolDown' for ALL Own Unit Attackable (UA) within Choke-Range
	int 				LIM_Choke_UA_DHPC_A_O	;		// AirWeapon: New	total 'Damage * HP / CoolDown' for ALL Own Unit Attackable (UA) within Choke-Range
	int 				LIM_Choke_UA_DHPC_O		;		// New	total 'Damage * HP / CoolDown' for ALL Own Unit Attackable (UA) within Choke-Range
	int 				LIM_Choke_UN_HP_O		;		// New	total 'HP' for ALL Own Unit Non-Attackable (UN) within Choke-Range
	int 				LIM_Choke_BA_HP_O		;		// New	total 'HP' for ALL Own Building Attackable (BA) within Choke-Range
	int 				LIM_Choke_BA_DHPC_G_O	;		// GroundWeapon: New	total 'Damage * HP / CoolDown' for ALL Own Building Attackable (BA) within Choke-Range
	int 				LIM_Choke_BA_DHPC_A_O	;		// AirWeapon: New	total 'Damage * HP / CoolDown' for ALL Own Building Attackable (BA) within Choke-Range
	int 				LIM_Choke_BA_DHPC_O		;		// New	total 'Damage * HP / CoolDown' for ALL Own Building Attackable (BA) within Choke-Range
	int 				LIM_Choke_BN_HP_O		;		// New	total 'HP' for ALL Own Building Non-Attackable (BN) within Choke-Range

	//$Sam-E 2014 Apr 28

};



class MapGrid {

	MapGrid();
	MapGrid(int mapWidth, int mapHeight, int cellSize);

	int							cellSize;
	int							mapWidth, mapHeight;
	int							rows, cols;
	int							lastUpdated;

	bool						contains(UnitVector & units, BWAPI::Unit * unit);

	std::vector< GridCell >		cells;

	void						calculateCellCenters();

	void						clearGrid();
	BWAPI::Position				getCellCenter(int x, int y);

	BWAPI::Position				naturalExpansion;

public:

	// yay for singletons!
	//$Sam 2014 May 14 - To make rows and cols Public.
//	int					rows, cols;

	static MapGrid &	Instance();

	void				update();
	void				GetUnits(UnitVector & units, BWAPI::Position center, int radius, bool ourUnits, bool oppUnits);
	BWAPI::Position		getLeastExplored();
	BWAPI::Position		getNaturalExpansion();

	GridCell & getCellByIndex(int r, int c)		{ return cells[r*cols + c]; }
	GridCell & getCell(BWAPI::Position pos)		{ return getCellByIndex(pos.y() / cellSize, pos.x() / cellSize); }
	GridCell & getCell(BWAPI::Unit * unit)		{ return getCell(unit->getPosition()); }

};
