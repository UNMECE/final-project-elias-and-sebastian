#include "acequia_manager.h"
#include <iostream>

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

//void solveProblems(AcequiaManager& manager)
//{
//	auto canals = manager.getCanals();
//	while(!manager.isSolved && manager.hour!=manager.SimulationMax)
//	{
//	//Students will implement this function to solve the probelms
//	//Example: Adjust canal flow rates and directions
//		if(manager.hour==0)
//		{
//			canals[0]->setFlowRate(1);
//			canals[0]->toggleOpen(true);
//		}
//		else if(manager.hour==1)
//		{
//			canals[1]->setFlowRate(0.5);
//			canals[1]->toggleOpen(true);
//		}
//		else if(manager.hour==82)
//		{
//			canals[0]->toggleOpen(false);
//			canals[1]->toggleOpen(false);
//		}
//	//student may add any necessary functions or check on the progress of each region as the simulation moves forward. 
//	//The manager takes care of updating the waterLevels of each region and waterSource while the student is just expected
//	//to solve how to address the state of each region
//
//		
//		manager.nexthour();
//	}
//}

void solveProblems(AcequiaManager& manager)
{
	auto canals = manager.getCanals();
	std::vector<Region*> regions = manager.getRegions();
	//creating named references for each canal since there are only 4
	//and referencing them with canal[0/1/2/3] gets confusing
	Canal* NtS = canals[0]; 
	Canal* StE = canals[1];
	Canal* NtE = canals[2];
	Canal* EtN = canals[3];
	/*std::cout << "NtS: " << NtS->sourceRegion->name << " to " << NtS->destinationRegion->name << std::endl;
	std::cout << "StE: " << StE->sourceRegion->name << " to " << StE->destinationRegion->name << std::endl;
	std::cout << "NtE: " << NtE->sourceRegion->name << " to " << NtE->destinationRegion->name << std::endl;
	std::cout << "EtN: " << EtN->sourceRegion->name << " to " << EtN->destinationRegion->name << std::endl;*/
	//also creating named references for each region
	//for the same reason as canals
	Region* North = canals[0]->sourceRegion;
	Region* South = canals[1]->sourceRegion;
	Region* East = canals[2]->destinationRegion;
	/*std::cout << "North: " << North->name << std::endl;
	std::cout << "South: " << South->name << std::endl;
	std::cout << "East: " << East->name << std::endl;*/

	//begin by analyzing water data
	//these variables will help decide how we want to solve the problem
	double totalNeed = North->waterNeed + South->waterNeed + East->waterNeed;
	double totalWater = North->waterLevel + South->waterLevel + East->waterLevel;
	double totalCapacity = North->waterCapacity + South->waterCapacity + East->waterCapacity;

	std::vector<std::string> locked;
    std::vector<std::string> readyToLock;
	double northStart = 0.0;
    double southStart = 0.0;
    double eastStart = 0.0;

	// Count how much water we have and what each region starts with
    for (int i = 0; i < regions.size(); i++) {
        if (regions[i]->name == "North") northStart = regions[i]->waterLevel;
        if (regions[i]->name == "South") southStart = regions[i]->waterLevel;
        if (regions[i]->name == "East")  eastStart  = regions[i]->waterLevel;
    }
	
	while (!manager.isSolved && manager.hour != manager.SimulationMax)
	{
		//Students will implement this function to solve the probelms
		//Example: Adjust canal flow rates and directions
		
		//recalculate total water in case of flood
		//double totalWater = North->waterLevel + South->waterLevel + East->waterLevel;

			std::cout << "HOUR " << manager.hour << ": ";
			for (Region* region : regions)
				std::cout << region->name << " = " << region->waterLevel << ", ";
			std::cout << std::endl;

		// int closedCanals = 0;
		// for (int i = 0; i < canals.size(); ++i)
		// {
		// 	//Sebastian's attempt at a general solution
		// 	//always flow water if the source region has more than its need
		// 	//but only flow in ONE direction (never use NtE)
		// 	Region * sourceRegion = canals[i]->sourceRegion;
		// 	Region * destinationRegion = canals[i]->destinationRegion;
		// 	if (sourceRegion->waterLevel > sourceRegion->waterNeed and canals[i] != NtE)
		// 	{
		// 		//flow rate may need to be adjusted to ensure that more than the excess is not given away
		// 		double excess = sourceRegion->waterLevel - sourceRegion->waterNeed;
		// 		//flow rate of 1 gives out 3.6 gallons of water per iteration
		// 		//so if there is less than 3.6, we don't want to give away more than what exists;
		// 		if(excess < 3.6)
		// 			canals[i]->setFlowRate((3.6-excess)/3.6);
		// 		else
		// 			canals[i]->setFlowRate(1);
		// 		canals[i]->toggleOpen(true);
		// 	}
		// 	else
		// 	{
		// 		//ensure that anything below its need is never giving away water.
		// 		canals[i]->toggleOpen(false);
		// 		closedCanals += 1;
		// 	}
		// 	if (closedCanals == 4)
		// 	{
		// 		std::cout << "There is a severe drought and thus not enough water for every region. Ending simulation" << std::endl;
		// 		while( manager.hour != manager.SimulationMax - 1)
		// 		{
		// 			manager.nexthour();
		// 		}
		// 	}
		// }

		if (totalWater >= totalCapacity)
		{
			//there is too much available water and at least 1 region will be flooded
			//in this scenario, we have decided to sacrifice 1 region for the greater good
			//might be a PR nightmare, but surely the excuse of "The engineering students
			//needed as many points as possible" will resonate well with the people

			//analysis of the random water generation has determined that the maximum starting water level
			// so this function only gets called if all 3 regions start off flooded
			//if only 1 region is flooded, then the other 2 are above water need but not flooded and the regions are solved as best as possible
			//make sure flow rates are above 0
			NtE->setFlowRate(1);
			NtS->setFlowRate(1);
			StE->setFlowRate(1);
			EtN->setFlowRate(1);

			if (North->isFlooded && !South->isFlooded && !East->isFlooded
			
			)
			{
				std::cout << manager.hour << ": North is flooded. Flooding from East" << std::endl;

				//I am going to try keeping the NtE gate open to see if overflowing the water allows me to get more points
				NtS->toggleOpen(false);
				NtE->toggleOpen(false);
				StE->toggleOpen(false);
				EtN->toggleOpen(true);
				//return;
			}
			else if(!North->isFlooded && South->isFlooded && !East->isFlooded)
			{
				std::cout << manager.hour << ": South is flooded. Flooding from North" << std::endl;
				NtS->toggleOpen(true);
				NtE->toggleOpen(false);
				StE->toggleOpen(false);
				EtN->toggleOpen(false);
			}
			else if(!North->isFlooded && !South->isFlooded && East->isFlooded)
			{
				std::cout << manager.hour << ": East is flooded. Flooding from South" << std::endl;
				NtS->toggleOpen(false);
				NtE->toggleOpen(false);
				StE->toggleOpen(true);
				EtN->toggleOpen(false);
			}

		//if 3 regions are flooded, east will be the easiest to sacrifice since there are 2 canals going in
		//If the group's understanding of the simulation is correct, this is theoretically the only statement that should get called
		//other than the 1 above. but it is good to have code for other scenarios just in case
			else if (North->isFlooded && East->isFlooded && South->isFlooded)
			{
				std::cout << manager.hour << ": All regions are flooded. Opening canals to east region" << std::endl;
				//open canals to release water from north and south
				NtE->toggleOpen(true);
				StE->toggleOpen(true);
				//close other canals
				EtN->toggleOpen(false);
				NtS->toggleOpen(false);
				//return;
			}
			else
			{
				//if 2 regions are flooded, pick one of the 2 to flood based on which 2 are flooded
				//2 regions being flooded likely means something went wrong in another step
				//so we should be sure that the 3rd region has its need met
				if (North->isFlooded && South->isFlooded)
				{
					std::cout << manager.hour << ": North and South are flooded. Flooding South" << std::endl;
					//North has a canal going to south, so flooding south is easier in this case
					NtS->toggleOpen(true);
					//make sure east's need is met
					if (East->waterLevel < East->waterNeed)
					{
						StE->toggleOpen(true);
					}
					else
					{
						StE->toggleOpen(false);
					}
					//ensure all other canals are closed
					NtE->toggleOpen(false);
					EtN->toggleOpen(false);
					//return;
				}
				if (North->isFlooded && East->isFlooded)
				{
					std::cout << manager.hour << ": North and East are flooded. Flooding North" << std::endl;
					//It is better to flood North in this scenario because North also has a canal going to south
					//so if south somehow doesn't have enough water, it can be directed to south
					EtN->toggleOpen(true);
					if (South->waterLevel < South->waterNeed)
					{
						NtS->toggleOpen(true);
					}
					else
					{
						NtS->toggleOpen(false);
					}
					//ensure other canals are closed
					NtE->toggleOpen(false);
					StE->toggleOpen(false);
				}
				if (South->isFlooded && East->isFlooded)
				{
					std::cout << manager.hour << ": South and East are flooded. Flooding East" << std::endl;
					//better to flood east since water can be redirected to north if needed
					StE->toggleOpen(true);
					if (North->waterLevel < North->waterNeed)
					{
						EtN->toggleOpen(true);
					}
					else
					{
						EtN->toggleOpen(false);
					}
					//close other canals
					NtE->toggleOpen(false);
					NtS->toggleOpen(false);
				}
			}

		}

		// else if (totalWater <= totalCapacity * 0.2)
		// {
		// 	// Bug check:
		// 	// It printed each hour’s water levels for all regions.
			
		// 	std::cout << "HOUR " << manager.hour << ": ";
		// 	for (Region* region : regions)
		// 		std::cout << region->name << " = " << region->waterLevel << ", ";
		// 	std::cout << std::endl;

		// 	//there is not enough water available and at least 1 region will be in drought
		// 	//like before, 1 region will be sacrificed. sacrificed region should be 

		// 	//the region sacrificed should be the one that needs the most water to not be droughted, 
		// 	//so that we are more likely to be able to meet the needs of 2 regions instead of only 1

		// 	//water need of undroughted regions will be checked afterwards
		// 	//ideally, we can meet the needs of those regions since the other region gets no water.
			

		// 	std::vector<Region*> targets;

		// 	// Collect regions that still need water (and are not locked)
		// 	for (int i = 0; i < regions.size(); i++) {
		// 		if (regions[i]->waterLevel < regions[i]->waterNeed) {
		// 			bool alreadyLocked = false;
		// 			for (int j = 0; j < locked.size(); j++) {
		// 				if (locked[j] == regions[i]->name) {
		// 					alreadyLocked = true;
		// 					break;
		// 				}
		// 			}
		// 			if (!alreadyLocked) {
		// 				targets.push_back(regions[i]);
		// 			}
		// 		}
		// 	}

		// 	// Sort target regions by smallest gap (closest to their water need)
		// 	for (int i = 0; i < targets.size(); i++) {
		// 		int best = i;
		// 		for (int j = i + 1; j < targets.size(); j++) {
		// 			double gap1 = targets[j]->waterNeed - targets[j]->waterLevel;
		// 			double gap2 = targets[best]->waterNeed - targets[best]->waterLevel;
		// 			if (gap1 < gap2) best = j;
		// 		}
		// 		if (best != i) {
		// 			Region* temp = targets[i];
		// 			targets[i] = targets[best];
		// 			targets[best] = temp;
		// 		}
		// 		//std::cout << "Targets[" << i << "]: " << targets[i]->name << std::endl;
		// 	}

		// 	// Add regions to ready list when they meet need
		// 	// These will be locked once at least 2 regions are ready
		// 	for (int i = 0; i < regions.size(); i++) {
		// 		Region* region = regions[i];
		// 		if (region->waterLevel >= region->waterNeed) {
		// 			bool isAlreadyReady = false;
		// 			bool isAlreadyLocked = false;

		// 			for (int j = 0; j < readyToLock.size(); j++) {
		// 				if (readyToLock[j] == region->name) isAlreadyReady = true;
		// 			}
		// 			for (int j = 0; j < locked.size(); j++) {
		// 				if (locked[j] == region->name) isAlreadyLocked = true;
		// 			}

		// 			if (!isAlreadyReady && !isAlreadyLocked)
		// 				readyToLock.push_back(region->name);
		// 		}
		// 	}

		// 	// If two or more are ready, lock them to prevent water level from dropping
		// 	if (readyToLock.size() >= 2) {
		// 		for (int i = 0; i < readyToLock.size(); i++) {
		// 			bool alreadyLocked = false;
		// 			for (int j = 0; j < locked.size(); j++) {
		// 				if (locked[j] == readyToLock[i]) alreadyLocked = true;
		// 			}
		// 			if (!alreadyLocked) locked.push_back(readyToLock[i]);
		// 		}
		// 	}

		// 	// Turn off all canals to start clean each hour
		// 	for (int i = 0; i < canals.size(); i++) {
		// 		canals[i]->toggleOpen(false);
		// 	}

		// 	// Let surplus regions flow to top 2 closest-to-goal regions
		// 	for (int i = 0; i < canals.size(); i++) {
		// 		Region* sourceRegion = canals[i]->sourceRegion;
		// 		Region* destinationRegion = canals[i]->destinationRegion;

		// 		// Check if either side is locked
		// 		bool sourceLocked = false;
		// 		bool destinationLocked = false;
		// 		for (int j = 0; j < locked.size(); j++) {
		// 			if (sourceRegion->name == locked[j])
		// 			{
		// 				 sourceLocked = true; 
		// 				 //std::cout << sourceRegion->name << " is locked!" << std::endl;
		// 			}
		// 				 if (destinationRegion->name == locked[j]) 
		// 				 {
		// 					destinationLocked = true; 
		// 					//std::cout << destinationRegion->name << " is locked!" << std::endl;
		// 				 }
		// 		}


		// 		// Check if this destination is one of the top 2 regions closest to reaching their water need
		// 		bool destinationTopTarget = false;
		// 		for (int j = 0; j < targets.size() && j < 2; j++) {
					
		// 			if (destinationRegion == targets[j]) {
		// 				destinationTopTarget = true;
		// 				//std::cout << destinationRegion->name << " is a target region!" << std::endl;
		// 				break;
		// 			}
		// 		}
		// 		if (sourceLocked)
		// 		std::cout << "Cannot flow from " << sourceRegion->name << " to " << destinationRegion->name
		// 		<< " because " << sourceRegion->name << " is locked." << std::endl;
		// 		if (destinationLocked)
		// 		std::cout << "Cannot flow from " << sourceRegion->name << " to " << destinationRegion->name
		// 		<< " because " << destinationRegion->name << " is locked." << std::endl;
		// 		/*if (!destinationTopTarget)
		// 		std::cout << "Cannot flow from " << sourceRegion->name << " to " << destinationRegion->name
		// 		<< " because " << destinationRegion->name << " is not a target" << std::endl;*/

		// 		// Flow water if source is safe and destination needs it
		// 		if (!sourceLocked && /*!destinationLocked &&*/ destinationTopTarget && destinationRegion->waterLevel <= destinationRegion->waterNeed)
		// 		{
		// 			bool sourceIsATarget = false;
		// 			for (int j = 0; j < targets.size() && j < 2; ++j)
		// 			{
		// 				if (sourceRegion == targets[j])
		// 				{
		// 					sourceIsATarget = true;
		// 				}
		// 			}
		// 			double surplus = sourceRegion->waterLevel - sourceRegion->waterNeed;
		// 			if (surplus >= 0.5 || (!sourceIsATarget && sourceRegion->waterLevel > 0 && sourceRegion->waterLevel != sourceRegion->waterNeed))
		// 			{
		// 				std::cout << "Flowing into " << destinationRegion->name << " from " << sourceRegion->name <<  std::endl;
		// 				double gap = destinationRegion->waterNeed - destinationRegion->waterLevel;
		// 				double flow = (gap >= 1.0) ? 1.0 : gap;
		// 				if (surplus < flow && surplus > 0) flow = surplus;
						
		// 				canals[i]->setFlowRate(flow/3.6);
		// 				canals[i]->toggleOpen(true);
		// 				//std::cout << "Flow rate: " << flow << std::endl;
		// 			}
					

		// 			//if the destination region is the #1 target region
		// 			//and the source region is not #2 target region or is not in drought
		// 			//and destination region still has not met its need
		// 			//and source region has water
		// 			//we can take water from the source region
		// 			/*if(destinationRegion == targets[0] 
		// 				&& (!sourceIsATarget || sourceRegion->isInDrought) 
		// 				&& destinationRegion->waterLevel < destinationRegion->waterNeed
		// 				&& sourceRegion->waterLevel > 0)
		// 				{
		// 					std::cout << "Flowing into " << destinationRegion->name << " from " << sourceRegion->name <<  std::endl;
		// 					canals[i]->setFlowRate(1);
		// 					canals[i]->toggleOpen(true);
		// 				}*/
					

		// 			//debugging
		// 			/*if (targets[2]->isInDrought)
		// 			{
		// 				std::cout<< "Sacrificed region " << targets[2]->name << " is droughted." << std::endl;
		// 			}
		// 			if (targets[0]->waterLevel < targets[0]->waterNeed)
		// 			{
		// 				std::cout << "Primary target " << targets[0]->name << " needs water." << std::endl;
		// 			}
		// 			if (!targets[1]->isInDrought)
		// 			{
		// 				std::cout << "Secondary target " << targets[1]->name << " can spare some water." << std::endl;
		// 			}
		// 			if(destinationRegion == targets[2])
		// 			{
		// 				std::cout << "This canal flows into sacrificed region " << targets[2]->name << std::endl;
		// 			}
		// 			if(sourceRegion == targets[2])
		// 			{
		// 				std::cout << "This canal flows from secondary target " << targets[1]->name << std::endl;
		// 			}*/

					
		// 		}
		// 		//if the sacrificed region (targets[2]) is COMPLETELY empty
		// 		//and the primary target (targets[0]) still hasn't met its need
		// 		//and the secondary target (targets[1]) is not in drought
		// 		//but the canal goes to the sacrificed region from the secondary target
		// 		//we can still open the canal even though we normally keep it locked
		// 		//the water that flows into the sacrificed region should flow into the primary target in a future iteration
				
		// 		if(targets.size() > 2)
		// 		{
		// 			if(targets[2]->waterLevel == 0
		// 			&& targets[0]->waterLevel < targets[0]->waterNeed
		// 			&& !targets[1]->isInDrought
		// 			&& destinationRegion == targets[2]
		// 			&& sourceRegion == targets[1])
		// 			{
		// 				std::cout << "Flowing spare water into " << destinationRegion->name << " from " << sourceRegion->name <<  std::endl;
		// 					canals[i]->setFlowRate(1);
		// 					canals[i]->toggleOpen(true);
		// 			}
		// 			//make sure any excess from the sacrificed region is given to other regions
		// 			double surplus = sourceRegion->waterLevel - sourceRegion->waterNeed;
					
		// 		}
		// 		/*if(sourceRegion == sacrificedRegion
		// 				&& sourceRegion->waterLevel > 0) 
						
		// 			{
			
		// 				double flow = 1.0;
		// 				if (sourceRegion->waterLevel < flow && sourceRegion->waterLevel > 0) flow = sourceRegion->waterLevel;
						
		// 				canals[i]->setFlowRate(flow/3.6);
		// 				canals[i]->toggleOpen(true);
		// 				std::cout << "Flowing excess water into " << destinationRegion->name << " from " << sourceRegion->name <<  std::endl;
		// 				std::cout << "Flow rate: " << flow/3.6 << std::endl;
		// 			}*/
		// 		if (canals[i] == NtE && canals[i]->isOpen)
		// 		{
		// 				//last check ensures that water does not flow back and forth between north and east
		// 				//water should only flow in ONE direction
		// 				canals[i]->toggleOpen(false);
		// 				std::cout << "Canal NtE should not be open. Closing canal." << std::endl;
		// 		}
				
		// 		/*if(canals[i]->isOpen)
		// 		{
		// 			std::cout << "Canal from " << sourceRegion->name << " to " << destinationRegion->name
		// 			<< " is open with a flow rate of " << canals[i]->flowRate << std::endl;
		// 		}*/
		// 	}

			
			


		// }

		// else
		// {

		// 	NtS->toggleOpen(false);
		// 		NtE->toggleOpen(false);
		// 		StE->toggleOpen(false);
		// 		EtN->toggleOpen(false);
		// 	std::cout << manager.hour << ": North: " << North->isFlooded << " " << North->waterLevel
		// 	<< " South: " << South->isFlooded << " " << South->waterLevel
		// 	<< " East: " << East->isFlooded << " " << East->waterLevel << std::endl;
		// 	if (!North->isFlooded && East->isFlooded && !South->isFlooded)
		// 	{
		// 		EtN->setFlowRate(1);
		// 		NtS->setFlowRate(0.5);
		// 		NtS->toggleOpen(true);
		// 		NtE->toggleOpen(false);
		// 		StE->toggleOpen(false);
		// 		EtN->toggleOpen(true);
		// 	}


		// 	//std::cout << manager.hour << ": Whoops, middle isn't implemented." << std::endl;
		// 	//there is enough water to ensure that no region is droughted or flooded
		// 	//however, we still need to check if we can meet the needs of all regions
		// 	if (totalNeed <= totalWater)
		// 	{
		// 		//there is enough water for everyone! now we just need to figure out
		// 		//how to distribute it so everyone is happy.
		// 	}
		// 	else
		// 	{
		// 		//the water should be distributed in order of who needs the least amount of water first
		// 		//this way, we ware more likely to be able to meet the needs of at least 2 regions
		// 	}
		// }


		//student may add any necessary functions or check on the progress of each region as the simulation moves forward. 
		//The manager takes care of updating the waterLevels of each region and waterSource while the student is just expected
		//to solve how to address the state of each region


		manager.nexthour();
	}
}

/*example 2*/
/*
void solveProblems(AcequiaManager& manager)
{
	auto canals = manager.getCanals();
	while(!manager.isSolved && manager.hour!=manager.SimulationMax)
	{
	//Students will implement this function to solve the probelms
	//Example: Adjust canal flow rates and directions
		if(manager.hour==1)
		{
			canals[0]->setFlowRate(0.1);
			canals[0]->toggleOpen(true);
			canals[1]->setFlowRate(0.2);
			canals[1]->toggleOpen(true);
		}
		else if(manager.hour==3)
		{
			canals[0]->toggleOpen(false);
			canals[1]->toggleOpen(false);
		}
	//student may add any necessary functions or check on the progress of each region as the simulation moves forward. 
	//The manager takes care of updating the waterLevels of each region and waterSource while the student is just expected
	//to solve how to address the state of each region

		
		manager.nexthour();
	}
}
*/


//In this solution, students can make functions that aid in identifying the best course of action for moving
//water resources. They can set conditions that then work on the canal vectors based on the information reported

//This can help in optimizing solutions for dynamic constraints such as weather (rain, or dried up waterSources) and
//make the solution to the problem more abstract, allowing the logic and algorithm to be the sole priority of the student
//while the computation is left for the Acequia Manager

//This would be the perfect opportunity to identify the tools learned from ECE 231L such as:
//data structures (stacks, queues, trees(?)), templates, vector class functions, etc... to aid in the algorithm solution

/*
int findCanal(std::vector<Canal *> canals, std::string region)
{
	int match;
	for(int i = 0; i< canals.size();i++)
	{
		if(canals[i]->sourceRegion->name == region)
		{
			match = i;
		}
	}
	return match;
}

void release(std::vector<Canal *> canals, std::string region)
{
	int match = findCanal(canals, region);
	canals[match]->toggleOpen(true);
	canals[match]->setFlowRate(1);
	return;
}

void close(std::vector<Canal *> canals, std::string region)
{
	int match = findCanal(canals, region);
	canals[match]->toggleOpen(false);
}


void solveProblems(AcequiaManager& manager)
{

	bool check = false;
	auto canals = manager.getCanals();
	auto regions = manager.getRegions();
	while(!manager.isSolved && manager.hour!=manager.SimulationMax)
	{
		
		if(manager.hour == 0)
		{
			for(int i = 0; i<canals.size(); i++)
			{
				canals[i]->toggleOpen(true);
				canals[i]->setFlowRate(1);
			}
		}

		for(int i =0 ; i<regions.size(); i++)
		{
			if(regions[i]->isFlooded == true)
			{
				//release water from that region by a canal
				release(canals, regions[i]->name);
			}
			else if(regions[i]->isInDrought = true)
			{
				//find canal to move water
				close
			}

			else if(regions[i]->isFlooded == true && regions[i]->isInDrought == true)
			{
				close(canals, regions[i]->name);
			}
		}
		
		manager.nexthour();
	}
}
*/

void DroughtFunction()
{
	std::cout << "Whoops, this isn't implemented." << std::endl;
}
void MiddleFunction()
{
	std::cout << "Whoops, this isn't implemented." << std::endl;
}