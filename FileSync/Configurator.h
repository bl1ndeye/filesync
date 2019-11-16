#pragma once
#include "stdafx.h"
#include "pugy\pugixml.hpp"
#include <sys/types.h>
#include <sys/stat.h>
class Configurator
{
public:
	Configurator();
	~Configurator();

	// if config file not exists
	// this function would create it
	// afterwards it initiates dialog with user 
	// requesting data for an first/additional filesync node
	// when dialog ends, recieved data gonna be written to file
	void addFileSyncConfiguration();

	// source directory
	//
	string SRCdir;
	string DSTdir;
	string transportType;
	// filename of torrent file 
	// that created during process of
	// synchronization from specified
	// source directory
	string outputTorrentFileName;
	// filename of torrent file that 
	// contains data about files 
	// which are must be downloaded to 
	// complete synchronization
	string inputTorrentFileName;

	
private:
	// default value is config.xml
	string configFileName;
	// variable for xml document
	pugi::xml_document doc;
};

