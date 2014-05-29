/*
 * gamepad.h
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

#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include <assert.h>
#include <bps/screen.h>
#include <bps/navigator.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <bps/sensor.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_CONTROLLERS 2
#define MAX_BUTTONS 32

// Structure representing a game controller.
typedef struct GameController_t {
    // Static device info.
    screen_device_t handle;
    int type;
    int analogCount;
    int buttonCount;
    char id[64];

    // Current state.
    int buttons;
    int analog0[3];
    int analog1[3];

    // Text to display to the user about this controller.
    char deviceString[256];
    char buttonsString[128];
    char analog0String[128];
    char analog1String[128];
} GameController;

// Each button type corresponds to a set of texture coordinates from those defined above.
typedef enum{
    LEFT=0,
    RIGHT=1,
    JUMP=2,
    MENU=3,
    BACK=4
} id_boton_t;

typedef enum{
	NO_ASOCIADO = 0,
	ESCUCHANDO = 1,
	ASOCIADO = 2,
	PULSADO = 4
} estado_botones_t;

//
typedef struct{
	estado_botones_t estado;
	int analogico;
	int eje;
	int boton;
} boton_t;

typedef struct{
	GameController* controlador;
	boton_t botonera[5];
} mando_de_juego_t;

mando_de_juego_t mando;

void inicializar_mando();

void initController(GameController* controller, int player);

void loadController(GameController* controller);

void pollDevices(GameController* Controladores);

void discoverControllers(GameController* Controladores, screen_context_t* ContextS);

#endif
