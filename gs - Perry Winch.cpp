#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <deque>



//	Implementation of the Gale Shapley Algorithm - Solution to the Stable Marriage Problem

//	Made by Perry Winch 


using namespace std;

deque<string> freeMen;

map<string, int> countProposals;

//Men and their preferences	
map<string, vector<string> > menPref =
{
	{ "victor" , { "bertha", "amy", "diane", "erika", "clare" }},
	{ "wyatt" , { "diane", "bertha", "amy", "clare", "erika" }},
	{ "xavier" , { "bertha", "erika", "clare", "diane", "amy" }},
	{ "yancey" , { "amy", "diane", "clare", "bertha", "erika" }},
	{ "zeus" , { "bertha", "diane", "amy", "erika", "clare" }}
};

//Women and their preferences
map<string, vector<string> > womenPref =
{
	{ "amy" , { "zeus", "victor", "wyatt", "yancey", "xavier" }},
	{ "bertha" , { "xavier", "wyatt", "yancey", "victor", "zeus" }},
	{ "clare" , { "wyatt", "xavier", "yancey", "zeus", "victor" }},
	{ "diane" , { "victor", "zeus", "yancey", "xavier", "wyatt" }},
	{ "erika" , { "", "wyatt", "zeus", "xavier", "victor" }}
};

//Initializes deque of all free men
deque<string> freeMenInitializer(deque<string> freeMen, map<string, vector<string> > menPref)
{
	for (auto const& man : menPref)
	{
		freeMen.push_back(man.first);
	}

	return freeMen;
}

//Initializes map with the key being the man and the value being the amount of proposals 
map<string, int> countInitializer(map<string, int> countProposals, deque<string> freeMen)
{
	for (auto const& man : freeMen)
	{
		countProposals.insert(pair<string, int>(man, 0));
	}
	return countProposals;
}

//Will implement
//m1 being the man proposing and m2 being the man currently matched with w
bool wPrefersm1Overm2(string w, string m1, string m2, map<string, vector<string> > womenPref)
{
	for (auto const& man : womenPref[w])
	{
		if (man == m1)
		{
			return true;
		}
		if (man == m2)
		{
			return false;
		}
	}
}


//Main function to makes stable matches
void stableMarriage(map<string, vector<string> > menPref, map<string, vector<string> > womenPref, deque<string> freeMen, map<string, int> countProposals)
{
	//Map containing pairs
	map<string, string> matches;
		
	//Initialize all men as free
	freeMen = freeMenInitializer(freeMen, menPref);

	//Count for men to see how many times they proposed
	countProposals = countInitializer(countProposals, freeMen);
	
	//While there are free men
	while (freeMen.size() > 0)
	{
		//Get the first man free 
		string m = freeMen.front();

		int count = countProposals[m];

		string w = menPref[m][count];


		//If there are any matches 
		if (matches.size() > 0)
		{

			if (matches.count(w) == 1)
			{
				cout << m << " proposes to " << w << endl;
				//Check to see if w prefers the man proposing over the man currently matched with w
				if (wPrefersm1Overm2(w, m, matches[w], womenPref))
				{
					cout << w << " accepts and matches with " << m << endl;
					cout << matches[w] << " is now unmatched and will search for a new partner " << endl;

					//Remove m from the freeMen deque
					freeMen.pop_front();
					//Add man who just got unmatched to freeMen deque
					freeMen.push_front(matches[w]);
					//Delete the pair in matches
					matches.erase(w);
					//Add new pair into matches
					matches.insert({ w, m });
				}
				else
				{
					cout << w << " rejects " << m << " and stays with " << matches[w] << endl;
				}
			}
			else
			{
				//w isn't in a match
				cout << m << " proposes to " << w << endl;
				cout << w << " accepts and matches with " << m << endl;
				matches.insert({ w, m });

				//Remove m from the freeMen deque
				freeMen.pop_front();
			}
		}
		else
		{
			//first match
			cout << m << " proposes to " << w << endl;
			cout << w << " accepts and matches with " << m << endl;
			matches.insert({ w, m });

			//Remove m from the freeMen deque
			freeMen.pop_front();
		}
		//The man just proposed again so increase count
		countProposals[m]++;
	}

	cout << endl << "----- SOLUTION -----" << endl;

	//Print out matches
	for (auto const& match : matches)
	{
		cout << match.first << " is matched with " << match.second << endl;
	}

}

int main()
{
	//Run algorithm
	stableMarriage(menPref, womenPref, freeMen, countProposals);

}