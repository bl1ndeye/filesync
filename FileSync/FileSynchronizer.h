#pragma once
#include "FilesDataList.h"
#include "FilesP2PTransport.h"
#include "Configurator.h"
#include <sys/stat.h>
class FileSynchronizer
{
public:
	FileSynchronizer();
	~FileSynchronizer();

	// find all missing files that doesn't exist 
	// in DST directory , which must  be copied
	// due to synchronization
	int FindDiff();

	// init lists of files information
	void fdlInit();


	// synchronization by copying files
	// using WIN API function CopyFile
	int Synchronize();



	// begin torrent sync
	int beginTorrentSynchronization();

private:
	// files data
	// src- what gonna be synchronized
	// dst - destination, where files should be copied
	FilesDataList fdlSRC;
	FilesDataList fdlDST;
	//diff - differencies between src and dst
	FilesDataList fdlDiff;


	// p2p client
	FilesP2PTransport fptClient;
	// find and compare specified file information object with objects in list
	// if there exist an exact copy of that object
	// this is basically mean that corresponding file exists
	// and no need in synchronizing
	// return true if found
	// false otherwise
	bool findFIinList(FileInformation fi, list <FileInformation> fiList);

	Configurator conf;

};

