This project contains the RenderDebug plugin DLL written by
John W. Ratcliff [mailto:jratcliffscarab@gmail.com](mailto:jratcliffscarab@gmail.com)

BITCOIN TIP JAR: "1BT66EoaGySkbY9J6MugvQRhMMXDwPxPya"

The purpose of this plugin is to allow you to add debug visualization
to any project, whether it's a console application or a full game
engine.

Currently the RenderDebug plugin only works on as a Windows DLL but
I may consider building it as a 'shared-object' library on Linux
one of these days.

The source code to the plugin is not provided at this time.  It may
be made available at some point in the future, but not currently.

No source code is provided for the debug visualization viewer
application either and probably will not be.

What this system let's you do is call a simple suite of debug
visualization methods that can be viewed in a seperate application.

This means that even a simple console application which has not graphics
capabilities whatsoever can still have rich visualization.

Another use case is for server applications which might be doing a
great deal of complex simulation but, typically, have no graphics
capabilities.

With this initial release on May 15, 2015, the RenderDebug plugin
simply records a data stream to disk (typically a .rec file).

In a future release, probably quite soon, it will also allow you to
use the DebugView visualization app as a real-time graphics visualization
server.

What this means is you will be able to run a console application which
has not graphics capabilities but in real-time on the DebugView tool
be able to interact and display content.

You will even be able to pass commands messages between the two applications.

There are three demonstrations for this application.

**ObjView**
**This is a console application which reads in a Wavefront.obj and uses
the RenderDebug plugin to stream a debug visualization of that data
to af ile on disk which you can then load with the DebugView application.**

**TestRenderDebug**
**This is a simple console application which creates a 500 frame debug
visualization that you can review the DebugView application.  This will
be the main 'test' application once real-time connectivity is implemented
and working.**

**BlockChain.rec**
**This is a recording created from a console bitcoin blochain analsysis tool
that contains a graph of the distribution of bitcoin value for every single
data for the past four and a half years.  You can run DebugView and then
import this file to view it.**



Directories

app/ObjView             : Constains source code to the WaveFront OBJ viewer demonstration application.

app/TestRenderDebug     : Contains source code for the demonstration debug visualization console app.

bin/win32				: Contains the 32 bit run-time binaries

bin/win64				: Contains the 64 bit run-time binaries

compiler/vc10win32      : Contains the Visual Studio 2010 solution and project files for the samples for 32 bit configuration

compiler/vc10win64      : Contains the Visual Studio 2010 solution and project files for the samples for 64 bit configuration

compiler/vc11win32      : Contains the Visual Studio 2012 solution and project files for the samples for 32 bit configuration

compiler/vc11win64      : Contains the Visual Studio 2012 solution and project files for the samples for 64 bit configuration

compiler/xpj			: Contains the project-builder scripts which automatically produce the visual studio solution and project files

media					: Contains sample media needed to run the DebugView application.

media/fonts				: Contains font data

media/materials			: Contains material definitions

media/recordings		: Contains the sample recording file 'BlockChain.rec'

media/shaders         	: Contains shader scripts for the DebugView application.

media/textures			: Contains textures used for the DebugView application

public/RenderDebug		: Contains the public API header for the RenderDebug plugin and the run-time binding code snippet
