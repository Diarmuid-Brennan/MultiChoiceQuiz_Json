#pragma once
class Player {
public:
	std::string name;
	int score;

	void printDetails()
	{
		std::cout << "Name : " << name << "	->	 Score :  " << score << "\n";
	}
};