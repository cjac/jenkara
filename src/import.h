/*
	Copyright (C) 2001 C.J. Collier (cjcollier@pobox.com)

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:
	
		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA

*/

#ifndef _IMPORT_H_
#define _IMPORT_H_

#define PALETTE_SIZE 768

#include "md3/md3.h"
#include "mdl/mdl.h"
#include "gldata.h"
#include "targa.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <GL/glut.h>
#include <libgen.h>

void
import_mdl(char *filename);

#endif /* _IMPORT_H_ */
