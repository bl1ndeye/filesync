#include "stdafx.h"
#include "Configurator.h"

bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

Configurator::Configurator()
{
	configFileName = "config.xml";
	pugi::xml_parse_result result = doc.load_file(configFileName.c_str());
	if (result) {
		for (pugi::xml_node node = doc.child("node"); node; node = node.next_sibling("node"))
		{
			string nodetype = node.attribute("nodeType").as_string();
			 transportType = node.attribute("transportType").as_string();
			 if (nodetype == "SRC")
			{
				 SRCdir= node.attribute("directory").as_string();
				 outputTorrentFileName = node.attribute("torrentFileName").as_string();
			}
			 else
				 {
					 DSTdir = node.attribute("directory").as_string();
					 inputTorrentFileName = node.attribute("torrentFileName").as_string();
				 }

		}
	}
	else
	{
		SRCdir = "empty";
		DSTdir = "empty";
		transportType = "empty";
		outputTorrentFileName = "out.torrent";
		inputTorrentFileName = "in.torrent";

		cout << "XML [" << configFileName << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
		cout << "Error description: " << result.description() << "\n";
		cout << "Error offset: " << result.offset << " (error at [..." << (configFileName.c_str() + result.offset) << "]\n\n";

		cout << "If configuration file config.xml doesn't exist in working directory" << endl;
		cout << "Then you probably should run reconfiguration, or add it manually" << endl;
	}

}


Configurator::~Configurator()
{
}

void Configurator::addFileSyncConfiguration()
{
	

	string nodetype;
	while (nodetype != "SRC" && nodetype != "DST") {
		cout << "Enter node type (SRC or DST)" << endl;
		cin >> nodetype;
	};
	while (transportType != "system" && transportType != "torrent")
	{
		cout << "Enter transport type (system or torrent)" << endl;
		cin >> transportType;
	};
	string directory;
	int dircheck=0;
	do
	{
		if (dircheck > 0)
			cout << "Directory not found." << endl;
		cout << "Enter directory. Example: d:\\temp\\, c:\\test\\" << endl;
		cin >> directory;
		dircheck++;
	} while (!dirExists(directory));
	string torrentFileName;
	if (transportType == "torrent") {
		cout << "Enter torrent file name" << endl;
		cout << "(if path not specified, then torrent file created in working directory, alongside executable file)" << endl;
		while (torrentFileName == "") {
			cin >> torrentFileName;
		}
	}


	pugi::xml_parse_result result = doc.load_file(configFileName.c_str());
	pugi::xml_node node = doc.append_child("node");
	if (result)
	{
		
		
		node.append_attribute("nodeType");
		node.attribute("nodeType").set_value(nodetype.c_str())  ;
		node.append_attribute("transportType");
		node.attribute("transportType").set_value(transportType.c_str());
		node.append_attribute("directory");
		node.attribute("directory").set_value(directory.c_str());
		if (transportType == "torrent")
			{
				node.append_attribute("torrentFileName");
				node.attribute("torrentFileName").set_value(torrentFileName.c_str());
			}
		doc.save_file(configFileName.c_str());

	}
	else
		{
		node.append_attribute("nodeType");
		node.attribute("nodeType").set_value(nodetype.c_str());
		node.append_attribute("transportType");
		node.attribute("transportType").set_value(transportType.c_str());
		node.append_attribute("directory");
		node.attribute("directory").set_value(directory.c_str());
		if (transportType == "torrent")
		{
			node.append_attribute("torrentFileName");
			node.attribute("torrentFileName").set_value(torrentFileName.c_str());
		}
		doc.save_file(configFileName.c_str());
		}

}
