/*
 * fichhs.h
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

#ifndef FICHHS_H_
#define FICHHS_H_

#include "gamepad.h"

#define CONF_FILE "data/.config"


typedef struct lista_hs{
	char ini[4];
	int score;
	struct lista_hs *sig;
}TlistaHS;


int fichero_a_listahs(char * path, TlistaHS ** l);

void add_nuevo_hs(TlistaHS ** l, char * nombre, int score);

int listahs_a_fichero(TlistaHS * l, char * path);

void destruye_listahs(TlistaHS ** l);

void limpia_lista(TlistaHS **l);

TlistaHS* p_lowscore(TlistaHS *l);

int guardar_configuracion(int conf_acc, int conf_gamepad, mando_de_juego_t mando);

int cargar_configuracion(int *conf_acc, int *conf_gamepad, mando_de_juego_t *mando);

#endif /* FICHHS_H_ */
