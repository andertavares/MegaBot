# MegaBot
Proof-of-concept StarCraft bot to test strategy selection. Currently it incorporates Skynet, Xelnaga and NUSBot.

Authors:
- Anderson Rocha Tavares
- Amanda Santos
- Hector Azpurua
- Luiz Chaimowicz

## Requirements to run
- StarCraft patched to 1.16.1
- BWAPI 3.7.4 (https://github.com/bwapi/bwapi/releases/tag/v3.7.4) -- it also works with BWAPI 3.7.5 (https://github.com/bwapi/bwapi/releases/tag/v3.7.5)

## Requirements to build
### Software
- Visual C++2008 with SP 1 (dowload link: http://download.microsoft.com/download/E/8/E/E8EEB394-7F42-4963-A2D8-29559B738298/VS2008ExpressWithSP1ENUX1504728.iso)
- Visual C++2010 (download link: http://download.microsoft.com/download/1/D/9/1D9A6C0E-FC89-43EE-9658-B9F0E3A76983/vc_web.exe)
- Visual Studio 2012 (download link: https://www.microsoft.com/en-us/download/details.aspx?id=34673)
- Boost 1.60.0 (https://sourceforge.net/projects/boost/files/boost-binaries/1.60.0/). There are many versions,  boost_1_60_0-msvc-9.0-32.exe is recommended;
-- NOTE: if you download boost from link above, it is possible that instead of a BOOST\_DIR/lib directory you get something like BOOST\_DIR/lib32-msvc-9.0. In this case, rename lib32-msvc-9.0 to lib (BOOST_DIR is the directory where your Boost is located). Otherwise, you'll get a link error.

## Instructions
### Environment setup
- Unpack BWAPI to c:\software\BWAPI (or other directory without spaces in the name)
- Install Visual C++2008 with SP1, 2010 and Visual Studio 2012 (all three are needed and VC++2008 without SP1 won't build the code)
- Install Boost 1.60.0 to c:\software\boost (or other directory without spaces)
- Rename C:\software\boost\_1\_60\_0\lib32-msvc-9.0 to C:\software\boost\_1\_60\_0\lib

Configure two environment variables:
- BWAPI_DIR pointing to your BWAPI directory
- BOOST_DIR pointing to your Boost directory


### Building
- Open MegaBot/MegaBot.v11.sln in VS 2012 (not MegaBot.sln or MegaBot.v10.sln)
- Make sure all projects will be built with VC++2008 (v90) toolset
- Set the configuration to 'Release'
- Hit "build" and wait for some time...
- At the end, MegaBot.dll is available at MegaBot/Release.

## Explanations
### Why three versions of Visual Studio are needed?
[explanations are coming]
