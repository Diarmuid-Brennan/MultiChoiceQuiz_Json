#pragma once
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>

class Graph {
public:
	std::vector<Player> topFive;
	void initializeTopFive()
	{
		Player player;
		for (int i = 0; i < 5; i++)
		{
			player.name = "";
			player.score = 0;
			topFive.push_back(player);
		}
	}

	void updateTopFive(std::vector<Player> players)
	{
		for (int i = 0; i < players.size() && i < 5; i++)
		{
			topFive[i] = players[i];
		}
	}

	void outputGraph(std::vector<Player> players, Player currentPlayer)
	{
		initializeTopFive();
		updateTopFive(players);
		sf::RenderWindow window(sf::VideoMode(750, 550), "Score Chart");
		sf::Font font;
		if (!font.loadFromFile("calibri.ttf"))
			std::cout << "Can't find font file";

		sf::String yHeading = "Score";
		sf::Text text(yHeading, font, 20);
		text.setPosition(50, window.getSize().y/2 );
		text.setFillColor(sf::Color::Green);

		sf::String xHeading = "Player";
		sf::Text text1(xHeading, font, 20);
		text1.setPosition(window.getSize().x / 2 - 50, window.getSize().y - 50);
		text1.setFillColor(sf::Color::Green);

		sf::String correct = "CORRECT";
		sf::Text text4(correct, font, 25);
		text4.setPosition(50, window.getSize().y/2 - 20);
		text4.setFillColor(sf::Color::Green);

		sf::String wrong = "WRONG";
		sf::Text text5(wrong, font, 25);
		text5.setPosition(50, window.getSize().y / 2 + 20);
		text5.setFillColor(sf::Color::Red);

		int namePoitionX = window.getSize().x/2 - 90;
		int namePoitionY = 50;
		sf::String groups = "TOP 5 SCORES";
		sf::Text text3(groups, font, 30);
		text3.setPosition(namePoitionX, namePoitionY);
		text3.setFillColor(sf::Color::Red);

		sf::String percantage = "How many you got right";
		sf::Text pieChartHeading(percantage, font, 30);
		pieChartHeading.setPosition(namePoitionX -70, namePoitionY -20);
		pieChartHeading.setFillColor(sf::Color::Yellow);

		const int basePointX = 150;
		const int basePointY = window.getSize().y - 100;

		//YXAXIS & XAXIS
		sf::VertexArray yline(sf::Lines, 10);
		sf::VertexArray xline(sf::Lines, 10);

		int nextPositionX = basePointX;
		int nextPositionY = basePointY;
		for (int i = 0; i < 6; i++)
		{
			yline[i].position = sf::Vector2f(basePointX, nextPositionY);
			yline[i].color = sf::Color::Red;
			xline[i].position = sf::Vector2f(nextPositionX, basePointY);
			xline[i].color = sf::Color::Red;

			nextPositionX += 90;
			nextPositionY -= 70;
		}

		//COLORSARRAY
		std::array<sf::Color, 8> groupColors= { sf::Color(255,165,0), sf::Color::Red, sf::Color::Blue, sf::Color::Yellow,
		sf::Color::Cyan, sf::Color::Magenta, sf::Color::White, sf::Color::Green };

		//YAXIS & XAXIS LABELS
		sf::String yAxisLabels[] = { "0", "1", "2", "3", "4", "5", "" };
		int yAxisLabelY = basePointY;
		int xAxisLabelY = basePointX - 20;

		sf::String names[] = { topFive[0].name, topFive[1].name, topFive[2].name, topFive[3].name, topFive[4].name, "" };
		int yAxisLabelX = basePointY + 10;
		int xAxisLabelX = basePointX + 35;

		int nextLabelPosX = 80;
		int nextLabelPos = 60;

		std::vector<sf::Text> xOutputs(6);
		std::vector<sf::Text> yOutputs(6);
		for (int labelIndex = 0; labelIndex < 6; labelIndex++)
		{
			createLabels(yOutputs[labelIndex], xAxisLabelY, yAxisLabelY, font, yAxisLabels[labelIndex], groupColors[6]);
			createLabels(xOutputs[labelIndex], xAxisLabelX, yAxisLabelX, font, names[labelIndex], groupColors[labelIndex]);

			yAxisLabelY -= nextLabelPos;
			xAxisLabelX += nextLabelPosX;
		}

		//BAR GRAPHS
		std::vector<sf::RectangleShape> barchart(5);
		float width = 40;
		int barSize = 80;
		float height;
		nextPositionX = basePointX + 20;

		for (int barsIndex = 0; barsIndex < 5; barsIndex++)
		{
			height = calcHeight(topFive[barsIndex].score);
			createBars(barchart[barsIndex], nextPositionX, basePointY, width, height, groupColors[barsIndex], barSize);
		}

		//PIE CHART
		sf::CircleShape pieChart(180);
		pieChart.setPosition(window.getSize().x / 2 -180, window.getSize().y / 2 -180);

		if (currentPlayer.score == 5)
		{
			pieChart.setFillColor(sf::Color::Green);
		}
		else
		{
			pieChart.setFillColor(sf::Color::Red);
		}

		sf::ConvexShape pieSlice;
		int centerX = window.getSize().x / 2;
		int centerY = window.getSize().y / 2;
		int radius = pieChart.getRadius();
		int nextPointX = centerX + radius;
		int nextPointY = centerY;
		float nextPoint = 0;
		int nextConvexPosition = 2;
		float angle = 0;
		angle = (360/5) * currentPlayer.score;
		float currentPlayersNextPoint = angle / 16;
		pieSlice.setPointCount(18);
		pieSlice.setPoint(0, sf::Vector2f(centerX, centerY));
		pieSlice.setPoint(1, sf::Vector2f(nextPointX, nextPointY));
		while (nextPoint < angle -1)
		{
			nextPoint += currentPlayersNextPoint;
			float radianSubangle = nextPoint * (3.142 / 180);
			nextPointX = centerX + radius * cos(radianSubangle);
			nextPointY = centerY + radius * sin(radianSubangle);
			pieSlice.setPoint(nextConvexPosition, sf::Vector2f(nextPointX, nextPointY));
			nextConvexPosition++;
		}

		pieSlice.setFillColor(sf::Color::Green);
		pieSlice.setOutlineColor(sf::Color::Black);
		pieSlice.setOutlineThickness(3);

		int windowDisplay = 0;
		//DISPLAY WINDOWS
		while (window.isOpen())
		{
			// check if the close window button is clicked on.
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::MouseButtonReleased)
				{
					if (windowDisplay == 0)
						windowDisplay = 1;

					else if (windowDisplay == 1)
						windowDisplay = 0;
				}
			}

			window.clear();
			
			if (windowDisplay == 0)
			{
				window.draw(text);
				window.draw(yline);
				for (int labels = 0; labels < 6; labels++)
				{
					window.draw(yOutputs[labels]);
					window.draw(xOutputs[labels]);
				}
				window.draw(xline);
				window.draw(text1);
				window.draw(text3);
				for (int barsIndex = 0; barsIndex < 5; barsIndex++)
				{
					window.draw(barchart[barsIndex]);
				}
			}
			else if (windowDisplay == 1)
			{
				window.draw(pieChartHeading);
				window.draw(pieChart);
				if (currentPlayer.score < 5 && currentPlayer.score > 0)
				{
					window.draw(pieSlice);
				}
				window.draw(text4);
				window.draw(text5);
			}
			window.display();
		}
	}
	

	void createLabels(sf::Text& text, int xpos, int ypos, sf::Font& font, sf::String& string, sf::Color& color)
	{
		text.setPosition(xpos, ypos);
		text.setFont(font);
		text.setString(string);
		text.setCharacterSize(20);
		text.setFillColor(color);
	}

	int calcHeight(int input)
	{
		return input * 60;
	}

	void createBars(sf::RectangleShape& rec, int& xpos, int ypos, int width, int height, sf::Color& color, int num)
	{
		rec.setPosition(xpos, ypos);
		rec.setSize(sf::Vector2f(width, -height));
		rec.setFillColor(color);
		xpos += num;
	}

};