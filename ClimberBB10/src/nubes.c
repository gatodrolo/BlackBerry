/*
 * nubes.c
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

#include "nubes.h"

void nubes_inicializar(){
	if (EXIT_SUCCESS != bbutil_load_texture("app/native/data/nube1.png", NULL, NULL, &tp_x, &tp_y, &txnube1)) {
		fprintf(stderr, "Fallo al cargar la textura de nube.\n");
	}
	nube1tcrd[0] = 0.0f;
	nube1tcrd[1] = 0.0f;
	nube1tcrd[2] = tp_x;
	nube1tcrd[3] = 0.0f;
	nube1tcrd[4] = 0.0f;
	nube1tcrd[5] = tp_y;
	nube1tcrd[6] = tp_x;
	nube1tcrd[7] = tp_y;

	if (EXIT_SUCCESS != bbutil_load_texture("app/native/data/nube2.png", NULL, NULL, &tp_x, &tp_y, &txnube2)) {
		fprintf(stderr, "Fallo al cargar la textura de nube.\n");
	}
	nube2tcrd[0] = 0.0f;
	nube2tcrd[1] = 0.0f;
	nube2tcrd[2] = tp_x;
	nube2tcrd[3] = 0.0f;
	nube2tcrd[4] = 0.0f;
	nube2tcrd[5] = tp_y;
	nube2tcrd[6] = tp_x;
	nube2tcrd[7] = tp_y;

	if (EXIT_SUCCESS != bbutil_load_texture("app/native/data/nube3.png", NULL, NULL, &tp_x, &tp_y, &txnube3)) {
		fprintf(stderr, "Fallo al cargar la textura de nube.\n");
	}
	nube3tcrd[0] = 0.0f;
	nube3tcrd[1] = 0.0f;
	nube3tcrd[2] = tp_x;
	nube3tcrd[3] = 0.0f;
	nube3tcrd[4] = 0.0f;
	nube3tcrd[5] = tp_y;
	nube3tcrd[6] = tp_x;
	nube3tcrd[7] = tp_y;
}

void nubes_posicionar(){
	nube1crd[0] = 1280.0f*(rand()/RAND_MAX);
	nube1crd[1] = 350.0f;
	nube1crd[2] = nube1crd[0]+700.0f;
	nube1crd[3] = nube1crd[1];
	nube1crd[4] = nube1crd[0];
	nube1crd[5] = nube1crd[1]+300.0f;
	nube1crd[6] = nube1crd[2];
	nube1crd[7] = nube1crd[5];

	nube2crd[0] = 512.0f*(rand()/RAND_MAX);
	nube2crd[1] = 300.0f;
	nube2crd[2] = nube2crd[0]+700.0f;
	nube2crd[3] = nube2crd[1];
	nube2crd[4] = nube2crd[0];
	nube2crd[5] = nube2crd[1]+300.0f;
	nube2crd[6] = nube2crd[2];
	nube2crd[7] = nube2crd[5];

	nube3crd[0] = 768.0f*(rand()/RAND_MAX);
	nube3crd[1] = 350.0f;
	nube3crd[2] = nube3crd[0]+700.0f;
	nube3crd[3] = nube3crd[1];
	nube3crd[4] = nube3crd[0];
	nube3crd[5] = nube3crd[1]+300.0f;
	nube3crd[6] = nube3crd[2];
	nube3crd[7] = nube3crd[5];
}

void nubes_actualizar_alto(int score){
	nube1crd[1]=nube1crd[1]-(0.09f*score);
	nube1crd[3]=nube1crd[3]-(0.09f*score);
	nube1crd[5]=nube1crd[5]-(0.09f*score);
	nube1crd[7]=nube1crd[7]-(0.09f*score);

	nube2crd[1]=nube2crd[1]-(0.09f*score);
	nube2crd[3]=nube2crd[3]-(0.09f*score);
	nube2crd[5]=nube2crd[5]-(0.09f*score);
	nube2crd[7]=nube2crd[7]-(0.09f*score);

	nube3crd[1]=nube3crd[1]-(0.09f*score);
	nube3crd[3]=nube3crd[3]-(0.09f*score);
	nube3crd[5]=nube3crd[5]-(0.09f*score);
	nube3crd[7]=nube3crd[7]-(0.09f*score);
}

void nubes_actualizar_ancho(){
	if(nube1crd[2]<-500){
		nube1crd[0] = 1280.0f;
		nube1crd[2] = nube1crd[0]+700.0f;
		nube1crd[4] = nube1crd[0];
		nube1crd[6] = nube1crd[2];
	}else{
		nube1crd[0]=nube1crd[0]-2.0f;
		nube1crd[2]=nube1crd[2]-2.0f;
		nube1crd[4]=nube1crd[4]-2.0f;
		nube1crd[6]=nube1crd[6]-2.0f;
	}

	if(nube2crd[2]<0){
		nube2crd[0] = 1280.0f;
		nube2crd[2] = nube2crd[0]+700.0f;
		nube2crd[4] = nube2crd[0];
		nube2crd[6] = nube2crd[2];
	}else{
		nube2crd[0]=nube2crd[0]-1.0f;
		nube2crd[2]=nube2crd[2]-1.0f;
		nube2crd[4]=nube2crd[4]-1.0f;
		nube2crd[6]=nube2crd[6]-1.0f;
	}

	if(nube3crd[2]<0){
		nube3crd[0] = 1280.0f;
		nube3crd[2] = nube3crd[0]+700.0f;
		nube3crd[4] = nube3crd[0];
		nube3crd[6] = nube3crd[2];
	}else{
		nube3crd[0]=nube3crd[0]-0.5f;
		nube3crd[2]=nube3crd[2]-0.5f;
		nube3crd[4]=nube3crd[4]-0.5f;
		nube3crd[6]=nube3crd[6]-0.5f;
	}
}



void nubes_render(){
	glPushMatrix();
	glVertexPointer(2, GL_FLOAT, 0, nube1crd);
	glTexCoordPointer(2, GL_FLOAT, 0, nube1tcrd);
	glBindTexture(GL_TEXTURE_2D, txnube1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	glPushMatrix();
	glVertexPointer(2, GL_FLOAT, 0, nube2crd);
	glTexCoordPointer(2, GL_FLOAT, 0, nube2tcrd);
	glBindTexture(GL_TEXTURE_2D, txnube2);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();

	glPushMatrix();
	glVertexPointer(2, GL_FLOAT, 0, nube3crd);
	glTexCoordPointer(2, GL_FLOAT, 0, nube3tcrd);
	glBindTexture(GL_TEXTURE_2D, txnube3);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();
}
