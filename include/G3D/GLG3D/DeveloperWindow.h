/**
  @file DeveloperWindow.h

  @maintainer Morgan McGuire, morgan@cs.williams.edu

  @created 2007-06-10
  @edited  2009-03-30
*/
#ifndef G3D_DeveloperWindow_h
#define G3D_DeveloperWindow_h

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/platform.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Pointer.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/Widget.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/UprightSplineManipulator.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/FirstPersonManipulator.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiWindow.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GConsole.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiTheme.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/CameraControlWindow.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/VideoRecordDialog.h"

namespace G3D {
/**
 Developer controls instantiated by GApp for debugging.
 @sa G3D::GApp, G3D::CameraControlWindow, G3D::GConsole
 */
class DeveloperWindow : public GuiWindow {
protected:

    DeveloperWindow
    (class GApp*                        app,
     const FirstPersonManipulatorRef&   manualManipulator,
     const UprightSplineManipulatorRef& trackManipulator,
     const Pointer<Manipulator::Ref>&   cameraManipulator,
     const GuiThemeRef&                 theme,
     GConsoleRef                        console,
     const Pointer<bool>&               debugVisible,
     bool*                              showStats,
     bool*                              showText);

public:

    typedef ReferenceCountedPointer<class DeveloperWindow> Ref;

    VideoRecordDialog::Ref        videoRecordDialog;
    CameraControlWindow::Ref      cameraControlWindow;
    GConsoleRef                   consoleWindow;

    static Ref create
    (GApp*                              app,
     const FirstPersonManipulatorRef&   manualManipulator,
     const UprightSplineManipulatorRef& trackManipulator,
     const Pointer<Manipulator::Ref>&   cameraManipulator,
     const GuiThemeRef&                 theme,
     GConsoleRef                        console,
     const Pointer<bool>&               debugVisible,
     bool*                              showStats,
     bool*                              showText);
    
    virtual void setManager(WidgetManager* manager);

    virtual bool onEvent(const GEvent& event);
};

}

#endif
