/**
 @file G3D.h

 This header includes all of the G3D libraries in
 appropriate namespaces.

 @maintainer Morgan McGuire, morgan@cs.williams.edu

 @created 2001-08-25
 @edited  2009-04-15

 Copyright 2000-2009, Morgan McGuire.
 All rights reserved.
*/

#ifndef G3D_G3D_h
#define G3D_G3D_h

#define NOMINMAX 1
#ifdef min
    #undef min
#endif
#ifdef max
    #undef max
#endif

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/platform.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Random.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Array.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/SmallArray.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Queue.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Crypto.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/format.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Vector2.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Vector3.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Vector4.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Color1.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Color3.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Color4.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Matrix2.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Matrix3.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Matrix4.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/CoordinateFrame.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/PhysicsFrame.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Plane.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Line.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Ray.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Sphere.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Box.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Box2D.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/AABox.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/WrapMode.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Cone.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Quat.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/stringutils.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/prompt.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Table.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Set.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/GUniqueID.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/BinaryFormat.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/BinaryInput.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/BinaryOutput.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/debug.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/G3DGameUnits.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/g3dmath.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/uint128.h"
#include "GC:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\3D/fileutils.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/ReferenceCount.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Welder.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/GMutex.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/PrecomputedRandom.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/MemoryManager.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/AreaMemoryManager.h"

template<class T> struct HashTrait< G3D::ReferenceCountedPointer<T> > {
    static size_t hashCode(G3D::ReferenceCountedPointer<T> key) { return reinterpret_cast<size_t>( key.pointer() ); }
};

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/GImage.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/CollisionDetection.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Intersect.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Log.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/serialize.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/TextInput.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/NetAddress.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/NetworkDevice.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/System.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/splinefunc.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Spline.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/UprightFrame.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/LineSegment.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Capsule.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Cylinder.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Triangle.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Color3uint8.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Color4uint8.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Vector2int16.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Vector3int16.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Vector3int32.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Vector4int8.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/ConvexPolyhedron.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/MeshAlg.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/vectorMath.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Rect2D.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/GCamera.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/GLight.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/KDTree.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/PointKDTree.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/TextOutput.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/MeshBuilder.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Stopwatch.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/AtomicInt32.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/GThread.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/ThreadSet.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/RegistryUtil.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/AnyVal.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/PointHashGrid.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Map2D.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Image1.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Image1uint8.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Image3.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Image3uint8.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Image4.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Image4uint8.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/filter.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/WeakCache.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Pointer.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Matrix.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/ImageFormat.h"

#ifdef _MSC_VER
#   pragma comment(lib, "zlib")
#   pragma comment(lib, "ws2_32")
#   pragma comment(lib, "winmm")
#   pragma comment(lib, "imagehlp")
#   pragma comment(lib, "gdi32")
#   pragma comment(lib, "user32")
#   pragma comment(lib, "kernel32")
#   pragma comment(lib, "version")
#   pragma comment(lib, "advapi32")
#   pragma comment(lib, "png")
#   pragma comment(lib, "jpeg")
#   pragma comment(lib, "zip")
#   ifdef _DEBUG
        // Don't link against G3D when building G3D itself.
#      ifndef G3D_BUILDING_LIBRARY_DLL
#         pragma comment(lib, "G3Dd.lib")
#      endif
#   else
        // Don't link against G3D when building G3D itself.
#      ifndef G3D_BUILDING_LIBRARY_DLL
#         pragma comment(lib, "G3D.lib")
#      endif
#   endif
#endif

#endif

