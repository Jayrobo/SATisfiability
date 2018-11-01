#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>

using namespace std;

int main()
{
	char* filename = "small_test.cnf";
	ifstream input_file;
	input_file.open(filename);
	string digits = "0123456789-"; //use for finding valid input
	int maxi = 0;
	int temp_int;

	while (!input_file.eof())
	{
		string temp_storage;
		input_file >> temp_storage;

		if (temp_storage.find_first_of(digits) != string::npos) //the read in input should only contain number
		{
			if (stoi(temp_storage) == 0)
				cout << endl;	//end of a line in the .cnf file
								//use to seperate clasues
			else
			{
				temp_int = stoi(temp_storage);
				if (maxi < abs(temp_int))
					maxi = abs(temp_int);

				cout << temp_int << " ";
			}
		}
		else
			getline(input_file, temp_storage); //if the line contains character, we want to skip a line when reading
	}

	cout << "This is the number of variable to solve: " << maxi << endl;
	system("pause");
	return 0;
}