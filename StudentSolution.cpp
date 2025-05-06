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
	//creating named references for each canal since there are only 4
	//and referencing them with canal[0/1/2/3] gets confusing
	Canal* NtS = canals[0]; 
	Canal* StE = canals[1];
	Canal* NtE = canals[2];
	Canal* EtN = canals[3];
	//also creating named references for each region
	//for the same reason as canals
	Region* North = canals[0]->sourceRegion;
	Region* South = canals[1]->destinationRegion;
	Region* East = canals[2]->destinationRegion;

	//begin by analyzing water data
	//these variables will help decide how we want to solve the problem
	double totalNeed = North->waterNeed + South->waterNeed + East->waterNeed;
	double totalWater = North->waterLevel + South->waterLevel + East->waterLevel;
	double totalCapacity = North->waterCapacity + South->waterCapacity + East->waterCapacity;
	
	while (!manager.isSolved && manager.hour != manager.SimulationMax)
	{
		//Students will implement this function to solve the probelms
		//Example: Adjust canal flow rates and directions
		

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
			if (totalNeed <= totalWater)
			{
				//there is enough water for everyone! now we just need to figure out
				//how to distribute it so everyone is happy.
			}
			else
			{
				//the water should be distributed in order of who needs the least amount of water first
				//this way, we ware more likely to be able to meet the needs of at least 2 regions
			}
		}


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