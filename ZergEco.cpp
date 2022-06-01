// C++ code for Optimal Zerg Income
// Author: Turb007

#include <iostream>
#include <vector>
using namespace std;

//Initialization of variables
int numSpires = 12;
int numSpawners = 24;
int supplyLevel = 0;
int supplyCount = 54;
int strikeCount = 0;
int strikeLimit = 60;
double spireCost = 220;
double lingCost = 70;
double strikeCost = 90;
double supplyCost = 0;
vector<int> array; // Spire == 0, Ling == 1, Strike == 2. This vector is not needed for the program to run, this is specifically for easy build information to plug in to a different simulation/code in Python

//Function prototypes
double eco(int numSpires, int numSpawners);
double supplyCostCalc(int supplyLevel);
double spireROI(int numSpires, int numSpawners, double spireCost);
double strikeROI(int numSpires, int numSpawners, double strikeCost);
double lingROI(int numSpires, int numSpawners, double lingCost);
void bestROI(int &numSpires, int &numSpawners, double spireCost, double lingCost, double strikeCost, int &supplyCount, int &strikeCount);
bool isStrikeWorth(int numSpires, int numSpawners, double strikeCost);

int main() {
	// While eco is under 6k, keep ecoing at optimal effeciency
	while (eco(numSpires, numSpawners) < 6000) {

		//Setting spire and spawner costs based off of supply cost
		spireCost = 220 + (supplyCostCalc(supplyLevel)/3);
		lingCost = 70 + (supplyCostCalc(supplyLevel)/11.25);
		strikeCost = 90 + (supplyCostCalc(supplyLevel)/15);
		
	/**	cout << "Cost of a Spire: " << spireCost << endl;
		cout << "Cost of a Ling Spawner: " << lingCost << endl;
		cout << "Cost of a Strike Spawner: " << strikeCost << endl; */
		
		//Loop to use up each supply down to at least 3
		while (supplyCount >= 3) {
			cout << "ROI of a Spire: " << spireROI(numSpires, numSpawners, spireCost) << endl;
			cout << "ROI of a Strike: " << strikeROI(numSpires, numSpawners, strikeCost) << endl;
			cout << "ROI of a Ling: " << lingROI(numSpires, numSpawners, lingCost) << endl;
			cout << "Eco increase from a spire: " << eco(numSpires+1, numSpawners) - eco(numSpires, numSpawners) << endl;
			if ((supplyCount == 3) && (isStrikeWorth(numSpires, numSpawners, strikeCost) == false))
				break;
			else {
				bestROI(numSpires, numSpawners, spireCost, lingCost, strikeCost, supplyCount, strikeCount);
			}
		}
		
		//Update supply count and level
		supplyLevel++;
		cout << "Left over supply: " << supplyCount << endl;
		supplyCount += 45;
		cout << "Supply level: " << supplyLevel << " Current economy: " << eco(numSpires, numSpawners) << endl;

	}
	cout << endl;
	cout << "Total number of Spawners: " << numSpawners << endl;
	cout << "Total number of Spires: " << numSpires << endl;
	cout << "Gugu's info: ";
	for (const int& i : array) {
		cout << i << ",";
	}
	return 0;
}


//Function that returns eco
double eco(int numSpires, int numSpawners) {
	return 160 + (numSpires * (30 + numSpawners)); //160 from base eco from hives
}

//Function for setting the supplyCost
double supplyCostCalc(int supplyLevel) {
	if (supplyLevel == 0) { //First 54 of supply is free
		supplyCost = 0;
		return supplyCost;
	}
	if (supplyLevel == 1) { //First supply cost is 600
		supplyCost = 600;
		return supplyCost;
	}
	if (supplyLevel % 2 == 1) { //Every other supply increases by 125
		supplyCost = (600 + (125*(supplyLevel/2)));
		return supplyCost;
	}
	else { // supplyLevel % 2 == 0 Every other level does not increase
		supplyCost = supplyCost;
		return supplyCost;
	}

}

//Function to determine return on investment for a spire
double spireROI(int numSpires, int numSpawners, double spireCost) {

	return (eco(numSpires+1, numSpawners) - eco(numSpires, numSpawners)) / (spireCost);

}

//Function to determine return on investment for 1 strike spawner
double strikeROI(int numSpires, int numSpawners, double strikeCost) {
	return (eco(numSpires, numSpawners+1) - eco(numSpires, numSpawners)) / (strikeCost);
}

//Function to determine return on investment for 1 ling spawner
double lingROI(int numSpires, int numSpawners, double lingCost) {
	return (eco(numSpires, numSpawners+1) - eco(numSpires, numSpawners)) / (lingCost);
}

//Function that determines the best ROI per supply
void bestROI(int &numSpires, int &numSpawners, double spireCost, double lingCost, double strikeCost, int &supplyCount, int &strikeCount) {
	if (supplyCount >= 15) {
		//Exception case of 20 supply or 23 supply (otherwise could end up with building 2 strikes and ending up with 2 leftover supply)
		if (supplyCount == 20 || supplyCount == 23) {
			if (spireROI(numSpires, numSpawners, spireCost) < lingROI(numSpires, numSpawners, lingCost)) {
				supplyCount -= 4;
				numSpawners++;
				array.push_back(1);
				cout << "1 Ling Spawner was built for a total of " << numSpawners << " Spawners" << endl;
			}
			else {
				supplyCount -= 15;
				numSpires++;
				array.push_back(0);
				cout << "1 Spire was built for a total of " << numSpires << " Spires" << endl;
			}
		}
		else if (supplyCount == 19) {
			supplyCount -=4;
			numSpawners++;
			array.push_back(1);
			cout << "1 Ling Spawner was built for a total of " << numSpawners << " Spawners" << endl;
		}
		
		//If a strike spawner gives best ROI and strike limit is not reached, build one
		else if ((strikeROI(numSpires, numSpawners, strikeCost)) > (spireROI(numSpires, numSpawners, spireCost)) && ((strikeROI(numSpires, numSpawners, strikeCost)) > (lingROI(numSpires, numSpawners, lingCost))) && (strikeCount < strikeLimit)) {
			supplyCount -= 3;
			numSpawners++;
			strikeCount++;
			array.push_back(2);
			cout << "1 Strike spawner was built for a total of "<< strikeCount << " Strike spawners " << endl;
		}
		//If a spire gives best ROI, build one
		else if (spireROI(numSpires, numSpawners, spireCost) > lingROI(numSpires, numSpawners, lingCost)) {
			supplyCount -= 15;
			numSpires++;
			array.push_back(0);
			cout << "1 Spire was built for a total of " << numSpires << " Spires" << endl;
		}

		//If a ling spawner gives best ROI, build one
		else {
			supplyCount -= 4;
			numSpawners++;
			array.push_back(1);
			cout << "1 Ling Spawner was built for a total of " << numSpawners << " Spawners" << endl;
			
		}
	}
	else if ((supplyCount < 15) && (supplyCount >= 4)) {
		//If a ling spawner gives better ROI than a strike spawner, build one
		if (lingROI(numSpires, numSpawners, lingCost) > strikeROI(numSpires, numSpawners, strikeCost)) {
			supplyCount -= 4;
			numSpawners++;
			array.push_back(1);
			cout << "1 Ling Spawner was built for a total of " << numSpawners << " Spawners" << endl;
		}
		//If 4, 5, or 8 supply is remaining, always best to make 1 or 2 ling spawner(s) instead of a strike
		else if ((supplyCount == 4) || (supplyCount == 5) || (supplyCount == 8)) {
			supplyCount -= 4;
			numSpawners++;
			array.push_back(1);
			cout << "1 Ling Spawner was built for a total of " << numSpawners << " Spawners" << endl;
		}
		//Builds a strike spawner if ROI is higher than ling spawner
		else if (strikeCount < strikeLimit) {
			supplyCount -= 3;
			numSpawners++;
			strikeCount++;
			array.push_back(2);
			cout << "1 Strike spawner was built for a total of "<< strikeCount << " Strike spawners " << endl;
		}
		//If strikes are maxed at 60 and less than 15 supply, build a ling spawner
		else {
			supplyCount -= 4;
			numSpawners++;
			array.push_back(1);
			cout << "1 Ling Spawner was built for a total of " << numSpawners << " Spawners" << endl;
		}

	}
	//Builds a strike spawner if it is worth it with 3 leftover supply
	else if (supplyCount == 3 && strikeCount < 60) {
		supplyCount -= 3;
		numSpawners++;
		strikeCount++;
		array.push_back(2);
		cout << "1 Strike spawner was built for a total of "<< strikeCount << " Strike spawners " << endl;
	}
	return;
}

//Bool function for 3 leftover supply
bool isStrikeWorth(int numSpires, int numSpawners, double strikeCost) {

		int supplyLevelRef;
		double spireCostRef;
		double lingCostRef;
		double strikeCostRef;
		supplyLevelRef = supplyLevel+1;
		spireCostRef = 220 + (supplyCostCalc(supplyLevelRef)/3);
		lingCostRef = 70 + (supplyCostCalc(supplyLevelRef)/11.25);
		strikeCostRef = 90 + (supplyCostCalc(supplyLevelRef)/15);

	if (strikeCount == strikeLimit)
		return false;
	else if ((strikeROI(numSpires, numSpawners, strikeCostRef) >= spireROI(numSpires, numSpawners, spireCostRef)) && ((strikeROI(numSpires, numSpawners, strikeCostRef)) >= (lingROI(numSpires, numSpawners, lingCostRef))))
		return true;
	else if (supplyLevel >= 6)
		return true;
	else
		return false;
}
