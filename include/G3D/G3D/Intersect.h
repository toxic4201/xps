/**
 @file Intersect.h
 
 @maintainer Morgan McGuire, morgan@cs.williams.edu
  
 @created 2009-06-29
 @edited  2009-06-29

 Copyright 2000-2009, Morgan McGuire.
 All rights reserved.

 From the G3D Innovation Engine
 http://g3d-cpp.sf.net
 */
#ifndef G3D_Intersect
#define G3D_Intersect

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/platform.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Ray.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/AABox.h"

namespace G3D {

/**
 @beta
 */
class Intersect {
public:

	/** \brief Returns true if the intersection of the ray and the solid box is non-empty. 

      \cite "Fast Ray / Axis-Aligned Bounding Box Overlap Tests using Ray Slopes" 
      by Martin Eisemann, Thorsten Grosch, Stefan M?ller and Marcus Magnor
      Computer Graphics Lab, TU Braunschweig, Germany and
      University of Koblenz-Landau, Germany
    */
	static bool __fastcall rayAABox(const Ray& ray, const AABox& box);

	/** \brief Returns true if the intersection of the ray and the solid box is non-empty. 
	 
     \param time If there is an intersection, set to the time to that intersection. If the ray origin is inside the box,
     this is a negative value indicating the distance backwards from the ray origin to the first intersection.
     \a time is not set if there is no intersection.
          
     \cite Slope-Mul method from "Fast Ray / Axis-Aligned Bounding Box Overlap Tests using Ray Slopes" 
      by Martin Eisemann, Thorsten Grosch, Stefan M?ller and Marcus Magnor
      Computer Graphics Lab, TU Braunschweig, Germany and
      University of Koblenz-Landau, Germany
      */
 	static bool __fastcall rayAABox(const Ray& ray, const AABox& box, float& time);
};

}

#endif
