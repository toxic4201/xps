/**
  @file CameraControlWindow.h

  @maintainer Morgan McGuire, morgan@cs.williams.edu

  @created 2002-07-28
  @edited  2008-07-14
*/
#ifndef G3D_CAMERACONTROLWINDOW_H
#define G3D_CAMERACONTROLWINDOW_H

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/platform.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/Widget.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/UprightSplineManipulator.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiWindow.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiLabel.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiCheckBox.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiDropDownList.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiTextBox.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiButton.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/FirstPersonManipulator.h"

namespace G3D {

/**
 Gui used by DeveloperWindow default for recording camera position and making splines.
 @sa G3D::DeveloperWindow, G3D::GApp
 */
//
// If you are looking for an example of how to create a straightforward
// GUI in G3D do not look at this class!  CameraControlWindow uses a number
// of unusual tricks to provide a fancy compact interface that you do not
// need in a normal program.  The GUI code in this class is more complex
// than what you would have to write for a less dynamic UI.
class CameraControlWindow : public GuiWindow {
public:

    typedef ReferenceCountedPointer<class CameraControlWindow> Ref;

protected:

    static const Vector2        smallSize;
    static const Vector2        bigSize;

    /** Returns a prettyprinted position*/
    std::string cameraLocation() const;

    /** Returns a CFrame constructor */
    std::string cameraLocationCode() const;

    /** Parses either prettyprinted or CFrame version */
    void setCameraLocation(const std::string& s);

    /** Name of the file in which current bookmarks are stored. */
    std::string                 m_bookmarkFilename;

    Array<std::string>          m_bookmarkName;

    /** Parallel to m_bookmarkName */
    Array<CoordinateFrame>      m_bookmarkPosition;

    /** Array of all .trk files in the current directory */
    Array<std::string>          trackFileArray;

    /** Index into trackFileArray */
    int                         trackFileIndex;

    GuiDropDownList*            trackList;

    enum {NO_BOOKMARK = -1};

    /** Selected bookmark.  When not NO_BOOKMARK, move to this location */
    int                         m_bookmarkSelection;

    GuiMenuRef                  m_menu;

    /** Allows the user to override the current camera position */
    GuiTextBox*                 cameraLocationTextBox;

    GuiRadioButton*             playButton;
    GuiRadioButton*             stopButton;
    GuiRadioButton*             recordButton;

    /** The manipulator from which the camera is copying its frame */
    Pointer<Manipulator::Ref>   cameraManipulator;

    FirstPersonManipulatorRef   manualManipulator;
    UprightSplineManipulatorRef trackManipulator;

    GuiCheckBox*                visibleCheckBox;
    GuiCheckBox*                cyclicCheckBox;

    /** Button to expand and contract additional manual controls. */
    GuiButton*                  drawerButton;

    /** The button must be in its own pane so that it can float over
        the expanded pane. */
    GuiPane*                    drawerButtonPane;
    GuiText                     drawerExpandCaption;
    GuiText                     drawerCollapseCaption;

    GuiButton*                  saveButton;

    GuiLabel*                   helpLabel;

    GuiText                     manualHelpCaption;
    GuiText                     autoHelpCaption;
    GuiText                     recordHelpCaption;
    GuiText                     playHelpCaption;

    GuiButton*                  m_showBookmarksButton;

    /** If true, the window is big enough to show all controls */
    bool                        m_expanded;

    /** True when the user has chosen to override program control of
        the camera. */
    bool                        manualOperation;

    CameraControlWindow(
        const FirstPersonManipulatorRef&    manualManipulator, 
        const UprightSplineManipulatorRef&  trackManipulator, 
        const Pointer<Manipulator::Ref>&    cameraManipulator,
        const GuiThemeRef&                   skin);

    /** Sets the controller for the cameraManipulator. */
    //void setSource(Source s);

    /** Control source that the Gui thinks should be in use */
    //Source desiredSource() const;

    void sync();

    void loadSpline(const std::string& filename);

    /** Updates the trackFileArray from the list of track files */
    void updateTrackFiles();

    void copyToClipboard();

    void saveBookmarks();

    void showBookmarkList();

    void onBookmarkButton();

public:

    /** Replace current bookmarks with those from this file. New bookmarks will
        be saved to this file when they are added. The default bookmark file is 
        "g3d-bookmarks.txt". */
    void setBookmarkFile(const std::string& filename);

    /** Add/replace bookmark and immediately update the file.*/
    void setBookmark(const std::string& name, const CoordinateFrame& frame);

    void removeBookmark(const std::string& name);

    /** Get a bookmark, or use the defaultValue if it is not present. */
    CoordinateFrame bookmark(const std::string& name, const CoordinateFrame& defaultValue = CoordinateFrame()) const;

    /** True if this bookmark is present */
    bool containsBookmark(const std::string& name) const {
        return m_bookmarkName.contains(name);
    }

    /** Name of the file storing the bookmarks.  */
    const std::string& bookmarkFile() const{
        return m_bookmarkFilename;
    }

    const Array<std::string>& bookmarkNameArray() const {
        return m_bookmarkName;
    }

    virtual void setManager(WidgetManager* manager);


    /**
     @param cameraManipulator The manipulator that should drive the camera.  This will be assigned to
     as the program runs.
     */
    static Ref create(
        const FirstPersonManipulatorRef&   manualManipulator,
        const UprightSplineManipulatorRef& trackManipulator,
        const Pointer<Manipulator::Ref>&   cameraManipulator,
        const GuiThemeRef&                  skin);

    virtual bool onEvent(const GEvent& event);
    virtual void onUserInput(UserInput*);
    virtual void setRect(const Rect2D& r);
};

}

#endif
