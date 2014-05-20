#ifndef RENDER_DEBUG_H
#define RENDER_DEBUG_H

// Note, if you get an compile error including <stdint.h> it means you are probably using a very, very, old C compiler
#include <stdint.h>

/*!
**
** Copyright (c) 2014 by John W. Ratcliff mailto:jratcliffscarab@gmail.com
**
**
** The MIT license:
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is furnished
** to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.

** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
** WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


**
** If you find this code snippet useful; you can tip me at this bitcoin address:
**
** BITCOIN TIP JAR: "1BT66EoaGySkbY9J6MugvQRhMMXDwPxPya"
**


*/

/*!
\file
\brief debug rendering classes and structures
*/

#define RENDER_DEBUG_VERSION 103

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
		ScreenSpace		= (1<<0),  //!< true if rendering in screenspace
		NoZbuffer		= (1<<1),  //!< true if zbuffering is disabled.
		SolidShaded		= (1<<2),  //!< true if rendering solid shaded.
		SolidWireShaded	= (1<<3),  //!< Render both as a solid shaded triangle and as a wireframe overlay.
		CounterClockwise= (1<<4),  //!< true if winding order is counter clockwise.
		CameraFacing	= (1<<5),  //!< True if text should be displayed camera facing
		InfiniteLifeSpan = (1<<6),  //!< True if the lifespan is infinite (overrides current display time value)
		CenterText		= (1<<7),  //!< True if the text should be centered.
	};
};

class RenderDebugInstance
{
public:
	RenderDebugInstance(void)
	{
		mTransform[0] = 0;
		mTransform[1] = 0;
		mTransform[2] = 0;

		mTransform[3] = 1;
		mTransform[4] = 0;
		mTransform[5] = 0;

		mTransform[6] = 0;
		mTransform[7] = 1;
		mTransform[8] = 0;

		mTransform[9] = 0;
		mTransform[10] = 0;
		mTransform[11] = 1;

	}
	float	mTransform[12]; // a 3x4 matrix (tanslation + scale/rotation)
};

class RenderDebugMeshVertex
{
public:
	float mPosition[3];				// The world-space position
	float mNormal[3];			// The normal vector to use for lighting
	float mTexel[2];			// Texture co-ordinates
};

/**
\brief Simple vertex with normal and color
 */
class RenderDebugSolidVertex
{
public:
	float mPos[3];				// The world-space position
	float mNormal[3];			// The normal vector to use for lighting
	uint32_t mColor;			// The diffuse color as 32 bit ARGB
};

/**
\brief simple unlit vertex with color
 */
class RenderDebugVertex
{
public:
	float mPos[3];				// The world-space position
	uint32_t mColor;			// The diffuse color as 32 bit ARGB
};



/**
\brief user provided rendering output methods
 */
class RenderDebugInterface
{
public:
  virtual void debugRenderLines(uint32_t lcount,		// The number of lines to draw (vertex count=lines*2)
								const RenderDebugVertex *vertices, // The pairs of vertices for each line segment 
								bool useZ,				// Whether or not these should be zbuffered
								bool isScreenSpace) = 0; // Whether or not these are in homogeneous screen-space co-ordinates

  virtual void debugRenderTriangles(uint32_t tcount,	// The number of triangles to render. (vertex count=tcount*2)
								const RenderDebugSolidVertex *vertices,	// The vertices for each triangle
								bool useZ,				// Whether or not these should be zbuffered
								bool isScreenSpace) = 0;// Whether or not these are in homogeneous screen-space co-ordinates

  virtual void debugMessage(const char *msg) = 0;	// A generic informational log message

  virtual void debugCommand(const char *cmd) = 0;	// A command to execute

  // The following methods are if your render interface binding supports instanced triangle meshes.
  // You are not required to implement these methods if you do not want or need this feature.

  	/**
	\brief Render a set of instanced triangle meshes.
	*/
	virtual bool renderTriangleMeshInstances(uint32_t /*meshId*/,		// The ID of the previously created triangle mesh
											 uint32_t /*instanceCount*/,	// The number of instances to render
											 const RenderDebugInstance * /*instances*/)	// The array of poses for each instance
	{
		return false;
	}

	/**
	\brief Create a triangle mesh that we can render.  Assumes an indexed triangle mesh.  User provides a *unique* id.  If it is not unique, this will fail.
	*/
	virtual bool createTriangleMesh(uint32_t /*meshId*/,	// The unique mesh ID
									uint32_t /*vcount*/,	// The number of vertices in the triangle mesh
									const RenderDebugMeshVertex * /*meshVertices*/,	// The array of vertices
									uint32_t /*tcount*/,	// The number of triangles (indices must contain tcount*3 values) If zero, assumed to just be a triangle list
									const uint32_t * /*indices*/) 	// The array of triangle mesh indices
	{
		return false;
	}

	/**
	\brief Release a previously created triangle mesh
	*/
	virtual bool releaseTriangleMesh(uint32_t /*meshId*/) 
	{
		return false;
	}

protected:
	virtual ~RenderDebugInterface(void)
	{

	}
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
	// used for client/server interface.
	enum PlaybackMode
	{
		PM_FORWARD,
		PM_REVERSE,
		PM_FORWARD_ONE_FRAME,
		PM_REVERSE_ONE_FRAME,
		PM_PAUSE
	};

	/**
	\brief A class to use when loading the DLL and initializing the RenderDebug library initial state.
	*/
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
		const char				*dllName;		// Name of the render-debug DLL to load.
		uint32_t				versionNumber;	// expected version number; if not equal then the DLL won't load.
		RunMode					runMode;		// startup mode
		const char				*recordFileName;	// If running in 'file' mode, this is the name of the file on disk the data will be rendering data will be recorded to.
		const char				*errorCode;		// If it failed to create the render-debug system; this will contain a string explaining why.
		bool					echoFileLocally;	// Is recording to a file, do we also want to echo the debugging commands locally (for both local render *and* record file at the same time).
	};

	virtual uint32_t getUpdateCount(void) const = 0;

	/**
	\brief Called once per frame to flush all debug visualization commands queued.
	*/
	virtual bool render(float dtime,	// The amount of time which has passed for this frame
					RenderDebugInterface *iface) = 0; // The optional callback interface to process the raw lines and triangles, NULL for applications connected to a file or server.

	/**
	\brief Create a 2d screen-space graph
	*/
	virtual void debugGraph(uint32_t numPoints,	// The number of points in the graph
							float * points,		// The set of data points.
							float graphMax,		// The maximum Y axis
							float graphXPos,	// The x screen space position for this graph
							float graphYPos,	// The y screen space position for this graph.
							float graphWidth,	// The width of the graph
							float graphHeight,	// The height of the graph
							uint32_t colorSwitchIndex = 0xFFFFFFFF) = 0;

	/**
	\brief Resets either a specific block of debug data or all blocks.
	*/
	virtual void  reset(int32_t blockIndex=-1) = 0; // -1 reset *everything*, 0 = reset everything except stuff inside blocks, > 0 reset a specific block of data.

	/**
	\brief Draw a grid visualization.
	*/
	virtual void  drawGrid(bool zup=false,
							uint32_t gridSize=40) = 0; // draw a grid.

	/**
	\brief Push the current render state on the stack
	*/
	virtual void  pushRenderState(void) = 0;

	/**
	\brief Pop the last render state off the stack.
	*/
	virtual void  popRenderState(void) = 0;

	/**
	\brief Set the current primary and secondary draw colors
	*/
	virtual void  setCurrentColor(uint32_t color=0xFFFFFF,uint32_t arrowColor=0xFF0000) = 0;

	/**
	\brief Get the current primary draw color
	*/
	virtual uint32_t getCurrentColor(void) const = 0;

	/**
	\brief Get the current secondary draw color (typically used for the color of arrow head, or the color for wire-frame outlines.
	*/
	virtual uint32_t getCurrentArrowColor(void) const = 0;

	/**
	\brief set a general purpose user pointer.
	*/
	virtual void  setCurrentUserPointer(void *userPtr) = 0;

	/**
	\brief Set the current user id
	*/
	virtual void  setCurrentUserId(int32_t userId) = 0;

	/**
	\brief Get the current user pointer
	*/
	virtual void * getCurrentUserPointer(void) const = 0;

	/**
	\brief Get the current user id
	*/
	virtual int32_t getCurrentUserId(void) = 0;

	/**
	\brief Set the current display time, this is the duration/lifetime of any draw primitives
	*/
	virtual void  setCurrentDisplayTime(float displayTime=0.0001f) = 0;

	/**
	\brief Get the current global debug rendering scale
	*/	
	virtual float getRenderScale(void) = 0;

	/**
	\brief Set the current global debug rendering scale.
	*/
	virtual void  setRenderScale(float scale) = 0;

	/**
	\brief Set the complete current set of RENDER_DEBUG::DebugRenderState bits explicitly. 
	*/
	virtual void  setCurrentState(uint32_t states=0) = 0;

	/**
	\brief Add a bit to the current render state.
	*/
	virtual void  addToCurrentState(RENDER_DEBUG::DebugRenderState::Enum state) = 0; // OR this state flag into the current state.

	/**
	\brief Clear a bit from the current render state.
	*/
	virtual void  removeFromCurrentState(RENDER_DEBUG::DebugRenderState::Enum state) = 0; // Remove this bit flat from the current state

	/**
	\brief Set the current scale for 3d text
	*/
	virtual void  setCurrentTextScale(float textScale) = 0;

	/**
	\brief Set the current arrow head size for rays and other pointer style primitives
	*/
	virtual void  setCurrentArrowSize(float arrowSize) = 0;

	/**
	\brief Get the current RENDER_DEBUG::DebugRenderState bit fields.
	*/
	virtual uint32_t getCurrentState(void) const = 0;

	/**
	\brief Set the entire render state in one call rather than doing discrete calls one a time.
	*/
	virtual void  setRenderState(uint32_t states=0,				// combination of render state flags
								uint32_t color=0xFFFFFF,		// base color
								float displayTime=0.0001f,		// duration of display items.
								uint32_t arrowColor=0xFF0000,	// secondary color, usually used for arrow head
								float arrowSize=0.1f,			// The size of arrow heads
								float renderScale=1.0f,			// The global render scale
								float textScale=1.0f) = 0;		// The global scale for 3d text


	/**
	\brief Get the entire current render state. Return the RENDER_DEBUG::DebugRenderState
	*/
	virtual uint32_t getRenderState(uint32_t &color,			// Primary color
									float &displayTime,			// display time
									uint32_t &arrowColor,		// Secondary color
									float &arrowSize,			// Arrow size
									float &renderScale,			// Global render scale
									float &textScale) const = 0;// Global text scale


	/**
	\brief Mark the end of a draw group.
	*/
	virtual void  endDrawGroup(void) = 0;

	/**
	\brief Get the current RENDER_DEBUG::DebugRenderState bit fields.
	*/
	virtual void  setDrawGroupVisible(int32_t groupId,		// The draw group ID
										bool state) = 0;	// Whether it should be visible or not.

	/**
	\brief Draw a 2d rectangle in screen-space coordinates.
	*/
	virtual void debugRect2d(float x1, // Upper left of 2d rectangle
							float y1,	// 
							float x2,	// Lower right of 2d rectangle
							float y2) = 0;

	/**
	\brief Draw a polygon; 'points' is an array of 3d vectors.
	*/
	virtual void  debugPolygon(uint32_t pcount,	// Number of points in the polygon
								const float *points,	// The array of Vec3 points x1,y1,z1,...x2,y2,z2...
								bool doubleSided) = 0;	// Whether or not to render the polygon double sided.  If so, the back-facing triangles will use the secondary color

	/**
	\brief Draw a single line
	*/
	virtual void  debugLine(const float p1[3],	// Start position
							const float p2[3]) = 0; // Line end position.

	/**
	\brief Draw a gradient line (different start color from end color)
	*/
	virtual void  debugGradientLine(const float p1[3],	// The starting location of the line in 3d
									const float p2[3],	// The ending location of the line in 3d
									const uint32_t &c1,	// The starting color
									const uint32_t &c2) = 0; // The ending color

	/**
	\brief Draw an oriented line, oriented relative to the provided 4x4 transformation matrix.
	*/
	virtual void  debugOrientedLine(const float p1[3],	// The starting position of the line
									const float p2[3],	// The ending position of the line
									const float transform[16]) = 0;	// The 4x4 transformation matrix to transform P1, and P2 through.

	/**
	\brief Draws a wireframe line with a small arrow head pointing along the direction vector ending at P2
	*/
	virtual void  debugRay(const float p1[3],	// The start of the ray
							const float p2[3]) = 0; // The end of the ray, where the arrow head will appear

	/**
	\brief Create a debug visualization of a cylinder from P1 to P2 with radius provided.
	*/
	virtual void  debugCylinder(const float p1[3],	// The starting point of the cylinder
								const float p2[3],	// The ending point of the cylinder 
								float radius) = 0;	// The radius of the cylinder

	/**
	\brief Creates a debug visualization of a 'thick' ray.  Extrudes a cylinder visualization with a nice arrow head.
	*/
	virtual void  debugThickRay(const float p1[3],	// Starting point of the ray
								const float p2[3],	// Ending point of the ray
								float raySize=0.02f) = 0;	// The thickness of the ray, the arrow head size is used for the arrow head.

	/**
	\brief Creates a debug visualization of a plane equation a couple of concentric circles
	*/	
	virtual void  debugPlane(const float normal[3],	// The normal of the plane equation.
							float dCoff,	// The D coefficient of the plane equation
							float radius1,	// The inner radius of the plane visualization
							float radius2) = 0; // The outder radius of the plane visualization

	/**
	\brief Debug visualize a 3d triangle.
	*/
	virtual void  debugTri(const float p1[3],	
							const float p2[3],
							const float p3[3], 
							bool doubleSided=false) = 0; // Whether or not to render the triangle double-sided.  If true the backside will use the secondary color. The lighting normal will be computed.

	/**
	\brief Debug visualize a 3d triangle with provided vertex lighting normals.
	*/
	virtual void  debugTriNormals(const float p1[3],
								const float p2[3],
								const float p3[3],
								const float n1[3],
								const float n2[3],
								const float n3[3]) = 0;

	/**
	\brief Debug visualize a 3d triangle with a unique color at each vertex.
	*/
	virtual void  debugGradientTri(const float p1[3],
									const float p2[3],
									const float p3[3],
									const uint32_t &c1,
									const uint32_t &c2,
									const uint32_t &c3) = 0;

	/**
	\brief Debug visualize a 3d triangle with provided vertex normals and colors
	*/
	virtual void  debugGradientTriNormals(const float p1[3],
											const float p2[3],
											const float p3[3],
											const float n1[3],
											const float n2[3],
											const float n3[3],
											const uint32_t &c1,
											const uint32_t &c2,
											const uint32_t &c3) = 0;

	/**
	\brief Debug visualize a 3d bounding box.
	*/
	virtual void  debugBound(const float bmin[3],	// The axis aligned minimum X,Y,Z coordinate of the bounding box
							const float bmax[3]) = 0; // The axis aligned maximum X,Y,Z coordinate of the bounding box

	/**
	\brief Debug visualize an oriented bounding box using a full 4x4 matrix transform and extents
	*/
	virtual void  debugOrientedBound(const float sides[3],	// The extents of the bounding box
									const float transform[16]) = 0; // The full 4x4 matrix transform for this bounding box orientation and translation.

	/**
	\brief Debug visualize an oriented bounding box using a pose format of a rotation quaternion (X,Y,Z,W) and a translation.
	*/
	virtual void  debugOrientedBound(const float bmin[3],	// The axis aligned minimum X,Y,Z coordinate of the bounding box
										const float bmax[3],// The axis aligned maximum X,Y,Z coordinate of the bounding box
										const float pos[3],	// The translation component of the pose
										const float quat[4]) = 0; // the rotation as a quaternion X,Y,Z,W

	/**
	\brief Debug visualize an oriented bounding box using a full 4x4 matrix transform with explicit axis aligned min and max bounding locations.
	*/
	virtual void  debugOrientedBound(const float bmin[3],	// The axis aligned minimum X,Y,Z coordinate of the bounding box
									const float bmax[3],	// The axis aligned maximum X,Y,Z coordinate of the bounding box
									const float xform[16]) = 0; // The full 4x4 transformation matrix


	/**
	\brief Debug visualize a crude sphere
	*/
	virtual void  debugSphere(const float pos[3],	// The center of the sphere
								float radius) = 0;	// The radius of the sphere.

	/**
	\brief Debug visualize an oriented sphere.
	*/
	virtual void  debugOrientedSphere(float radius,	// The radius of the sphere.
										uint32_t subdivision,	// The subdivision recursion for the sphere visualization.  Only use like 2 or 3, otherwise you will get an incredibly detailed sphere.
										const float transform[16]) = 0; // The full 4x4 transformation matrix

	/**
	\brief Debug visualize an oriented capsule with a 4x4 transformation.
	*/
	virtual void  debugOrientedCapsule(float radius,	// The radius of the capsule
										float height,	// The height of the capsule
										uint32_t subdivision,	// The number of subdivisions for the approximation, keep this very small. Like 2 or 3.
										const float transform[16]) = 0; // The full 4x4 transformation matrix of the capsule

	/**
	\brief Debug visualize an oriented and tapered capsule
	*/
	virtual void  debugOrientedCapsuleTapered(float radius1,	// The start radius
											float radius2,		// The end radius
											float height,		// The height
											uint32_t subdivision, // The number of subdivisions, keep it small; 2 or 3
											const float transform[16]) = 0; // The full 4x4 transformation matrix

	/**
	\brief Debug visualize an oriented cylinder.
	*/
	virtual void  debugOrientedCylinder(float radius,		// The radius of the cylinder
										float height,		// The height
										uint32_t subdivision, // The approximation subdivision (keep it small)
										bool closeSides,		// Whether or not the ends of the cylinder should be capped.
										const float transform[16]) = 0; // The full 4x4 rotation matrix.

	/**
	\brief Debug visualize an oriented circle.
	*/
	virtual void  debugOrientedCircle(float radius,		// The radius of the circle
									uint32_t subdivision,	// The number of subdivisions
									const float transform[16]) = 0;	// The full 4x4 rotation/translation matrix

	/**
	\brief Debug visualize a simple point
	*/
	virtual void  debugPoint(const float pos[3],	// The position of the point
							float radius) = 0;	// The size of the point visualization.

	/**
	\brief Debug visualize a simple point with provided scale.
	*/
	virtual void  debugPoint(const float pos[3],	// The position of the point
							const float scale[3]) = 0; // The X/Y/Z scale of the crosshairs.

	/**
	\brief Debug visualize a quad in screenspace (always screen facing)
	*/
	virtual void  debugQuad(const float pos[3],		// The position of the quad
							const float scale[2],	// The 2d scale
							float orientation) = 0;	// The 2d orientation

	/**
	\brief Debug visualize a 4x4 transform.
	*/
	virtual void  debugAxes(const float transform[16],	// The matrix we are trying to visualize
							float distance=0.1f,		// The size of the visualization
							float brightness=1.0f,		// The brightness of the axes
							bool showXYZ=false,			// Whether or not to print 3d text XYZ labels
							bool showRotation=false,	// Whether or not to visualize rotation or translation axes
							uint32_t axisSwitch=0) = 0;	// Which axis is currently selected/highlighted.
	/**
	\brief Debug visualize an arc
	*/
	virtual void debugArc(const float center[3],	// The center of the arc
							const float p1[3],		// The starting position of the arc
							const float p2[3],		// The ending position of the arc
							float arrowSize=0.1f,	// The size of the arrow head for the arc.
							bool showRoot=false) = 0; // Whether or not to debug visualize the center of the arc

	/**
	\brief Debug visualize an thick arc
	*/
	virtual void debugThickArc(const float center[3],// The center of the arc
								const float p1[3],	// The starting position of the arc
								const float p2[3],	// The ending position of the arc
								float thickness=0.02f,// How thick the arc is.
								bool showRoot=false) = 0;// Whether or not to debug visualize the center of the arc

	/**
	\brief Debug visualize a text string rendered a 3d wireframe lines.  Uses a printf style format.  Not all special symbols available, basic upper/lower case an simple punctuation.
	*/
	virtual void debugText(const float pos[3],	// The position of the text
							const char *fmt,...) = 0;	// The printf style format string

	/**
	\brief Debug visualize an oriented text string rendered a 3d wireframe lines.  Uses a printf style format.  Not all special symbols available, basic upper/lower case an simple punctuation. 
	*/
	virtual void debugOrientedText(const float pos[3],		// The position of the text
									const float rot[4],		// The quaternion rotation, XYZW
									const char *fmt,...) = 0;	// The print style format string.

	/**
	\brief Debug visualize an oriented text string rendered a 3d wireframe lines.  Uses a printf style format.  Not all special symbols available, basic upper/lower case an simple punctuation. 
	*/
	virtual void debugOrientedText(const float xform[16],	// Orientation and translation provided via a full 4x4 matrix
									const char *fmt,...) = 0;	// The printf style format string

	/**
	\brief Set's the view matrix as a full 4x4 matrix.  Required for screen-space aligned debug rendering. If you are not trying to do screen-facing or 2d screenspace rendering this is not required.
	*/
	virtual void setViewMatrix(const float view[16]) = 0;

	/**
	\brief Set's the projection matrix as a full 4x4 matrix.  Required for screen-space aligned debug rendering.
	*/
	virtual void setProjectionMatrix(const float projection[16]) = 0;

	/**
	\brief Sends a debug log message to the remote client/server or recorded to a log file.
	*/
	virtual void debugMessage(const char *fmt,...) = 0;

	/**
	\brief  Sends a command to the remote client/server
	*/
	virtual void debugCommand(const char *fmt,...) = 0;

	/**
	\brief Returns the current view*projection matrix
	*/
	virtual const float* getViewProjectionMatrix(void) const = 0;

	/**
	\brief Returns the current view matrix we are using
	*/
	virtual const float *getViewMatrix(void) const = 0;

	/**
	\brief Set's the current projection matrix.
	*/
	virtual const float *getProjectionMatrix(void) const = 0;

	/**
	\brief A convenience helper method to convert euler angles (in degrees) into a standard XYZW quaternion
	*/
	virtual void  eulerToQuat(const float angles[3],float q[4]) = 0; // angles are in degrees.

	/**
	\brief Begin's a draw group relative to this 4x4 matrix.  Returns the draw group id.
	*/
	virtual int32_t beginDrawGroup(const float pose[16]) = 0;

	/**
	\brief Revises the transform for a previously defined draw group.
	*/
	virtual void  setDrawGroupPose(int32_t blockId,const float pose[16]) = 0;

	/**
	\brief Debug visualize a detailed sphere.
	*/
	virtual void  debugDetailedSphere(const float pos[3],	// The center of the sphere
									float radius,	// The radius of the sphere
									uint32_t stepCount) = 0;	// The subdivision count; keep this low!

	/**
	\brief Sets the global pose for the current debug-rendering context.
	*/
	virtual void  setPose(const float pose[16]) = 0;

	/**
	\brief Gets the global pose for teh current debug rendering context
	*/
	virtual const float * getPose(void) const = 0;

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

	/**
	\brief deprecated?
	*/
	virtual void getDebugBounds(float bmin[3],float bmax[3]) const = 0;

	/**
	\brief deprecated?
	*/
	virtual void setFrameTime(float ftime) = 0;

	/**
	\brief deprecated?
	*/
	virtual float getFrameTime(void) const = 0;

	/**
	\brief Begins a file-playback session. Returns the number of recorded frames in the recording file.  Zero if the file was not valid.
	*/
	virtual uint32_t setFilePlayback(const char *fileName) = 0; 

	/**
	\brief Set's the file playback to a specific frame.  Returns true if successful.
	*/
	virtual bool setPlaybackFrame(uint32_t playbackFrame) = 0;

	/**
	\brief Returns the number of recorded frames in the debug render recoring file.
	*/
	virtual uint32_t getPlaybackFrameCount(void) const = 0;

	/**
	\brief Stops the current recording playback.
	*/
	virtual void stopPlayback(void) = 0;

	/**
	\brief Do a 'try' lock on the global render debug mutex.  This is simply provided as an optional convenience if you are accessing RenderDebug from multiple threads and want to prevent contention.
	*/
	virtual bool trylock(void) = 0;

	/**
	\brief Lock the global render-debug mutex to avoid thread contention.
	*/
	virtual void lock(void) = 0; 

	/**
	\brief Unlock the global render-debug mutex
	*/
	virtual void unlock(void) = 0;

	/**
	\brief In server mode controls how the shared-memory buffer is accessed; provides limited replay feature
	*/
	virtual void setPlaybackMode(PlaybackMode pm) = 0;

	/**
	\brief Convenience method to return a unique mesh id number (simply a global counter to avoid clashing with other ids
	*/
	virtual uint32_t getMeshId(void) = 0;

	/**
	\brief Render a set of instanced triangle meshes.
	*/
	virtual bool renderTriangleMeshInstances(uint32_t meshId,		// The ID of the previously created triangle mesh
											 uint32_t instanceCount,	// The number of instances to render
											 const RenderDebugInstance *instances) = 0;	// The array of poses for each instance

	/**
	\brief Create a triangle mesh that we can render.  Assumes an indexed triangle mesh.  User provides a *unique* id.  If it is not unique, this will fail.
	*/
	virtual bool createTriangleMesh(uint32_t meshId,	// The unique mesh ID
									uint32_t vcount,	// The number of vertices in the triangle mesh
									const RenderDebugMeshVertex *meshVertices,	// The array of vertices
									uint32_t tcount,	// The number of triangles (indices must contain tcount*3 values)
									const uint32_t *indices) = 0;	// The array of triangle mesh indices

	/**
	\brief Release a previously created triangle mesh
	*/
	virtual bool releaseTriangleMesh(uint32_t meshId) = 0;

	/**
	\brief Release the render debug class
	*/
	virtual void release(void) = 0;

protected:

	virtual ~RenderDebug(void) { };
};

// A convenience helper-class to create a scoped mutex-lock around calls into the debug render library, if you might potentially be calling it from
// multiple threads.
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
