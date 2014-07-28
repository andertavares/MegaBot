#include "Common.h"
#include "MapGrid.h"

MapGrid & MapGrid::Instance() 
{
	static MapGrid instance(BWAPI::Broodwar->mapWidth()*32, BWAPI::Broodwar->mapHeight()*32, Options::Tools::MAP_GRID_SIZE);
	return instance;
}

MapGrid::MapGrid() {}
	
MapGrid::MapGrid(int mapWidth, int mapHeight, int cellSize) 
	: mapWidth(mapWidth)
	, mapHeight(mapHeight)
	, cellSize(cellSize)
	, cols((mapWidth + cellSize - 1) / cellSize)
	, rows((mapHeight + cellSize - 1) / cellSize)
	, cells(rows * cols)
	, lastUpdated(0)
{
	calculateCellCenters();
}

BWAPI::Position MapGrid::getNaturalExpansion() 
{
	return naturalExpansion;
}

BWAPI::Position MapGrid::getLeastExplored() 
{
	int minSeen = 1000000;
	double minSeenDist = 100000;
	int leastRow(0), leastCol(0);

	for (int r=0; r<rows; ++r)
	{
		for (int c=0; c<cols; ++c)
		{
			// get the center of this cell
			BWAPI::Position cellCenter = getCellCenter(r,c);

			// don't worry about places that aren't connected to our start locatin
			if (!BWTA::isConnected(BWAPI::TilePosition(cellCenter), BWAPI::Broodwar->self()->getStartLocation()))
			{
				continue;
			}

			BWAPI::Position home(BWAPI::Broodwar->self()->getStartLocation());
			double dist = home.getDistance(getCellByIndex(r, c).center);
			if ((getCellByIndex(r, c).timeLastVisited < minSeen) || ((getCellByIndex(r, c).timeLastVisited == minSeen) && (dist < minSeenDist)))
			{
				leastRow = r;
				leastCol = c;
				minSeen = getCellByIndex(r, c).timeLastVisited;
				minSeenDist = dist;
			}
		}
	}

	return getCellCenter(leastRow, leastCol);
}

void MapGrid::calculateCellCenters()
{
	for (int r=0; r < rows; ++r)
	{
		for (int c=0; c < cols; ++c)
		{
			GridCell & cell = getCellByIndex(r,c);

			int centerX = (c * cellSize) + (cellSize / 2);
			int centerY = (r * cellSize) + (cellSize / 2);

			// if the x position goes past the end of the map
			if (centerX > mapWidth)
			{
				// when did the last cell start
				int lastCellStart		= c * cellSize;

				// how wide did we go
				int tooWide				= mapWidth - lastCellStart;
				
				// go half the distance between the last start and how wide we were
				centerX = lastCellStart + (tooWide / 2);
			}
			else if (centerX == mapWidth)
			{
				centerX -= 50;
			}

			if (centerY > mapHeight)
			{
				// when did the last cell start
				int lastCellStart		= r * cellSize;

				// how wide did we go
				int tooHigh				= mapHeight - lastCellStart;
				
				// go half the distance between the last start and how wide we were
				centerY = lastCellStart + (tooHigh / 2);
			}
			else if (centerY == mapHeight)
			{
				centerY -= 50;
			}

			cell.center = BWAPI::Position(centerX, centerY);
			assert(cell.center.isValid());
		}
	}
}

BWAPI::Position MapGrid::getCellCenter(int row, int col)
{
	return getCellByIndex(row, col).center;
}

// clear the vectors in the grid
void MapGrid::clearGrid() { 

	for (size_t i(0); i<cells.size(); ++i) 
	{
		cells[i].ourUnits.clear();
		cells[i].oppUnits.clear();

	//$Sam-S 2014 May 11

	//GridCell::
	// Control flag to differenciate the times of update of a certain Grid cell within a frame.
	cells[i].BIM_Unit_Grid_1st_Time_Flag_E = true;
	cells[i].BIM_Unit_Grid_1st_Time_Flag_O = true;
	cells[i].BIM_Building_Grid_1st_Time_Flag_E = true;
	cells[i].BIM_Building_Grid_1st_Time_Flag_O = true;
	cells[i].BIM_Choke_Grid_1st_Time_Flag_E = true;
	cells[i].BIM_Choke_Grid_1st_Time_Flag_O = true;
	//$Sam-E 2014 May 11
	

	}
}

// populate the grid with units
void MapGrid::update() 
{
	for (int i=0; i<cols; i++) 
	{
		if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawLineMap(i*cellSize, 0, i*cellSize, mapHeight, BWAPI::Colors::Blue);
	}

	for (int j=0; j<rows; j++) 
	{
		if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawLineMap(0, j*cellSize, mapWidth, j*cellSize, BWAPI::Colors::Blue);
	}

	for (int r=0; r < rows; ++r)
	{
		for (int c=0; c < cols; ++c)
		{
			GridCell & cell = getCellByIndex(r,c);
			
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y(),    "Last Seen : %d", cell.timeLastVisited);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+10, "Row/Col (%d, %d)", r, c);
			
			//$Sam-S 2014 Apr 28
			//Display some Influence Map variables on screen
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+20, "Choke ProCtr: %d", cell.BIM_Mineral_Amount);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+30, "Choke Cells#: %d", cell.BIM_Gas_Amount);

			// Own Unit & Building LIM on screen
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+40,  "UA_HP_O____: %d", cell.BIM_UA_HP_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+50,  "UA_DHPC_O__: %d", cell.BIM_UA_DHPC_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+60,  "UA_DHPC_G_O: %d", cell.BIM_UA_DHPC_G_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+70,  "UA_DHPC_A_O: %d", cell.BIM_UA_DHPC_A_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+80,  "UN_HP_O____: %d", cell.BIM_UN_HP_O);

			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+100, "BA_HP_O____: %d", cell.BIM_BA_HP_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+110, "BA_DHPC_O__: %d", cell.BIM_BA_DHPC_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+120, "BA_DHPC_G_O: %d", cell.BIM_BA_DHPC_G_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+130, "BA_DHPC_A_O: %d", cell.BIM_BA_DHPC_A_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()+140, "BN_HP_O____: %d", cell.BIM_BN_HP_O);

			// Enemy Unit & Building LIM on screen
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()-120, "UA_HP_E____: %d", cell.BIM_UA_HP_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()-110, "UA_DHPC_E__: %d", cell.BIM_UA_DHPC_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()-100, "UA_DHPC_G_E: %d", cell.BIM_UA_DHPC_G_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()-90,  "UA_DHPC_A_E: %d", cell.BIM_UA_DHPC_A_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()-80,  "UN_HP_E____: %d", cell.BIM_UN_HP_E);

			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()-60,  "BA_HP_E____: %d", cell.BIM_BA_HP_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()-50,  "BA_DHPC_E__: %d", cell.BIM_BA_DHPC_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()-40,  "BA_DHPC_G_E: %d", cell.BIM_BA_DHPC_G_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()-30,  "BA_DHPC_A_E: %d", cell.BIM_BA_DHPC_A_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x(), cell.center.y()-20,  "BN_HP_E____: %d", cell.BIM_BN_HP_E);


			// Choke BIM on screen
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y(),    "Choke_Pos: (%d, %d)", cell.BIM_Choke_Position.x(), cell.BIM_Choke_Position.y());
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+10, "Choke_Width: %d", cell.BIM_Choke_Width);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+20, "Choke_Range: %d", cell.BIM_Choke_Range);

			// Own Choke&Unit LIM on screen
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+40,  "C_UA_HP_O____: %d", cell.LIM_Choke_UA_HP_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+50,  "C_UA_DHPC_O__: %d", cell.LIM_Choke_UA_DHPC_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+60,  "C_UA_DHPC_G_O: %d", cell.LIM_Choke_UA_DHPC_G_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+70,  "C_UA_DHPC_A_O: %d", cell.LIM_Choke_UA_DHPC_A_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+80,  "C_UN_HP_O____: %d", cell.LIM_Choke_UN_HP_O);

			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+100, "C_BA_HP_O____: %d", cell.LIM_Choke_BA_HP_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+110, "C_BA_DHPC_O__: %d", cell.LIM_Choke_BA_DHPC_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+120, "C_BA_DHPC_G_O: %d", cell.LIM_Choke_BA_DHPC_G_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+130, "C_BA_DHPC_A_O: %d", cell.LIM_Choke_BA_DHPC_A_O);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()+140, "C_BN_HP_O____: %d", cell.LIM_Choke_BN_HP_O);

			// Enemy Choke&Unit LIM on screen
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()-120, "C_UA_HP_E____: %d", cell.LIM_Choke_UA_HP_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()-110, "C_UA_DHPC_E__: %d", cell.LIM_Choke_UA_DHPC_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()-100, "C_UA_DHPC_G_E: %d", cell.LIM_Choke_UA_DHPC_G_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()-90,  "C_UA_DHPC_A_E: %d", cell.LIM_Choke_UA_DHPC_A_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()-80,  "C_UN_HP_E____: %d", cell.LIM_Choke_UN_HP_E);

			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()-60,  "C_BA_HP_E____: %d", cell.LIM_Choke_BA_HP_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()-50,  "C_BA_DHPC_E__: %d", cell.LIM_Choke_BA_DHPC_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()-40,  "C_BA_DHPC_G_E: %d", cell.LIM_Choke_BA_DHPC_G_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()-30,  "C_BA_DHPC_A_E: %d", cell.LIM_Choke_BA_DHPC_A_E);
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(cell.center.x()-160, cell.center.y()-20,  "C_BN_HP_E____: %d", cell.LIM_Choke_BN_HP_E);

			//$Sam-E 2014 Apr 28

		}
	}

	// clear the grid
	clearGrid();

	//BWAPI::Broodwar->printf("MapGrid info: WH(%d, %d)  CS(%d)  RC(%d, %d)  C(%d)", mapWidth, mapHeight, cellSize, rows, cols, cells.size());

	// add our units to the appropriate cell
	BOOST_FOREACH(BWAPI::Unit * unit, BWAPI::Broodwar->self()->getUnits()) 
	{
		getCell(unit).ourUnits.push_back(unit);
		getCell(unit).timeLastVisited = BWAPI::Broodwar->getFrameCount();
			//$Sam-S 2014 May 11
			//Here to populate all IM variable values for Own units
			//getCell(unit).BIM_Mineral_Amount	= [TBD]; 
			//getCell(unit).BIM_Gas_Amount		= [TBD]; 
		

		//[TBD] Add a frequency control for updating LIMs?? Not on each frame?
		//if (BWAPI::Broodwar->getFrameCount() % 20 == 0)
		
		// below is for Own Unit (non-Building) LIM
		if (!unit->getType().isBuilding()) 
		{

			if (getCell(unit).BIM_Unit_Grid_1st_Time_Flag_O) 
			 {

			 
				getCell(unit).BIM_Unit_Grid_1st_Time_Flag_O = false;

				if (unit->getType().canAttack()) // Beta version: false for units that can only inflict damage via special abilities (such as Protoss High Templar).
				{ 
					getCell(unit).BIM_UA_HP_O = unit->getHitPoints() + unit->getShields();

					//Error handling for Units with 'None' ground/air Weapon
					if (unit->getType().groundWeapon().targetsGround())
					{
						//ddd
						//getCell(unit).BIM_Mineral_Amount += 1; 
						getCell(unit).BIM_UA_DHPC_G_O = unit->getType().groundWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().groundWeapon().damageCooldown();
					}
					if (unit->getType().airWeapon().targetsAir())
					{
						//ddd
						//getCell(unit).BIM_Gas_Amount += 1; 
				 		getCell(unit).BIM_UA_DHPC_A_O = unit->getType().airWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().airWeapon().damageCooldown();
					}

					getCell(unit).BIM_UA_DHPC_O = getCell(unit).BIM_UA_DHPC_G_O + getCell(unit).BIM_UA_DHPC_A_O;
					
				}
				else
				{
					getCell(unit).BIM_UN_HP_O = unit->getHitPoints() + unit->getShields();

				}
								
			 }
			 else // not 1st timer for Unit (non-Building) variables
			 {
				//getCell(unit).BIM_Unit_Grid_1st_Time_Flag_O = false;

				if (unit->getType().canAttack()) // Beta version: false for units that can only inflict damage via special abilities (such as Protoss High Templar).
				{ 
					getCell(unit).BIM_UA_HP_O += (unit->getHitPoints() + unit->getShields());

					//Error handling for Units with 'None' ground/air Weapon
					if (unit->getType().groundWeapon().targetsGround())
					{
						//ddd
						//getCell(unit).BIM_Mineral_Amount += 1; 
						getCell(unit).BIM_UA_DHPC_G_O += (unit->getType().groundWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().groundWeapon().damageCooldown());
					}
					if (unit->getType().airWeapon().targetsAir())
					{
						//ddd
						//getCell(unit).BIM_Gas_Amount += 1; 
				 		getCell(unit).BIM_UA_DHPC_A_O += (unit->getType().airWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().airWeapon().damageCooldown());
					}
					// Note: Use = instead of +=
					getCell(unit).BIM_UA_DHPC_O = (getCell(unit).BIM_UA_DHPC_G_O + getCell(unit).BIM_UA_DHPC_A_O);

				}
				else
				{
					getCell(unit).BIM_UN_HP_O += (unit->getHitPoints() + unit->getShields());
				}

			 }
			
		}

		else 		// below is for Own Building LIM
		{

			if (getCell(unit).BIM_Building_Grid_1st_Time_Flag_O) 
			 {

			 
				getCell(unit).BIM_Building_Grid_1st_Time_Flag_O = false;

				if (unit->getType().canAttack()) // Beta version: false for Building/units that can only inflict damage via special abilities (such as Protoss High Templar).
				{ 
					getCell(unit).BIM_BA_HP_O = unit->getHitPoints() + unit->getShields();

					//Error handling for Building with 'None' ground/air Weapon
					if (unit->getType().groundWeapon().targetsGround())
					{
						//ddd
						//getCell(unit).BIM_Mineral_Amount += 1; 
						getCell(unit).BIM_BA_DHPC_G_O = unit->getType().groundWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().groundWeapon().damageCooldown();
					}
					if (unit->getType().airWeapon().targetsAir())
					{
						//ddd
						//getCell(unit).BIM_Gas_Amount += 1; 
				 		getCell(unit).BIM_BA_DHPC_A_O = unit->getType().airWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().airWeapon().damageCooldown();
					}

					getCell(unit).BIM_BA_DHPC_O = getCell(unit).BIM_BA_DHPC_G_O + getCell(unit).BIM_BA_DHPC_A_O;
					
				}
				else
				{
					getCell(unit).BIM_BN_HP_O = unit->getHitPoints() + unit->getShields();

				}
								
			 }
			 else // not 1st timer for Building variables
			 {
				//getCell(unit).BIM_Building_Grid_1st_Time_Flag_O = false;

				if (unit->getType().canAttack()) // Beta version: false for units that can only inflict damage via special abilities (such as Protoss High Templar).
				{ 
					getCell(unit).BIM_BA_HP_O += (unit->getHitPoints() + unit->getShields());

					//Error handling for Building with 'None' ground/air Weapon
					if (unit->getType().groundWeapon().targetsGround())
					{
						//ddd
						//getCell(unit).BIM_Mineral_Amount += 1; 
						getCell(unit).BIM_BA_DHPC_G_O += (unit->getType().groundWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().groundWeapon().damageCooldown());
					}
					if (unit->getType().airWeapon().targetsAir())
					{
						//ddd
						//getCell(unit).BIM_Gas_Amount += 1; 
				 		getCell(unit).BIM_BA_DHPC_A_O += (unit->getType().airWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().airWeapon().damageCooldown());
					}

					// Note: Use = instead of +=
					getCell(unit).BIM_BA_DHPC_O = (getCell(unit).BIM_BA_DHPC_G_O + getCell(unit).BIM_BA_DHPC_A_O);

				}
				else
				{
					getCell(unit).BIM_BN_HP_O += (unit->getHitPoints() + unit->getShields());
				}

			 }
			
		}
		// End of Own Unit/Building LIM

			//$Sam-E 2014 Apr 28
	}

	// add enemy units to the appropriate cell
	BOOST_FOREACH(BWAPI::Unit * unit, BWAPI::Broodwar->enemy()->getUnits()) 
	{
		if (unit->getHitPoints() > 0) 
		{
			getCell(unit).oppUnits.push_back(unit);
			getCell(unit).timeLastOpponentSeen = BWAPI::Broodwar->getFrameCount();
			//$Sam-S 2014 May 11
			//Here to populate all IM variable values for Enemy units
			//getCell(unit).BIM_Mineral_Amount	= [TBD]; 
			//getCell(unit).BIM_Gas_Amount		= [TBD]; 
		

		//[TBD] Add a frequency control for updating LIMs?? Not on each frame?
		//if (BWAPI::Broodwar->getFrameCount() % 20 == 0)
		
		// below is for Enemy Unit (non-Building) LIM
		if (!unit->getType().isBuilding()) 
		{

			if (getCell(unit).BIM_Unit_Grid_1st_Time_Flag_E) 
			 {

			 
				getCell(unit).BIM_Unit_Grid_1st_Time_Flag_E = false;

				if (unit->getType().canAttack()) // Beta version: false for units that can only inflict damage via special abilities (such as Protoss High Templar).
				{ 
					getCell(unit).BIM_UA_HP_E = unit->getHitPoints() + unit->getShields();

					//Error handling for Units with 'None' ground/air Weapon
					if (unit->getType().groundWeapon().targetsGround())
					{
						//ddd
						//getCell(unit).BIM_Mineral_Amount += 1; 
						getCell(unit).BIM_UA_DHPC_G_E = unit->getType().groundWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().groundWeapon().damageCooldown();
					}
					if (unit->getType().airWeapon().targetsAir())
					{
						//ddd
						//getCell(unit).BIM_Gas_Amount += 1; 
				 		getCell(unit).BIM_UA_DHPC_A_E = unit->getType().airWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().airWeapon().damageCooldown();
					}

					getCell(unit).BIM_UA_DHPC_E = getCell(unit).BIM_UA_DHPC_G_E + getCell(unit).BIM_UA_DHPC_A_E;
					
				}
				else
				{
					getCell(unit).BIM_UN_HP_E = unit->getHitPoints() + unit->getShields();

				}
								
			 }
			 else // not 1st timer for Unit (non-Building) variables
			 {
				//getCell(unit).BIM_Unit_Grid_1st_Time_Flag_E = false;

				if (unit->getType().canAttack()) // Beta version: false for units that can only inflict damage via special abilities (such as Protoss High Templar).
				{ 
					getCell(unit).BIM_UA_HP_E += (unit->getHitPoints() + unit->getShields());

					//Error handling for Units with 'None' ground/air Weapon
					if (unit->getType().groundWeapon().targetsGround())
					{
						//ddd
						//getCell(unit).BIM_Mineral_Amount += 1; 
						getCell(unit).BIM_UA_DHPC_G_E += (unit->getType().groundWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().groundWeapon().damageCooldown());
					}
					if (unit->getType().airWeapon().targetsAir())
					{
						//ddd
						//getCell(unit).BIM_Gas_Amount += 1; 
				 		getCell(unit).BIM_UA_DHPC_A_E += (unit->getType().airWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().airWeapon().damageCooldown());
					}
					// Note: Use = instead of +=
					getCell(unit).BIM_UA_DHPC_E = (getCell(unit).BIM_UA_DHPC_G_E + getCell(unit).BIM_UA_DHPC_A_E);

				}
				else
				{
					getCell(unit).BIM_UN_HP_E += (unit->getHitPoints() + unit->getShields());
				}

			 }
			
		}

		else 		// below is for Enemy Building LIM
		{

			if (getCell(unit).BIM_Building_Grid_1st_Time_Flag_E) 
			 {

			 
				getCell(unit).BIM_Building_Grid_1st_Time_Flag_E = false;

				if (unit->getType().canAttack()) // Beta version: false for Building/units that can only inflict damage via special abilities (such as Protoss High Templar).
				{ 
					getCell(unit).BIM_BA_HP_E = unit->getHitPoints() + unit->getShields();

					//Error handling for Building with 'None' ground/air Weapon
					if (unit->getType().groundWeapon().targetsGround())
					{
						//ddd
						//getCell(unit).BIM_Mineral_Amount += 1; 
						getCell(unit).BIM_BA_DHPC_G_E = unit->getType().groundWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().groundWeapon().damageCooldown();
					}
					if (unit->getType().airWeapon().targetsAir())
					{
						//ddd
						//getCell(unit).BIM_Gas_Amount += 1; 
				 		getCell(unit).BIM_BA_DHPC_A_E = unit->getType().airWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().airWeapon().damageCooldown();
					}

					getCell(unit).BIM_BA_DHPC_E = getCell(unit).BIM_BA_DHPC_G_E + getCell(unit).BIM_BA_DHPC_A_E;
					
				}
				else
				{
					getCell(unit).BIM_BN_HP_E = unit->getHitPoints() + unit->getShields();

				}
								
			 }
			 else // not 1st timer for Building variables
			 {
				//getCell(unit).BIM_Building_Grid_1st_Time_Flag_E = false;

				if (unit->getType().canAttack()) // Beta version: false for units that can only inflict damage via special abilities (such as Protoss High Templar).
				{ 
					getCell(unit).BIM_BA_HP_E += (unit->getHitPoints() + unit->getShields());

					//Error handling for Building with 'None' ground/air Weapon
					if (unit->getType().groundWeapon().targetsGround())
					{
						//ddd
						//getCell(unit).BIM_Mineral_Amount += 1; 
						getCell(unit).BIM_BA_DHPC_G_E += (unit->getType().groundWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().groundWeapon().damageCooldown());
					}
					if (unit->getType().airWeapon().targetsAir())
					{
						//ddd
						//getCell(unit).BIM_Gas_Amount += 1; 
				 		getCell(unit).BIM_BA_DHPC_A_E += (unit->getType().airWeapon().damageAmount() * (unit->getHitPoints() + unit->getShields()) / unit->getType().airWeapon().damageCooldown());
					}

					// Note: Use = instead of +=
					getCell(unit).BIM_BA_DHPC_E = (getCell(unit).BIM_BA_DHPC_G_E + getCell(unit).BIM_BA_DHPC_A_E);

				}
				else
				{
					getCell(unit).BIM_BN_HP_E += (unit->getHitPoints() + unit->getShields());
				}

			 }
			
		}
		// End of Enemy Unit/Building LIM
		//$Sam-E 2014 May 11


		}
	}


	
/////////////////////////////////////////////////////////////////////////////////////////////////////
//$Sam-S 2014 May 14 - Choke LIM
// refer to BWTA APIs https://code.google.com/p/bwta/wiki/BWTAManual
/////////////////////////////////////////////////////////////////////////////////////////////////////

	BOOST_FOREACH (BWTA::Chokepoint * choke, BWTA::getChokepoints())
	{
		// BWAPI::Position choke->getCenter()
		// BWAPI::Position GridCell & getCell(choke->getCenter())
		getCell(choke->getCenter()).BIM_Choke_Position = choke->getCenter();
		getCell(choke->getCenter()).BIM_Choke_Width = choke->getWidth();
		// Phase1: hard-code radius for BIM_Choke_Range; not used in Phase1
		getCell(choke->getCenter()).BIM_Choke_Range = choke->getWidth();

		// The LIM_Choke value below is the sum-up of (1 self-centered Gird + values of neighbouring 8 Grids); In total, 9 Grids' BIM values.
		
		// Step1: Clear Choke LIM variables
		getCell(choke->getCenter()).LIM_Choke_UA_HP_E = 	0;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_G_E = 	0;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_A_E = 	0;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_E = 	0;
		getCell(choke->getCenter()).LIM_Choke_UN_HP_E = 	0;
		getCell(choke->getCenter()).LIM_Choke_BA_HP_E = 	0;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_G_E = 	0;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_A_E = 	0;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_E = 	0;
		getCell(choke->getCenter()).LIM_Choke_BN_HP_E = 	0;

		getCell(choke->getCenter()).LIM_Choke_UA_HP_O = 	0;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_G_O = 	0;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_A_O = 	0;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_O = 	0;
		getCell(choke->getCenter()).LIM_Choke_UN_HP_O = 	0;
		getCell(choke->getCenter()).LIM_Choke_BA_HP_O = 	0;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_G_O = 	0;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_A_O = 	0;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_O = 	0;
		getCell(choke->getCenter()).LIM_Choke_BN_HP_O = 	0;

		
		// Step2: Loop to sumup(9 Grids); Take note of valid Grid check for map boundary.

		// Row of Choke-center Cell
		int ChokeR =		choke->getCenter().y() / cellSize; 
		// Col of Choke-center Cell
		int ChokeC =		choke->getCenter().x() / cellSize; 

		//ddd
		//getCellByIndex(ChokeR , ChokeC).BIM_Mineral_Amount = 0; 
		getCellByIndex(ChokeR , ChokeC).BIM_Gas_Amount = 0; 
		for (int i=-1; i < 2; ++i) // Row -1, +0, +1
		{
			for (int j=-1; j < 2; ++j) // Col -1, +0, +1
			{  
//				if (getCellByIndex(ChokeR + i , ChokeC + j).center.isValid()) // Take note of valid Grid check for map boundary.
				if (BWAPI::Position(choke->getCenter().y() + i*320, choke->getCenter().x() + j*320).isValid()) // Take note of valid Grid check for map boundary.
				{
				//ddd
				getCellByIndex(ChokeR , ChokeC).BIM_Mineral_Amount += 1; 
				getCellByIndex(ChokeR , ChokeC).BIM_Gas_Amount += 1; 

				// for Enemy LIM
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_UA_HP_E += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_UA_HP_E;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_UA_DHPC_G_E += 	getCellByIndex(ChokeR + i , ChokeC + j).BIM_UA_DHPC_G_E;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_UA_DHPC_A_E += 	getCellByIndex(ChokeR + i , ChokeC + j).BIM_UA_DHPC_A_E;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_UA_DHPC_E += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_UA_DHPC_E;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_UN_HP_E += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_UN_HP_E;

				getCellByIndex(ChokeR , ChokeC).LIM_Choke_BA_HP_E += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_BA_HP_E;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_BA_DHPC_G_E += 	getCellByIndex(ChokeR + i , ChokeC + j).BIM_BA_DHPC_G_E;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_BA_DHPC_A_E += 	getCellByIndex(ChokeR + i , ChokeC + j).BIM_BA_DHPC_A_E;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_BA_DHPC_E += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_BA_DHPC_E;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_BN_HP_E += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_BN_HP_E;

				// for Own LIM
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_UA_HP_O += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_UA_HP_O;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_UA_DHPC_G_O += 	getCellByIndex(ChokeR + i , ChokeC + j).BIM_UA_DHPC_G_O;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_UA_DHPC_A_O += 	getCellByIndex(ChokeR + i , ChokeC + j).BIM_UA_DHPC_A_O;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_UA_DHPC_O += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_UA_DHPC_O;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_UN_HP_O += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_UN_HP_O;

				getCellByIndex(ChokeR , ChokeC).LIM_Choke_BA_HP_O += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_BA_HP_O;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_BA_DHPC_G_O += 	getCellByIndex(ChokeR + i , ChokeC + j).BIM_BA_DHPC_G_O;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_BA_DHPC_A_O += 	getCellByIndex(ChokeR + i , ChokeC + j).BIM_BA_DHPC_A_O;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_BA_DHPC_O += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_BA_DHPC_O;
				getCellByIndex(ChokeR , ChokeC).LIM_Choke_BN_HP_O += 		getCellByIndex(ChokeR + i , ChokeC + j).BIM_BN_HP_O;
				
				}  
			}
		}

		/* Below is for testing, using only one grid.
		// for Enemy LIM
		getCell(choke->getCenter()).LIM_Choke_UA_HP_E = 		getCell(choke->getCenter()).BIM_UA_HP_E;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_G_E = 	getCell(choke->getCenter()).BIM_UA_DHPC_G_E;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_A_E = 	getCell(choke->getCenter()).BIM_UA_DHPC_A_E;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_E = 		getCell(choke->getCenter()).BIM_UA_DHPC_E;
		getCell(choke->getCenter()).LIM_Choke_UN_HP_E = 		getCell(choke->getCenter()).BIM_UN_HP_E;

		getCell(choke->getCenter()).LIM_Choke_BA_HP_E = 		getCell(choke->getCenter()).BIM_BA_HP_E;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_G_E = 	getCell(choke->getCenter()).BIM_BA_DHPC_G_E;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_A_E = 	getCell(choke->getCenter()).BIM_BA_DHPC_A_E;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_E = 		getCell(choke->getCenter()).BIM_BA_DHPC_E;
		getCell(choke->getCenter()).LIM_Choke_BN_HP_E = 		getCell(choke->getCenter()).BIM_BN_HP_E;

		// for Own LIM
		getCell(choke->getCenter()).LIM_Choke_UA_HP_O = 		getCell(choke->getCenter()).BIM_UA_HP_O;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_G_O = 	getCell(choke->getCenter()).BIM_UA_DHPC_G_O;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_A_O = 	getCell(choke->getCenter()).BIM_UA_DHPC_A_O;
		getCell(choke->getCenter()).LIM_Choke_UA_DHPC_O = 		getCell(choke->getCenter()).BIM_UA_DHPC_O;
		getCell(choke->getCenter()).LIM_Choke_UN_HP_O = 		getCell(choke->getCenter()).BIM_UN_HP_O;

		getCell(choke->getCenter()).LIM_Choke_BA_HP_O = 		getCell(choke->getCenter()).BIM_BA_HP_O;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_G_O = 	getCell(choke->getCenter()).BIM_BA_DHPC_G_O;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_A_O = 	getCell(choke->getCenter()).BIM_BA_DHPC_A_O;
		getCell(choke->getCenter()).LIM_Choke_BA_DHPC_O = 		getCell(choke->getCenter()).BIM_BA_DHPC_O;
		getCell(choke->getCenter()).LIM_Choke_BN_HP_O = 		getCell(choke->getCenter()).BIM_BN_HP_O;
		*/

	}
		
			//$Sam-E 2014 May 14
/////////////////////////////////////////////////////////////////////////////////////////////////////


}

void MapGrid::GetUnits(UnitVector & units, BWAPI::Position center, int radius, bool ourUnits, bool oppUnits)
{
	const int x0(std::max( (center.x() - radius) / cellSize, 0));
	const int x1(std::min( (center.x() + radius) / cellSize, cols-1));
	const int y0(std::max( (center.y() - radius) / cellSize, 0));
	const int y1(std::min( (center.y() + radius) / cellSize, rows-1));
	const int radiusSq(radius * radius);
	for(int y(y0); y<=y1; ++y)
	{
		for(int x(x0); x<=x1; ++x)
		{
			int row = y;
			int col = x;

			GridCell & cell(getCellByIndex(row,col));
			if(ourUnits)
			{
				BOOST_FOREACH(BWAPI::Unit * unit, cell.ourUnits)
				{
					BWAPI::Position d(unit->getPosition() - center);
					if(d.x() * d.x() + d.y() * d.y() <= radiusSq)
					{
						if (!contains(units, unit)) 
						{
							units.push_back(unit);
						}
					}
				}
			}
			if(oppUnits)
			{
				BOOST_FOREACH(BWAPI::Unit * unit, cell.oppUnits) if (unit->getType() != BWAPI::UnitTypes::Unknown && unit->isVisible())
				{
					BWAPI::Position d(unit->getPosition() - center);
					if(d.x() * d.x() + d.y() * d.y() <= radiusSq)
					{
						if (!contains(units, unit)) 
						{ 
							units.push_back(unit); 
						}
					}
				}
			}
		}
	}
}

bool MapGrid::contains(UnitVector & units, BWAPI::Unit * unit) 
{
	for (size_t i(0); i<units.size(); ++i) 
	{
		if (units[i] == unit) 
		{
			return true;
		}
	}

	return false;
}


//$Sam-S 2014 May 11 - re-design constructor
GridCell::GridCell()
{
	timeLastVisited = 0;
	timeLastOpponentSeen = 0;

	//LIM:
	BIM_Mineral_Amount = 0;
	BIM_Gas_Amount = 0;

	BIM_Unit_Grid_1st_Time_Flag_E = true;
	BIM_UA_HP_E = 0;
	BIM_UA_DHPC_E = 0;

	BIM_UA_DHPC_G_E = 0;
	BIM_UA_DHPC_A_E = 0;

	BIM_UN_HP_E = 0;

	BIM_Unit_Grid_1st_Time_Flag_O = true;
	BIM_UA_HP_O = 0;
	BIM_UA_DHPC_O = 0;

	BIM_UA_DHPC_G_O = 0;
	BIM_UA_DHPC_A_O = 0;

	BIM_UN_HP_O = 0;

	BIM_Building_Grid_1st_Time_Flag_E = true;
	BIM_BA_HP_E = 0;
	BIM_BA_DHPC_E = 0;

	BIM_BA_DHPC_G_E = 0;
	BIM_BA_DHPC_A_E = 0;

	BIM_BN_HP_E = 0;

	BIM_Building_Grid_1st_Time_Flag_O = true;
	BIM_BA_HP_O = 0;
	BIM_BA_DHPC_O = 0;

	BIM_BA_DHPC_G_O = 0;
	BIM_BA_DHPC_A_O = 0;

	BIM_BN_HP_O = 0;

//	BIM_Choke_Position
	BIM_Choke_Width = 0;
	BIM_Choke_Range = 0;

	BIM_Choke_Grid_1st_Time_Flag_E = true;
	LIM_Choke_UA_HP_E = 0;
	LIM_Choke_UA_DHPC_G_E = 0;
	LIM_Choke_UA_DHPC_A_E = 0;
	LIM_Choke_UA_DHPC_E = 0;
	LIM_Choke_UN_HP_E = 0;
	LIM_Choke_BA_HP_E = 0;
	LIM_Choke_BA_DHPC_G_E = 0;
	LIM_Choke_BA_DHPC_A_E = 0;
	LIM_Choke_BA_DHPC_E = 0;
	LIM_Choke_BN_HP_E = 0;

	BIM_Choke_Grid_1st_Time_Flag_O = true;
	LIM_Choke_UA_HP_O = 0;
	LIM_Choke_UA_DHPC_G_O = 0;
	LIM_Choke_UA_DHPC_A_O = 0;
	LIM_Choke_UA_DHPC_O = 0;
	LIM_Choke_UN_HP_O = 0;
	LIM_Choke_BA_HP_O = 0;
	LIM_Choke_BA_DHPC_G_O = 0;
	LIM_Choke_BA_DHPC_A_O = 0;
	LIM_Choke_BA_DHPC_O = 0;
	LIM_Choke_BN_HP_O = 0;

	}
//$Sam-E 2014 May 11 - re-design constructor