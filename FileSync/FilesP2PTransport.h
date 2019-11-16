#pragma once
#include "stdafx.h"
#include "libtorrent\create_torrent.hpp"
#include "libtorrent\file_storage.hpp"
#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/torrent_info.hpp"
#include "libtorrent/storage.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/announce_entry.hpp"
#include "libtorrent/bdecode.hpp"
#include "libtorrent/magnet_uri.hpp"
#include "libtorrent/torrent_status.hpp"

#include <functional>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cinttypes>
#ifdef TORRENT_WINDOWS
#include <direct.h> // for _getcwd
#endif

using namespace std::placeholders;



/*
Library that using libtorrent 1.2.1 for p2p 
main functions are:
creating torrent files that allows to copy
file through network by p2p protocol
*/




class FilesP2PTransport
{
public:
	FilesP2PTransport();
	~FilesP2PTransport();



	// create torrent file in which all files that needs to be copied
	// due synchronization
	void CreateSyncTorrentFile(string srcDir,string tFN);
	// session type dethermines what type of session gonna be created
	// if true, then its seeding
	// if false, the its downloading
	// dir - where or from files gonna be saved/taken
	// torrentFileName - is pretty self explanatory
	void addTorrentSessionForSync(bool sessionType, string dir, string torrentFileName);

	void PrintInforAboutTorrentFile(string torrentFileName);

	void printCurrentSessionsState();
private:
	lt::session fptSession;

	lt::torrent_handle thSeed;
	lt::torrent_handle thDownload;

};

