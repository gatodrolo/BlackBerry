/*
 * bola.c
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

#include "bola.h"

float bola_desplazar_X(t_bola *b){
	return (b->vel_x)*(b->dir_x);
}

int loc_colision(t_bola *b, t_plataforma *p, GLfloat scr_x, GLfloat scr_y){
	t_plataforma *aux=0;
	int colision=0;

	aux=p;
	while(aux!=NULL && (aux->crd[5] <= b->crd[1]) && (colision==0)){
		if(((b->crd[1] - aux->crd[5]) <= 1) &&
		(b->crd[2] > aux->crd[4]) &&
		(b->crd[0] < aux->crd[6])){
			colision = aux->crd[5];
		}else{
			aux=aux->sig;
		}

	}
	if(b->crd[5]<0) colision=-1;

	return colision;
}

int bola_actualizar(t_bola *b, t_plataforma *p, GLfloat scr_x, GLfloat scr_y){
	int desp,i;
	int colision=0;

	desp = (-2*b->crd[5] / 21) + 20;

	//altura
	if(b->dir_y==-1){
		i = 0;
		while(i<desp){
			b->crd[1] = b->crd[1]-1;
			b->crd[3] = b->crd[3]-1;
			b->crd[5] = b->crd[5]-1;
			b->crd[7] = b->crd[7]-1;
			colision = loc_colision(b,p,scr_x,scr_y);
			if(colision>0){
				b->dir_y = 1;
				b->vel_y = 0;
				return colision;
			}
			i++;
		}
	}else{
		b->crd[1] = b->crd[1]+desp;
		b->crd[3] = b->crd[3]+desp;
		b->crd[5] = b->crd[5]+desp;
		b->crd[7] = b->crd[7]+desp;
	}

	//anchura
	if((b->crd[2]<=scr_x) && (b->crd[0]>=0)){
		b->crd[0] = b->crd[0]+bola_desplazar_X(b);
		b->crd[2] = b->crd[2]+bola_desplazar_X(b);
		b->crd[4] = b->crd[4]+bola_desplazar_X(b);
		b->crd[6] = b->crd[6]+bola_desplazar_X(b);
	}else  if(b->crd[2]>scr_x){
		b->crd[0] = scr_x-b->tamano*2;
		b->crd[2] = scr_x;
		b->crd[4] = b->crd[0];
		b->crd[6] = scr_x;
	}else{
		b->crd[0] = 0;
		b->crd[2] = b->tamano*2;
		b->crd[4] = 0;
		b->crd[6] = b->crd[2];
	}

	if(desp <= 1) b->dir_y = -1;
	b->vel_y = b->vel_y+1;

	return colision;
}

void bola_nueva_oportunidad(t_bola *bola){
	bola->dir_y=1;
}

void bola_inicializar(t_bola *bola){
	float tb_x, tb_y;
	if (EXIT_SUCCESS != bbutil_load_texture("app/native/data/ball.png", NULL, NULL, &tb_x, &tb_y, &txbola)) {
		fprintf(stderr, "Fallo al cargar la textura de la bola.\n");
	}
	bola->tcrd[0] = 0.0f;
	bola->tcrd[1] = 0.0f;
	bola->tcrd[2] = tb_x;
	bola->tcrd[3] = 0.0f;
	bola->tcrd[4] = 0.0f;
	bola->tcrd[5] = tb_y;
	bola->tcrd[6] = tb_x;
	bola->tcrd[7] = tb_y;
}

void bola_posicionar(t_bola *bola, t_plataforma *p){
	//inicializar la bola
	bola->tamano=10;
	bola->vel_y=BOLA_VEL;
	bola->crd[0] = (p->crd[0] + p->tam_x/2) - (bola->tamano);
	bola->crd[1] = 50 - (bola->tamano);
	bola->crd[2] = (p->crd[0] + p->tam_x/2) + (bola->tamano);
	bola->crd[3] = bola->crd[1];
	bola->crd[4] = bola->crd[0];
	bola->crd[5] = 50 + (bola->tamano);
	bola->crd[6] = bola->crd[2];
	bola->crd[7] = bola->crd[5];
	bola->vel_x = 0;
	bola->dir_x = 0;
	bola->dir_y = -1;
}

void bola_render(t_bola *bola){
	glPushMatrix();
	glVertexPointer(2, GL_FLOAT, 0, bola->crd);
	glTexCoordPointer(2, GL_FLOAT, 0, bola->tcrd);
	glBindTexture(GL_TEXTURE_2D, txbola);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glPopMatrix();
}

