#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <direct.h>

#pragma warning(disable:4100)

#include "RenderDebugBinding.h"

const float FM_PI = 3.1415926535897932384626433832795028841971693993751f;
const float FM_DEG_TO_RAD = ((2.0f * FM_PI) / 360.0f);
const float FM_RAD_TO_DEG = (360.0f / (2.0f * FM_PI));

void testRenderDebug(void)
{
	printf("Generating 500 frames of debug visualization data.\r\n");

	gRenderDebug->debugMessage("Running the TestRenderDebug console application.\r\n");

	uint32_t frameCount = 500;
	if ( gRenderDebug->getRunMode() == RENDER_DEBUG::RenderDebug::RM_CLIENT )
	{
		frameCount = 500000;
	}
	
	float t = 0;

	for (uint32_t i=0; i<frameCount; i++)
	{
		gRenderDebug->debugMessage("Processing frame #%d of %d.\r\n", i+1, frameCount );
#if 1
		gRenderDebug->drawGrid();
		gRenderDebug->pushRenderState();
		gRenderDebug->addToCurrentState(RENDER_DEBUG::DebugRenderState::SolidWireShaded);
		gRenderDebug->addToCurrentState(RENDER_DEBUG::DebugRenderState::CameraFacing);

		for (uint32_t i=0; i<360; i+=15)
		{
			float a = (float)i*FM_DEG_TO_RAD;
			float r = 0.5f + (a*0.1f);
			a+=t;

			float distance = 10+cosf(a)*1.0f;

			float pos[3];
			pos[0] = distance*cosf(a);
			pos[2] = distance*sinf(a);

			pos[1] = distance+sinf(a*4);

			uint32_t index = (i/15)+1;
			RENDER_DEBUG::DebugColors::Enum dc = (RENDER_DEBUG::DebugColors::Enum)((index%(RENDER_DEBUG::DebugColors::NUM_COLORS-RENDER_DEBUG::DebugColors::Red)+RENDER_DEBUG::DebugColors::Red));
			uint32_t c = gRenderDebug->getDebugColor(dc);
			gRenderDebug->setCurrentColor(c,0xFFFFFF);

			gRenderDebug->debugSphere(pos,r);

			{
				float p2[3];
				p2[0] = pos[0];
				p2[1] = pos[1];
				p2[2] = pos[2];
				p2[1]-=r;

				float origin[3] = {0,0,0};

				gRenderDebug->debugThickRay(origin,p2,0.1f);

			}

			pos[1]+=r*2;
			gRenderDebug->debugText(pos,"Sphere%d", index );

		}
		gRenderDebug->popRenderState();
#endif
		gRenderDebug->render(1.0f/60.0f,NULL);
		t+=0.01f;

		uint32_t argc;
		const char **argv = gRenderDebug->getCommandFromServer(argc);
		if ( argc )
		{
			const char *cmd = argv[0];
			if ( strcmp(cmd,"client_stop") == 0)
			{
				break;
			}
		}
	}
}

int main(int argc,const char **argv)
{
	// Set the current working directory to the EXE location
	char dirname[512];
	strcpy(dirname,argv[0]);
	int len = (int)strlen(dirname);
	char *scan = &dirname[len-1];
	while ( len )
	{
		if ( *scan == '\\' )
		{
			*scan = 0;
			break;
		}
		scan--;
		len--;
	}

	_chdir( dirname );

	RENDER_DEBUG::RenderDebug::Desc desc;

	desc.runMode = RENDER_DEBUG::RenderDebug::RM_CLIENT_OR_FILE;
	desc.recordFileName = "TestRenderDebug.rec"; // the name of the test recording file

#ifdef PX_X64
	desc.dllName = "RenderDebug_x64.dll";
#else
	desc.dllName = "RenderDebug_x86.dll";
#endif
	gRenderDebug = RENDER_DEBUG::createRenderDebug(desc);

	if ( gRenderDebug == NULL )
	{
		printf("Failed to load the RenderDebug DLL for the following reason: %s\r\n", desc.errorCode );
	}
	else
	{
		testRenderDebug();
	}

	return 0;
}
