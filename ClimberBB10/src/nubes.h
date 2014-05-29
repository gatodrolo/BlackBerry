/*
 * nubes.h
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

#ifndef NUBES_H_
#define NUBES_H_

#include "bbutil.h"
#include <GLES/gl.h>
#include <stdlib.h>


static GLuint txnube1,txnube2,txnube3;
static float nube1crd[8], nube1tcrd[8],nube2crd[8], nube2tcrd[8],nube3crd[8], nube3tcrd[8];
float tp_x, tp_y;

void nubes_inicializar();
void nubes_posicionar();
void nubes_actualizar_alto(int score);
void nubes_actualizar_ancho();
void nubes_render();




#endif /* NUBES_H_ */
