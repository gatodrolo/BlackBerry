/*
 * plataforma.h
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

#ifndef PLATAFORMA_H_
#define PLATAFORMA_H_

#define P_TAM_X 80
#define P_TAM_Y 15
#define TECHO 20000

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <GLES/gl.h>

#include "bbutil.h"


typedef struct r_plataforma{
	int tam_x;
	int tam_y;
	struct r_plataforma *sig;
	GLfloat crd[8];
}t_plataforma;

static float pcrd[8];
static GLuint txptfm;

void plataformas_inicializar(t_plataforma **plat, GLfloat scr_x);

void plataformas_texturizar();

void plataformas_posicionar(t_plataforma *p, int score, GLfloat scr_x, GLfloat scr_y);

void plataformas_destruir(t_plataforma **p);

void plataformas_desplazar(t_plataforma **p, GLfloat destino);

void plataformas_render(t_plataforma *p);



#endif /* PLATAFORMA_H_ */
