========================================================================
    CONSOLE APPLICATION : FileSync Project Overview
========================================================================

AppWizard has created this FileSync application for you.

This file contains a summary of what you will find in each of the files that
make up your FileSync application.


FileSync.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

FileSync.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

FileSync.cpp
    This is the main application source file.

Configurator.h Configurator.cpp
    This are files, that contains class which provides all interactions with xml
    files. It reads data from xml file, create a new one xml file if it doesn't exist.
    Also it can retrieve data from user and add it to xml file.
    That specific xml file (name by default is config.xml), is a configuration file.
    It determines what actions application should apply.

FilesDataList.h FilesDataList.cpp 
    This files contains class, that search all files in specified directory,
    get all file information about them, such as filename, file attributes, creation datetime 
    and etc.

FilesP2PTransport.h FilesP2PTransport.cpp
    This files contains class which provides some functions for bittorent client.
    This functions are: creating a new session that seed/download files, display a session
    status, print information about torrent file.
    Also it has a function that creates a new torrent files, from specified directory.

FileSynchronizer.h  FileSynchronizer.cpp
    This files contains class, which is a main application class, and provides all 
    interactions between instances of other classes that listed above.
    It creates an instance of "Configurator" class,that reads configuration data, and
    selects further actions accordingly to that retrieven data.
    It has an instance of "FilesP2PTransport" class, that class creates new sessions, 
    creates torrent file from specified directory, prints status of current sessions.
    
/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named FileSync.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
