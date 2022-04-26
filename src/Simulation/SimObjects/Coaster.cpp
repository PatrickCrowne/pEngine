#include "Coaster.h"
#include "../../glstate.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "../AssetReader.h"
/// <summary>
/// Constructs a new coaster class
/// </summary>
Coaster::Coaster(std::string nodes, std::string style, Scene* scene)
{

	std::cout << "lol\n";

	// Configure the style
	parseStyle(style);


	// Create a new track spline from the node list
	TrackSpline* trackSpline = new TrackSpline(nodes, coasterStyle);

	

	// Add a new train
	Train *train = new Train(20, 6, trackSpline);
	scene->registerSimObject(train);
	trains.push_back(train);

}

Coaster::~Coaster()
{

}

void Coaster::parseStyle(std::string path) {

	std::ifstream simObjectConfigFileStream;
	simObjectConfigFileStream.open(path);

	coasterStyle = new CoasterStyle();

	std::string buffer;
	// Read all the lines of the material configuration file
	while (getline(simObjectConfigFileStream, buffer)) {

		if (buffer._Starts_with("RAIL")) {
			
			// TODO: implement this
			struct railPos newRailPos;
			newRailPos.offset = AssetReader::getVector(buffer);
			newRailPos.radius = AssetReader::getFloat(buffer.substr(buffer.find_last_of(" ")));
			coasterStyle->railPos.push_back(newRailPos);
			continue;
		}
		
		if (buffer._Starts_with("TIE")) {
			coasterStyle->crosstieModel = AssetReader::getString(buffer);
			continue;
		}

		if (buffer._Starts_with("FRONTCAR")) {
			coasterStyle->frontCar = AssetReader::getString(buffer);
			continue;
		}

		if (buffer._Starts_with("MIDDLECAR")) {
			coasterStyle->middleCar = AssetReader::getString(buffer);
			continue;
		}

	}

}
