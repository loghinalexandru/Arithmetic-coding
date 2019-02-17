#include "arc.h"
#include <fstream>
#include <string.h>

class PPM: public ArithmeticCoding{

	public:

	void setProbability(){
		for(std::map<char,double>::iterator it = this->probabilities.begin(); it != this->probabilities.end(); ++it){
			it->second /= this->inputCount;
		}
	}

	void setRange(){
		double sum = 0;
		double low = 0;
		double high = 0;
		for(std::map<char,std::pair<double , double>>::iterator it = this->rangeLowHigh.begin(); it != this->rangeLowHigh.end(); ++it){
			low = high;
			high += this->probabilities[it->first] / this->inputCount;
			it->second = std::pair<double , double>(low , high);
		}
	}

    void update(std::string input){
		this->inputCount += input.length();
		for(int i = 0; i < input.length(); ++i){
			if(probabilities.find(input[i]) != probabilities.end()){
				probabilities[input[i]] += 1;
			}
			else{
				rangeLowHigh[input[i]] = std::pair<double , double>(0 , 0);
				probabilities[input[i]] = 1;
			}
		}
	}

	bool isInRange(double low , double high , double input){
		if(low <= input && input <= high)
			return true;
		return false;
	}

	char* decode(double input , int inputSize){
		char output[inputSize+1];
		memset(output, 0, inputSize + 1);
		unsigned int index = 0;
		std::pair<double , double> currentRange;
		std::pair<double , double> previousRange = {0.0,1.0};
		for(int i = 0; i < inputSize; ++i){
			for(std::map<char,std::pair<double , double>>::iterator it = this->rangeLowHigh.begin(); it != this->rangeLowHigh.end(); ++it){
				currentRange.first = previousRange.first + (previousRange.second - previousRange.first) * it->second.first;
				currentRange.second = previousRange.first + (previousRange.second - previousRange.first) * it->second.second;
				if(isInRange(currentRange.first , currentRange.second , input)){
					output[index] = it->first; 
					index++;
					previousRange = currentRange;
					break;
				}
			}
		}
		std::cout << output;
		return output;
	}

	double encode(std::string input) {
		std::pair<double , double> currentRange;
		std::pair<double , double> previousRange = {0.0,1.0};
		for(int i = 0; i < input.length(); ++i){
			std::pair<double , double> range = this->rangeLowHigh[input[i]];
			currentRange.first = previousRange.first + (previousRange.second - previousRange.first) * range.first;
			currentRange.second = previousRange.first + (previousRange.second - previousRange.first) * range.second;
			previousRange = currentRange;
		}

		return (currentRange.first + currentRange.second) / 2;
	}
};

int main(int argc , char* argv[]){
	PPM compressionAlg = PPM();
	std::ifstream in;
	std::string buffer;

	if(argc < 2){
		std::cout << "Need a file to encode!";
		return 0;
	}
	in.open(argv[1]);
	while(getline(in , buffer)){
		compressionAlg.update(buffer);
	}
	in.close();
	compressionAlg.setRange();
	double compression = compressionAlg.encode("int main");
	int size = strlen("int main");
	std::cout << compression << " " << compressionAlg.decode(compression , size);
	return 0;
}
