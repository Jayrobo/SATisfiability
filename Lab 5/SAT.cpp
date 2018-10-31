#include <fstream>
#include <string>
#include <ctype.h>

using namespace std;

int main()
{
	char* filename = "Test_File.cnf";
	ifstream input_file;
	input_file.open(filename);

	while (!input_file.eof)
	{
		unsigned temp_storage;
		input_file >> temp_storage;

		if (isdigit(temp_storage))
		{
			if (temp_storage == 0)
				cout << endl;
			else
				cout << temp_storage << " ";
		}
	}
}