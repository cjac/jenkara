# Process this file with autoconf to produce a configure script.
AC_INIT([jenkara], [0.1], [cjcollier@pobox.com])
AM_INIT_AUTOMAKE([jenkara], [0.1], [cjcollier@pobox.com])
GNOME_COMMON_INIT
AC_CONFIG_SRCDIR([ui/jenkara.glade])
AM_CONFIG_HEADER(config.h)

CFLAGS = `libglade-config --cflags gnome`
LDFLAGS = `libglade-config --libs gnome`

AM_PATH_GLIB_2_0(1.3.15, ,AC_MSG_ERROR([Need glib 1.3.15 or greater]))
AM_PATH_GTK_2_0(1.3.15, ,AC_MSG_ERROR([Need GTK+ 1.3.15 or greater]))
AM_PATH_GTKGL(,AC_MSG_ERROR([Need GtkGLArea 5.0 or greater]))
AM_PATH_LIBGLADE(,AC_MSG_ERROR([Need Libglade 0.17 or greater]))

PKG_CHECK_MODULES(jenkara, libglade-2.0 >= 0.17 gtk+-2.0 >= 1.3.15 libgnomeui-2.0 >= 1.3.15 )
AC_SUBST(jenkara_CFLAGS)
AC_SUBST(jenkara_LIBS)

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
