#pragma once
class FileInformation {
public:
	WIN32_FILE_ATTRIBUTE_DATA fileAttributes;
	string fileName;
};


// class that contains information about all files in selected directory
// such as filenames, size, datetime of creation and etc.
class FilesDataList
{
public:
	FilesDataList(string fsd);
	FilesDataList();
	~FilesDataList();

	// get all filenames in specified directory (specified by attr file search directory) 
	int getFileNames();
	// print all filenames
	void printFileNames();
	// get file information for files that enumerated in filenames list
	int getFilesInformation();
	// print file infromation
	void printFileInformation();
	// get fileSearchDir
	string getFSD();
	// set fileSearchDir
	void setFSD(string FSD);

private:
	// path to selected directory
	// it will be scanned recursively for all files
	// in directories and subdirectories
	string fileSearchDir;
	list <WIN32_FILE_ATTRIBUTE_DATA> fileAttributes;
	list <string> fileNames;
public:
	list <FileInformation> filesInformation;
};

