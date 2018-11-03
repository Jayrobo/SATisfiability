#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <vector>
#include <cmath>
#include <omp.h>

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
	int sum = 0;
	int power = 0;
	size_t pos = guessed_answer.size() - 1;

	while(pos > 0) //stored the guessed answer into true and false scheme
	{
		if (guessed_answer[pos] < 0)
			sum += pow(2,power);

		//cout << power << " " << sum << endl;
		pos--;
		power++;
	}

	//----------------------------------------------------------------------------------//
	//           Increment or Decrement Depending on Initial guess                      //
	//----------------------------------------------------------------------------------//
	if (guessed_answer[0] == 1)
		sum++; //backtrack solution is increment as an guess update
	else
		sum--; //when initial guess start out from 1

	power--; //to compensate the last increment by the previous for loop
	//----------------------------------------------------------------------------------//
	//					  Conversion back to boolean (binary)							//
	//----------------------------------------------------------------------------------//
	vector<bool> Back_in_binary;
	while (power >= 0)
	{
		if (sum / pow(2,power) == 1)
			Back_in_binary.push_back(true);
		else
			Back_in_binary.push_back(false);

		sum  = remainder(sum, pow(2,power));
		power--;
		//cout << power << " " << sum << endl;
	}
	//----------------------------------------------------------------------------------//
	//								Update guess										//
	//----------------------------------------------------------------------------------//
	for (size_t i = 0; i < Back_in_binary.size(); i++)
	{
		if (Back_in_binary[i] == true)
			guessed_answer[i] = -guessed_answer[i];
	}
	//----------------------------------------------------------------------------------//
	return guessed_answer;
}

vector<int> Solution_Check(vector<int> guessed_answer, vector<vector<int>> Clauses)
{
	int k = 0; // guessed_answer index
	bool clause_true = false; //keep track of true clauses
	
	for (size_t i = 0; i < Clauses.size(); i++)
	{
		for (size_t j = 0; j < Clauses[i].size(); j++)
		{
			k = abs(Clauses[i][j]);
			if (Clauses[i][j] == guessed_answer[k-1]) {
				clause_true = true;
				break; //check next clause
			}
		}
		
		if (!clause_true) { //end of clause and clause still false
			guessed_answer = Update_Guess(guessed_answer); //update guess
			i = -1; //start from top clause again
			continue;
		}
	}
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


	//---------------------------------------------------------------------------------------//
	//-------------------------Initiate guesses and release threads -------------------------//
	//---------------------------------------------------------------------------------------//

	/*vector<int> Possible_Ans;
	for (int i = 0; i < maxi; i++)
		Possible_Ans.push_back(i + 1); //initialize guessed answer
									   //which starts at assuming all positive */

	vector<int> Positive_Ans, Negative_Ans; //initialize guessed answer
	for (int i = 0; i < maxi; i++) {
		Positive_Ans.push_back(i + 1); //for thread 1 to test
		Negative_Ans.push_back(-(i + 1)); // for thread 2 to test

	}
	vector<int> final_Solution; // store final result returned

	omp_set_num_threads(2);
	#pragma omp parallel for
	for (int i = 0; i < 2; i++)
	{
		if (i == 1)

			final_Solution = Solution_Check(Positive_Ans, Clauses);
		else
			final_Solution = Solution_Check(Negative_Ans, Clauses);

		if (!final_Solution.empty()) {
			#pragma omp cancel for //signal cancellation
		}
	}

	//---------------------------------------------------------------------------------------//
	//----------------------------- Print out SAT Solution ----------------------------------//
	//---------------------------------------------------------------------------------------//
	cout << endl <<"Solution = (";
	for (int k = 0; k < maxi; k++)
		cout << final_Solution[k] << " ";
	cout << ")" << endl;

	//Testing 2D Vector
	/*vector<vector<int> > Clauses;
	vector<int> Indi_Clauses;

	Indi_Clauses.push_back(2);
	Indi_Clauses.push_back(3);
	Clauses.push_back(Indi_Clauses);

	cout << "Rows of 2D Vector " << Clauses.size() << endl;
	cout << "First row size " << Clauses[0].size() << endl;
	*/

	vector<int> debug;
	debug = Update_Guess(Possible_Ans);

	for (int i = 0; i < Possible_Ans.size(); i++)
		cout << debug[i];

	cout << endl;
	debug = Update_Guess(debug);
	for (int i = 0; i < Possible_Ans.size(); i++)
		cout << debug[i];
	cout << endl;


	system("pause");
	return 0;
}