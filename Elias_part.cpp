#include "acequia_manager.h"
#include <iostream>
#include <vector>
/*Instructions for this problem:

	The intend of this project is to simulate water management conservation principles in the context of New Mexico

	In this simulation, there exists several Regions (North, South, etc.). Each region class includes both:
	- a given water level
	- a given water need
	- a indicator boolean for if the region is flooded
	- an indicator boolean for if the region is in drought

	With each region, there are given waterSources provided (perhaps a .dat file list each waterSource to  a region) 
	and certain waterSources have canals connected to them to deliver water across regions.

	Given the current state of the region, students wil be asked to utlize the canals that connect regions to
	develop the logic and algorithm for finding a solution. The simulation has a fixed time



	The student solution will be evaluated on the criteria that each region meets the following:
	- a given region is NOT flooded
	- a given region is NOT in drought
	- the region waterNeed does not exceed the region waterLevel 
*/

/*This will be how the solveProblems function is set up. The student may enter their on  */
/*
void solveProblems(AcequiaManager& manager)
{
	//the student can call the members of the canals object such as name of canal. sourceRegion, and destinationRegion
	//This could be helpful in informing the students strategy to solve the problem
	auto canals = manager.getCanals();
	//students may call to get Region and WaterSource objects to inform decisions 


	while(!manager.isSolved && manager.hour!=manager.SimulationMax)
	{	
		//enter student code here


		manager.nexthour();
	}
}
*/


/*example 2 format*/
Canal* getCanal(Region* source, Region* destination, AcequiaManager &manager){
	auto canals = manager.getCanals();
	for(auto canal : canals){
		if( canal->sourceRegion == source && canal->destinationRegion == destination){
			return canal;
		}
	}	
	return nullptr;
}

void solveProblems(AcequiaManager& manager)
{
	auto canals = manager.getCanals();
	//creating named references for each canal since there are only 4
	//and referencing them with canal[0/1/2/3] gets confusing
	Canal* NtS = canals[0]; 
	Canal* StE = canals[1];
	Canal* NtE = canals[2];
	Canal* EtN = canals[3];
	//also creating named references for each region
	//for the same reason as canals
	Region* North = canals[0]->sourceRegion;
	Region* South = canals[1]->sourceRegion;
	Region* East = canals[2]->destinationRegion;

	//begin by analyzing water data
	//these variables will help decide how we want to solve the problem

    auto resetCanals = [&]() {
        for (auto canal : canals) {
            canal->setFlowRate(0);
            canal->toggleOpen(false);
        }
    };
	
	double totalNeed = North->waterNeed + South->waterNeed + East->waterNeed;
	double totalWater = North->waterLevel + South->waterLevel + East->waterLevel;
	double totalCapacity = North->waterCapacity + South->waterCapacity + East->waterCapacity;

	//Checks if each reagion is flooded or droughted
	
	while (!manager.isSolved && manager.hour != manager.SimulationMax)
	{
		std::cerr << "[DEBUG] ===== Hour " << manager.hour << " =====" << std::endl;
		//Students will implement this function to solve the probelms
		//Example: Adjust canal flow rates and directions
		resetCanals();
		    
		if (totalWater > totalCapacity)
		{
			//there is too much available water and at least 1 region will be flooded
			//in this scenario, we have decided to sacrifice 1 region for the greater good
			//might be a PR nightmare, but surely the excuse of "The engineering students
			//needed as many points as possible" will resonate well with the people

			//region 2 (East) because it has 2 canals going in, and 1 going out

			//water need will be checked afterwards. If there is not enough, water will be pulled in
			//from the east region. Since there is an excess, we are guaranteed to be able to meet the needs

			if (North->waterNeed > North->waterLevel && South->waterNeed > South->waterLevel)
			{
				//both regions need water. Adjust flow rate so they fill at different speeds.
			}
			if (North->waterNeed > North->waterLevel && South->waterNeed <= South->waterLevel)
			{
				//close canal to south, and only flow into north
			}
			if (North->waterNeed <= North->waterLevel && South->waterNeed > South->waterLevel)
			{
				//open both canals at the same rate (1 is probably best). This will have a net change of 0 on the north region
			}
			if (North->waterNeed <= North->waterLevel && South->waterNeed <= South->waterLevel)
			{
				//both regions have enough water and the simulation can end.
			}

		}

		else if (totalWater < totalCapacity * 0.2)
		{
			//there is not enough water available and at least 1 region will be in drought
			//like before, 1 region will be sacrificed. sacrificed region should be 

			//the region sacrificed should be the one that needs the most water to not be droughted, 
			//so that we are more likely to be able to meet the needs of 2 regions instead of only 1

			//water need of undroughted regions will be checked afterwards
			//ideally, we can meet the needs of those regions since the other region gets no water.
			if (North->waterCapacity > South->waterCapacity && North->waterCapacity > East->waterCapacity)
			{
				//north requires the most water and is the best region to dry out
			}
			else if (South->waterCapacity > North->waterCapacity && South->waterCapacity > East->waterCapacity)
			{

			}

		}
   
		else
		{
			//there is enough water to ensure that no region is droughted or flooded
			//however, we still need to check if we can meet the needs of all regions
			
				/**/
				//Check for droughts and floods, send highest over max to droughts, farthest under need takes from floods
				//sort by largest surplus?? what if there is none??
				struct R { Region* ptr; double gap; };
  				std::vector<R> deficits, surpluses;
  				for (auto* r : manager.getRegions()) {
    			double gap = r->waterNeed - r->waterLevel;
    			if (gap > 0){                
					std::cerr << "[DEBUG] " << r->name << " deficit=" << gap << std::endl;
					deficits.push_back({r, gap});}
    			else {
					std::cerr << "[DEBUG] " << r->name << " surplus=" << gap << std::endl;
					surpluses.push_back({r, gap});}
  				}
				std::cerr << "[DEBUG] Total deficits=" << deficits.size() << ", surpluses=" << surpluses.size() << std::endl;

				for (auto& d : deficits) {
    				for (auto& s : surpluses) {
      					auto* canal = getCanal(s.ptr, d.ptr, manager);
      					if (!canal){
							std::cerr << "[DEBUG] No direct canal from " << s.ptr->name << " to " << d.ptr->name << ". Using fallback." << std::endl;
							if(s.ptr->name == "South"){
								std::cerr << "[DEBUG] Fallback: south drains via StE and EtN." << std::endl;
								if(s.gap < -3.6){
								StE->setFlowRate(1.0);
								EtN->setFlowRate(1.0);
								StE->toggleOpen(true);
								EtN->toggleOpen(true);
								}else if(s.gap < -0.36){
									EtN->setFlowRate(0.1);
									NtS->setFlowRate(0.1);	
									StE->toggleOpen(true);
									EtN->toggleOpen(true);
								}else{
									StE->toggleOpen(false);
									EtN->toggleOpen(false);
								}
								
							}else{
								std::cerr << "[DEBUG] Fallback: " << s.ptr->name << " drains via EtN and NtS." << std::endl;
								if(s.gap < -3.6){
								EtN->setFlowRate(1.0);
								NtS->setFlowRate(1.0);
								EtN->toggleOpen(true);
								NtS->toggleOpen(true);

								}else if(s.gap < -0.36){
									EtN->setFlowRate(0.1);
									NtS->setFlowRate(0.1);	
									EtN->toggleOpen(true);
									NtS->toggleOpen(true);
								}else{
								EtN->toggleOpen(false);
								NtS->toggleOpen(false);

								}

							}
							continue;
	  					}
						std::cerr << "[DEBUG] Direct canal from " << s.ptr->name << " to " << d.ptr->name << " opened at rate=1.0." << std::endl;
						if(s.gap < -3.6){
						canal->setFlowRate(1.0);
						canal->toggleOpen(true); 
						}else if(s.gap < -0.36){
						canal->setFlowRate(0.1);
						canal->toggleOpen(true);
						}else{
						canal->toggleOpen(false);
						}
					}
				}
        		/* std::cout << "[Logic] Perfect State - Distribute Minor Gaps\n";
        		if (isSurplus(North)) {
            		if (S_gap > 0) {
                		NtS->setFlowRate(1.0);
                		NtS->toggleOpen(true);
            		}
            		if (E_gap > 0) {
                		NtE->setFlowRate(1.0);
                		NtE->toggleOpen(true);
            		}
        		}

        		if (isSurplus(South)) {
            		if (E_gap > 0) {
                		StE->setFlowRate(1.0);
                		StE->toggleOpen(true);
            		}
            		if (N_gap > 0) {
                		EtN->setFlowRate(0.5);
                		EtN->toggleOpen(true);
            		}
        		}

        		if (isSurplus(East)) {
            		if (N_gap > 0) {
                		EtN->setFlowRate(1.0);
                		EtN->toggleOpen(true);
            		}
            		if (S_gap > 0) {
                		EtN->setFlowRate(1.0);
						NtS->setFlowRate(1.0);
						EtN->toggleOpen(true);
						NtS->toggleOpen(true);
            		}
        		}
		//This case should never happen
        		if (!isSurplus(North) && !isSurplus(South) && !isSurplus(East)) {
            		std::cout << "[Logic] No Surplus, some Deficits\n";
			
            		if (N_gap > 0 && S_gap < 0) {
                		NtS->setFlowRate(1.0);
                		NtS->toggleOpen(true);
            		}
            		if (E_gap > 0 && S_gap < 0) {
                		StE->setFlowRate(1.0);
                		StE->toggleOpen(true);
            		}
					if (S_gap > 0 && N_gap < 0) {
                		StE->setFlowRate(1.0);
                		StE->toggleOpen(true);
            		}
					if (E_gap > 0 && S_gap < 0) {
                		StE->setFlowRate(1.0);
                		StE->toggleOpen(true);
            		}
        		}
    
			}
			else
			{
    			std::cout << "[Logic] Not Enough Total Water\n";
    
        		std::cout << "[Logic] Distribute in Shortage Mode\n";
        		if (isSurplus(North)) {
            		if (S_gap > 0) {
                		NtS->setFlowRate(1.0);
                		NtS->toggleOpen(true);
            		}
            		if (E_gap > 0) {
                		NtE->setFlowRate(1.0);
                		NtE->toggleOpen(true);
            		}
        		}

        		if (isSurplus(South)) {
            		if (E_gap > 0) {
                		StE->setFlowRate(1.0);
                		StE->toggleOpen(true);
            		}
            		if (N_gap > 0) {
                		EtN->setFlowRate(0.5);
                		EtN->toggleOpen(true);
            		}
        		}

        		if (isSurplus(East)) {
            		if (N_gap > 0) {
                		EtN->setFlowRate(1.0);
                		EtN->toggleOpen(true);
            		}
        		}

        		if (!isSurplus(North) && !isSurplus(South) && !isSurplus(East)) {
            		std::cout << "[Logic] Emergency Mode - Pulling From Deficits\n";
            		if (N_gap > 0 && S_gap < 0) {
                		NtS->setFlowRate(1.0);
               	 		NtS->toggleOpen(true);
            		}
            		if (E_gap > 0 && S_gap < 0) {
                		StE->setFlowRate(1.0);
                		StE->toggleOpen(true); 
            		}
        		}*/
    		
		}
		//student may add any necessary functions or check on the progress of each region as the simulation moves forward. 
		//The manager takes care of updating the waterLevels of each region and waterSource while the student is just expected
		//to solve how to address the state of each region
		manager.nexthour();
	}	
}


/*example 2*/
/*
*/
