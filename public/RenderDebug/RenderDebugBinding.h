#ifndef RENDER_DEBUG_BINDING_H

#define RENDER_DEBUG_BINDING_H

#include "RenderDebug.h"

namespace RENDER_DEBUG
{

RENDER_DEBUG::RenderDebug *createRenderDebug(RenderDebug::Desc &desc);

}

extern RENDER_DEBUG::RenderDebug *gRenderDebug;

#endif
