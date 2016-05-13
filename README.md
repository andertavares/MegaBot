# MegaBot
Proof-of-concept StarCraft bot to test strategy selection. Currently it incorporates Skynet, Xelnaga and NUSBot.

## Requirements
### Software to build
- BWAPI 3.7.5 (https://github.com/bwapi/bwapi/releases/tag/v3.7.5)
- Visual Studio 2012 
- Visual C++2008 toolset (its weird that project needs VS 2012 with 2008 toolset but that's the way it is...)
- Boost 1.60.0 (http://iweb.dl.sourceforge.net/project/boost/boost-binaries/1.60.0/). There are actually many versions, one that particularly worked for Visual C++2008 toolset was boost_1_60_0-msvc-9.0-32.exe

### Environment variables
- BWAPI_DIR pointing to BWAPI 3.7.5 directory
- BOOST_DIR pointing to Boost directory


## Building
- Open MegaBot/MegaBot.sln in VS 2012
- Make sure all projects will be built with VC++2008 (v90) toolset
- Set the configuration to 'Release'
- Hit "build" and wait for some time...
- At the end, MegaBot.dll is available at MegaBot/Release.
