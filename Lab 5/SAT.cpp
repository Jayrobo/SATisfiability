#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <vector>
#include <omp.h>


using namespace std;

//global variable 
int maxi = 0; //for finding out number of variable needed to be solved
int clauses = 0; //number of clauses in the file

vector<vector<int>> Inputter()
{
	ifstream input_file;
	string filename;
	
	do 
	{
	   cout << "Please enter the file name with .cnf extension: ";
	   cin >> filename;
	   input_file.open(filename);
	   if (input_file.fail())
	   	cout << "Error, make sure you type the correct file name or file is in the folder, try again\n";
	}while(input_file.fail());
	
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
				//cout << endl;	//end of a line in the .cnf file
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

double pow(int iter)
{
	double sum = 1;
	for (int i = 0; i < iter; i++)
		sum = sum*2;

	return sum;
}

vector<int> Update_Guess(vector<int> guessed_answer, char option)
{
	double sum = 0;
	int power = 0;
	size_t pos = guessed_answer.size() - 1;

	while(pos > 0) //stored the guessed answer into true and false scheme
	{
		if (guessed_answer[pos] < 0)
			sum += pow(power);

		//cout << power << " " << sum << endl;
		pos--;
		power++;
	}

	//----------------------------------------------------------------------------------//
	//           Increment or Decrement Depending on Initial guess                      //
	//----------------------------------------------------------------------------------//
	if (option == 'I')
		sum++; //backtrack solution is increment as an guess update
	else
	{
		sum += pow(power);
		sum--; //when initial guess start out from 1
	}
	//power--; //to compensate the last increment by the previous for loop
	//----------------------------------------------------------------------------------//
	//					  Conversion back to boolean (binary)							//
	//----------------------------------------------------------------------------------//
	vector<bool> Back_in_binary;
	while (power >= 0)
	{
		if (sum / pow(power) >= 1)
		{
			Back_in_binary.push_back(false);
			sum = sum - pow(power); //update sum value
		}
		else
			Back_in_binary.push_back(true);

		power--;
		//cout << power << " " << sum << endl;
	}
	//----------------------------------------------------------------------------------//
	//								Update guess										//
	//----------------------------------------------------------------------------------//
	for (size_t i = 0; i < Back_in_binary.size(); i++)
	{
		if (Back_in_binary[i] == true)
			guessed_answer[i] = abs(guessed_answer[i]);
		else
			guessed_answer[i] = -abs(guessed_answer[i]);
	}
	//----------------------------------------------------------------------------------//
	return guessed_answer;
}

vector<int> Solution_Check(vector<int> guessed_answer, vector<vector<int>> Clauses, char option)
{
	int k = 0; // guessed_answer index
	bool clause_true = false; //keep track of true clauses
	
	for (size_t i = 0; i < Clauses.size(); i++)
	{
		clause_true = false; //reset at every clause
		for (size_t j = 0; j < Clauses[i].size(); j++)
		{
			k = abs(Clauses[i][j]);
			if (Clauses[i][j] == guessed_answer[k-1]) {
				clause_true = true;
				break; //check next clause
			}
		}
		
		if (i == (Clauses.size() - 1) && !clause_true)
		{
			guessed_answer[0] = maxi + 1; //to help identify if the solution does not exist
										  //since maxi is already the maximum variable....in any case there shouldn't be a maxi+1 unless solution not found
		}
		else if (!clause_true) 
		{ //end of clause and clause still false
			guessed_answer = Update_Guess(guessed_answer, option); //update guess
			i = -1; //start from top clause again
			continue;
		}
	}
	return guessed_answer;
}

int main()
{
	vector<vector<int>> Clauses = Inputter();

	cout << "This is the number of variable to solve: " << maxi << endl;
	cout << "Number of clauses in the file is " << clauses << endl;
	cout << "Clauses:" << endl;
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

	//vector<int> Possible_Ans;
	//for (int i = 0; i < maxi; i++)
	//	Possible_Ans.push_back(i + 1); //initialize guessed answer
									   //which starts at assuming all positive

	//for (int i = 0; i < Possible_Ans.size(); i++)
	//	cout << Possible_Ans[i];
	//cout << endl;

	vector<int> Positive_Ans, Negative_Ans, final_Solution_1, final_Solution_2; // positive and negative initial solutions
	for (int i = 0; i < maxi; i++) {
		Positive_Ans.push_back(i + 1);
		Negative_Ans.push_back(-(i + 1));
	}

	omp_set_num_threads(2);
	#pragma omp parallel for
	for (int i = 0; i<2; i++)
	{
		if (i == 0)
		{
			//#pragma omp critical
			{
			final_Solution_1 = Solution_Check(Positive_Ans, Clauses, 'I'); //All positive means its all set to 0s
			}
			//#pragma omp cancel for //signal cancellation if one is done...
		}
		else
		{
			//#pragma omp critical
			{
				final_Solution_2 = Solution_Check(Negative_Ans, Clauses, 'D'); //All negative means its all set to 1s
			}
			//#pragma omp cancel for //signal cancellation if one is done...
		}
	}
	//#pragma omp cancellation point for
	
	//Testing 2D Vector
	/*vector<vector<int> > Clauses;
	vector<int> Indi_Clauses;

	Indi_Clauses.push_back(2);
	Indi_Clauses.push_back(3);
	Clauses.push_back(Indi_Clauses);

	cout << "Rows of 2D Vector " << Clauses.size() << endl;
	cout << "First row size " << Clauses[0].size() << endl;
	*/
	
	//---------------------------------------------------------------------------------------//
	//-------------------------Debug on the Function Update_Guess ---------------------------//
	//---------------------------------------------------------------------------------------//
	/*vector<int> debug;
	debug = Update_Guess(Positive_Ans, 'I');

	for (int i = 0; i < Positive_Ans.size(); i++)
		cout << debug[i];
	cout << endl;
	debug = Update_Guess(debug, 'I');
	for (int i = 0; i < Positive_Ans.size(); i++)
		cout << debug[i];
	cout << endl;
	debug = Update_Guess(debug, 'I');
	for (int i = 0; i < Positive_Ans.size(); i++)
		cout << debug[i];
	cout << endl;
	debug = Update_Guess(debug, 'I');
	for (int i = 0; i < Positive_Ans.size(); i++)
		cout << debug[i];
	cout << endl;*/

	//---------------------------------------------------------------------------------------//
	//----------------------------- Print out SAT Solution ----------------------------------//
	//---------------------------------------------------------------------------------------//
	if (final_Solution_1[0] != (maxi + 1)) //output only solution exist
	{
		cout << endl << "Solution = (";
		for (int k = 0; k < maxi; k++)
			cout << final_Solution_1[k] << " ";
		cout << ")" << endl;
	}
	else if (final_Solution_2[0] != (maxi + 1)) //output only solution exist
	{
		cout << endl << "Solution = (";
		for (int k = 0; k < maxi; k++)
			cout << final_Solution_2[k] << " ";
		cout << ")" << endl;
	}
	else 
		cout << endl << "Case unsatisfiable.\n";
	
	system("pause");
	return 0;
}
