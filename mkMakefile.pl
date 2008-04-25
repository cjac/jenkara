#!/usr/bin/perl -w


# 	Copyright (C) 2001 C.J. Collier (cjcollier@sinclair.net)
#
# 	This program is free software; you can redistribute it and/or
# 	modify it under the terms of the GNU General Public License
# 	as published by the Free Software Foundation; either version 2
# 	of the License, or (at your option) any later version.
#
# 	This program is distributed in the hope that it will be useful,
# 	but WITHOUT ANY WARRANTY; without even the implied warranty of
# 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
#
# 	See the GNU General Public License for more details.
#
# 	You should have received a copy of the GNU General Public License
# 	along with this program; if not, write to:
#	
# 		Free Software Foundation, Inc.
# 		59 Temple Place - Suite 330
# 		Boston, MA  02111-1307, USA



use strict;

my $target = "jenkara";

my @objs   = qw(
		gldata.o gl_window.o widgets.o trackball.o
		gl_handlers.o toolbox_handlers.o menu_handlers.o
		gl_menu_handlers.o import.o targa.o
	       );

unshift(@objs, "$target.o");

my @cfiles = @objs;
map{ s/\.o$/.c/ }@cfiles;

my @hfiles = @objs;
map{ s/\.o$/.h/ }@hfiles;

my $cflags ="`libglade-config --cflags gnome` -g -Wall";
my $static_files ="/usr/lib/libgtkgl.a ";
my $cc     = "gcc";
#my $libs   = "`libglade-config --libs gnome` /usr/lib/libgtkgl.a -lGL -lglut";
my $libs   = "`libglade-config --libs gnome` -lgtkgl -lGL -lglut";

open MAKEFILE, ">Makefile";

print MAKEFILE
  ("OBJS   = ", join(" ", @objs,"mdl/load_mdl.o", "md3/load_md3.o"), "\n",
   "CFILES = ", join(" ", @cfiles), "\n",
   "HFILES = ", join(" ", @hfiles), "\n",
   "\n",
   "CC     = $cc\n",
   "LIBS   = $libs\n",
   "CFLAGS = $cflags\n",
   "\n",
   "$target: \$(OBJS)\n",
   "\t\$(CC) -o $target \$(OBJS) \$(LIBS) \$(CFLAGS)\n",
   "\n",
  );

for(my $i = 0; defined($objs[$i]); $i++){
  print MAKEFILE ("$objs[$i]: $cfiles[$i] $hfiles[$i]\n",
		  "\t\$(CC) -c $cfiles[$i] \$(CFLAGS)\n"
		 );
}

print MAKEFILE
  (
   "mdl/load_mdl.o: mdl/load_mdl.h mdl/load_mdl.c\n",
   "\tcd mdl; make;\n",
   "\n",
   "md3/load_md3.o: md3/load_md3.h md3/load_md3.c\n",
   "\tcd md3; make;\n",
   "\n",
  );


print MAKEFILE
  ("\n",
   "clean:\n",
   "\tfor i in ", join(" ", @objs, $target ), '; do if [ -f $$i ]; then rm $$i; fi; done', "\n\n",
   );

close MAKEFILE;
