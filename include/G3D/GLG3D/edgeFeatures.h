/**
 @file edgeFeatures.h
 
 @maintainer Morgan McGuire, morgan@cs.williams.edu
 
 @created 2004-04-20
 @edited  2006-02-03
 */

#ifndef G3D_FEATUREEDGES_H
#define G3D_FEATUREEDGES_H

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/platform.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/g3dmath.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/Surface.h"

namespace G3D {

/**
  Renders boundaries, contours, and sharp creases as lines. 
  The caller should set the line width, color, and enable
  alpha blending (for smooth lines).

  Primarily useful for cartoon rendering.

  If the lines are lost in the object's body due to z-fighting,
  render the model first with a negative value for RenderDevice::setPolygonOffset.

  @param creaseAngle Angle below which a sharp corner is considered a crease.  
       Zero makes everything a crease,  G3D_PI/2 makes nothing a crease.

  
 <B>BETA API</B> -- subject to change.
 */
void drawFeatureEdges(class RenderDevice* renderDevice, const Surface::Ref& model, float creaseAngle = toRadians(40));

}

#endif
