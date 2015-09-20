#include "fltk4lua.hxx"
#include "f4l_valuator.hxx"
#include "f4l_widget.hxx"
#include <FL/Fl_Dial.H>
#include <FL/Fl_Fill_Dial.H>
#include <FL/Fl_Line_Dial.H>
#include <cstring>
#include <climits>

namespace {

  inline Fl_Dial* check_dial( lua_State* L, int idx ) {
    void* p = moon_checkobject( L, idx, F4L_DIAL_NAME );
    return static_cast< Fl_Dial* >( p );
  }

  int dial_index_( lua_State* L, Fl_Dial* d,
                   char const* key, size_t n ) {
    switch( n ) {
      case 6:
        if( F4L_MEMCMP( key, "angle1", 6 ) == 0 ) {
          lua_pushinteger( L, d->angle1() );
          return 1;
        } else if( F4L_MEMCMP( key, "angle2", 6 ) == 0 ) {
          lua_pushinteger( L, d->angle2() );
          return 1;
        }
        break;
    }
    return 0;
  }

  int dial_newindex_( lua_State* L, Fl_Dial* d,
                      char const* key, size_t n ) {
    switch( n ) {
      case 6:
        if( F4L_MEMCMP( key, "angle1", 6 ) == 0 ) {
          d->angle1( moon_checkint( L, 3, SHRT_MIN, SHRT_MAX ) );
          return 1;
        } else if( F4L_MEMCMP( key, "angle2", 6 ) == 0 ) {
          d->angle2( moon_checkint( L, 3, SHRT_MIN, SHRT_MAX ) );
          return 1;
        }
        break;
    }
    return 0;
  }

  int dial_index( lua_State* L ) {
    Fl_Dial* d = check_dial( L, 1 );
    size_t n = 0;
    char const* key = luaL_checklstring( L, 2, &n );
    F4L_TRY {
      if( !dial_index_( L, d, key, n ) &&
          !f4l_valuator_index_( L, d, key, n ) &&
          !f4l_widget_index_( L, d, key, n ) )
        lua_pushnil( L );
    } F4L_CATCH( L );
    return 1;
  }

  int dial_newindex( lua_State* L ) {
    Fl_Dial* d = check_dial( L, 1 );
    size_t n = 0;
    char const* key = luaL_checklstring( L, 2, &n );
    F4L_TRY {
      (void)(dial_newindex_( L, d, key, n ) ||
             f4l_valuator_newindex_( L, d, key, n ) ||
             f4l_widget_newindex_( L, d, key, n ));
    } F4L_CATCH( L );
    return 0;
  }

  int new_dial( lua_State* L ) {
    F4L_TRY {
      f4l_new_widget< Fl_Dial>( L, F4L_DIAL_NAME );
    } F4L_CATCH( L );
    return 1;
  }

  int new_fill_dial( lua_State* L ) {
    F4L_TRY {
      f4l_new_widget< Fl_Fill_Dial>( L, F4L_FILL_DIAL_NAME );
    } F4L_CATCH( L );
    return 1;
  }

  int new_line_dial( lua_State* L ) {
    F4L_TRY {
      f4l_new_widget< Fl_Line_Dial>( L, F4L_LINE_DIAL_NAME );
    } F4L_CATCH( L );
    return 1;
  }

  int dial_angles( lua_State* L ) {
    Fl_Dial* d = check_dial( L, 1 );
    short a = moon_checkint( L, 2, SHRT_MIN, SHRT_MAX );
    short b = moon_checkint( L, 3, SHRT_MIN, SHRT_MAX );
    F4L_TRY {
      d->angles( a, b );
    } F4L_CATCH( L );
    return 0;
  }

} // anonymous namespace


MOON_LOCAL void f4l_dial_setup( lua_State* L ) {
  luaL_Reg const functions[] = {
    { "Dial", new_dial },
    { "Fill_Dial", new_fill_dial },
    { "Line_Dial", new_line_dial },
    { NULL, NULL }
  };
  luaL_Reg const methods[] = {
    F4L_WIDGET_METHODS,
    F4L_VALUATOR_METHODS,
    { "angles", dial_angles },
    { "__index", dial_index },
    { "__newindex", dial_newindex },
    { NULL, NULL }
  };

  moon_defobject( L, F4L_DIAL_NAME, 0, methods, 0 );
  moon_defcast( L, F4L_DIAL_NAME, F4L_VALUATOR_NAME,
                f4l_cast< Fl_Dial, Fl_Valuator > );
  moon_defcast( L, F4L_DIAL_NAME, F4L_WIDGET_NAME,
                f4l_cast< Fl_Dial, Fl_Widget > );

  moon_defobject( L, F4L_FILL_DIAL_NAME, 0, methods, 0 );
  moon_defcast( L, F4L_FILL_DIAL_NAME, F4L_DIAL_NAME,
                f4l_cast< Fl_Fill_Dial, Fl_Dial > );
  moon_defcast( L, F4L_FILL_DIAL_NAME, F4L_VALUATOR_NAME,
                f4l_cast< Fl_Fill_Dial, Fl_Valuator > );
  moon_defcast( L, F4L_FILL_DIAL_NAME, F4L_WIDGET_NAME,
                f4l_cast< Fl_Fill_Dial, Fl_Widget > );

  moon_defobject( L, F4L_LINE_DIAL_NAME, 0, methods, 0 );
  moon_defcast( L, F4L_LINE_DIAL_NAME, F4L_DIAL_NAME,
                f4l_cast< Fl_Line_Dial, Fl_Dial > );
  moon_defcast( L, F4L_LINE_DIAL_NAME, F4L_VALUATOR_NAME,
                f4l_cast< Fl_Line_Dial, Fl_Valuator > );
  moon_defcast( L, F4L_LINE_DIAL_NAME, F4L_WIDGET_NAME,
                f4l_cast< Fl_Line_Dial, Fl_Widget > );

  luaL_setfuncs( L, functions, 0 );
}

