/**
  @file VideoRecordDialog.h

  @maintainer Morgan McGuire, morgan@cs.williams.edu

  @created 2008-10-14
  @edited  2008-10-14
*/
#ifndef G3D_VideoRecordDialog_h
#define G3D_VideoRecordDialog_h

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/platform.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/Widget.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiWindow.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiCheckBox.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiDropDownList.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiButton.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiNumberBox.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/VideoOutput.h"

namespace G3D {

// forward declare heavily dependent classes
class RenderDevice;


/**
   @brief A dialog that allows the user to launch recording of the
   on-screen image to a movie.

   The playback rate is the frames-per-second value to be stored in
   the movie file.  The record rate 1/G3D::GApp::simTimeStep.

   Set enabled to false to prevent hot-key handling.
 */
class VideoRecordDialog : public GuiWindow {
public:

    typedef ReferenceCountedPointer<class VideoRecordDialog> Ref;

protected:
    GApp*                        m_app;

    /** For drawing messages on the screen */
    GFontRef                     m_font;

    Array<VideoOutput::Settings> m_settingsTemplate;

    /** Parallel array to m_settingsTemplate of the descriptions for
        use with a drop-down list. */
    Array<std::string>           m_formatList;

    Array<std::string>           m_ssFormatList;

    /** Index into m_settingsTemplate and m_formatList */
    int                          m_templateIndex;

    /** Index into m_ssFormatList */
    int                          m_ssFormatIndex;

    float                        m_playbackFPS;
    float                        m_recordFPS;

    bool                         m_halfSize;
    bool                         m_enableMotionBlur;
    int                          m_motionBlurFrames;

    /** Recording modifies the GApp::simTimeStep; this is the old value */
    float                        m_oldSimTimeStep;
    float                        m_oldDesiredFrameRate;

    /** Tells the invisible window to record a screenshot when the next frame is rendered.*/
    bool                         m_screenshotPending;

    /** For downsampling */
    Texture::Ref                 m_downsampleSrc;
    Texture::Ref                 m_downsampleDst;
    Framebuffer::Ref             m_downsampleFBO;
    
    /** Motion blur frames */
    GuiNumberBox<int>*           m_framesBox;

    /**
       When false, the screen is captured at the beginning of 
       Posed2DModel rendering from the back buffer, which may 
       slow down rendering.
       
       When true, the screen is captured from the the previous 
       frame, which will not introduce latency into rendering.
    */
    bool                         m_captureGUI;

    /** Draw a software cursor on the frame after capture, since the
     hardware cursor will not be visible.*/
    bool                         m_showCursor;

    GuiButton*                   m_recordButton;

    /** Key to start/stop recording even when the GUI is not
        visible.
      */
    GKey                         m_hotKey;
    GKeyMod                      m_hotKeyMod;

    /** Hotkey + mod as a human readable string */
    std::string                  m_hotKeyString;
    
    // Screenshot keys
    GKey                         m_ssHotKey;
    GKeyMod                      m_ssHotKeyMod;
    std::string                  m_ssHotKeyString;

    /** 
        Inserts itself into the bottom of the Posed2D model drawing
        list to call recordFrame so that the rest of the GUI is not
        yet visible.
     */
    class Recorder : public Surface2D {
    public:
        VideoRecordDialog*       dialog;

        virtual Rect2D bounds () const {
            return Rect2D::xywh(0,0,1,1);
        }
            
        virtual float depth () const {
            // Lowest possible depth
            return inf();
        }

        virtual void render (RenderDevice *rd) const;
    };
    typedef ReferenceCountedPointer<Recorder> RecorderRef;

    RecorderRef                  m_recorder;

    /** Non-NULL while recording */
    VideoOutput::Ref             m_video;

    VideoRecordDialog(const GuiThemeRef& theme, GApp* app);

    /** Called from constructor */
    void makeGUI();

    /** Generate the next filename to write to */
    static std::string nextFilenameBase();

    /** Actually write a video frame */
    void recordFrame(RenderDevice* rd);

    /** Actually take a screen shot */
    void screenshot(RenderDevice* rd);
    
    /** Calls recordFrame() when video recording is in progress and 
       screenshot() when a shot is pending. */
    void maybeRecord(RenderDevice* rd);

public:

    /**
       @param app If not NULL, the VideoRecordDialog will set the app's
       simTimeStep.
     */
    static Ref create(const GuiThemeRef& theme, GApp* app = NULL);
    static Ref create(GApp* app);

    /** Automatically invoked when the record button or hotkey is pressed. 
        Can be called explicitly to force recording.*/
    void startRecording();
    void stopRecording();


    /** Automatically invoked when the hotkey is pressed. 
        Can be called explicitly to force a screenshot. 
        The actual screenshot will be captured on rendering 
        of the next frame.*/
    void takeScreenshot();

    virtual void onPose (Array<SurfaceRef> &posedArray, Array< Surface2DRef > &posed2DArray);

    virtual void onAI();
    virtual bool onEvent(const GEvent& event);
};

}

#endif
