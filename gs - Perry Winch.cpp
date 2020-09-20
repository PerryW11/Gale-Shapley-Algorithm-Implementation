#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <chrono>
#include <algorithm>
#include <random>


//	Implementation of the Gale Shapley Algorithm - Solution to the Stable Marriage Problem

//	Made by Perry Winch 


using namespace std;


// Initialize list of preferences for each man and then randomize it dynamically
vector< vector<int> > MenPreferenceInitializer(int num)
{

	vector< vector<int> > menPref(num);

	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			menPref[i].push_back(j);
		}
	}

	//Randomize each list of preference
	for (int i = 0; i < num; i++)
	{
		random_device r;
		seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
		mt19937 eng(seed);
		shuffle(begin(menPref[i]), end(menPref[i]), eng);
	}

	return menPref;

}

//Initialize list of preferences for each woman and then randomize it dynamically
vector< vector<int> > WomenPreferenceInitializer(int num)
{

	vector< vector<int> > womenPref(num);

	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			womenPref[i].push_back(j);
		}
	}

	//Randomize each list of preference
	for (int i = 0; i < num; i++)
	{
		random_device r;
		seed_seq seed{ r(), r(), r(), r(), r(), r(), r(), r() };
		mt19937 eng(seed);
		shuffle(begin(womenPref[i]), end(womenPref[i]), eng);
	}

	return womenPref;

}

//Initialize queue with each man being free initially
queue<int> FreeMenInitializer(const vector< vector<int> >& menPref)
{
	//priority_queue<int, vector<int>, greater<int> > freeMen;
	queue<int> freeMen;

	for (int i = 0; i < menPref.size(); i++)
	{
		freeMen.push(i);
	}

	return freeMen;
}

//Initialize each man having the amount of proposals made at 0
vector<int> CountInitializer(queue<int>& freeMen)
{

	vector<int> countProposals(freeMen.size());

	for (int i = 0; i < freeMen.size(); i++)
	{
		countProposals.push_back(0);
	}
	return countProposals;
}

//Inverse each list of preferences for each woman to make unstable match condition check O(1) 
vector< vector<int> > InverseWomenPref(const vector< vector<int> >& womenPref, int num)
{
	vector< vector<int> > inverseWomenPref(num);
	for (int i = 0; i < inverseWomenPref.size(); i++)
		inverseWomenPref[i].resize(num);

	//For each list of preferences
	for (int i = 0; i < womenPref.size(); i++)
	{
		//for each element in the list of preference
		for (int j = 0; j < womenPref[i].size(); j++)
		{
			//Inverse the man to be the index and the preference ranking as the value
			int k = womenPref[i][j];
			inverseWomenPref[i][k] = j;
		}
	}
	return inverseWomenPref;
}

//Print out each list of preferences if desired
void PrintPreferences(const vector< vector<int> >& menPref, const vector< vector<int> >& womenPref)
{

	std::cout << "--- LIST OF PREFERENCES ---" << endl << endl;
	for (int i = 0; i < menPref.size(); i++)
	{
		std::cout << "Man: " << i << " - ";

		for (int j = 0; j < menPref[i].size(); j++)
		{
			std::cout << menPref[i][j] << " ";
		}

		std::cout << endl;

	}
	//Printing women and their preferences
	for (int i = 0; i < womenPref.size(); i++)
	{
		std::cout << "Woman: " << i << " - ";
		for (int j = 0; j < womenPref[i].size(); j++)
		{
			std::cout << womenPref[i][j] << " ";
		}

		std::cout << endl;

	}

	std::cout << endl;

}


//Main function to makes stable matches
void StableMarriage(int num)
{


	//Indexes represent the women and the value at the index is the man matched with her
	vector<int> matches(num, -1);
	
	//Vector containing each man's list of preferences, indexes of menPref being the men
	vector< vector<int> > menPref = MenPreferenceInitializer(num);

	//Vector containing each woman's list of preferences, indexes of womenPref being the women
	const vector< vector<int> > womenPref = WomenPreferenceInitializer(num);

	//Queue with all the free men
	queue<int> freeMen = FreeMenInitializer(menPref);

	//Amount of proposals made by each man, index representing the man
	vector<int> countProposals = CountInitializer(freeMen);

	//Inverse of the list of preferences for each woman, indexes being the woman
	const vector< vector<int> > inverseWomenPref = InverseWomenPref(womenPref, num);

	//PrintPreferences(menPref, womenPref);

	//std::cout << "--- STARTING MATCHING PROCESS ---" << endl << endl;


	//Now recording elapsed main algorithm now that pre-processing is done
	auto startLoop = chrono::steady_clock::now();

	//While there are free men
	while (freeMen.size() > 0)
	{
		//Get the first man free 
		int m = freeMen.front();

		//Get the amount of proposals the man has made
		int count = countProposals[m];

		//Get w based on the amount of proposals the man has made
		int w = menPref[m][count];

		//If -1, the woman is free, and if anything else, then that's the man w is matched with
		int m1 = matches[w];
		
		//If w is unmatched
		if (m1 == -1)
		{
			//cout << "Woman " << w << " is initially unmatched and will match with Man " << m << endl;
			
			//m now matches with w
			matches[w] = m;

			//remove m from the queue of free men
			freeMen.pop();
		}
		//If w prefers the man proposing vs the man she is matched with
		else if (inverseWomenPref[w][m] < inverseWomenPref[w][m1])
		{
			//Match her with the new man
			matches[w] = m;
			//Remove m from the freeMen deque
			freeMen.pop();
			//Add man back to free men
			freeMen.push(m1);
			//cout << "Woman " << w << " leaves " << m1 << " for " << m << " leaving " << m1 << " free once more" << endl;
			//auto end = chrono::steady_clock::now();
			//double elapsedTime = double(chrono::duration_cast <chrono::nanoseconds>(end - start).count());
			//std::cout << "Elapsed Time when replaced: " << elapsedTime << " nanoseconds" << endl;
		}
		//If she rejects the man proposing and stays with her current match
		else
		{
			//cout << "Woman " << w << " rejects Man " << m << " and stays with Man " << m1 << endl;
		}

		//Man just proposed so increase proposal count
		countProposals[m]++;

	}

	//Marking time at the end of the algorithm and printing it out
	auto endLoop = chrono::steady_clock::now();
	chrono::microseconds elapsedTime = chrono::duration_cast <chrono::microseconds>(endLoop - startLoop);
	std::cout << elapsedTime.count() << std::endl;;

	//std::cout << endl << "----- SOLUTION -----" << endl;

	//Print out matches
	//for (int i = 0; i < matches.size(); i++)
	//{
	//	cout << "Man " << matches[i] << " is matched with Woman " << i << endl;
	//}

	

}


int main(int argc, char* argv[])
{

	if (argc == 2)
	{
		int num = stoi(argv[1]);

		//Run algorithm
		StableMarriage(num);

		return 0;
	}

	return 1;

}
