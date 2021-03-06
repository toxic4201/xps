/** 
  @file WrapMode.h
 
  @maintainer Morgan McGuire, graphics3d.com
 
  @created 2007-04-17
  @edited  2007-04-17

  Copyright 2000-2007, Morgan McGuire.
  All rights reserved.
 */

#ifndef G3D_WRAPMODE_H
#define G3D_WRAPMODE_H

#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/platform.h"
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/enumclass.h"

#ifdef IGNORE
#   undef IGNORE
#endif
#ifdef ZERO
#   undef ZERO
#endif
#ifdef ERROR
#   undef ERROR
#endif

namespace G3D {

/** 
  Describes the behavior of G3D::Texture, G3D::Map2D, G3D::Image3, etc. when accessing an out-of-bounds
  pixel.  Not all classes support all modes.

  Refer to these as scoped enums, e.g., <code>WrapMode m = WrapMode::CLAMP;</code>.

  WrapMode::IGNORE silently discards attempts to write to out 
     of bounds locations and returns an undefined value for reading
     from out of bounds locations.

  WrapMode::ERROR generates an error when the 
     pixel indices are out of bounds

  WrapMode::CLAMP makes out of bounds pixels equal to the last in-range pixel along that dimension.

  WrapMode::TILE computes out of bounds pixels modulo the dimension

  WrapMode::ZERO treats out of bounds values as the zero value, which varies in definition
  according to the class used.  For example, with a G3D::Texture, ZERO = Color4(0,0,0,0).

  Uses the "Intelligent Enum" design pattern 
  http://www.codeguru.com/cpp/cpp/cpp_mfc/article.php/c4001/
 */ 
class WrapMode {
public:
    /** Don't use this enum; use WrapMode instances instead. */
    enum Value {
        CLAMP, 
        TILE, 
        ZERO,
        IGNORE, 
        ERROR
    };

private:
    
    Value value;

public:

    G3D_DECLARE_ENUM_CLASS_METHODS(WrapMode);

    inline const char* toString() const {
        static const char* s[] = {"CLAMP", "TILE", "ZERO", "IGNORE", "ERROR"};
        return s[value];
    }

    inline explicit WrapMode(const std::string& x) : value(ERROR) {
        static const char* s[] = {"CLAMP", "TILE", "ZERO", "IGNORE", "ERROR"};
        for (int i = 0; i < 5; ++i) {
            if (x == s[i]) {
                value = (Value)i;
            }
        }
    }
};

} // namespace G3D

G3D_DECLARE_ENUM_CLASS_HASHCODE(G3D::WrapMode);

#endif
