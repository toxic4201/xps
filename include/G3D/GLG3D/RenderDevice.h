/**
  @file RenderDevice.h

  Graphics hardware abstraction layer (wrapper for OpenGL).

  You can freely mix OpenGL calls with RenderDevice, just make sure you put
  the state back the way you found it or you will confuse RenderDevice.

  @maintainer Morgan McGuire, morgan@cs.williams.edu
  @created 2001-05-29
  @edited  2009-05-20

  Copyright 2001-2009, Morgan McGuire
*/

#ifndef GLG3D_RenderDevice_h
#define GLG3D_RenderDevice_h

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/platform.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/constants.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Array.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/GLight.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/TextOutput.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/MeshAlg.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/Texture.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/Milestone.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/VertexProgram.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/PixelProgram.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/Shader.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/VertexBuffer.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/OSWindow.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GLCaps.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/VertexRange.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/Framebuffer.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/Lighting.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Stopwatch.h"

namespace G3D {

class VertexRange;


/**
 \brief Abstraction of a graphics rendering context.  
 
 Implemented with an
 OpenGL context, but designed so that it can support other APIs
 (e.g., OpenGL ES, DirectX) as back ends.

 Unlike OpenGL, in release mode, no RenderDevice call will trigger a
 pipeline flush, and all redundant state calls are automatically
 detected and optimized out.  This includes querries: reading any
 RenderDevice state is instantaneous and does not flush the GPU
 (unlike glGet*).

 In future releases, fixed function state will either be removed
 or isolated in RenderDevice.  Wherever possible, structure your
 code to use Framebuffer, VertexRange, and Shader instead of fixed-function.

 You must call RenderDevice::init() before using the RenderDevice.
  
 OpenGL is a basically good API with some rough spots.  Three of these
 are addressed by RenderDevice.  First, OpenGL state management is
 both tricky and potentially slow.  Second, OpenGL functions are
 difficult to use because many extensions have led to an evolutionary
 rather than designed API.  For type safety, new enums are introduced
 for values instead of the traditional OpenGL GLenum's, which are just
 ints.  Third, OpenGL intialization is complicated.  This interface
 simplifies it significantly.

 <P> NICEST line and point smoothing is enabled by default (however,
 you need to set your alpha blending mode to see it).

 <P> glEnable(GL_NORMALIZE) is set by default.  glEnable(GL_COLOR_MATERIAL) 
     is enabled by default.  You may be able to get a slight speed increase
     by disabling GL_NORMALIZE or using GL_SCALE_NORMAL instead.

 <P> For stereo rendering, set <CODE>OSWindow::Settings::stereo = true</CODE>
     and use RenderDevice::setDrawBuffer to switch which eye is being rendered.  Only
     use RenderDevice::beginFrame/RenderDevice::endFrame once per frame,
     but do clear both buffers separately.

 <P> The only OpenGL calls <B>NOT</B> abstracted by RenderDevice are
     fog and texture coordinate generation.  For everything else, use
     RenderDevice.

 <P>
 Example
  <PRE>
   RenderDevice renderDevice = new RenderDevice();
   renderDevice->init(OSWindow::Settings());
  </PRE>

  <P>
  Example 2 (textured quad)
  <PRE>
    RenderDevice* renderDevice = new RenderDevice();
    renderDevice->init(640, 480);

    Texture::Ref sprite = new Texture("Grass Texture", "image.jpg");

    renderDevice->beginFrame();
    renderDevice->pushState();
    renderDevice->clear(true, true, true);
    renderDevice->setCullFace(RenderDevice::CULL_NONE);
    renderDevice->setProjectionMatrix(Matrix4::perspectiveProjection(-.2, .2, -.15, .15, .2, 200));
    renderDevice->setTexture(0, sprite);
    renderDevice->setColor(Color3::white());
    renderDevice->beginPrimitive(PrimitiveType::QUADS);
        renderDevice->setTexCoord(0,  Vector2(0, 1));
        renderDevice->sendVertex(Vector3(-3, -3, -5));
        
        renderDevice->setTexCoord(0,  Vector2(1, 1));
        renderDevice->sendVertex(Vector3( 3, -3, -5));
        
        renderDevice->setTexCoord(0,  Vector2(1, 0));
        renderDevice->sendVertex(Vector3( 3,  3, -5));

        renderDevice->setTexCoord(0,  Vector2(0, 0));
        renderDevice->sendVertex(Vector3(-3,  3, -5));
    renderDevice->endPrimitive();
    renderDevice->popState();

    renderDevice->endFrame();

    while (true);

    renderDevice->cleanup();
    </PRE>

  <P>

  G3D::RenderDevice supports "X_CURRENT" as an option for most settings.

  <P>
 <B>Stereo Rendering</B>
  You can render in stereo (on a stereo capable card) by rendering twice,
  once for each eye's buffer:

  <pre>
    void onGraphics(...) {
        rd->setDrawBuffer(RenderDevice::DRAW_LEFT); 
        for (int count = 0; count < 2; ++count) {
           ... (put your normal rendering code here)
           rd->setDrawBuffer(RenderDevice::DRAW_RIGHT);
        }
    }
  </pre>
  
  <B>Multiple displays</B>
  If you are using multiple synchronized displays (e.g. the CAVE),
  see:
  http://www.nvidia.com/object/IO_10753.html
  and
  http://www.cs.unc.edu/Research/stc/FAQs/nVidia/FrameLock-V1.0C.pdf

  GLCaps loads the relevant extensions for you, but you must make
  the synchronizing calls yourself (typically, immediately before
  you call swap buffers).

  <P>
  <B>Raw OpenGL Calls</B>
  RenderDevice allows you to mix your own OpenGL calls with RenderDevice calls.
  It assumes that you restored the OpenGL state after your OpenGL calls, however.
  It is not safe to mix arbitrary OpenGL calls with Shaders, however.  The 
  G3D::Shader API supports more features than OpenGL shaders and does not work
  well with low-level OpenGL.  You may find that the "wrong" shader is bound
  when you execute OpenGL calls.
 */
class RenderDevice {
public:

    typedef PrimitiveType Primitive;
   	
    /** RENDER_CURRENT = preserve whatever the render mode is currently set to.  */
    enum RenderMode {RENDER_SOLID, RENDER_WIREFRAME, RENDER_POINTS, RENDER_CURRENT};
    
    enum {MAX_LIGHTS = 8};

private:

    friend class VertexRange;
    friend class VertexBuffer;
    friend class Milestone;
    friend class UserInput;
    friend class VertexAndPixelShader;

    OSWindow*                    _window;

    /** Should the destructor delete _window?*/
    bool                        deleteWindow;

    /**
     The current GLGeom generation.  Used for vertex arrays.
     */
    uint32                      generation;

    /** \deprecated (Not supported on most LCDs) */
    void setGamma(
        double                  brightness,
        double                  gamma);

	void setVideoMode();

    /**
     For counting the number of beginFrame/endFrames.
     */
    int                         beginEndFrame;

    bool                        _swapBuffersAutomatically;

    /** True after endFrame until swapGLBuffers is invoked.
        Used to correctly manage value changes of _swapBuffersAutomatically
        so that a frame not intended to be seen is never rendered.*/
    bool                        swapGLBuffersPending;

    /** Sets the texture matrix without checking to see if it needs to
        be changed.*/
    void forceSetTextureMatrix(int unit, const float* m);
    void forceSetTextureMatrix(int unit, const double* m);

    /** Argument to last beginPrimitive() */
    Primitive                   currentPrimitive;

    /** Number of vertices since last beginPrimitive() */
    int                         currentPrimitiveVertexCount;
   
    /** The area used inside of an indexedPrimitives call. */
    VertexBuffer::Ref                currentVARArea;
    
    std::string                 cardDescription;

    /** Helper for setXXXArray.  Sets the currentVARArea and
        makes some consistency checks.*/
    void setVARAreaFromVAR(const class VertexRange& v);

    /** Updates the triangle count based on the primitive.
        
        LINE and POINT primitives are given one triangle count each. */
    void countTriangles(RenderDevice::Primitive primitive, int numVertices);

    /**
     Sets the milestones on the currentVARArea.
     */
    void setVARAreaMilestone();

    /** Called by sendIndices. */
    void internalSendIndices
    (RenderDevice::Primitive primitive,
     size_t                  indexSize, 
     int                     numIndices, 
     const void*             index,
     int                     numInstances,
     bool                    useInstances);
    
    /**
     VertexAndPixelShader is set lazily.  This ensures that the
     current value is actually bound so that the argument list can be
     bound.  Called from VertexAndPixelShader::bindArgList and from
     beforePrimitive.*/
    void forceVertexAndPixelShaderBind();

    static std::string dummyString;

    /** Returns true if the frame buffer is complete, or a string in 
        whyIncomplete explaining the problem.
        Potentially slow. */
    bool checkFramebuffer(std::string& whyIncomplete = dummyString) const;

    /** Sets the glDrawBuffersARB to match the current FBO's 
        capabilities.  Called from clear() and beforePrimitive(). */
    void syncDrawBuffer(bool alreadyBound);

public:
    
    /** \brief Reports measured GPU performance and throughput.
        
        "OpenGL state changes" are those that forced underlying OpenGL
        state changes; RenderDevice optimizes away redundant state
        changes so many changes will not affect OpenGL. 
        
        Many programs are limited by the number of GL state
        changes; sorting objects in the render pipeline to minimize this 
        can dramatically increase performance.

        Major state changes are texture, shader, camera, and projection changes.
        These are particularly expensive because they can cause cache misses and
        pipeline stalls.  Graphics cards can handle about
        100 of these in real-time. Minor state changes are color, vertex,
        and test changes.  About 2000 of these can be made per frame in real-time.

        One way to locate the code that causes state changes is to put breakpoints
        in RenderDevice::minGLStateChange and RenderDevice::majGLStateChange
        and look at the call stack.

        Zeroed by beginFrame.*/
    class Stats {
    public:
        uint32			minorStateChanges;
        uint32			minorOpenGLStateChanges;
        
        uint32			majorStateChanges;
        uint32			majorOpenGLStateChanges;
        
        uint32			pushStates;
        
        /** Number of individual primitives (e.g., number of triangles) */
        uint32			primitives;
        
        /** Number of triangles since last beginFrame() */
        uint32			triangles;
        
        /** Exponentially weighted moving average of Stats::triangles.*/
        double                  smoothTriangles;
        
        /** Amount of time spent in swapbuffers (when large, indicates 
            that the GPU is blocking the CPU). */
        RealTime		swapbuffersTime;
        
        /** Inverse of beginframe->endframe time.  Accounts for the frame
            timing of the system as a whole, not just graphics.*/
        float			frameRate;
        
        /** Exponentially weighted moving average of frame rate */
        float                   smoothFrameRate;
        
        double			triangleRate;
        
        /** Exponentially weighted moving average of triangleRate */
        double                  smoothTriangleRate;
        
        Stats();

        void reset();
    };

protected:

    /** Time at which the previous endFrame() was called */
    RealTime            lastTime;

    Stats               m_stats;

    /** Storage for setVARs.  Cleared by endIndexedPrimitives. */
    Array<VertexRange>          m_tempVAR;

    class VARState {
    public:
        int             highestEnabledTexCoord;

        VARState() : highestEnabledTexCoord(-1) {}
    };
    
    /** Note: note backed up by push/pop, since push/pop can't be
        called inside indexed primitives */
    VARState            m_varState;

public:

    inline const Stats& stats() {
        return m_stats;
    }

    // These are abstracted to make it easy to put breakpoints in them
    /**
       State change to RenderDevice.
       Use to update the state change statistics when raw OpenGL calls are made. */
    inline void majStateChange(int inc = 1) {
       m_stats.majorStateChanges += inc;
    }

    /** 
      State change to RenderDevice.
     Use to update the state change statistics when raw OpenGL calls are made. */
    inline void minStateChange(int inc = 1) {
        m_stats.minorStateChanges += inc;
    }

    /** 
     State change to OpenGL (possibly because of a state change to RenderDevice).
      Use to update the state change statistics when raw OpenGL calls are made. */
    inline void majGLStateChange(int inc = 1) {
        m_stats.majorOpenGLStateChanges += inc;
    }

    /** 
      State change to OpenGL (possibly because of a state change to RenderDevice).
      Use to update the state change statistics when raw OpenGL calls are made. */
    inline void minGLStateChange(int inc = 1) {
        m_stats.minorOpenGLStateChanges += inc;
    }

    /** Allows the UserInput to find the RenderDevice 
        @deprecated */
    static RenderDevice*        lastRenderDeviceCreated;

protected:

    /** Times swapbuffers */
    Stopwatch                   m_swapTimer;

public:

    RenderDevice();

    ~RenderDevice();

    /**
     Prints a human-readable description of this machine
     to the text output stream.  Either argument may be NULL.
     */
    void describeSystem(TextOutput& t);

    void describeSystem(std::string& s);

    /**
     \brief Checkmarks all RenderDevice state (anything that can be
     set using RenderDevice methods) except for the currently bound
     vertex arrays.

     If you are using some other OpenGL state that is not covered by
     any of the above (e.g., the glReadBuffer and other buffer
     options), you can call glPushAttrib(GL_ALL_ATTRIB_BITS)
     immediately before pushState to ensure that it is pushed as well.
     */
    void pushState();

    /** \brief Pushes the current state, then set the specified frame
        buffer and matches the viewport to it.*/
    void pushState(const FramebufferRef& fb);

    /**
     Sets all state to a clean rendering environment.
     */
    void resetState();

    /**
     Restores all state to whatever was pushed previously.  Push and 
     pop must be used in matching pairs.
     */
    void popState();

    /** To clear the alpha portion of the color buffer, remember to
        enable alpha write */
    void clear(bool clearColor, bool clearDepth, bool clearStencil);

    /** Clears color, depth, and stencil. */
    inline void clear() {
        clear(true, true, true);
    }

    /** \sa drawBuffer() */
    enum DrawBuffer {
        DRAW_NONE = GL_NONE, 
        DRAW_FRONT_LEFT = GL_FRONT_LEFT, 
        DRAW_FRONT_RIGHT = GL_FRONT_RIGHT, 
        DRAW_BACK_LEFT = GL_BACK_LEFT, 
        DRAW_BACK_RIGHT = GL_BACK_RIGHT, 
        DRAW_FRONT = GL_FRONT, 
        DRAW_BACK = GL_BACK, 
        DRAW_LEFT = GL_LEFT, 
        DRAW_RIGHT = GL_RIGHT, 
        DRAW_FRONT_AND_BACK = GL_FRONT_AND_BACK, 
        DRAW_AUX0 = GL_AUX0, 
        DRAW_AUX1 = GL_AUX1, 
        DRAW_AUX2 = GL_AUX2, 
        DRAW_AUX3 = GL_AUX3,

        DRAW_COLOR0 = GL_COLOR_ATTACHMENT0, 
        DRAW_COLOR1 = GL_COLOR_ATTACHMENT1, 
        DRAW_COLOR2 = GL_COLOR_ATTACHMENT2, 
        DRAW_COLOR3 = GL_COLOR_ATTACHMENT3, 
        DRAW_COLOR4 = GL_COLOR_ATTACHMENT4, 
        DRAW_COLOR5 = GL_COLOR_ATTACHMENT5, 
        DRAW_COLOR6 = GL_COLOR_ATTACHMENT6, 
        DRAW_COLOR7 = GL_COLOR_ATTACHMENT7, 
        DRAW_COLOR8 = GL_COLOR_ATTACHMENT8, 
        DRAW_COLOR9 = GL_COLOR_ATTACHMENT9, 
        DRAW_COLOR10 = GL_COLOR_ATTACHMENT10, 
        DRAW_COLOR11 = GL_COLOR_ATTACHMENT11, 
        DRAW_COLOR12 = GL_COLOR_ATTACHMENT12, 
        DRAW_COLOR13 = GL_COLOR_ATTACHMENT13, 
        DRAW_COLOR14 = GL_COLOR_ATTACHMENT14, 
        DRAW_COLOR15 = GL_COLOR_ATTACHMENT15,

        DRAW_CURRENT
    };

    /** The constants that correspond to DrawBuffer have the same
        value, so that you can safely cast between them. All have
        the corresponding OpenGL constant.
      
        \sa readBuffer()
    */
    enum ReadBuffer {
        READ_NONE = GL_NONE,
        READ_FRONT_LEFT = GL_FRONT_LEFT, 
        READ_FRONT_RIGHT = GL_FRONT_RIGHT, 
        READ_BACK_LEFT = GL_BACK_LEFT,
        READ_BACK_RIGHT = GL_BACK_RIGHT,
        READ_FRONT = GL_FRONT, 
        READ_BACK = GL_BACK, 
        READ_LEFT = GL_LEFT,
        READ_RIGHT = GL_RIGHT, 
        READ_AUX0 = GL_AUX0, 
        READ_AUX1 = GL_AUX1, 
        READ_AUX2 = GL_AUX2, 
        READ_AUX3 = GL_AUX3,

        READ_COLOR0 = GL_COLOR_ATTACHMENT0, 
        READ_COLOR1 = GL_COLOR_ATTACHMENT1, 
        READ_COLOR2 = GL_COLOR_ATTACHMENT2, 
        READ_COLOR3 = GL_COLOR_ATTACHMENT3, 
        READ_COLOR4 = GL_COLOR_ATTACHMENT4, 
        READ_COLOR5 = GL_COLOR_ATTACHMENT5, 
        READ_COLOR6 = GL_COLOR_ATTACHMENT6, 
        READ_COLOR7 = GL_COLOR_ATTACHMENT7, 
        READ_COLOR8 = GL_COLOR_ATTACHMENT8, 
        READ_COLOR9 = GL_COLOR_ATTACHMENT9, 
        READ_COLOR10 = GL_COLOR_ATTACHMENT10, 
        READ_COLOR11 = GL_COLOR_ATTACHMENT11, 
        READ_COLOR12 = GL_COLOR_ATTACHMENT12, 
        READ_COLOR13 = GL_COLOR_ATTACHMENT13, 
        READ_COLOR14 = GL_COLOR_ATTACHMENT14, 
        READ_COLOR15 = GL_COLOR_ATTACHMENT15,
        READ_CURRENT
    };
    
    enum DepthTest {
        DEPTH_GREATER = GL_GREATER,
        DEPTH_LESS = GL_LESS,
        DEPTH_GEQUAL = GL_GEQUAL,  
        DEPTH_LEQUAL = GL_LEQUAL, 
        DEPTH_NOTEQUAL = GL_NOTEQUAL, 
        DEPTH_EQUAL = GL_EQUAL,   
        DEPTH_ALWAYS_PASS = GL_ALWAYS,
        DEPTH_NEVER_PASS = GL_NEVER, 
        DEPTH_CURRENT
    };

    enum AlphaTest {
        ALPHA_GREATER = GL_GREATER,
        ALPHA_LESS = GL_LESS,
        ALPHA_GEQUAL = GL_GEQUAL,  
        ALPHA_LEQUAL = GL_LEQUAL,
        ALPHA_NOTEQUAL = GL_NOTEQUAL,
        ALPHA_EQUAL = GL_EQUAL,  
        ALPHA_ALWAYS_PASS = GL_ALWAYS, 
        ALPHA_NEVER_PASS = GL_NEVER,
        ALPHA_CURRENT
    };

    enum StencilTest {
        STENCIL_GREATER = GL_GREATER,  
        STENCIL_LESS = GL_LESS, 
        STENCIL_GEQUAL = GL_GEQUAL,
        STENCIL_LEQUAL = GL_LEQUAL, 
        STENCIL_NOTEQUAL = GL_NOTEQUAL,
        STENCIL_EQUAL = GL_EQUAL, 
        STENCIL_ALWAYS_PASS = GL_ALWAYS, 
        STENCIL_NEVER_PASS = GL_NEVER,
        STENCIL_CURRENT
    };

    enum BlendFunc {
        BLEND_SRC_ALPHA = GL_SRC_ALPHA,
        BLEND_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA, 
        BLEND_ONE = GL_ONE,
        BLEND_ZERO = GL_ZERO,
        BLEND_SRC_COLOR = GL_SRC_COLOR, 
        BLEND_DST_COLOR = GL_DST_COLOR,  
        BLEND_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR, 
        BLEND_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR, 
        BLEND_CURRENT
    };
    
    enum BlendEq {
        BLENDEQ_MIN = GL_MIN, 
        BLENDEQ_MAX = GL_MAX,
        BLENDEQ_ADD = GL_FUNC_ADD,
        BLENDEQ_SUBTRACT = GL_FUNC_SUBTRACT,
        BLENDEQ_REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
        BLENDEQ_CURRENT
    };

    enum StencilOp {
        STENCIL_INCR_WRAP = GL_INCR_WRAP, 
        STENCIL_DECR_WRAP = GL_DECR_WRAP,
        STENCIL_KEEP = GL_KEEP, 
        STENCIL_INCR = GL_INCR,
        STENCIL_DECR = GL_DECR,
        STENCIL_REPLACE = GL_REPLACE,
        STENCIL_ZERO = GL_ZERO,  
        STENCIL_INVERT = GL_INVERT, 
        STENCILOP_CURRENT
    };

    enum CullFace {
        CULL_FRONT = GL_FRONT,
        CULL_BACK = GL_BACK,
        CULL_NONE = GL_NONE,
        CULL_FRONT_AND_BACK = GL_FRONT_AND_BACK,
        CULL_CURRENT
    };

    enum ShadeMode {
        SHADE_FLAT = GL_FLAT,  
        SHADE_SMOOTH = GL_SMOOTH,
        SHADE_CURRENT
    };

    /**
      Arguments to setTextureCombineMode

      TEX_INTERPOLATE = GL_DECAL (blend between previous and current texture based on current alpha)

      Note: GL_INTERPOLATE_ARB is not supported because it takes two arguments.  You can access it
      yourself using glTexEnvi.

      TEX_BLEND = GL_BLEND (for luminance textures, blend against a constant)
      
      TEX_ADD is only present if GLCaps::supports("GL_EXT_texture_env_add")
      See http://oss.sgi.com/projects/ogl-sample/registry/ARB/texture_env_add.txt
    
      TEX_ADD_SIGNED and TEX_SUBTRACT are only present if GLCaps::supports("GL_ARB_texture_env_combine")
      See http://oss.sgi.com/projects/ogl-sample/registry/ARB/texture_env_combine.txt

      TEX_DOT3_RGB and TEX_DOT3_RGBA are only present if GLCaps::supports("GL_ARB_texture_env_dot3")
      http://oss.sgi.com/projects/ogl-sample/registry/ARB/texture_env_dot3.txt
    */
    enum CombineMode {TEX_REPLACE, TEX_BLEND, TEX_INTERPOLATE, TEX_ADD, TEX_MODULATE, 
                      TEX_ADD_SIGNED, TEX_SUBTRACT, TEX_DOT3_RGB, TEX_DOT3_RGBA,
                      TEX_CURRENT};

    #define MAX_BUFFER_SIZE 27

    /**
     Call to begin the rendering frame.
     */
    void beginFrame();

    /**
     Call to end the current frame and schedules a OSWindow::swapGLBuffers call
     to occur some time before beginFrame.  Because that swapGLBuffers might not
     actually occur until the next beginFrame, there is up to one frame of latency
     on the image displayed.  This allows the CPU to execute while 
     the GPU is still rendering, providing net higher performance.
     */
    void endFrame();

    inline bool swapBuffersAutomatically() const {
        return _swapBuffersAutomatically;
    }

    /** Manually swap the front and back buffers.  Using swapBuffersAutomatically() is recommended
        instead of manually swapping because it has higher performance.*/
    void swapBuffers();

    /** 
        By default, OSWindow::swapGLBuffers is invoked automatically
        between RenderDevice::endFrame and the following RenderDevice::beginFrame
        to update the front buffer (what the user sees) from the back buffer (where
        rendering commands occur).  You may want to suppress this behavior, for example,
        in order
        to render to the back buffer and capture the result in a texture.
    
        The state of swapBuffersAutomatically is <B>not</B> stored by
        RenderDevice::pushState because it is usually invoked outside
        of RenderDevice::beginFrame / endFrame. 
    */
    void setSwapBuffersAutomatically(bool b);

    /** Measures the amount of time spent in swapBuffers.  If high, indicates that
        the CPU and GPU are not working in parallel*/
    const Stopwatch& swapBufferTimer() const {
        return m_swapTimer;
    }

    /**
     Use ALWAYS_PASS to shut off testing.
     */
    void setDepthTest(DepthTest test);
    void setStencilTest(StencilTest test);

    void setRenderMode(RenderMode mode);
    RenderMode renderMode() const;

    /**
     Sets the constant used in the stencil test and operation (if op == STENCIL_REPLACE)
     */
    void setStencilConstant(int reference);

	/** If the alpha test is ALPHA_CURRENT, the reference is ignored */
    void setAlphaTest(AlphaTest test, double reference);

    AlphaTest alphaTest() const;

    double alphaTestReference() const;

    /**
     Sets the frame buffer that is written to.  Used to intentionally
     draw to the front buffer and for stereo rendering.  Its operation
     is sensitive to the current framebuffer being written to. If the
     framebuffer is the primary display then only visible buffers may
     be specified.  If framebuffer() is non-NULL then the drawBuffer
     is stored for later use but the actual draw buffers remain the
     ones specified by the framebuffer.

     When using a G3D::Framebuffer it is not necessary to explicitly set the
     draw buffer; that is handled automatically by beforePrimitive().
     */
    void setDrawBuffer(DrawBuffer drawBuffer);

    inline DrawBuffer drawBuffer() const {
        return state.drawBuffer;
    }

    void setReadBuffer(ReadBuffer readBuffer);

    inline ReadBuffer readBuffer() const {
        return state.readBuffer;
    }

    inline void setDepthRange(double low, double high);

    /** Color writing is on by default.  Disabling color write allows a program to 
        render to the depth and stencil buffers without creating a visible image in the frame buffer.  
        This is useful for occlusion culling, shadow rendering, and some computational solid geometry algorithms.  
        Rendering may be significantly accelerated when color write is disabled. */
    inline void setColorWrite(bool b);

    /** Returns true if colorWrite is enabled */
    bool colorWrite() const;

    /** The frame buffer may optionally have an alpha channel for each pixel, depending on how
        the G3D::OSWindow was initialized (see G3D::RenderDevice::init, and G3D::OSWindow::Settings).
        When the alpha channel is present, rendering to the screen also renders to the alpha 
        channel by default.  Alpha writing is used for render-to-texture and deferred lighting effects.
    
        Rendering to the alpha channel does not produce transparency effects--this is an alpha
        output, not an alpha input.  See RenderDevice::setBlendFunc for a discussion of blending.*/
    inline void setAlphaWrite(bool b);

    /** Defaults to true */
    inline void setDepthWrite(bool b);

    /** Returns true if depthWrite is enabled */
    bool depthWrite() const;

    /** Returns true if alphaWrite is enabled */
    bool alphaWrite() const;

    /**
     Equivalent to glShadeModel
     */
    inline void setShadeMode(ShadeMode s);

    /**
     If wrapping is not supported on the device, the nearest mode is
     selected.  Unlike OpenGL, stencil writing and testing are
     independent. You do not need to enable the stencil test to use
     the stencil op.

     Use KEEP, KEEP, KEEP to disable stencil writing.  Equivalent to a
     combination of glStencilTest, glStencilFunc, and glStencilOp.


     If there is no depth buffer, the depth test always passes.  If there
     is no stencil buffer, the stencil test always passes.
     */
    void setStencilOp(
        StencilOp                       fail,
        StencilOp                       zfail,
        StencilOp                       zpass);

    /**
     When GLCaps::GL_ARB_stencil_two_side is true, separate
     stencil operations can be used for front and back faces.  This
     is useful for rendering shadow volumes.
     */
    void setStencilOp(
        StencilOp                       frontStencilFail,
        StencilOp                       frontZFail,
        StencilOp                       frontZPass,
        StencilOp                       backStencilFail,
        StencilOp                       backZFail,
        StencilOp                       backZPass);

    /**
     Equivalent to <A HREF="http://developer.3dlabs.com/GLmanpages/glblendfunc.htm">glBlendFunc</A>
	 and <A HREF="http://developer.apple.com/documentation/Darwin/Reference/ManPages/man3/glBlendEquation.3.html">glBlendEquation</A>.

     Use 
       <CODE>setBlendFunc(RenderDevice::BLEND_ONE, RenderDevice::BLEND_ZERO, RenderDevice::BLENDEQ_ADD)</CODE>
     to shut off blending.
     
     Use 
       <CODE>setBlendFunc(RenderDevice::BLEND_SRC_ALPHA, RenderDevice::BLEND_ONE_MINUS_SRC_ALPHA, RenderDevice::BLENDEQ_ADD)</CODE>
     for unmultiplied alpha blending and
       <CODE>setBlendFunc(RenderDevice::BLEND_ONE, RenderDevice::BLEND_ONE_MINUS_SRC_ALPHA, RenderDevice::BLENDEQ_ADD)</CODE>
     for premultiplied alpha.

	 %Draw your objects from back to front, objects with alpha last. Objects with alpha only get drawn properly if the things
	 they're occluding have been drawn before the alpha'd objects. 

	 Generally, turn alpha on, draw your alpha-blended things, then turn alpha off. 
     */
    void setBlendFunc(
        BlendFunc                       src,
        BlendFunc                       dst,
        BlendEq                         eq = BLENDEQ_ADD);

    /** 
      Sets a 2D clipping region (OpenGL scissor region) relative to the current window
      dimensions (not the viewport). Prevents rendering outside the clip region.

      Set to Rect2D::inf() to disable. Default is disabled.

      Note that the clip uses G3D 2D coordinates, where the upper-left of the window
      is (0, 0).
      */
    void setClip2D(const Rect2D& clip);

    /** If enabled, returns the current clip region, othrwise the viewport. */
    Rect2D clip2D() const;

    /**
     Equivalent to glLineWidth.
     */
    void setLineWidth(
        double                          width);

    /**
     Equivalent to glPointSize.
     */
    void setPointSize(
        double                          diameter);

    /**
     This is not the OpenGL MODELVIEW matrix: it is a matrix that maps
     object space to world space.  The actual MODELVIEW matrix
     is cameraToWorld.inverse() * objectToWorld.  You can retrieve it
     with getModelViewMatrix.
     */
    void setObjectToWorldMatrix(
        const CoordinateFrame&          cFrame);

    const CoordinateFrame& objectToWorldMatrix() const;

    /**
     See RenderDevice::setObjectToWorldMatrix.
     */
    void setCameraToWorldMatrix(
        const CoordinateFrame&          cFrame);

    const CoordinateFrame& cameraToWorldMatrix() const;

    Matrix4 projectionMatrix() const;

    /**
     cameraToWorld.inverse() * objectToWorld
     */
    CoordinateFrame modelViewMatrix() const;

    /**
     projection() * cameraToWorld.inverse() * objectToWorld
     */
    Matrix4 modelViewProjectionMatrix() const;


    /**
       To set a typical 3D perspective matrix, use either

       <code> renderDevice->setProjectionMatrix(Matrix4::perspectiveProjection(...)) </code>

       or call setProjectionAndCameraMatrix.
     */
    void setProjectionMatrix(const Matrix4& P);

    /**
     m is a 16-element matrix in row major order for multiplying
     texture coordinates:

     v' = M v

     Matrix is column major.

     All texture operations check textureUnit against the number of
     available texture units when in debug mode.

     Equivalent to glMatrixMode(GL_TEXTURE); glLoadMatrix(...); on a transposed matrix.
     */
    void setTextureMatrix(
        uint32                  textureUnit,
        const float*            m);

    void setTextureMatrix(
        uint32                  textureUnit,
        const double*           m);

    void setTextureMatrix(
        uint32                  textureUnit,
        const Matrix4&          m);

    void setTextureMatrix(
        uint32                  textureUnit,
        const CoordinateFrame&  c);

    /** A bias affects which MIP-map level is used. bias = 0 uses the MIP-map level
        automatically computed.  Bias > 0 selects a blurrier mipmap (avoids aliasing
        artifacts).  Bias < 0 selects a sharper mipmap (avoids blur).  Integer
        values correspond to a whole mip-level shift.  Default is 0.

        See also http://oss.sgi.com/projects/ogl-sample/registry/EXT/texture_lod_bias.txt.
    */
    void setTextureLODBias(
        uint32                  unit,
        float                   bias);

    /**
     The matrix returned may not be the same as the
     underlying hardware matrix-- the y-axis is flipped
     in hardware when a texture with invertY = true is specified.
     */
    Matrix4 getTextureMatrix(uint32 textureUnit);

    /**
     The combine mode specifies how to combine the result of a texture
     lookup with the accumulated fragment value (e.g. the output of
     the previous combine or the constant color for the first texture
     unit).

     Consider using shaders on newer cards instead of texture combiners; 
     they are much easier to use.  See also
     http://oss.sgi.com/projects/ogl-sample/registry/ARB/texture_env_crossbar.txt
     http://oss.sgi.com/projects/ogl-sample/registry/ARB/texture_env_dot3.txt

     The initial combine op is TEX_MODULATE 
     Equivalent to glTexEnv.
     */
    void setTextureCombineMode(
        uint32                    textureUnit,
        const CombineMode         texCombine);


    /**
     Resets the matrix, texture, combine op, and constant for a texture unit.
     */
    void resetTextureUnit(
        uint32                    textureUnit);

    /**
     Equivalent to glPolygonOffset
     */
    void setPolygonOffset(
        double                  offset);

    /**
     Set the vertex color (equivalent to glColor).
     */
    inline void setColor(const Color4& color) {
        state.color = color;
        glColor4fv(state.color);
    }


    inline void setColor(const Color3& color) {
        state.color = Color4(color, 1);
        glColor3fv(state.color);
    }

    /**
     Equivalent to glNormal
     */
    void setNormal(const Vector3& normal);

    /**
     Equivalent to glTexCoord
     */
    void setTexCoord(uint32 textureUnit, const Vector4& texCoord);
    void setTexCoord(uint32 textureUnit, const Vector3& texCoord);
    void setTexCoord(uint32 textureUnit, const Vector3int16& texCoord);
    void setTexCoord(uint32 textureUnit, const Vector2& texCoord);
    void setTexCoord(uint32 textureUnit, const Vector2int16& texCoord);
    void setTexCoord(uint32 textureUnit, double texCoord);

    /**
     Equivalent to glCullFace
     */
    void setCullFace(CullFace f);

    inline CullFace cullFace() const {
        return state.cullFace;
    }

    /**
     (0, 0) is the <B>upper</B>-left corner of the screen.
     */
    void setViewport(const Rect2D& v);
    Rect2D viewport() const;

    /**
     Vertices are "sent" rather than "set" because they
     cause action.
     */
    void sendVertex(const Vector2& vertex);
    void sendVertex(const Vector3& vertex);
    void sendVertex(const Vector4& vertex);

    void setProjectionAndCameraMatrix(const class GCamera& camera);

    /** Computes a GCamera that matches the current projection and
        camera matrix. This is not necessarily the same camera that
        was used with setProjectionAndCameraMatrix().  Fails if
        not in perspective projection mode (e.g., if push2D was used).
    */
    GCamera projectionAndCameraMatrix() const;

    /**
     Analogous to glBegin.  See the example in the detailed description
     section of this page.
     */
    void beginPrimitive(Primitive p);

    /**
     Analogous to glEnd.  See the example in the detailed description
     section of this page.
     */
    void endPrimitive();

    void beginIndexedPrimitives();
    void endIndexedPrimitives();

    /** The vertex, normal, color, and tex coord arrays need not come from
        the same VertexBuffer. 

        The format of a VertexRange array is restricted depending on its use.  The
        following table (from http://oss.sgi.com/projects/ogl-sample/registry/ARB/vertex_program.txt)
        shows the underlying OpenGL restrictions:

     <PRE>

                                       Normal    
      Command                 Sizes    ized?   Types
      ----------------------  -------  ------  --------------------------------
      VertexPointer           2,3,4     no     short, int, float, double
      NormalPointer           3         yes    byte, short, int, float, double
      ColorPointer            3,4       yes    byte, ubyte, short, ushort,
                                               int, uint, float, double
      IndexPointer            1         no     ubyte, short, int, float, double
      TexCoordPointer         1,2,3,4   no     short, int, float, double
      EdgeFlagPointer         1         no     boolean
      VertexAttribPointerARB  1,2,3,4   flag   byte, ubyte, short, ushort,
                                               int, uint, float, double
      WeightPointerARB        >=1       yes    byte, ubyte, short, ushort,
                                               int, uint, float, double
      VertexWeightPointerEXT  1         n/a    float
      SecondaryColor-         3         yes    byte, ubyte, short, ushort,
        PointerEXT                             int, uint, float, double
      FogCoordPointerEXT      1         n/a    float, double
      MatrixIndexPointerARB   >=1       no     ubyte, ushort, uint

      Table 2.4: Vertex array sizes (values per vertex) and data types.  The
      "normalized" column indicates whether fixed-point types are accepted
      directly or normalized to [0,1] (for unsigned types) or [-1,1] (for
      singed types). For generic vertex attributes, fixed-point data are
      normalized if and only if the <normalized> flag is set.

  </PRE>
    
    */
    void setVertexArray(const class VertexRange& v);
    void setNormalArray(const class VertexRange& v);
    void setColorArray(const class VertexRange& v);
    void setTexCoordArray(unsigned int unit, const class VertexRange& v);

    /** 
        Set a series of vertex arrays
     */
    void setVARs(const class VertexRange& vertex, const class VertexRange& normal, const class VertexRange& color,
                 const Array<VertexRange>& texCoord);

    /** Set a series of vertex arrays simultaneously. This call allows you to change the
        VertexBuffer that the VARs are in within a single beginIndexedPrimitives() call,
        saving some state changes. */
    void setVARs(const class VertexRange& vertex, const class VertexRange& normal = VertexRange(), const class VertexRange& texCoord0 = VertexRange(),
                 const class VertexRange& texCoord1 = VertexRange());

    /** Returns the OSWindow used by this RenderDevice */
    OSWindow* window() const;

    /**
     Vertex attributes are a generalization of the various per-vertex
     attributes that relaxes the format restrictions.  There are at least
     16 attributes on any card (some allow more).  These attributes have
     special meaning under the fixed function pipeline, as follows:

    <PRE>
    Generic
    Attribute   Conventional Attribute       Conventional Attribute Command
    ---------   ------------------------     ------------------------------
         0      vertex position              Vertex
         1      vertex weights 0-3           WeightARB, VertexWeightEXT
         2      normal                       Normal
         3      primary color                Color
         4      secondary color              SecondaryColorEXT
         5      fog coordinate               FogCoordEXT
         6      -                            -
         7      -                            -
         8      texture coordinate set 0     MultiTexCoord(TEXTURE0, ...)
         9      texture coordinate set 1     MultiTexCoord(TEXTURE1, ...)
        10      texture coordinate set 2     MultiTexCoord(TEXTURE2, ...)
        11      texture coordinate set 3     MultiTexCoord(TEXTURE3, ...)
        12      texture coordinate set 4     MultiTexCoord(TEXTURE4, ...)
        13      texture coordinate set 5     MultiTexCoord(TEXTURE5, ...)
        14      texture coordinate set 6     MultiTexCoord(TEXTURE6, ...)
        15      texture coordinate set 7     MultiTexCoord(TEXTURE7, ...)
       8+n      texture coordinate set n     MultiTexCoord(TEXTURE0+n, ...)
    </PRE>

      @param normalize If true, the coordinates are forced to a [0, 1] scale
    */
    void setVertexAttribArray(unsigned int attribNum, const class VertexRange& v, bool normalize);

    /**
     Draws the specified kind of primitive from the current vertex array.

     @deprecated Use sendIndices(RenderDevice::Primitive, const VertexRange&)
     */
    template<class T>
    void sendIndices
    (RenderDevice::Primitive primitive, int numIndices, const T* index) {
        debugAssertM(currentFramebufferComplete(), "Incomplete Framebuffer");
        internalSendIndices(primitive, sizeof(T), numIndices, index, 1, false);
        
        // Mark all active arrays as busy.
        setVARAreaMilestone();
        
        countTriangles(primitive, numIndices);
    }

    /** Send indices from an index buffer stored inside a vertex
     buffer. This is faster than sending from main
     memory on most GPUs.
    */
    void sendIndices
    (RenderDevice::Primitive primitive, const VertexRange& indexVAR);

    /** Send indices from an index buffer stored inside a vertex
     buffer. This is faster than sending from main
     memory on most GPUs.

     Inside the vertex shader, gl_InstanceID is
     bound to the number of the instance.  The first instance is 0,
     the last is \a numInstances - 1.

     (Equivalent to glDrawElementsInstanced)

     \param numInstances number of instances of these indices to send
    */
    void sendIndicesInstanced
    (RenderDevice::Primitive primitive, 
     const VertexRange&              indexVAR,
     int                     numInstances);

private:

    /** Called by both sendIndices and sendIndicesInstanced */
    void sendIndices
    (RenderDevice::Primitive primitive, 
     const VertexRange&              indexVAR,
     int                     numInstances, 
     bool                    drawInstanced);

public:

    /**
     Renders sequential vertices from the current vertex array.
     (Equivalent to glDrawArrays)
     */
    void sendSequentialIndices
    (RenderDevice::Primitive primitive, 
     int                     numVertices);

    /**
     Renders sequential vertices from the current vertex array for
     multiple instances. 

     Inside the vertex shader, gl_InstanceID is
     bound to the number of the instance.  The first instance is 0,
     the last is \a numInstances - 1.

     (Equivalent to glDrawArraysInstanced)

     \param numInstances number of instances of these indices to send
     */
    void sendSequentialIndicesInstanced
    (RenderDevice::Primitive primitive, int numVertices, int numInstances);

    /**
     Draws the specified kind of primitive from the current vertex array.
     
     @deprecated Use sendIndices(RenderDevice::Primitive, const VertexRange&)
     */
    template<class T>
    void sendIndices(RenderDevice::Primitive primitive, 
                     const Array<T>& index) {
        sendIndices(primitive, index.size(), index.getCArray());
    }
    
    void setStencilClearValue(int s);
    void setDepthClearValue(double d);
    void setColorClearValue(const Color4& c);

    /**
     @param textureUnit >= 0
     @param texture Set to NULL to disable the unit
     */
    void setTexture(
        uint32                textureUnit,
        const Texture::Ref&   texture);

    /** Returns the number of textures available.  May be higher
        than the number of texture units if the programmable
        pipeline provides more textures than the fixed function
        pipeline.*/
    uint32 numTextures() const;

    /** Returns the number of texture units 
        (texture + reg combiner + matrix) available.
        This only applies to the fixed function pipeline.
    */
    uint32 numTextureUnits() const;

    /** Returns the number of texture coordinates allowed.
        This may be greater than the number of texture matrices.*/
    uint32 numTextureCoords() const;

    /**
     Used by G3D::Shader.  Most programs do not need to call this directly.

     Set the current shader.  You may call VertexAndPixelShader::bindArgList either
     before or after this, or use the variation of this call that
     includes an arg list.

     See also G3D::setShader. 
     */
    void setVertexAndPixelShader(const VertexAndPixelShaderRef& s);

    /** Throws VertexAndPixelShader::ArgumentError if the arguments provided
      do not match the arguments declared */
    void setVertexAndPixelShader(const VertexAndPixelShaderRef& s,
            const VertexAndPixelShader::ArgList& args);

    /**
     A G3D::Shader abstracts the programmable graphics pipeline.
     It contains routines that execute before and after
     each primitive group to set other RenderDevice state.     
     */
	void setShader(const ShaderRef& s);

    /**
     Automatically enables vertex programs when they are set. 
     Assumes GLCaps::supports_GL_ARB_vertex_program() is true.

     Don't mix VertexPrograms (old API) with VertexShaders (new API).

     @param vp Set to NULL to use the fixed function pipeline.
     @deprecated Use G3D::RenderDevice::setShader
     */
    void setVertexProgram(const VertexProgramRef& vp);

    /**
     Sets vertex program arguments for vertex programs outputted by Cg.
     The names of arguments are read from comments.

     <PRE>
        ArgList args;
        args.set("MVP", renderDevice->getModelViewProjection());
        args.set("height", 3);
        args.set("viewer", Vector3(1, 2, 3));
        renderDevice->setVertexProgram(toonShadeVP, args);
     </PRE>


     Don't mix VertexPrograms (old API) with VertexShaders (new API).
     @param args must include *all* arguments or an assertion will fail
     @deprecated Use G3D::RenderDevice::setShader
     */
    void setVertexProgram(const VertexProgramRef& vp,
                          const GPUProgram::ArgList& args);

    /**
     (Automatically enables pixel programs when they are set.) 
     Assumes GPUProgram() is true.
     Don't mix PixelPrograms (old API) with PixelShaders (new API).
     @param pp Set to NULL to use the fixed function pipeline.
     @deprecated Use G3D::RenderDevice::setShader
     */
    void setPixelProgram(const PixelProgramRef& pp);

    /**
     It is recommended to call RenderDevice::pushState immediately before
     setting the pixel program, since the arguments can affect texture
     state that will only be restored with RenderDevice::popState.
     Don't mix PixelPrograms (old API) with PixelShaders (new API).
     @deprecated Use RenderDevice::setShader
     */
    void setPixelProgram(const PixelProgramRef& pp,
                         const GPUProgram::ArgList& args);
    
    /**
      Reads a depth buffer value (1 @ far plane, 0 @ near plane) from
      the given screen coordinates (x, y) where (0,0) is the top left
      corner of the width x height screen.  Result is undefined for x, y not
      on screen.

      The result is sensitive to the projection and camera to world matrices.

      If you need to read back the entire depth buffer, use OpenGL glReadPixels
      calls instead of many calls to getDepthBufferValue.
     */
    double getDepthBufferValue(
        int                 x,
        int                 y) const;


    /**
     Description of the graphics card and driver version.
     */
    std::string getCardDescription() const;


    /** When enabled and lighting is enabled, shades backfaces with reversed normals.
        See glLightModeli. */
    void enableTwoSidedLighting();
    void disableTwoSidedLighting();

	/** Automatically called immediately before a primitive group.  User code should only
        call this if making raw OpenGL calls (i.e., "glBegin"), in which case it should be
        called immediately before the glBegin and afterPrimitve should be called immediately
        after the glEnd.
      */
    void beforePrimitive();

    /** Automatically called immediately after a primitive group. See also beforePrimitive.*/
    void afterPrimitive();

private:

    /**
     For performance, we don't actually unbind a texture when
     turning off a texture unit, we just disable it.  If it 
     is enabled with the same texture, we've saved a swap.
    */
    uint32               currentlyBoundTexture[GLCaps::G3D_MAX_TEXTURE_UNITS];

    /**
     Snapshot of the state maintained by the render device.
     */
    // WARNING: if you add state, you must initialize it in
    // the constructor and RenderDevice::init and set it in
    // setState().
    class RenderState {
    public:
        friend class RenderDevice;

        class TextureUnit {
        public:
            Vector4                 texCoord;

            /** NULL if not bound */
            Texture::Ref            texture;

            float                   textureMatrix[16];

            /** Only available for Fixed Function, low number texture units */
            CombineMode             combineMode;
            float                   LODBias;

            TextureUnit();

            inline bool operator==(const TextureUnit& other) const {
                return 
                    (texCoord == other.texCoord) &&
                    (texture == other.texture) &&
                    (memcmp(textureMatrix, other.textureMatrix, sizeof(float)*16) == 0) &&
                    (combineMode == other.combineMode) &&
                    (LODBias == other.LODBias);
            }

            inline bool operator!=(const TextureUnit& other) const {
                return !(*this == other);
            }
        };

        class Lights {
        public:
            GLight                      light[MAX_LIGHTS];
            bool                        lightEnabled[MAX_LIGHTS];
            bool                        twoSidedLighting;

            // Ambient light level
            Color4                      ambient;
            bool                        lighting;

            /** True if lights have been written to since the last pushState.
                This is set to false in pushState and to true whenever
                a light changes.*/
            bool                        changed;

            // default changed to true so that a fresh Lights object will
            // provoke a state reset.  Pushstate supresses this
            Lights() : changed(true) {}

            bool operator==(const Lights& other) const;

            inline bool operator!=(const Lights& other) const {
                return !(*this == other);
            }
        };

        class Stencil {
        public:
            StencilTest                 stencilTest;
            int                         stencilReference;
            int                         stencilClear;
            StencilOp                   frontStencilFail;
            StencilOp                   frontStencilZFail;
            StencilOp                   frontStencilZPass;
            StencilOp                   backStencilFail;
            StencilOp                   backStencilZFail;
            StencilOp                   backStencilZPass;
          
            bool operator==(const Stencil& other) const;

            inline bool operator!=(const Stencil& other) const {
                return !(*this == other);
            }
        };

        class Matrices {
        public:
            CoordinateFrame             objectToWorldMatrix;
            CoordinateFrame             cameraToWorldMatrix;
            CoordinateFrame             cameraToWorldMatrixInverse;
            Matrix4                     projectionMatrix;
            bool                        changed;

            inline Matrices() : changed(true) {}

            bool operator==(const Matrices& other) const;

            inline bool operator!=(const Matrices& other) const {
                return !(*this == other);
            }
        };

        Lights                      lights;

        Rect2D                      viewport;
        Rect2D                      clip2D;
        bool                        useClip2D;

        bool                        depthWrite;
        bool                        colorWrite;
        bool                        alphaWrite;

        DrawBuffer                  drawBuffer;
        ReadBuffer                  readBuffer;

        FramebufferRef              framebuffer;
        
        DepthTest                   depthTest;
        AlphaTest                   alphaTest;
        double                      alphaReference;

        double                      depthClear;
        Color4                      colorClear;

        CullFace                    cullFace;

        Stencil                     stencil;
        
        BlendFunc                   srcBlendFunc;
        BlendFunc                   dstBlendFunc;
        BlendEq                     blendEq;
        
        ShadeMode                   shadeMode;
    
        double                      polygonOffset;

        RenderMode                  renderMode;

        Color3                      specular;
        double                      shininess;

        double                      lowDepthRange;
        double                      highDepthRange;

        VertexAndPixelShaderRef     vertexAndPixelShader;

        ShaderRef                   shader;

        /** @deprecated */
        VertexProgramRef            vertexProgram;
        /** @deprecated */
        PixelProgramRef             pixelProgram;

        double                      lineWidth;
        double                      pointSize;

        Color4                      color;
        Vector3                     normal;

        /** Index of the highest texture unit that changed since pushState,
            used for short-circuiting work in popstate */
        int                         highestTextureUnitThatChanged;
        TextureUnit                 textureUnit[GLCaps::G3D_MAX_TEXTURE_UNITS];
        Matrices                    matrices;

        inline void touchedTextureUnit(int u) {
            highestTextureUnitThatChanged = iMax(highestTextureUnitThatChanged, u);
        }

        RenderState(int width = 1, int height = 1, int highestTextureUnitThatChanged = GLCaps::G3D_MAX_TEXTURE_UNITS);

        //bool operator==(const RenderState& other) const;
    };

    VertexAndPixelShaderRef         lastVertexAndPixelShader;

    GLint toGLStencilOp(RenderDevice::StencilOp op) const;

    /**
     True between beginPrimitive and endPrimitive
     */
    bool                            inPrimitive;

    /** Has beginOpenGL been called? */
    bool                            inRawOpenGL;

    /** True while invoking a Shader's methods */
    bool                            inShader;

    bool                            inIndexedPrimitive;

    int                             _numTextureUnits;

    int                             _numTextures;

    int                             _numTextureCoords;

    /**
     Called from the various setXLight functions.
     @param force When true, OpenGL state is changed
     regardless of whether RenderDevice thinks it is up to date.
     */
    void setLight(int i, const GLight* light, bool force);

    /**
     Current render state.
     */
    RenderState                     state;

    /**
     Old render states
     */
    Array<RenderState>              stateStack;

    /** Only called from pushState */
    void setState(
        const RenderState&          newState);

    bool                            _initialized;
    bool                            cleanedup;

    /** Cache of values supplied to supportsImageFormat.
        Works on pointers since there is no way for users
        to construct their own ImageFormats.
     */
    Table<const ImageFormat*, bool>      _supportedImageFormat;

	void push2D(const FramebufferRef& fb, const Rect2D& viewport);

    /** Sets the stencil test using the two-sided extension appropriate to this machine.*/
    void _setStencilTest(StencilTest test, int reference);

public:

    /** @deprecated 
      Use GLCaps::supports instead.
     */
    bool supportsOpenGLExtension(const std::string& extension) const;

    /**
      When true, the 6-argument version of RenderDevice::setStencilOp
      can set the front and back operations to different values.

      @deprecated 
      Use GLCaps::supports_two_sided_stencil instead.
     
    */
    bool supportsTwoSidedStencil() const;

    /**
     When true, Texture::DIM_2D_RECT textures can be created.
     @deprecated
     Use GLCaps::supports_GL_EXT_texture_rectangle instead.
     */
    bool supportsTextureRectangle() const;

    /**
     @deprecated
     Use VertexAndPixelShader
     */
    inline bool supportsVertexProgram() const;

    /**
     When true, NVIDIA Vertex Program 2.0 vertex programs can
     be loaded by VertexProgram.
     @deprecated
     Use VertexAndPixelShader
     */
    bool supportsVertexProgramNV2() const;

    /**
     @deprecated
     Use VertexAndPixelShader
     */
    inline bool supportsPixelProgram() const;

    /**
     When true, VertexRange arrays will be in video, not main memory,
     and much faster.
     @deprecated
     Use GLCaps::supports_GL_ARB_vertex_buffer_object instead.
     */
    bool supportsVertexBufferObject() const;

    /**
     Sets the framebuffer to render to.  Use NULL to set the desired rendering 
     target to the windowing system display.

	 Note that if the new framebuffer has different dimensions than the current one
	 the projectionMatrix and viewport will likely be incorrect. 

     See RenderDevice::pushState and push2D for a way to set the frame buffer and viewport
     simultaneously.

     If the current readBuffer() is not legal for \a fbo, it will be changed to READ_COLOR0
     or READ_NONE if there is no color0 attachment.

     @param fbo Framebuffer to render to.
    */
    void setFramebuffer(const Framebuffer::Ref &fbo);

    Framebuffer::Ref framebuffer() const {
        return state.framebuffer;
    }

    /**
     Checks to ensure that the current framebuffer is complete and error free.  

     @return false On Incomplete Framebuffer Error
     @return true On Complete Framebuffer
    */
    inline bool currentFramebufferComplete(std::string& whyIncomplete = dummyString) const {
        return state.framebuffer.isNull() || 
			   checkFramebuffer(whyIncomplete);
    }

    void push2D();

	/** 
	 Pushes all state, switches to the new framebuffer, and resizes the viewport and projection matrix accordingly.
	 */
    void push2D(const FramebufferRef& fb);

    /**
     Set up for traditional 2D rendering (origin = upper left, y increases downwards).
     
     Note: the viewport will range up to the number of pixels (e.g., (0,0)-(640,480)), as
     recommended in http://msdn.microsoft.com/library/default.asp?url=/library/en-us/opengl/apptips_7wqb.asp .
     Push2D also translates (sets the cameraToWorldMatrix) by (0.375, 0.375, 0) as recommended
     in the OpenGL manual.  This helps avoid rasterization holes due to float-to-int roundoff.

     */
    void push2D(const Rect2D& viewport);
    void pop2D();
 
    /**
     Automatically constructs a Win32Window (on Win32), X11Window (on Linux) or
     CarbonWindow (OS X) then calls the other init
     routine (provided for backwards compatibility).  The constructed
     window is deleted on shutdown.
     */
    void init(const OSWindow::Settings& settings = OSWindow::Settings());

    /**
     The renderDevice will <B>not</B> delete the window on cleanup.
     */
    void init(OSWindow* window);

    /** Returns true after RenderDevice::init has been called. */
    bool initialized() const;

    inline const Color4& color() const {
        return state.color;
    }

    inline ShadeMode shadeMode() const {
        return state.shadeMode;
    }

	/**
	 Shuts down the system.  This should be the last call you make.
	 */
    void cleanup();

    /** Returns the format of the backbuffer/COLOR0 buffer (NULL if there isn't such a buffer). */
    const ImageFormat* colorFormat() const;

    /**
     Takes a JPG screenshot of the front buffer and saves it to a file.
     Returns the name of the file that was written.
     Example: renderDevice->screenshot("screens/"); 

     Pressing the "movie" icon in the GApp developer HUD or pressing F6 also
     allows direct screenshot capture.
     */
    std::string screenshot(const std::string& filepath) const;

    /**
       @brief Takes a screenshot and puts the data into the G3D::GImage dest variable.

       Reads from the current read buffer; use setReadBuffer(RenderDevice::READ_FRONT)
       to explicitly read from the front buffer, which is substantially faster than
       reading from the back buffer.

       @param getAlpha If true, the alpha channel of the frame buffer is also read back.

       @param invertY It is faster to read back images upside down
       because that is how OpenGL stores them.  Set invertY=false for
       this fast but upsidedown result.
     */
    void screenshotPic(GImage& dest, bool getAlpha = false, bool invertY = true) const;

	/**
	 Pixel dimensions of the OpenGL window interior
	 */
    int width() const;

	/**
	 Pixel dimensions of the OpenGL window interior
	 */
    int height() const;

    /**
     You must also enableLighting.  Ambient light is handled separately.

     Lights are specified in <B>world space</B>-- they are not affected
     by the camera or object matrix.  Unlike OpenGL, you do not need to
     reset lights after you change the camera matrix.

     Inside a shader, lights set with setLight (which appear as gl_Light)
     are still in world space.

     setLight(i, NULL) disables a light.
     */
    void setLight(int num, const GLight& light);
    void setLight(int num, void*);

    /**
     Sets the current specular coefficient used in the lighting equation.
     Should be on the range 0 (perfectly diffuse) to 1 (bright specular
     highlight).
     */
    void setSpecularCoefficient(float s);
    void setSpecularCoefficient(const Color3& c);

    /**
     Sets the current shininess exponent used in the lighting equation.
     On the range 0 (large highlight) to 255 (tiny, focussed highlight).
     */
    void setShininess(float s);

    /**
     You must also RenderDevice::enableLighting.
     */
    void setAmbientLightColor(
        const Color3&        color);

    void setAmbientLightColor(
        const Color4&        color);

    const Color4& ambientLightColor() const {
        return state.lights.ambient;
    }

    /** Returns an approximation of the current fixed function lighting state */
    void getFixedFunctionLighting(const LightingRef& lighting) const;

    /**
     Equivalent to glEnable(GL_LIGHTING).

     On initialization, RenderDevice configures the color material as follows
     (it will be this way unless you change it):
     <PRE>
      float spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
     </PRE>
     */
    void enableLighting();
    void disableLighting();

    /** Begin a section of raw OpenGL calls.  All RenderDevice state is 
        synchronized with OpenGL and backed up.
        
        You can always make raw OpenGL calls with G3D, however in some cases
        RenderDevice makes lazy state changes and you'll be surprised by the outcome.
        This lets you switch to a pure OpenGL mode.  Do not make other RenderDevice
        calls while in beginOpenGL...endOpenGL.
        */
    void beginOpenGL();

    /** The state of the previous beginOpenGL is restored.*/
    void endOpenGL();

    /**
     Multiplies v by the current object to world and world to camera matrices,
     then by the projection matrix to obtain a 2D point and z-value.  
     
     The result is the 2D position to which the 3D point v corresponds.  You
     can use this to make results rendered with push2D() line up with those
     rendered with a 3D transformation.

     The value returned is relative to the current viewport.

     See G3D::glToScreen
     */
    Vector4 project(const Vector4& v) const;
    Vector4 project(const Vector3& v) const;


    /**
     Returns a new Milestone that can be passed to setMilestone and waitForMilestone.
     Milestones are garbage collected.
     */
    MilestoneRef createMilestone(const std::string& name);

    /**
     Inserts a milestone into the GPU processing list.  You can later call
     waitForMilestone to force the CPU to stall until the GPU has reached
     this milestone.
     <P>
     A milestone may be set multiple times, even without waiting for it in between.
     There is no requirement that a milestone be waited for once set.  Milestone
     setting transcends and is not affected by pushState()/popState() or beginFrame()/endFrame().
     */
    void setMilestone(const MilestoneRef& m);

    /**
     Stalls the CPU until the GPU has finished the milestone.  It is an error
     to wait for a milestone that was not set since it was last waited for.
     */
    void waitForMilestone(const MilestoneRef& m);

    /**
     Call within RenderDevice::pushState()...popState() so that you can
     restore the texture coordinate generation that will be configured
     on the unit as a result of this call.

     @param lightMVP The modelview projection matrix that
            was used to render the shadow map originally
            (you can get this from RenderDevice::getModelViewProjectionMatrix() 
            while rendering the shadow map).
     @param textureUnit The texture unit to use for shadowing. 0...RenderDevice::numTextureUnits()
            That unit cannot be used for texturing at the same time.
     */
    void configureShadowMap(
        uint32              textureUnit,
        const Matrix4&      lightMVP,
        const Texture::Ref& shadowMap);

    void configureShadowMap(
        uint32              unit,
        const ReferenceCountedPointer<class ShadowMap>& shadowMap);

    /**
     Call within RenderDevice::pushState()...popState() so that you can
     restore the texture coordinate generation.  Note that you can 
     obtain the reflection texture (aka environment map) from G3D::Sky
     or by loading it with G3D:Texture::fromFile.

     @param textureUnit The texture unit to use for shadowing. 0...RenderDevice::numTextureUnits()
            That unit cannot be used for texturing at the same time.
     */

    void configureReflectionMap(
        uint32              textureUnit,
        Texture::Ref        reflectionTexture);

    #ifdef G3D_WIN32
        HDC getWindowHDC() const;
    #endif

};


inline void RenderDevice::setAlphaWrite(bool a) {
    debugAssert(! inPrimitive);
    minStateChange();
    if (state.alphaWrite != a) {
        minGLStateChange();
        GLboolean c = state.colorWrite ? GL_TRUE : GL_FALSE;
        state.alphaWrite = a;
        glColorMask(c, c, c, state.alphaWrite ? GL_TRUE : GL_FALSE);
    }
}


inline void RenderDevice::setColorWrite(bool a) {
    debugAssert(! inPrimitive);
    minStateChange();
    if (state.colorWrite != a) {
        minGLStateChange();
        GLboolean c = a ? GL_TRUE : GL_FALSE;
        state.colorWrite = a;
        glColorMask(c, c, c, state.alphaWrite ? GL_TRUE : GL_FALSE);
    }
}


inline void RenderDevice::setDepthWrite(bool a) {
    debugAssert(! inPrimitive);
    minStateChange();
    if (state.depthWrite != a) {
        minGLStateChange();
        glDepthMask(a);
        state.depthWrite = a;
    }
}


inline void RenderDevice::setShadeMode(ShadeMode s) {
    minStateChange();
    if (s != state.shadeMode) {
        state.shadeMode = s;
        glShadeModel((s == SHADE_FLAT) ? GL_FLAT : GL_SMOOTH);
        minGLStateChange();
    }
}


inline void RenderDevice::setDepthRange(
    double              low,
    double              high) {

    majStateChange();

    if ((state.lowDepthRange != low) ||
        (state.highDepthRange != high)) {
        glDepthRange(low, high);
        state.lowDepthRange = low;
        state.highDepthRange = high;

        minGLStateChange();
    }
}


inline bool RenderDevice::supportsPixelProgram() const {
    return GLCaps::supports_GL_ARB_fragment_program();
}


inline bool RenderDevice::supportsVertexProgram() const {
    return GLCaps::supports_GL_ARB_vertex_program();
}


} // namespace

#endif
