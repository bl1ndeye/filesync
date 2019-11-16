#include "stdafx.h"
#include "FileSynchronizer.h"

// comparison for WIN32

bool operator==(const WIN32_FILE_ATTRIBUTE_DATA & l, const WIN32_FILE_ATTRIBUTE_DATA & r)
{
	if ((l.dwFileAttributes==r.dwFileAttributes) && (l.nFileSizeLow==r.nFileSizeLow))
	return true;
	
	return false;
}

bool fileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}



FileSynchronizer::FileSynchronizer()
{
	fdlSRC.setFSD("E:\\test\\");
	fdlDST.setFSD("E:\\test1\\");
}


FileSynchronizer::~FileSynchronizer()
{
}

int FileSynchronizer::FindDiff()
{

	if (fdlDST.filesInformation.size() > 0) {

		for (auto const fiSRC : fdlSRC.filesInformation) {
			string fnSRC = fiSRC.fileName;
		
			if (!findFIinList(fiSRC, fdlDST.filesInformation))
			{
				fdlDiff.filesInformation.push_back(fiSRC);
				
			}
		}
		if (fdlDiff.filesInformation.size() == 0) {
			cout << "Directories are currently sycnhronized" << endl;
		};
		return fdlDiff.filesInformation.size();
	}
	else
	{
		for (auto const fiSRC : fdlSRC.filesInformation) {
			fdlDiff.filesInformation.push_back(fiSRC);
		}
		
		return fdlDiff.filesInformation.size();
	}
	
	return 0;
}

bool FileSynchronizer::findFIinList(FileInformation fiCompare, list<FileInformation> fiList)
{
	// path to files need to be trimmed
	string lSRCtrim = fdlSRC.getFSD();
	string lDSTtrim = fdlDST.getFSD();
	fiCompare.fileName.erase(0, lSRCtrim.size());
	for ( auto f : fiList)
	{
		f.fileName.erase(0, lDSTtrim.size());
		if (f.fileName == fiCompare.fileName)
		{
			if (f.fileAttributes == fiCompare.fileAttributes) {
				return true;
			}
		}
	}

	return false;
}

void FileSynchronizer::fdlInit()
{
	// TODO: add checks for errors
	fdlSRC.getFileNames();
	fdlSRC.getFilesInformation();
	fdlDST.getFileNames();
	fdlDST.getFilesInformation();
	

	
}

int FileSynchronizer::Synchronize()
{
	string lSRCprefix = fdlSRC.getFSD();
	string lDSTprefix = fdlDST.getFSD();
	int copycount=0;
	for (const auto fi : fdlDiff.filesInformation)
	{
		string fn = fi.fileName;
		fn.erase(0, lSRCprefix.size());
		string srcfn = lSRCprefix + fn;
		string dstfn = lDSTprefix + fn;
		if (CopyFileA(srcfn.c_str(), dstfn.c_str(), true))
			copycount++;
	}

	return copycount;
}



int FileSynchronizer::beginTorrentSynchronization()
{
	int sessCount=0;
	if (conf.SRCdir != "" && conf.transportType=="torrent") {
		
		if (!fileExists(conf.outputTorrentFileName)) {

		cout << "Creating torrent file from specified source directory" << endl;
		fptClient.CreateSyncTorrentFile(conf.SRCdir,conf.outputTorrentFileName);
		}

		cout << "Creating session to seed files" << endl;
		fptClient.addTorrentSessionForSync(true,conf.SRCdir,conf.outputTorrentFileName);
		sessCount++;
	}
	if (conf.DSTdir != "" && conf.transportType == "torrent") {
		cout <<" Creating session to download missing files" << endl;
		fptClient.addTorrentSessionForSync(false, conf.DSTdir, conf.inputTorrentFileName);
		sessCount++;
	}
	

	cout << "Printing status of sessions" << endl;
	fptClient.printCurrentSessionsState();
	return sessCount;
}
