/*
 * fondo.c
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

#include "fondo.h"

void fondo_inicializar(){
	float tp_x, tp_y;

	if (EXIT_SUCCESS != bbutil_load_texture("app/native/data/fondo.png", NULL, NULL, &tp_x, &tp_y, &txfondo)) {
		fprintf(stderr, "Fallo al cargar la textura del fondo.\n");
	}
	fondoT_crd[0] = 0.0f;
	fondoT_crd[1] = 0.0f;
	fondoT_crd[2] = tp_x;
	fondoT_crd[3] = 0.0f;
	fondoT_crd[4] = 0.0f;
	fondoT_crd[5] = tp_y;
	fondoT_crd[6] = tp_x;
	fondoT_crd[7] = tp_y;
}

void fondo_posicionar(){
	//inicia fondo
	fondo_crd[0] = 0.0f;
	fondo_crd[1] = 0.0f;
	fondo_crd[2] = 1280.0f;
	fondo_crd[3] = 0.0f;
	fondo_crd[4] = 0.0f;
	fondo_crd[5] = 3000.0f;
	fondo_crd[6] = 1280.0f;
	fondo_crd[7] = 3000.0f;
}

void fondo_actualizar(int score){
	//el fondo se debe de desplazar desde 0 a 1800 en y1=0 y de 600 a 2400 en y2=600
	//para puntuacion igual a 0 -> y1=0 y2=600
	//para puntuacion igual a 20000 -> y1=1800 y2=2400
	fondo_crd[1]=fondo_crd[1]-(0.09f*score);
	fondo_crd[3]=fondo_crd[3]-(0.09f*score);
	fondo_crd[5]=fondo_crd[5]-(0.09f*score);
	fondo_crd[7]=fondo_crd[7]-(0.09f*score);
}

void fondo_render(){
	glPushMatrix();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glVertexPointer(2, GL_FLOAT, 0, fondo_crd);
	glTexCoordPointer(2, GL_FLOAT, 0, fondoT_crd);
	glBindTexture(GL_TEXTURE_2D, txfondo);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();
}

