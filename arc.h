#include <iostream>
#include <string>
#include <map>
#include <vector>

class ArithmeticCoding{

	//Temporary PUBLIC for testing

	public:
		long long inputCount = 0;
		std::map<char , double> probabilities;
		std::map<char , std::pair<double , double>> rangeLowHigh;

	public:
		virtual char* decode(double input , int inputSize) = 0;

		virtual double encode(std::string input) = 0;

		virtual void update(std::string input) = 0;

};
