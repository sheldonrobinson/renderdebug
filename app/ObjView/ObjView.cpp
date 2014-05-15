#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wavefront.h"
#include "RenderDebugBinding.h"

int main(int argc,const char **argv)
{
	if ( argc == 2 )
	{

		const char *dllName=NULL;
#ifdef WIN64
		dllName = "RenderDebug_x64.dll";
#else
		dllName = "RenderDebug_x86.dll";
#endif
		printf("Loading RenderDebug DLL\r\n");
		RENDER_DEBUG::RenderDebug::Desc desc;
		desc.dllName = dllName;

		char recordName[512];
		sprintf_s(recordName,512,"%s", argv[1]);
		char *obj = strstr(recordName,".obj");
		if ( obj )
		{
			*obj = 0;
			strcat(recordName,".rec");
		}


		desc.recordFileName = recordName;
		desc.runMode = RENDER_DEBUG::RenderDebug::RM_FILE;
		gRenderDebug = RENDER_DEBUG::createRenderDebug(desc);
		if ( gRenderDebug )
		{
			WavefrontObj w;
			printf("Loading wavefront.obj file '%s'\r\n", argv[1] );
			w.loadObj(argv[1]);
			if ( w.mTriCount )
			{
				printf("Found: %d triangles.\r\n", w.mTriCount );

				gRenderDebug->addToCurrentState(RENDER_DEBUG::DebugRenderState::CenterText);

				float pos[3] = { 0, 2, 0 };
				gRenderDebug->debugText(pos,"%s", argv[1] );

				gRenderDebug->addToCurrentState(RENDER_DEBUG::DebugRenderState::SolidWireShaded);
				gRenderDebug->setCurrentColor(0xFFFF00);

				for (int32_t i=0; i<w.mTriCount; i++)
				{
					uint32_t i1 = w.mIndices[i*3+0];
					uint32_t i2 = w.mIndices[i*3+1];
					uint32_t i3 = w.mIndices[i*3+2];

					const float *p1 = &w.mVertices[i1*3];
					const float *p2 = &w.mVertices[i2*3];
					const float *p3 = &w.mVertices[i3*3];

					gRenderDebug->debugTri(p3,p2,p1);
				}
				gRenderDebug->render(1.0f/60.0f,NULL);
				gRenderDebug->render(1.0f/60.0f,NULL);
				gRenderDebug->render(1.0f/60.0f,NULL);
				gRenderDebug->render(1.0f/60.0f,NULL);
			}
			else
			{
				printf("Failed to find any data in file.\r\n");
			}
			gRenderDebug->release();
		}
		else
		{
			printf("Failed to load RenderDebug DLL (%s)\r\n", desc.dllName );
		}
	}
	else
	{
		printf("ObjView <fname.obj>\r\n");
	}
	return 0;
}
