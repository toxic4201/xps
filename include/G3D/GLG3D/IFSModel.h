/**
  @file IFSModel.h
  
  @maintainer Morgan McGuire, morgan@cs.williams.edu

  @cite Original IFS code by Nate Robbins

  @created 2003-11-12
  @edited  2009-02-10
 */ 


#ifndef GLG3D_IFSModel_h
#define GLG3D_IFSModel_h

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/platform.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Sphere.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/AABox.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/System.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/Surface.h"

namespace G3D {

typedef ReferenceCountedPointer<class IFSModel> IFSModelRef;


/**
 Loads the IFS, PLY2, and OFF research file formats.
 You can also load these using ArticulatedModel, which provides more sophisticated 
 rendering mechanisms.
 */
class IFSModel : public ReferenceCountedObject {
public:

    typedef ReferenceCountedPointer<class IFSModel> Ref;

protected:
    class PosedIFSModel : public Surface {
    public:
        IFSModelRef             model;
        CoordinateFrame         cframe;
        bool                    perVertexNormals;
        bool                    useMaterial;
        GMaterial               material;

        PosedIFSModel(IFSModelRef _model, const CoordinateFrame& _cframe, bool _pvn,
            const GMaterial& _mat, bool _useMat);
        virtual ~PosedIFSModel() {}
        virtual std::string name() const;
        virtual void getCoordinateFrame(CoordinateFrame&) const;
        virtual const MeshAlg::Geometry& objectSpaceGeometry() const;
        virtual const Array<MeshAlg::Face>& faces() const;
        virtual const Array<MeshAlg::Edge>& edges() const;
        virtual const Array<MeshAlg::Vertex>& vertices() const;
        virtual const Array<MeshAlg::Face>& weldedFaces() const;
        virtual const Array<MeshAlg::Edge>& weldedEdges() const;
        virtual const Array<MeshAlg::Vertex>& weldedVertices() const;
        virtual const Array<Vector2>& texCoords() const;
        virtual bool hasTexCoords() const;
        virtual const Array<int>& triangleIndices() const;
        virtual void getObjectSpaceBoundingSphere(Sphere&) const;
        virtual void getObjectSpaceBoundingBox(AABox&) const;
        virtual void render(RenderDevice* renderDevice) const;
        virtual int numBoundaryEdges() const;
        virtual int numWeldedBoundaryEdges() const;
        virtual const Array<Vector3>& objectSpaceFaceNormals(bool normalize = true) const;
        virtual void sendGeometry(RenderDevice* rd) const;
    };

    friend class PosedIFSModel;

    std::string                 name;
    std::string                 filename;
    Array<int>                  indexArray;
    Array<MeshAlg::Vertex>      vertexArray;
    Array<Vector2>				texArray;

    MeshAlg::Geometry           geometry;
    Array<MeshAlg::Face>        faceArray;
    Array<MeshAlg::Edge>        edgeArray;
    Array<Vector3>              faceNormalArray;
    Array<MeshAlg::Face>        weldedFaceArray;
    Array<MeshAlg::Edge>        weldedEdgeArray;
    Array<MeshAlg::Vertex>      weldedVertexArray;
    int                         numBoundaryEdges;
    int                         numWeldedBoundaryEdges;
    Sphere                      boundingSphere;
    AABox                       boundingBox;


    /** Shared by all models */
    static VertexBufferRef           varArea;

    // Used by the last object to render.  If the
    // new model is the same as the old one, an upload
    // step can be avoided.
    static IFSModelRef          lastModel;
    static VertexRange                  lastVertexVAR;
    static VertexRange                  lastNormalVAR;
    static VertexRange                  lastTexCoordVAR;

    IFSModel();
    
    /** Only called from computes all other values from the current index, vertex, and texCoord arrays. */
    void init(const std::string& filename, const Vector3& scale, const CoordinateFrame& cframe, const bool weld, bool removeDegenerateFaces);

    /** Only called from create */
    void reset();

public:

    static void* operator new(size_t size) {
        return System::malloc(size);
    }

    static void operator delete(void* p) {
        System::free(p);
    }

    virtual ~IFSModel();

    /**
     Throws an std::string describing the error if anything
     goes wrong.
     @param scale 3D scale factors to apply to vertices while loading (*after* cframe)
     @param cframe Coordinate transform to apply to vertices while loading.  
                   This is not part of the object to world transformation
                   for the model when posed; it really modifies the object
                   space geometry.
	 @param weld   Toggles welding colocated vertices, an O(n^2) operation. Defaults to false
     @param removeDegenerateFaces Removes any faces that contain two copies of the same vertex. Defaults to false
     */
    static IFSModelRef fromFile(const std::string& filename, const Vector3& scale = Vector3(1,1,1), const CoordinateFrame& cframe = CoordinateFrame(), const bool weld=false, bool removeDegenerateFaces = false);
    static IFSModelRef fromFile(const std::string& filename, const double scale, const CoordinateFrame& cframe = CoordinateFrame(), const bool weld = false, bool removeDegenerateFaces = false);
    static IFSModelRef fromData(const std::string& name, const Array<Vector3>& vertex, const Array<int>& index, const Array<Vector2>& texCoord = Array<Vector2>(), const Vector3& scale = Vector3(1,1,1), const CoordinateFrame& cframe = CoordinateFrame(), bool weld = false, bool removeDegenerateFaces = false);

    /**
     If perVertexNormals is false, the model is rendered with per-face normals,
     which are slower.  No color or texture is set when rendered (that is,
     the existing RenderDevice values are used.)
     */
    virtual Surface::Ref pose(const CoordinateFrame& cframe = CoordinateFrame(), bool perVertexNormals = true);

    /**
     If perVertexNormals is false, the model is rendered with per-face normals,
     which are slower.
     */
    virtual Surface::Ref pose(const CoordinateFrame& cframe, const GMaterial& material, bool perVertexNormals = true);

    virtual size_t mainMemorySize() const;

    /** Writes an IFS or PLY2 file from data in arrays. (Note PLY2 Files do not support texCoords.) */
    static void save(const std::string& filename, const std::string& name,
             const Array<int>& index, const Array<Vector3>& vertex, const Array<Vector2>& texCoord);

    /** Parses an IFS or PLY2 file from disk into arrays; does no cleanup or welding */
    static void load(const std::string& filename, std::string& name,
        Array<int>& index, Array<Vector3>& vertex, Array<Vector2>& texCoord);
};

} //namespace


#endif
