/*
 * fondo.h
 *
 *  Created on: 22/02/2012
 *      Author: Javier López Romero
 * 		Copyright (c) 2012-2013 Javier López Romero
 *
 *	This file is part of Climber.
 *
 *  Climber is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License v2 as published by
 *  the Free Software Foundation.
 *
 *  Climber is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Climber.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FONDO_H_
#define FONDO_H_

#include <GLES/gl.h>
#include <stdlib.h>
#include "bbutil.h"

static GLuint txfondo;
static float fondo_crd[8],fondoT_crd[8];

void fondo_inicializar();

void fondo_posicionar();

void fondo_actualizar(int score);

void fondo_render();

#endif /* FONDO_H_ */
