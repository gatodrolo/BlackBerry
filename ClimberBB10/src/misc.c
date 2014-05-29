/*
 * misc.c
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

#include "misc.h"

int misc_obtener_retardo_us_60fps(){
	//ajusta el retardo para conseguir un ciclo cada 16000 microsegundos (60fps)
	int microsegundos=0;
	tiempo2 = clock();
	if(tiempo2>tiempo1){
		//De acuerdo a la documentacion, los ticks de reloj se dan en microsegundos
		microsegundos = (tiempo2-tiempo1);
		tiempo1 = tiempo2;
	}

	if(microsegundos>16000) microsegundos = 0;
	else microsegundos = 16000-microsegundos;

	return microsegundos;
}
