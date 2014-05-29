/*
 * interfaz.h
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

#include <GLES/gl.h>
#include <bps/bps.h>
#include <bps/screen.h>
#include <string.h>
#include <stdlib.h>
#include "fichhs.h"
#include "bbutil.h"
#include "gamepad.h"

typedef enum{
	//Interfaz en modo juego
	INTERFAZ_JUEGO = 0,

	//Interfaz mostrando lista de highscore
	INTERFAZ_HIGHSCORES = 1,

	//Interfaz solicitando iniciales de nuevo record
	INTERFAZ_RECORD = 2,

	//Interfaz jugador pierde
	INTERFAZ_PIERDE = 3,

	//Interfaz de menu en juego
	INTERFAZ_OPCIONES = 4
}interfaz_t;

typedef enum{
	//Zona no reconocida
	INTERFAZ_ZONA_INDEFINIDA = 0,

	//Cuadrado highscore
	INTERFAZ_ZONA_HIGHSCORE = 1,

	//Espacio de juego
	INTERFAZ_ZONA_JUEGO = 2,
	INTERFAZ_ZONA_CHECKBOX_ACC = 3,
	INTERFAZ_ZONA_CHECKBOX_JOY = 4,
	INTERFAZ_ZONA_LEFT = 5,
	INTERFAZ_ZONA_RIGHT = 6,
	INTERFAZ_ZONA_JUMP = 7,
	INTERFAZ_ZONA_MENU = 8,
	INTERFAZ_ZONA_BACK = 9,
	INTERFAZ_ZONA_EXIT = 10
}interfaz_zona_t;

//variables para exportar
int invertir_acc, usar_gamepad;

extern mando_de_juego_t mando;

static interfaz_t interfaz_en_uso;
static int animacion_completada;

static float recth_crd[8], gp_marco_esperacrd[8], gp_esperacrd[8], triangulos[18], opciones[8], checkbox_control[8], checkbox_accel[8],
             checkbox_tick[12], asoc_left[8], asoc_right[8], asoc_jump[8], asoc_menu[8], asoc_back[8], boton_exit[8];
static font_t* font13, *font20, *font32;
static char cadena[255], ini[4]="___\0";
static int puntuacion, maxima_puntuacion;
static TlistaHS *lista_puntuaciones;
static int surface_width, surface_height;
static int margen_pantalla, tiempo_oportunidad;
static float fraccionH1=0,fraccionH2=0,fraccionW1=0,fraccionW2=0;

int interfaz_inicializar(int surf_w, int surf_h, screen_context_t s_cxt, int margen, int t_oportunidad);


void interfaz_actualizar(int punt, int max_punt, TlistaHS *lista, int progr_pausa);


/* int interfaz_cambiar(interfaz_t interfaz_destino);
 * Procesa las coordenadas de los objetos para cambiar hasta la interfaz
 * de destino. Devuelve TRUE cuando la transición se completa.
 */
int interfaz_cambiar(interfaz_t interfaz_destino);


/* interfaz_zona_t interfaz_localizar_toque(int* coord);
 * Se busca sobre cual de las zonas definidas ha pulsado el usuario.
 * Se ha añadido un margen de 30 pixeles para facilitar el toque sobre
 * pantallas táctiles.
 */
interfaz_zona_t interfaz_localizar_toque(int* coord);

/* void interfaz_limpiar_escuchas();
 * Elimina los flags de escucha de las zonas de configuración del controlador.
 */
void interfaz_limpiar_escuchas();

/* interfaz_t interfaz_actual()
 * Devuelve el la interfaz que está siendo mostrada en la pantalla
 */
interfaz_t interfaz_actual();


/* void interfaz_registrar_inicial(char valor, int index)
 * Introduce la inicial proporcionada en el hueco indicado del array ini[4]
 */
void interfaz_registrar_inicial(char valor, int index);


/* char* interfaz_obtener_inicial()
 * Devuelve la dirección de memoria donde comienza el array ini[4]
 */
char* interfaz_obtener_inicial();


/* int interfaz_inicial_valida()
 * Comprueba que el array es un nombre valido
 */
int interfaz_inicial_valida();


/* void interfaz_render()
 * Renderiza los distintos tipos de interfaces
 */
void interfaz_render();
