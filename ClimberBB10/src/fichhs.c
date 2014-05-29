/*
 * fichhs.c
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

#include "fichhs.h"
#include <stdio.h>
#include <malloc.h>

int fichero_a_listahs(char * path, TlistaHS ** l){
	FILE * hsf;
	char ini[4];
	int score;

	hsf = fopen(path,"rb");
	if(hsf != NULL){
		while(!feof(hsf) && (fread(ini,sizeof(char),4,hsf)>0) &&
				(fread(&score,sizeof(int),1,hsf)>0)){
			add_nuevo_hs(&*l,ini,score);
		}
		fclose(hsf);
		return 0;
	}else{
		l=NULL;
		fprintf(stderr, "imposible abrir fichero\n");
		return 1;
	}

}

void add_nuevo_hs(TlistaHS ** l, char * nombre, int score){
	TlistaHS * nuevohs = (TlistaHS*)malloc(sizeof(TlistaHS));
	TlistaHS *aux;
	sprintf(nuevohs->ini,nombre);
	nuevohs->score = score;
	aux = *l;
	if(*l == NULL){
		nuevohs->sig = *l;
		*l = nuevohs;
	}else if (score > aux->score){
		nuevohs->sig = *l;
		*l = nuevohs;
	}else{
		while((aux->sig != NULL) && (aux->sig->score > score)){
				aux = aux->sig;
		}
		nuevohs->sig = aux->sig;
		aux->sig = nuevohs;
	}
	limpia_lista(&*l);
}

int listahs_a_fichero(TlistaHS * l, char * path){
	FILE * hsf;
	hsf = fopen(path,"wb");
	if(hsf != NULL){
		while(l != NULL){
			fwrite(l->ini,sizeof(char),4,hsf);
			fwrite(&l->score,sizeof(int),1,hsf);
			l = l->sig;
		}
		fclose(hsf);
		return 0;
	}else{
		fprintf(stderr, "imposible abrir fichero\n");
		return 1;
	}
}

void destruye_listahs(TlistaHS **l){
	TlistaHS *aux;
	aux = *l;
	while(aux != NULL){
		*l = aux->sig;
		free(aux);
		aux=*l;
	}

}

void limpia_lista(TlistaHS **l){
	int i=0;
	TlistaHS * aux;
	aux = *l;

	while((aux != NULL) && (i<9)){
		aux = aux->sig;
		i++;
	}
	if(aux!=NULL) destruye_listahs(&aux->sig);
}

TlistaHS* p_lowscore(TlistaHS *l){
	TlistaHS *rl;
	rl=l;
	while((rl != NULL) && (rl->sig!=NULL)){
		rl=rl->sig;
	}
	return rl;
}

int guardar_configuracion(int conf_acc, int conf_gamepad, mando_de_juego_t mando){
	FILE * cfg_ds;
	cfg_ds = fopen(CONF_FILE,"wb");
	if(cfg_ds != NULL){
		fwrite(&conf_acc,sizeof(int),1,cfg_ds);
		fwrite(&conf_gamepad,sizeof(int),1,cfg_ds);
		fwrite(&mando,sizeof(mando_de_juego_t),1,cfg_ds);
		fclose(cfg_ds);
		return 0;
	}else{
		fprintf(stderr, "imposible abrir fichero\n");
		return 1;
	}
}

int cargar_configuracion(int *conf_acc, int *conf_gamepad, mando_de_juego_t *mando){
	FILE * cfg_ds;

	cfg_ds = fopen(CONF_FILE,"rb");
	if(!cfg_ds){
		cfg_ds = fopen("app/native/data/.config","rb");
	}
	if(cfg_ds != NULL){
		fread(conf_acc,sizeof(int),1,cfg_ds);
		fread(conf_gamepad,sizeof(int),1,cfg_ds);
		fread(mando,sizeof(mando_de_juego_t),1,cfg_ds);
		fclose(cfg_ds);
		return 0;
	}else{
		fprintf(stderr, "imposible abrir fichero\n");
		return 1;
	}
}


