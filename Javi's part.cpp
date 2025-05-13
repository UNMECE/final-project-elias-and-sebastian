#include "acequia_manager.h"
#include <iostream>
#include <vector>
#include <string>

void solveProblems(AcequiaManager& manager)
{
	std::vector<Canal*> canals = manager.getCanals();
	std::vector<Region*> regions = manager.getRegions();

	std::vector<std::string> locked;
	std::vector<std::string> readyToLock;

	bool printedStallNotice = false;

	// Begin simulation loop until time runs out or everything is solved
	while (!manager.isSolved && manager.hour != manager.SimulationMax) {
		std::vector<Region*> targets;

		// Loop to find regions that still need water and are not locked
		for (int i = 0; i < regions.size(); i++) {
			if (regions[i]->waterLevel < regions[i]->waterNeed) {
				bool alreadyLocked = false;

				// Check if the region is already locked
				for (int j = 0; j < locked.size(); j++) {
					if (locked[j] == regions[i]->name) {
						alreadyLocked = true;
						break;
					}
				}

				if (!alreadyLocked) {
					targets.push_back(regions[i]);
				}
			}
		}

		// Loop to sort target regions by smallest gap to water need
		for (int i = 0; i < targets.size(); i++) {
			int best = i;

			// Find the region with the smallest water gap
			for (int j = i + 1; j < targets.size(); j++) {
				double gap1 = targets[j]->waterNeed - targets[j]->waterLevel;
				double gap2 = targets[best]->waterNeed - targets[best]->waterLevel;

				if (gap1 < gap2) {
					best = j;
				}
			}

			if (best != i) {
				Region* temp = targets[i];
				targets[i] = targets[best];
				targets[best] = temp;
			}
		}

		// Loop to mark which regions are ready to lock
		for (int i = 0; i < regions.size(); i++) {
			Region* region = regions[i];

			if (region->waterLevel >= region->waterNeed) {
				bool isAlreadyReady = false;
				bool isAlreadyLocked = false;

				// Check if region is already in readyToLock list
				for (int j = 0; j < readyToLock.size(); j++) {
					if (readyToLock[j] == region->name) {
						isAlreadyReady = true;
					}
				}

				// Check if region is already locked
				for (int j = 0; j < locked.size(); j++) {
					if (locked[j] == region->name) {
						isAlreadyLocked = true;
					}
				}

				if (!isAlreadyReady && !isAlreadyLocked) {
					readyToLock.push_back(region->name);
				}
			}
		}

		// Loop to lock regions once at least 2 are ready
		if (readyToLock.size() >= 2) {
			for (int i = 0; i < readyToLock.size(); i++) {
				bool alreadyLocked = false;

				for (int j = 0; j < locked.size(); j++) {
					if (locked[j] == readyToLock[i]) {
						alreadyLocked = true;
					}
				}

				if (!alreadyLocked) {
					locked.push_back(readyToLock[i]);
				}
			}
		}

		// Loop to turn off all canals at start of the hour
		for (int i = 0; i < canals.size(); i++) {
			canals[i]->toggleOpen(false);
		}

		bool anyCanalOpened = false;

		// Loop to try and move water from surplus regions to top 2 targets
		for (int i = 0; i < canals.size(); i++) {
			Region* sourceRegion = canals[i]->sourceRegion;
			Region* destinationRegion = canals[i]->destinationRegion;

			bool sourceLocked = false;
			bool destinationLocked = false;

			// Check if either end of canal is locked
			for (int j = 0; j < locked.size(); j++) {
				if (sourceRegion->name == locked[j]) sourceLocked = true;
				if (destinationRegion->name == locked[j]) destinationLocked = true;
			}

			// Check if destination is a top priority
			bool destinationTopTarget = false;

			for (int j = 0; j < targets.size() && j < 2; j++) {
				if (destinationRegion == targets[j]) {
					destinationTopTarget = true;
					break;
				}
			}

			// Flow water if safe and destination is valid
			if (!sourceLocked && !destinationLocked && destinationTopTarget) {
				double surplus = sourceRegion->waterLevel - sourceRegion->waterNeed;

				if (surplus >= 0.5) {
					double gap = destinationRegion->waterNeed - destinationRegion->waterLevel;

					double flow = 0.0;
					if (gap >= 1.0) {
						flow = 1.0;
					} else {
						flow = gap;
					}

					if (surplus < flow) {
						flow = surplus;
					}

					// Prevent donating if it drops below source need
					if (sourceRegion->waterLevel - flow >= sourceRegion->waterNeed) {
						canals[i]->setFlowRate(flow);
						canals[i]->toggleOpen(true);
						anyCanalOpened = true;
					}
				}
			}
		}

		// Print explanation if stalled — Bonus comment added for assignment
		if (!anyCanalOpened && !printedStallNotice) {
			std::cout << "// SIMULATION STALLED: CANNOT MOVE WATER WITHOUT RISKING DROUGHT OR LOSING POINTS.\n";
			printedStallNotice = true;
		}

		/*
		// Debug print water levels every hour
		std::cout << "HOUR " << manager.hour << ": ";
		for (Region* region : regions){
			std::cout << region->name << " = " << region->waterLevel << ", ";
		}
		std::cout << std::endl;
		*/

		manager.nexthour(); // Move to the next hour
	}
}


















/*
double totalWater = 0;
double totalNeed  = 0;

for (int i = 0; i < regions.size(); i++) { // Calculates total water and total required water
    totalWater += regions[i]->waterLevel;
    totalNeed  += regions[i]->waterNeed;
}

// --- SCENARIO 1: FULL COVERAGE POSSIBLE (Perfect Condition) ---
if (totalWater >= totalNeed) {


} 
// --- SCENARIO 2: FLOOD RISK (Too Much Water) ---
else if (totalWater >= totalNeed * 1.3) { 

} 
// --- SCENARIO 3: DROUGHT MODE (Your Section) ---
else {

}

*/