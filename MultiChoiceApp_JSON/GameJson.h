#pragma once
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include<time.h>
#include "QuestionJson.h"
#include "PlayerJson.h"
#include "GraphJson.h"
using namespace rapidjson;

class Game {
public:
	Player newPlayer;
	std::vector<Question> questionList;
	Graph scoreGraph;

	void getQuestionsFromFile()
	{
		std::ifstream myfile("multiChoiceQuiz.json");
		std::string line;

		char json[10000] = "";
		int ji = 0;

		if (myfile.is_open())
		{
			while (std::getline(myfile, line))
			{
				for (int i = 0; i < line.length(); i++)
				{
					// line.at(i) returns char at position i in string.
					char c = line.at(i);
					json[ji++] = c;
				}

			}
			myfile.close();
		}

		else
		{
			std::cout << "Unable to open file";
		}
		Document document;
		if (document.Parse(json).HasParseError())
			std::cout << "ERROR PARSING DOCUMENT";

		assert(document.IsObject());
		int numQuestions = document["numQuestions"].GetInt();
		const Value& questions = document["questions"];
		for (SizeType questionIndex = 0; questionIndex < numQuestions; questionIndex++)
		{
			Question currentQuestion;
			currentQuestion.question = questions[questionIndex]["question"].GetString();
			std::vector<std::string> options;
			const Value& data = questions[questionIndex]["options"];
			assert(data.IsArray());
			for (SizeType dataIndex = 0; dataIndex < data.Size(); dataIndex++)
			{   
				currentQuestion.options.push_back(data[dataIndex].GetString());

			}
			currentQuestion.answer = questions[questionIndex]["answer"].GetString();
			questionList.push_back(currentQuestion);
		}
	}

	void playQuiz()
	{
		std::string name;
		std::cout << "	NEW GAME\n";
		std::cout << "\nPlease enter players name ;	";
		std::cin >> name;
		newPlayer.name = name;
		std::cout << "\n";
		std::cout << "*****QUIZ******\n";

		getQuestionsFromFile();

		srand(time(0));

		int numQuestions = questionList.size();
		int totalCorrect = 0;

		while (numQuestions > 0)
		{
			int question = rand() % numQuestions;
			questionList[question].printQuestion();
			int choice = questionList[question].printOptions();
			totalCorrect += questionList[question].printResult(choice);
			std::vector<Question>::iterator it;
			it = questionList.begin();
			it += question;
			questionList.erase(it);
			numQuestions--;
		}

		std::cout << "Your final score is " << totalCorrect << "\n";
		newPlayer.score = totalCorrect;
		retrieveResultsFromFile();
	
	}


	void retrieveResultsFromFile()
	{
		std::ifstream myfile("PlayerScores.json");
		std::string line;

		char json[10000] = "";
		int ji = 0;

		if (myfile.is_open())
		{
			while (std::getline(myfile, line))
			{
				for (int i = 0; i < line.length(); i++)
				{
					// line.at(i) returns char at position i in string.
					char c = line.at(i);
					json[ji++] = c;
				}

			}
			myfile.close();
		}

		else
		{
			std::cout << "Unable to open file";
		}
		Document document;
		if (document.Parse(json).HasParseError())
			std::cout << "ERROR PARSING DOCUMENT";

		assert(document.IsArray());
		convertJSON(document);

		
		uploadPlayerScoreToFile(document);
		
		
	}

	void convertJSON(Document& currentDoc)
	{
		std::vector<Player> players;
		Player currentPlayer;
		for (SizeType playerIndex = 0; playerIndex < currentDoc.Size(); playerIndex++)
		{
			currentPlayer.name = currentDoc[playerIndex]["name"].GetString();
			currentPlayer.score = currentDoc[playerIndex]["score"].GetInt();
			players.push_back(currentPlayer);
		}
		sortScores(players);

	}

	void sortScores(std::vector<Player> playerScores)
	{
		
		playerScores.push_back(newPlayer);
		for (int outerLoop = playerScores.size(); outerLoop >= 0; outerLoop--)
		{
			for (int innerLoop = 0; innerLoop < outerLoop - 1; innerLoop++)
			{
				if (playerScores[innerLoop].score < playerScores[innerLoop + 1].score)
				{
					Player swapPositions;
					swapPositions = playerScores[innerLoop];
					playerScores[innerLoop] = playerScores[innerLoop+1];
					playerScores[innerLoop + 1] = swapPositions;

				}
			}
		}
		
		outputScoreTable(playerScores);
	}


	void outputScoreTable(std::vector<Player> playerScores)
	{
		std::cout << "*******RESULTS TABLE******\n";
		for (int i = 0; i < playerScores.size(); i++)
		{
			playerScores[i].printDetails();
		}
		std::cout << "\n";
		scoreGraph.outputGraph(playerScores, newPlayer);
	}

	void uploadPlayerScoreToFile(Document& document)
	{
		assert(document.IsArray());
		Value name;
		name.SetString(newPlayer.name.c_str(), document.GetAllocator());
		Value addPlayer(kObjectType);
		addPlayer.AddMember("name", name, document.GetAllocator());
		addPlayer.AddMember("score", newPlayer.score, document.GetAllocator());
		
		document.PushBack(addPlayer, document.GetAllocator());

		std::ofstream file("PlayerScores.json");
		StringBuffer sb;
		PrettyWriter<StringBuffer> writer(sb);
		document.Accept(writer);
		//puts(sb.GetString());
		std::string jsonOut(sb.GetString(), sb.GetSize());
		file << jsonOut;

	}


	void playAgain()
	{

		bool playAgain = true;
		while (playAgain)
		{
			char ans;
			std::cout << "PLAY AGAIN? (please enter y or n):	";
			std::cin >> ans;
			std::cout << "\n";
			if (ans == 'y' || ans == 'Y')
			{
				playQuiz();
			}
			else if (ans == 'n' || ans == 'N')
			{
				std::cout << "You have exited the game\n";
				break;
			}
			else
				std::cout << "That is not an option.Please select another\n";
		}
	}
};