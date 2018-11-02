#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <vector>

using namespace std;

//global variable 
int maxi = 0; //for finding out number of variable needed to be solved
int clauses = 0; //number of clauses in the file

vector<vector<int>> Inputter(char* filename)
{
	ifstream input_file;
	input_file.open(filename);
	string digits = "0123456789-"; //use for finding valid input
	vector<vector<int>> Clauses; //matrix of clauses
	vector<int> temp_int;

	while (!input_file.eof())
	{
		string temp_storage;


		input_file >> temp_storage;

		if (temp_storage.find_first_of(digits) != string::npos) //the read in input should only contain number
		{
			if (stoi(temp_storage) == 0)
			{
				Clauses.push_back(temp_int);
				temp_int.clear(); //clear for a new clause
				cout << endl;	//end of a line in the .cnf file
								//use to seperate clasues
				clauses++;
			}
			else
			{
				temp_int.push_back(stoi(temp_storage));
				if (maxi < abs(stoi(temp_storage)))
					maxi = abs(stoi(temp_storage)); //figure out the number of variable needed to be solved

				//cout << temp_int << " ";
			}
		}
		else
			getline(input_file, temp_storage); //if the line contains character, we want to skip a line when reading
	}

	return Clauses;
}

vector<int> Update_Guess(vector<int> guessed_answer)
{

	return guessed_answer;
}

vector<int> Solution_Check(vector<int> guessed_answer)
{

	return guessed_answer;
}

int main()
{
	char* filename = "small_test.cnf";
	vector<vector<int>> Clauses = Inputter(filename);

	cout << "This is the number of variable to solve: " << maxi << endl;
	cout << "Number of clauses in the file is " << clauses << endl;

	//output to see if clauses are stored properly
	for (size_t i = 0; i < Clauses.size(); i++)
	{
		for (size_t j = 0; j < Clauses[i].size(); j++)
		{
			cout << Clauses[i][j] << " ";
		}
		cout << endl;
	}

	vector<int> Possible_Ans;
	for (int i = 0; i < maxi; i++)
		Possible_Ans.push_back(i + 1); //initialize guessed answer 
									   //which starts at assuming all positive

	for (int i = 0; i < Possible_Ans.size(); i++)
		cout << Possible_Ans[i] << endl;

	/*
	//serial soultion test:
	bool clause_true = false; //keeps track if clause is true
	int i = 0; //row
	int j = 0; //col
	int k;// possible ans index

	while (i < Clauses.size()) {
		while (!clause_true) {
			for (k = 0; k < Possible_Ans.size(); k++) { // check if clause has item in Possible Ans
				if (Clauses[i][j] == Possible_Ans[k]) {
					clause_true = true; //check next element
					i++; j = 0; // check next clause
					cout << endl << "Clauses "<<i<<"_"<<j << " == " << Possible_Ans[k]<<endl;
					break; //just need one true to validate clause
				} //else keep checking other elements of solution
			}
			if (!clause_true && j < Clauses[i].size()) {
				j++; //check next variable
			}
			if (!clause_true && j > Clauses[i].size()){
				Possible_Ans[k-1] = 0 - (Possible_Ans[k-1]); //try again
				 j= 0;}
		}
	}
	*/


	//Testing 2D Vector
	/*vector<vector<int> > Clauses;
	vector<int> Indi_Clauses;

	Indi_Clauses.push_back(2);
	Indi_Clauses.push_back(3);
	Clauses.push_back(Indi_Clauses);

	cout << "Rows of 2D Vector " << Clauses.size() << endl;
	cout << "First row size " << Clauses[0].size() << endl;
	*/

	system("pause");
	return 0;
}