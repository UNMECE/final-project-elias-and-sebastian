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

    // Manually initialize totals
    double totalWater = 0.0;
    double totalNeed = 0.0;
    double northStart = 0.0;
    double southStart = 0.0;
    double eastStart = 0.0;

    // Count how much water we have and what each region starts with
    for (int i = 0; i < regions.size(); i++) {
        totalWater += regions[i]->waterLevel;
        totalNeed += regions[i]->waterNeed;
        if (regions[i]->name == "North") northStart = regions[i]->waterLevel;
        if (regions[i]->name == "South") southStart = regions[i]->waterLevel;
        if (regions[i]->name == "East")  eastStart  = regions[i]->waterLevel;
    }

    // Begin simulation loop until time runs out or everything is solved
    while (!manager.isSolved && manager.hour != manager.SimulationMax)
    {
        std::vector<Region*> targets;

        // Collect regions that still need water (and are not locked)
        for (int i = 0; i < regions.size(); i++) {
            if (regions[i]->waterLevel < regions[i]->waterNeed) {
                bool alreadyLocked = false;
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

        // Sort target regions by smallest gap (closest to their water need)
        for (int i = 0; i < targets.size(); i++) {
            int best = i;
            for (int j = i + 1; j < targets.size(); j++) {
                double gap1 = targets[j]->waterNeed - targets[j]->waterLevel;
                double gap2 = targets[best]->waterNeed - targets[best]->waterLevel;
                if (gap1 < gap2) best = j;
            }
            if (best != i) {
                Region* temp = targets[i];
                targets[i] = targets[best];
                targets[best] = temp;
            }
        }

        // Add regions to ready list when they meet need
        // These will be locked once at least 2 regions are ready
        for (int i = 0; i < regions.size(); i++) {
            Region* region = regions[i];
            if (region->waterLevel >= region->waterNeed) {
                bool isAlreadyReady = false;
                bool isAlreadyLocked = false;

                for (int j = 0; j < readyToLock.size(); j++) {
                    if (readyToLock[j] == region->name) isAlreadyReady = true;
                }
                for (int j = 0; j < locked.size(); j++) {
                    if (locked[j] == region->name) isAlreadyLocked = true;
                }

                if (!isAlreadyReady && !isAlreadyLocked)
                    readyToLock.push_back(region->name);
            }
        }

        // If two or more are ready, lock them to prevent water level from dropping
        if (readyToLock.size() >= 2) {
            for (int i = 0; i < readyToLock.size(); i++) {
                bool alreadyLocked = false;
                for (int j = 0; j < locked.size(); j++) {
                    if (locked[j] == readyToLock[i]) alreadyLocked = true;
                }
                if (!alreadyLocked) locked.push_back(readyToLock[i]);
            }
        }

        // Turn off all canals to start clean each hour
        for (int i = 0; i < canals.size(); i++) {
            canals[i]->toggleOpen(false);
        }

        // Let surplus regions flow to top 2 closest-to-goal regions
        for (int i = 0; i < canals.size(); i++) {
            Region* sourceRegion = canals[i]->sourceRegion;
            Region* destinationRegion = canals[i]->destinationRegion;

            // Check if either side is locked
            bool sourceLocked = false;
            bool destinationLocked = false;
            for (int j = 0; j < locked.size(); j++) {
                if (sourceRegion->name == locked[j]) sourceLocked = true;
                if (destinationRegion->name == locked[j]) destinationLocked = true;
            }

// Check if this destination is one of the top 2 regions closest to reaching their water need
            bool destinationTopTarget = false;
            for (int j = 0; j < targets.size() && j < 2; j++) {
                if (destinationRegion == targets[j]) {
                    destinationTopTarget = true;
                    break;
                }
            }

            // Flow water if source is safe and destination needs it
            if (!sourceLocked && !destinationLocked && destinationTopTarget)
            {
                double surplus = sourceRegion->waterLevel - sourceRegion->waterNeed;
                if (surplus >= 0.5)
                {
                    double gap = destinationRegion->waterNeed - destinationRegion->waterLevel;
                    double flow = (gap >= 1.0) ? 1.0 : gap;
                    if (surplus < flow) flow = surplus;

                    canals[i]->setFlowRate(flow);
                    canals[i]->toggleOpen(true);
                }
            }
        }

        // Bug check:
        // It printed each hour’s water levels for all regions.
        /*
        std::cout << "HOUR " << manager.hour << ": ";
        for (Region* region : regions)
            std::cout << region->name << " = " << region->waterLevel << ", ";
        std::cout << std::endl;
        */

        manager.nexthour(); // Advance to next hour
    }
}
