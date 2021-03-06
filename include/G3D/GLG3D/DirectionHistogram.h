/**
  @file DirectionHistogram.h
  @maintainer Morgan McGuire, morgan@cs.williams.edu
  @created 2009-03-25
  @edited  2009-03-25
*/

#ifndef GLG3D_DirectionHistogram_h
#define GLG3D_DirectionHistogram_h

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/platform.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Array.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/GThread.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Vector3.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Color3.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Color4.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/VertexBuffer.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/VertexRange.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/TriTree.h"

namespace G3D {

class RenderDevice;
    
/** \brief A histogram on the surface of a sphere.
    Useful for visualizing BSDFs. 
    
    Requires <code>sphere.ifs</code> to be in the current 
    directory or a location that System::findDataFile can 
    find it.

    The histogram drawn is a smoothing of the actual distribution
    by a \f$ \cos^{sharp} \f$ filter to ensure that it is not 
    undersampled by the underlying histogram mesh and buckets.

    Storage size is constant in the amount of data.  Input is 
    immediately inserted into a bucket and then discarded.
  */
class DirectionHistogram {
private:

    int                 m_slices;

    /** Vertices of the visualization mesh, on the unit sphere. */
    Array<Vector3>      m_meshVertex;

    /** Indices into meshVertex of the trilist for the visualization mesh. */
    Array<int>          m_meshIndex;

    /** Histogram buckets.  These are the scales of the corresponding m_meshVertex.*/
    Array<float>        m_bucket;

    /** Used to quickly find the quad.  The Tri::data field is the pointer 
       (into a subarray of m_meshIndex) of the four vertices of the quad hit.*/
    TriTree             m_tree;

    /** m_area[i] = inverse of the sum of the areas adjacent to vertex[i] */
    Array<float>        m_invArea;

    VertexRange         m_gpuMeshIndex;
    VertexRange         m_gpuMeshVertex;

    /** True when the VertexRange needs to be recomputed */
    bool                m_dirty;

    float               m_sharp;

    int                 m_numSamples;

    /** Volume of a tetrahedron whose 4th vertex is at the origin.  
        The vertices are assumed to be
        in ccw order.*/
    static float tetrahedronVolume(const Vector3& v0, const Vector3& v1, const Vector3& v2);

    /** Compute the total volume of the distribution */
    float totalVolume() const;

    void sendGeometry(RenderDevice* rd);

    /** Assumes vector has unit length.

       @param startIndex Inclusive
       @param stopIndex  Inclusive
    */
    void insert(const Vector3& vector, float weight, int startIndex, int stopIndex);

public:

    /**
       \param axis place histogram buckets relative to this axis

       \param numSlices Number of lat and long slices to make.*/
    DirectionHistogram(int numSlices = 50, const Vector3& axis = Vector3::unitZ());

    /** Discard all data */
    void reset();

    /**
     \brief Insert a new data point into the set.
      Only the direction of @a vector matters; it will be normalized.
     */
    void insert(const Vector3& vector, float weight = 1.0f);
    void insert(const Array<Vector3>& vector, const Array<float>& weight);

    /** Draw a wireframe of the distribution.  Renders with approximately constant volume. */
    void render(
        class RenderDevice* rd, 
        const Color3& solidColor = Color3::white(), 
        const Color4& lineColor = Color3::black());

};

} // G3D

#endif
