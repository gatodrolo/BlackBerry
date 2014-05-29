/*
 * bola.h
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

#ifndef BOLA_H_
#define BOLA_H_

#define BOLA_VEL 26
#define BOLA_ALC 210

#include <GLES/gl.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "bbutil.h"

#include <AL/al.h>

#include "plataforma.h"

typedef struct{
	int tamano;
	float vel_y;
	float vel_x;
	int dir_x;
	int dir_y;

	//las primeras 8 posiciones corresponden a la bola standard,
	GLfloat crd[8];
	float tcrd[8];
	}t_bola;

static GLuint txbola;

float bola_desplazar_X(t_bola *b);

int loc_colision(t_bola *b, t_plataforma *p, GLfloat scr_x, GLfloat scr_y);

int bola_actualizar(t_bola *b, t_plataforma *p, GLfloat scr_x, GLfloat scr_y);

void bola_nueva_oportunidad(t_bola *bola);

void bola_inicializar(t_bola *bola);
void bola_posicionar(t_bola *b, t_plataforma *p);

void bola_render(t_bola *bola);


#endif /* BOLA_H_ */
