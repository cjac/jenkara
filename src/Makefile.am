## Process this file with automake to produce Makefile.in

bin_PROGRAMS = jenkara
jenkara_SOURCES = jenkara.c \
                  gl_handlers.c \
                  gl_window.c import.c \
                  menu_handlers.c \
                  toolbox_handlers.c \
                  widgets.c \
                  gl_menu_handlers.c \
                  gldata.c \
                  targa.c \
                  trackball.c

jenkara_LDADD   = mdl/libmdl.a md3/libmd3.a $(GNOMEUI_LIBS) $(GLADE_LIBS)
#jenkara_LDFLAGS = -export-dynamic
jenkara_CFLAGS = -include /usr/include/errno.h $(GNOMEUI_CFLAGS) $(GLADE_CFLAGS)

SUBDIRS = mdl md3
