#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <math.h>

#include "wavefront.h"
#include "RenderDebugBinding.h"

#define TSCALE1 (1.0f/4.0f)

float fm_computePlane(const float *A,const float *B,const float *C,float *n) // returns D
{
	float vx = (B[0] - C[0]);
	float vy = (B[1] - C[1]);
	float vz = (B[2] - C[2]);

	float wx = (A[0] - B[0]);
	float wy = (A[1] - B[1]);
	float wz = (A[2] - B[2]);

	float vw_x = vy * wz - vz * wy;
	float vw_y = vz * wx - vx * wz;
	float vw_z = vx * wy - vy * wx;

	float mag = ::sqrtf((vw_x * vw_x) + (vw_y * vw_y) + (vw_z * vw_z));

	if ( mag < 0.000001f )
	{
		mag = 0;
	}
	else
	{
		mag = 1.0f/mag;
	}

	float x = vw_x * mag;
	float y = vw_y * mag;
	float z = vw_z * mag;


	float D = 0.0f - ((x*A[0])+(y*A[1])+(z*A[2]));

	n[0] = x;
	n[1] = y;
	n[2] = z;

	return D;
}

class MeshBuilder
{
public:	
	MeshBuilder(uint32_t maxVertices)
	{
		mVertices.reserve(maxVertices);
	}

	void getVertex(const float *p,const float *n,uint32_t i1,uint32_t i2)
	{
		RENDER_DEBUG::RenderDebugMeshVertex v;

		v.mPosition[0] = p[0];
		v.mPosition[1] = p[1];
		v.mPosition[2] = p[2];

		v.mNormal[0] = n[0];
		v.mNormal[1] = n[1];
		v.mNormal[2] = n[2];

		v.mTexel[0] = p[i1]*TSCALE1;
		v.mTexel[1] = p[i2]*TSCALE1;

		mVertices.push_back(v);
	}

	void addTriangle(const float *p1,const float *p2,const float *p3)
	{
		float normal[3];
		fm_computePlane(p3,p2,p1,normal);

		double nx = fabs(normal[0]);
		double ny = fabs(normal[1]);
		double nz = fabs(normal[2]);

		uint32_t i1 = 0;
		uint32_t i2 = 0;

		if ( nx <= ny && nx <= nz ) 
			i1 = 0;
		if ( ny <= nx && ny <= nz ) 
			i1 = 1;
		if ( nz <= nx && nz <= ny ) 
			i1 = 2;

		switch ( i1 )
		{
		case 0:
			if ( ny < nz )
				i2 = 1;
			else
				i2 = 2;
			break;
		case 1:
			if ( nx < nz )
				i2 = 0;
			else
				i2 = 2;
			break;
		case 2:
			if ( nx < ny )
				i2 = 0;
			else
				i2 = 1;
			break;
		}

		getVertex(p1,normal,i1,i2);
		getVertex(p2,normal,i1,i2);
		getVertex(p3,normal,i1,i2);
	}

	std::vector< RENDER_DEBUG::RenderDebugMeshVertex > mVertices;
};



int main(int argc,const char **argv)
{
	if ( argc == 2 )
	{

		const char *dllName=NULL;
#ifdef WIN64
		dllName = "RenderDebug_x64.dll";
#else
#ifdef _DEBUG
		dllName = "RenderDebug_x86.dll";
#else
		dllName = "RenderDebug_x86.dll";
#endif
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
		desc.runMode = RENDER_DEBUG::RenderDebug::RM_CLIENT_OR_FILE;
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

				{
					MeshBuilder mb(w.mTriCount*3);

					for (int32_t i=0; i<w.mTriCount; i++)
					{
						uint32_t i1 = w.mIndices[i*3+0];
						uint32_t i2 = w.mIndices[i*3+1];
						uint32_t i3 = w.mIndices[i*3+2];

						const float *p1 = &w.mVertices[i1*3];
						const float *p2 = &w.mVertices[i2*3];
						const float *p3 = &w.mVertices[i3*3];

						mb.addTriangle(p3,p2,p1);

					}

					uint32_t meshId = gRenderDebug->getMeshId();

					float pos[3] = { 0, 2, 0 };
					gRenderDebug->debugText(pos,"%s", argv[1] );


					gRenderDebug->createTriangleMesh(meshId,(uint32_t)mb.mVertices.size(), &mb.mVertices[0], 0, NULL );

#define INSTANCE_BLOCK 1
#define INSTANCE_COUNT (INSTANCE_BLOCK*INSTANCE_BLOCK)

					RENDER_DEBUG::RenderDebugInstance instances[INSTANCE_COUNT];
					for (uint32_t x=0; x<INSTANCE_BLOCK; x++)
					{
						for (uint32_t z=0; z<INSTANCE_BLOCK; z++)
						{
							uint32_t index = x*INSTANCE_BLOCK+z;
							instances[index].mTransform[0] = (float)x*2;
							instances[index].mTransform[2] = (float)z*2;
						}
					}


					uint32_t frameCount = 2;
					if ( gRenderDebug->getRunMode() == RENDER_DEBUG::RenderDebug::RM_CLIENT )
					{
						frameCount = 500000;
					}
					bool solid=true;

					for (uint32_t i=0; i<frameCount; i++)
					{
						float pos[3] = { 0, 2, 0 };
						gRenderDebug->debugText(pos,"%s", argv[1] );
						gRenderDebug->addToCurrentState(RENDER_DEBUG::DebugRenderState::SolidWireShaded);
						gRenderDebug->addToCurrentState(RENDER_DEBUG::DebugRenderState::CameraFacing);
						gRenderDebug->setCurrentColor(0xFFFF00);

						if ( solid )
						{
							gRenderDebug->renderTriangleMeshInstances(meshId,INSTANCE_COUNT,instances);
						}
						else
						{
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
						}

						gRenderDebug->render(1.0f/60.0f,NULL);

						uint32_t argc;
						const char **argv = gRenderDebug->getCommandFromServer(argc);
						if ( argv )
						{
							const char *cmd = argv[0];
							if ( strcmp(cmd,"client_stop") == 0 )
							{
								break;
							}
							else if (strcmp(cmd,"toggle") == 0  )
							{
								solid = solid ? false : true;
							}
						}
					}

				}

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
