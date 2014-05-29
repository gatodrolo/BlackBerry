/*
 * plataforma.c
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

#include "plataforma.h"


void plataformas_inicializar(t_plataforma **plat, GLfloat scr_x){
	if(*plat == NULL) *plat = (t_plataforma*)malloc(sizeof(t_plataforma));
	(*plat)->tam_x = P_TAM_X;
	(*plat)->tam_y = P_TAM_Y;
	(*plat)->crd[0] = (scr_x / 2) + (P_TAM_X / 2);
	(*plat)->crd[1] = 0;
	(*plat)->crd[2] = (*plat)->crd[0]+P_TAM_X;
	(*plat)->crd[3] = 0;
	(*plat)->crd[4] = (*plat)->crd[0];
	(*plat)->crd[5] = P_TAM_Y;
	(*plat)->crd[6] = (*plat)->crd[2];
	(*plat)->crd[7] = (*plat)->crd[5];
	(*plat)->sig = NULL;
}

void plataformas_texturizar(){
	float tp_x, tp_y;
	if (EXIT_SUCCESS != bbutil_load_texture("app/native/data/ptfm.png", NULL, NULL, &tp_x, &tp_y, &txptfm)) {
		fprintf(stderr, "Fallo al cargar la textura de la plataforma.\n");
	}
	pcrd[0] = 0.0f;
	pcrd[1] = 0.0f;
	pcrd[2] = tp_x;
	pcrd[3] = 0.0f;
	pcrd[4] = 0.0f;
	pcrd[5] = tp_y;
	pcrd[6] = tp_x;
	pcrd[7] = tp_y;
}

//rellena la pantalla con plataformas
//p es la direccion de memoria que contiene un puntero a una plataforma
void plataformas_posicionar(t_plataforma *p, int score, GLfloat scr_x, GLfloat scr_y){
	int signo=0;
	t_plataforma *aux;

	aux=p;

	while(aux->sig!=NULL){
		aux=aux->sig;
	}

	while(aux->crd[5] < (scr_y - 50)){
		signo=rand();

		if(RAND_MAX/2 > signo){
			signo=1;
		}else{
			signo=-1;
		}

		aux->sig = (t_plataforma*)malloc(sizeof(t_plataforma));
		aux->tam_x = P_TAM_X;
		aux->tam_y = P_TAM_Y;
		aux->sig->crd[0] = (scr_x - aux->tam_x) * ((float)rand() / RAND_MAX);
		aux->sig->crd[1] = aux->crd[5] + 								   //ultima plataforma
						(((float)rand() / RAND_MAX) * 20) +				       //distancia minima basica
						(140 * (score+1)/TECHO); //distancia acorde a la dificultad
		//aux->sig->crd[0] = aux->crd[0] + signo*((0.613f*(aux->sig->crd[1]-aux->crd[1])+27.69f) *
		//				((float)rand() / RAND_MAX));
		aux->sig->crd[2] = aux->sig->crd[0]+p->tam_x;
		aux->sig->crd[3] = aux->sig->crd[1];
		aux->sig->crd[4] = aux->sig->crd[0];
		aux->sig->crd[5] = aux->sig->crd[1]+p->tam_y;
		aux->sig->crd[6] = aux->sig->crd[2];
		aux->sig->crd[7] = aux->sig->crd[5];
		aux = aux->sig;
		aux->sig=NULL;
	}
}

void plataformas_destruir(t_plataforma **p){
	t_plataforma *aux;
	aux = *p;
	while(aux!=NULL && aux->crd[5]<=0){
			*p = aux->sig;
			free(aux);
			aux = *p;
	}
}

void plataformas_desplazar(t_plataforma **p, GLfloat destino){
	t_plataforma *aux;
	aux = *p;
	while(aux != NULL){
		aux->crd[1] = aux->crd[1] - destino;
		aux->crd[3] = aux->crd[3] - destino;
		aux->crd[5] = aux->crd[5] - destino;
		aux->crd[7] = aux->crd[7] - destino;
		aux=aux->sig;
	}
}

void plataformas_render(t_plataforma *p){
	t_plataforma *aux_p;
	aux_p=p;
	while(aux_p != NULL){
		glPushMatrix();
		glVertexPointer(2, GL_FLOAT, 0, aux_p->crd);
		glTexCoordPointer(2, GL_FLOAT, 0, pcrd);
		glBindTexture(GL_TEXTURE_2D, txptfm);
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glPopMatrix();
		aux_p=aux_p->sig;
	}
}
