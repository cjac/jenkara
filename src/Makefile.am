## Process this file with automake to produce Makefile.in

bin_PROGRAMS = jenkara
jenkara_SOURCES = jenkara.c gl_handlers.c gl_window.c import.c menu_handlers.c \
                  toolbox_handlers.c gl_menu_handlers.c gldata.c \
                  targa.c trackball.c widgets.c
jenkara_LDADD   = -lglade-2.0 -lgnome-2 -lglib-1.3 mdl/libmdl.a md3/libmd3.a
jenkara_LDFLAGS = -export-dynamic `pkg-config --libs libgnomeui-2.0` `pkg-config --libs libglade-2.0` #for libglade
jenkara_CFLAGS = `pkg-config --cflags libgnomeui-2.0` `pkg-config --cflags libglade-2.0`

SUBDIRS = mdl md3
