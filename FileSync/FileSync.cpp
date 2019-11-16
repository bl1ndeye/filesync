// FileSync.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileSynchronizer.h"

int main()
{
	setlocale(LC_CTYPE, "rus");
	FileSynchronizer FS;
	FS.beginTorrentSynchronization();

	system("pause");
    return 0;
}

