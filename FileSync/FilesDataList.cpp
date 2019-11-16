#include "stdafx.h"
#include "FilesDataList.h"

FilesDataList::FilesDataList() {
	fileSearchDir = "E:\\test\\";

}
FilesDataList::FilesDataList(string fsd="E:\\test\\")
{
	fileSearchDir = fsd;
}


wstring s2ws(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void printSystemTime(string msg,const  SYSTEMTIME st) {
	const auto w2 = std::setw(2);
	cout <<msg <<std::setfill('0') << w2 << st.wDay
		<< '.' << w2 << st.wMonth << '.' << std::setw(4) << st.wYear << ' ' << ' ' << w2 << st.wHour
		<< ':' << w2 << st.wMinute << ':' << w2 << st.wSecond << endl;
}

FilesDataList::~FilesDataList()
{
}

int FilesDataList::getFileNames() {

	for (const auto & entry : fs::recursive_directory_iterator(fileSearchDir))
	{
		fileNames.push_back(entry.path().string());
	}

	if (fileNames.size() > 0)
		return fileNames.size();
	else
		return -1;
}

void FilesDataList::printFileNames() {
	for (auto const & fn : fileNames) {
		std::cout << fn<<endl;
	}

}

int FilesDataList::getFilesInformation() {
	wstring fnw;
	for (auto const & fn : fileNames) {
		WIN32_FILE_ATTRIBUTE_DATA fAttr;
		fnw = s2ws(fn);
		GetFileAttributesEx(fnw.c_str(), GetFileExInfoStandard,&fAttr);
		fileAttributes.push_back(fAttr);
		FileInformation fi;
		fi.fileName = fn;
		fi.fileAttributes = fAttr;
		filesInformation.push_back(fi);
	}
	if (fileAttributes.size() > 0)
		return fileAttributes.size();
	else
		return -1;
}

void FilesDataList::printFileInformation() {
	for (auto const & fi : filesInformation) {
		cout << "filename:" << fi.fileName ;
		if (fi.fileAttributes.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			cout << "\t DIRECTORY" << endl;
		}
		else
		{
			cout << " size:" << fi.fileAttributes.nFileSizeLow << " bytes" << endl;
		}
		SYSTEMTIME st;


		FILETIME ft = fi.fileAttributes.ftCreationTime;
		FileTimeToLocalFileTime(&ft, &ft);
		FileTimeToSystemTime(&ft, &st);
		printSystemTime("  CreationTime:\t", st); 
		ft = fi.fileAttributes.ftLastAccessTime;
		FileTimeToLocalFileTime(&ft, &ft);
		FileTimeToSystemTime(&ft, &st);
		printSystemTime("LastAccessTime:\t", st);
		ft = fi.fileAttributes.ftLastWriteTime;
		FileTimeToLocalFileTime(&ft, &ft);
		FileTimeToSystemTime(&ft, &st);
		printSystemTime(" LastWriteTime:\t", st);

		/* heap broke
		memory leak,  after calling getdate gettime format
		
		possible cause:
		using of wchar, but that doesn't make much sense
		some bug in those methods

		LPWSTR szLocalDate, szLocalTime;
		szLocalDate = new wchar_t(100);
		szLocalTime = new wchar_t(100);
		GetDateFormatW(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL,
			szLocalDate, 10);
		GetTimeFormatW(LOCALE_USER_DEFAULT, 0, &st, NULL, szLocalTime, 10);
		cout << "Last edited:   " << *szLocalDate << ":" << *szLocalTime << endl;*/
		


		
	}
}

string FilesDataList::getFSD()
{
	return fileSearchDir;
}

void FilesDataList::setFSD(string FSD)
{
	// TODO: add checks for errors, correct path and etc.
	fileSearchDir = FSD;

}
