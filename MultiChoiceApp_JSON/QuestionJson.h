#pragma once
class Question {
public:
	std::string question;
	std::vector < std::string > options;
	std::string answer;

	void printQuestion()
	{
		std::cout << question << "\n";

	}
	int printOptions()
	{
		int optionChoice = -1;
		for (int i = 0; i < options.size(); i++)
		{
			std::cout << i + 1 << ".	" << options[i] << "\n";
		}
		std::cout << "\n";
		std::cout << "Please enter the option you have chosen(1-3):	";
		std::cin >> optionChoice;
		bool isValid = false;
		while (!isValid)
		{
			if (optionChoice < 1 || optionChoice >3)
			{
				std::cout << "The option you have chosen is not valid. Please enter another:	";
				std::cin >> optionChoice;
			}
			else
			{
				isValid = true;
			}
		}

		return optionChoice;

	}
	int printResult(int option)
	{
		if (options.at(option - 1).compare(answer) == 0)
		{
			std::cout << "CORRECT\n\n";
			return 1;
		}
		else
		{
			std::cout << "WRONG\n\n";
			return 0;
		}
	}
};
