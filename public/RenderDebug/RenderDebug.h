#ifndef RENDER_DEBUG_H
#define RENDER_DEBUG_H

#include <stdint.h>

/*!
\file
\brief debug rendering classes and structures
*/

#define RENDER_DEBUG_VERSION 102

namespace RENDER_DEBUG
{

/**
\brief Enums for debug colors
 */
struct DebugColors
{
	enum Enum
	{
		Default = 0,
		PoseArrows,
		MeshStatic,
		MeshDynamic,
		Shape,
		Text0,
		Text1,
		ForceArrowsLow,
		ForceArrowsNorm,
		ForceArrowsHigh,
		Color0,
		Color1,
		Color2,
		Color3,
		Color4,
		Color5,
		Red,
		Green,
		Blue,
		DarkRed,
		DarkGreen,
		DarkBlue,
		LightRed,
		LightGreen,
		LightBlue,
		Purple,
		DarkPurple,
		Yellow,
		Orange,
		Gold,
		Emerald,
		White,
		Black,
		Gray,
		LightGray,
		DarkGray,
		NUM_COLORS
	};
};

/**
\brief State flags for debug renderable
 */
struct DebugRenderState
{
	enum Enum
	{
		ScreenSpace      = (1<<0),  //!< true if rendering in screenspace
		NoZbuffer        = (1<<1),  //!< true if zbuffering is disabled.
        SolidShaded      = (1<<2),  //!< true if rendering solid shaded.
        SolidWireShaded  = (1<<3),  //!< Render both as a solid shaded triangle and as a wireframe overlay.
        CounterClockwise = (1<<4),  //!< true if winding order is counter clockwise.
        CameraFacing     = (1<<5),  //!< True if text should be displayed camera facing
        InfiniteLifeSpan = (1<<6),  //!< True if the lifespan is infinite (overrides current display time value)
        CenterText       = (1<<7),  //!< True if the text should be centered.
	};
};

/**
\brief Simple vertex with normal and color
 */
class RenderDebugSolidVertex
{
public:
	float mPos[3];
	float mNormal[3];
	uint32_t mColor;
};

/**
\brief simple vertex with color
 */
class RenderDebugVertex
{
public:

	float mPos[3];
	uint32_t mColor;
};



/**
\brief user provided rendering output methods
 */
class RenderDebugInterface
{
public:
  virtual void debugRenderLines(uint32_t lcount,const RenderDebugVertex *vertices,bool useZ,bool isScreenSpace) = 0;
  virtual void debugRenderTriangles(uint32_t tcount,const RenderDebugSolidVertex *vertices,bool useZ,bool isScreenSpace) = 0;
  virtual void debugMessage(const char *msg) = 0;
  virtual void debugCommand(const char *cmd) = 0;

protected:
	virtual ~RenderDebugInterface(void)
	{

	}
};

/**
\brief The maximum number of graphs that can be displayed at one time.  (0-5)
*/
static const uint32_t MAX_GRAPHS			(6);
/**
\brief The width of the graphs (x axis) when created automatically
*/
static const float GRAPH_WIDTH_DEFAULT	(+0.8f);
/**
\brief The height of the graphs (y axis) when created automatically
*/
static const float GRAPH_HEIGHT_DEFAULT	(+0.4f);

/**
\brief definition of the debugGraph descriptor used to create graphs
*/
struct DebugGraphDesc 
{
	DebugGraphDesc(void) 
	{ 
		mPoints = 0;
		mGraphXLabel = 0;
		mGraphYLabel = 0;
	};


	/**
	\brief The number of float data points to graph
	*/
	uint32_t	mNumPoints;
	/**
	\brief Pointer to the float data points to graph
	*/
	const float*	mPoints;
	/**
	\brief optional cutoff line drawn horizontally on the graph.  It should be between 0 and mGraphMax.
	0.0f indicates not to draw the cutoff line.
	*/
	float	mCutOffLevel;
	/**
	\brief The maximum value that the graph should be be able to display.
	Noramlly this is slightly larger than the greatest value in the mPoints array to make room for
	future samples that are higher then the current set.
	*/
	float	mGraphMax;
	/**
	\brief The bottom left x coordinate of the graph.
	This is set automatically by the constructor that takes graphNum as an argument.
	NOTE: display coordinates range from -1.0f - + 1.0f
	*/
	float	mGraphXPos;
	/**
	\brief bottom left y coordinate of the graph.
	This is set automatically by the constructor that takes graphNum as an argument.
	NOTE: display coordinates range from -1.0f - + 1.0f
	*/
	float	mGraphYPos;
	/**
	\brief The width of the graph.
	This is set automatically by the constructor that takes graphNum as an argument.
	NOTE: display coordinates range from -1.0f - + 1.0f
	*/
	float	mGraphWidth;
	/**
	\brief The height of the graph.
	This is set automatically by the constructor that takes graphNum as an argument.
	NOTE: display coordinates range from -1.0f - + 1.0f
	*/
	float	mGraphHeight;
	/**
	\brief The color of the data.
	This is set automatically by the constructor that takes graphNum as an argument.
	NOTE: display coordinates range from -1.0f - + 1.0f
	This is set automatically by the constructor that takes graphNum as an argument.
	*/
	uint32_t	mGraphColor;
	/**
	\brief The alternate color of the data if mColorSwitchIndex is set to a value that is a valid
	index to the mPoints array.  The points after mColorSwitchIndex are drawn in this color.
	This is set automatically by the constructor that takes graphNum as an argument.
	*/
	uint32_t	mArrowColor;
	/**
	\brief A pointer to the label for the X axis.
	*/
	const char*	mGraphXLabel;
	/**
	\brief A pointer to the label for the Y axis.
	*/
	const char*	mGraphYLabel;
	/**
	\brief The (optional!) index in the data set at which to switch the color to the color specified by
	mArrorColor.  By default this is set to -1 indicating that no color switch should be performed.
	*/
	uint32_t	mColorSwitchIndex;

};


/**
\brief class that draws debug rendering primitives
 */
class RenderDebug
{
public:
	enum RunMode
	{
		RM_SERVER,		// running as a server; there can only be one server active at a time; a server can receive data from up to 4 clients and the server is responsible for displaying it.
		RM_CLIENT,		// This is a client which intends to get rendering services from a server; if the server is not found then no connection is created.
		RM_LOCAL,		// Just uses the render-debug library for rendering only; disables all client/server communications; just lets an app use it as a debug renderer.
		RM_CLIENT_OR_FILE, // Runs as a client which connects to a server (if found) or, if no server found, then writes the output to a file that the server can later load and replay
		RM_FILE,		// Run's *only* as a file; never tries to connect to a server.
	};

	class Desc
	{
	public:
		Desc(void)
		{
			// default name; you should assign this based on your build configuration x86 or x64 
			// and use debug if you want to debug at the source level.  Also, if your DLL isn't 
			// in the current working directory; you can use a fully qualified path name here.
			dllName = "RenderDebug_x86.dll"; 
			versionNumber = RENDER_DEBUG_VERSION;
			runMode = RM_LOCAL;
			recordFileName = "RenderDebug.rec";
			errorCode = 0;
			echoFileLocally = false;
		}
		const char *dllName;		// Name of the render-debug DLL to load.
		uint32_t	versionNumber;	// expected version number; if not equal then the DLL won't load.
		RunMode		runMode;		// startup mode
		const char *recordFileName;	// If running in 'file' mode, this is the name of the file on disk the data will be rendering data will be recorded to.
		const char *errorCode;		// If it failed to create the render-debug system; this will contain a string explaining why.
		bool		echoFileLocally;	// Is recording to a file, do we also want to echo the debugging commands locally (for both local render *and* record file at the same time).
	};

	virtual uint32_t getUpdateCount(void) const = 0;

    virtual bool render(float dtime,RenderDebugInterface *iface) = 0;

	virtual void debugGraph(uint32_t numPoints, float * points, float graphMax, float graphXPos, float graphYPos, float graphWidth, float graphHeight, uint32_t colorSwitchIndex = 0xFFFFFFFF) = 0;

	virtual void  reset(int32_t blockIndex=-1) = 0; // -1 reset *everything*, 0 = reset everything except stuff inside blocks, > 0 reset a specific block of data.

	virtual void  drawGrid(bool zup=false,uint32_t gridSize=40) = 0; // draw a grid.

	virtual void  pushRenderState(void) = 0;

    virtual void  popRenderState(void) = 0;

	virtual void  setCurrentColor(uint32_t color=0xFFFFFF,uint32_t arrowColor=0xFF0000) = 0;
	virtual uint32_t getCurrentColor(void) const = 0;
	virtual uint32_t getCurrentArrowColor(void) const = 0;

	virtual void  setCurrentUserPointer(void *userPtr) = 0;

	virtual void  setCurrentUserId(int32_t userId) = 0;

	virtual void * getCurrentUserPointer(void) const = 0;
	virtual int32_t getCurrentUserId(void) = 0;

	virtual void  setCurrentDisplayTime(float displayTime=0.0001f) = 0;

	virtual float getRenderScale(void) = 0;

	virtual void  setRenderScale(float scale) = 0;

	virtual void  setCurrentState(uint32_t states=0) = 0;

	virtual void  addToCurrentState(DebugRenderState::Enum state) = 0; // OR this state flag into the current state.

	virtual void  removeFromCurrentState(DebugRenderState::Enum state) = 0; // Remove this bit flat from the current state

	virtual void  setCurrentTextScale(float textScale) = 0;

	virtual void  setCurrentArrowSize(float arrowSize) = 0;

	virtual uint32_t getCurrentState(void) const = 0;

	virtual void  setRenderState(uint32_t states=0,  // combination of render state flags
	                             uint32_t color=0xFFFFFF, // base color
                                 float displayTime=0.0001f, // duration of display items.
	                             uint32_t arrowColor=0xFF0000, // secondary color, usually used for arrow head
                                 float arrowSize=0.1f,
								 float renderScale=1.0f,
								 float textScale=1.0f) = 0;      // seconary size, usually used for arrow head size.


	virtual uint32_t getRenderState(uint32_t &color,float &displayTime,uint32_t &arrowColor,float &arrowSize,float &renderScale,float &textScale) const = 0;


	virtual void  endDrawGroup(void) = 0;

	virtual void  setDrawGroupVisible(int32_t groupId,bool state) = 0;

	virtual void debugRect2d(float x1,float y1,float x2,float y2) = 0;

	virtual void  debugPolygon(uint32_t pcount,const float *points, bool doubleSided) = 0;

	virtual void  debugLine(const float p1[3],const float p2[3]) = 0;

	virtual void  debugGradientLine(const float p1[3],const float p2[3],const uint32_t &c1,const uint32_t &c2) = 0;

	virtual void  debugOrientedLine(const float p1[3],const float p2[3],const float transform[16]) = 0;

	virtual void  debugRay(const float p1[3],const float p2[3]) = 0;

	virtual void  debugCylinder(const float p1[3],const float p2[3],float radius) = 0;

	virtual void  debugThickRay(const float p1[3],const float p2[3],float raySize=0.02f) = 0;

	virtual void  debugPlane(const float normal[3],float dCoff,float radius1,float radius2) = 0;

	virtual void  debugTri(const float p1[3],const float p2[3],const float p3[3], bool doubleSided=false) = 0;

	virtual void  debugTriNormals(const float p1[3],const float p2[3],const float p3[3],const float n1[3],const float n2[3],const float n3[3]) = 0;

	virtual void  debugGradientTri(const float p1[3],const float p2[3],const float p3[3],const uint32_t &c1,const uint32_t &c2,const uint32_t &c3) = 0;

	virtual void  debugGradientTriNormals(const float p1[3],const float p2[3],const float p3[3],const float n1[3],const float n2[3],const float n3[3],const uint32_t &c1,const uint32_t &c2,const uint32_t &c3) = 0;

	virtual void  debugBound(const float bmin[3],const float bmax[3]) = 0;

	virtual void  debugOrientedBound(const float sides[3],const float transform[16]) = 0;

	virtual void  debugOrientedBound(const float bmin[3],const float bmax[3],const float pos[3],const float quat[4]) = 0; // the rotation as a quaternion

	virtual void  debugOrientedBound(const float bmin[3],const float bmax[3],const float xform[16]) = 0; // 

	virtual void  debugSphere(const float pos[3],float radius) = 0;

	virtual void  debugOrientedSphere(float radius, uint32_t subdivision, const float transform[16]) = 0;

	virtual void  debugOrientedCapsule(float radius,float height,uint32_t subdivision,const float transform[16]) = 0;

	virtual void  debugOrientedCapsuleTapered(float radius1, float radius2, float height, uint32_t subdivision, const float transform[16]) = 0;

	virtual void  debugOrientedCylinder(float radius,float height,uint32_t subdivision,bool closeSides,const float transform[16]) = 0;

	virtual void  debugOrientedCircle(float radius,uint32_t subdivision,const float transform[16]) = 0;

	virtual void  debugPoint(const float pos[3],float radius) = 0;
	virtual void  debugPoint(const float pos[3],const float scale[3]) = 0;
	virtual void  debugQuad(const float pos[3],const float scale[2],float orientation) = 0;

	virtual void  debugAxes(const float transform[16],float distance=0.1f,float brightness=1.0f,bool showXYZ=false,bool showRotation=false, uint32_t axisSwitch=0) = 0;

    virtual void debugArc(const float center[3],const float p1[3],const float p2[3],float arrowSize=0.1f,bool showRoot=false) = 0;

    virtual void debugThickArc(const float center[3],const float p1[3],const float p2[3],float thickness=0.02f,bool showRoot=false) = 0;

    virtual void debugText(const float pos[3],const char *fmt,...) = 0;

    virtual void debugOrientedText(const float pos[3],const float rot[4],const char *fmt,...) = 0;

    virtual void debugOrientedText(const float xform[16],const char *fmt,...) = 0;

	virtual void setViewMatrix(const float view[16]) = 0;

	virtual void setProjectionMatrix(const float projection[16]) = 0;

	// Sends a debug log message to the remote client/server or recorded to a log file.
	virtual void debugMessage(const char *fmt,...) = 0;

	// Sends a command to the remote client/server
	virtual void debugCommand(const char *fmt,...) = 0;

	virtual const float* getViewProjectionMatrix(void) const = 0;
	virtual const float *getViewMatrix(void) const = 0;
	virtual const float *getProjectionMatrix(void) const = 0;

	virtual void  eulerToQuat(const float angles[3],float q[4]) = 0; // angles are in degrees.

	virtual int32_t beginDrawGroup(const float pose[16]) = 0;

	virtual void  setDrawGroupPose(int32_t blockId,const float pose[16]) = 0;

	virtual void  debugDetailedSphere(const float pos[3],float radius,uint32_t stepCount) = 0;

	virtual void  setPose(const float pose[16]) = 0;

	virtual const float * getPose(void) const = 0;

	/* \brief Create an createDebugGraphDesc.  This is the manual way of setting up a graph.  Every parameter can
	and must be customized when using this constructor.
	*/
	virtual DebugGraphDesc* createDebugGraphDesc(void) = 0;

	virtual void releaseDebugGraphDesc(DebugGraphDesc *desc) = 0;

	/**
	\brief Create an createDebugGraphDesc using the minimal amount of work.  This constructor provides for six custom
	graphs to be simultaneously drawn on the display at one time numbered 0 to 5.  The position, color, and size
	of the graphs are automatically set based on the graphNum argument.
	*/
	virtual DebugGraphDesc* createDebugGraphDesc(uint32_t graphNum,uint32_t dataCount,const float *dataArray, float maxY, char* xLabel, char* yLabel) = 0;

	virtual void debugGraph(const DebugGraphDesc& graphDesc) = 0;

	/**
	\brief Set a debug color value by name.
	*/
	virtual void setDebugColor(DebugColors::Enum colorEnum, uint32_t value) = 0;

	/**
	\brief Return a debug color value by name.
	*/
	virtual uint32_t getDebugColor(DebugColors::Enum colorEnum) const = 0;

	/**
	\brief Debug visualize a view*projection matrix frustum.
	*/
	virtual void debugFrustum(const float viewMatrix[16],const float projMatrix[16]) = 0;

	/**
	\brief Return a debug color value by RGB inputs
	*/
	virtual uint32_t getDebugColor(float red, float green, float blue) const = 0;

	virtual void getDebugBounds(float bmin[3],float bmax[3]) const = 0;

	virtual void setFrameTime(float ftime) = 0;
	virtual float getFrameTime(void) const = 0;

	virtual uint32_t setFilePlayback(const char *fileName) = 0; // returns the number of frames in the playback file; zero if none.

	virtual bool setPlaybackFrame(uint32_t playbackFrame) = 0;
	virtual uint32_t getPlaybackFrameCount(void) const = 0;
	virtual void stopPlayback(void) = 0;

	// provides a single global mutex lock for thread safe access to this debug-render interface
	virtual bool trylock(void) = 0;
	virtual void lock(void) = 0; 
	virtual void unlock(void) = 0;

	virtual void release(void) = 0;

protected:

	virtual ~RenderDebug(void) { };
};

class ScopedRenderDebug
{
public:
	ScopedRenderDebug(RenderDebug *rd)
	{
		mRenderDebug = rd;
		if ( mRenderDebug )
		{
			mRenderDebug->lock();
		}
	}

	~ScopedRenderDebug(void)
	{
		if ( mRenderDebug )
		{
			mRenderDebug->unlock();
		}
	}

	RenderDebug *mRenderDebug;
};

}; // end of namespace

#define SCOPED_RENDER_DEBUG(x) RENDER_DEBUG::ScopedRenderDebug _lockRenderDebug(x)

#endif // RENDER_DEBUG_H
