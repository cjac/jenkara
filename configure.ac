# Process this file with autoconf to produce a configure script.
AC_INIT([jenkara], [0.1], [cjcollier@pobox.com])
AM_INIT_AUTOMAKE([jenkara], [0.1], [cjcollier@pobox.com])
GNOME_COMMON_INIT
AC_CONFIG_SRCDIR([ui/jenkara.glade])
AM_CONFIG_HEADER(config.h)

GTK_REQUIRED_VERSION=2.0.0
GLADE_REQUIRED_VERSION=2.0.0
GNOMEUI_REQUIRED_VERSION=2.0.0
GTKGL_REQUIRED_VERSION=2.0.0

PKG_CHECK_MODULES(GLIB, gobject-2.0 >= $GTK_REQUIRED_VERSION)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= $GTK_REQUIRED_VERSION)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

PKG_CHECK_MODULES(GLADE, libglade-2.0 >= $GLADE_REQUIRED_VERSION)
AC_SUBST(GLADE_CFLAGS)
AC_SUBST(GLADE_LIBS)

PKG_CHECK_MODULES(GNOMEUI, libgnomeui-2.0 >= $GNOMEUI_REQUIRED_VERSION)
AC_SUBST(GNOMEUI_CFLAGS)
AC_SUBST(GNOMEUI_LIBS)

PKG_CHECK_MODULES(GTKGL, gtkgl-2.0 >= $GTKGL_REQUIRED_VERSION)
AC_SUBST(GLADE_CFLAGS)
AC_SUBST(GLADE_LIBS)

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB([GL], [glBegin])
AC_CHECK_LIB([glut], [gluNewQuadric])
AC_CHECK_LIB([gtkgl], [gtk_gl_area_get_type])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h GL/glut.h GL/gl.h gtk/gtk.h gtkgl/gtkglarea.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset sqrt strdup strerror strrchr])

AC_CONFIG_FILES([Makefile
                 ui/Makefile
                 src/Makefile
                 src/md3/Makefile
                 src/mdl/Makefile])
AC_OUTPUT
