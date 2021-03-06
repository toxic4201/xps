/**
 @file GLG3D/GuiDropDownList.h

 @created 2007-06-15
 @edited  2009-04-20

 G3D Library http://g3d-cpp.sf.net
 Copyright 2001-2009, Morgan McGuire morgan@cs.williams.edu
 All rights reserved.
*/
#ifndef G3D_GuiDropDownList_h
#define G3D_GuiDropDownList_h

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Pointer.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/Array.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiWindow.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiControl.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/GuiMenu.h"

namespace G3D {

class GuiPane;

/** List box for viewing strings.  Fires
    a G3D::GuiEvent of type G3D::GEventType::GUI_ACTION on the containing window when
    the user selects a new value, GEventType::GUI_CANCEL when the user opens the dropdown and then clicks off or presses ESC.
*/
class GuiDropDownList : public GuiControl {
    friend class GuiWindow;
    friend class GuiPane;

protected:

    /** Pop-up list menu; call menu() to create this. */
    GuiMenuRef                      m_menu;
    
    GuiMenuRef menu();

    /** The index of the currently selected item. */
    Pointer<int>                    m_indexValue;

    /** m_indexValue points to this if no external pointer was provided. */
    int                             m_myInt;

    Array<GuiText>                  m_listValue;

    /** True when the menu is open */
    bool                            m_selecting;

    GuiDropDownList
       (GuiContainer*               parent, 
        const GuiText&              caption, 
        const Pointer<int>&         indexValue, 
        const Array<GuiText>&       listValue);

    /** Called by GuiPane */
    virtual void render(RenderDevice* rd, const GuiThemeRef& skin) const;

    virtual bool onEvent(const GEvent& event);
   
    /** Makes the menu appear */
    void showMenu();

public:

    void setList(const Array<GuiText>& c);

    void setList(const Array<std::string>& c);

    /** Remove all values from the list */
    void clear();

    void append(const GuiText& c);

    inline const GuiText& get(int i) const {
        return m_listValue[i];
    }

    inline void set(int i, const GuiText& v) {
        m_listValue[i] = v;
        m_menu = NULL;
    }

    virtual void setRect(const Rect2D&);

    /** Returns the currently selected value */
    const GuiText& selectedValue() const;
    
    /** The index of the currently selected value */
    inline int selectedIndex() const {
        return iClamp(*m_indexValue, 0, m_listValue.size() - 1);
    }

    inline void setSelectedIndex(int i) {
        *m_indexValue = i;
    }

    /** Selects the first value whose text() is equal to \a s. If not
        found, leaves the index unchanged. */
    void setSelectedValue(const std::string& s);

    inline void resize(int n) {
        m_listValue.resize(n);
        *m_indexValue = selectedIndex();
        m_menu = NULL;
    }

};

} // G3D

#endif
