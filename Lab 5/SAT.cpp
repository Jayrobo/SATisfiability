#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <vector>

using namespace std;

//global variable 
int maxi = 0; //for finding out number of variable needed to be solved
int clauses = 0; //number of claues in the file

vector<vector<int>> Inputter(char* filename)
{
	ifstream input_file;
	input_file.open(filename);
	string digits = "0123456789-"; //use for finding valid input
	vector<vector<int>> Clauses;
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