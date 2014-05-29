/*
 * main.c
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

#include <assert.h>
#include <screen/screen.h>
#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <bps/sensor.h>
#include <bps/virtualkeyboard.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/keycodes.h>

#include <AL/al.h>
#include <AL/alut.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "bbutil.h"
#include "bola.h"
#include "fichhs.h"
#include "gamepad.h"
#include "nubes.h"
#include "interfaz.h"
#include "fondo.h"
#include "misc.h"

#define NUM_AL_BUFFERS 1
#define NUM_AL_SOURCES 1
#define HS_PATH "data/.climberhs"
#define MARGEN 10
#define TIEMPO_OPORTUNIDAD 100

// This macro provides error checking for all calls to libscreen APIs.
int rc;
#define SCREEN_API(x, y) rc = x; \
    if (rc) fprintf(stderr, "\n%s in %s: %d", y, __FUNCTION__, errno)

t_plataforma *plataforma=NULL;
static t_bola bola;
int exit_application = 0;

static EGLint surface_width, surface_height;
static int score=0, scorenew=0, highscore, hsi=0;
static screen_context_t screen_cxt;
TlistaHS *l_hs;

extern int usar_gamepad, invertir_acc;

static GameController Controladores[MAX_CONTROLLERS];
extern mando_de_juego_t mando;
static int pulsado_izq, pulsado_der;
static int pausa_gamepad;

static ALuint buffer[NUM_AL_BUFFERS];
static ALuint source[NUM_AL_SOURCES];


static void handleScreenEvent(bps_event_t *event);
static void handleSensorEvent(bps_event_t *event);
int initialize();
void actuar_controles();
void inicia_juego();
void render();
void act_bckgnd();



int main(int argc, char *argv[]) {
	int rc,i;
	TlistaHS *lowscore;
	float fraccion=0;
	bps_event_t *event;
	int domain;
	screen_window_t screen_window = 0;
	int idle_mode = SCREEN_IDLE_MODE_KEEP_AWAKE;

	//Inicializa controlador
    for (i = 0; i < MAX_CONTROLLERS; ++i) {
        initController(&Controladores[i], i);
    }

    inicializar_mando();


	//Create a screen context that will be used to create an EGL surface to to receive libscreen events
	screen_create_context(&screen_cxt, 0);
	//CREAR VENTANA PARA MANTENER DESPIERTA
	screen_create_window(&screen_window, screen_cxt);
	screen_set_window_property_iv(screen_window,SCREEN_PROPERTY_IDLE_MODE, &idle_mode);

	//Initialize BPS library
	bps_initialize();

	//Use utility code to initialize EGL for rendering with GL ES 1.1
	if (EXIT_SUCCESS != bbutil_init_egl(screen_cxt, GL_ES_1)) {
		fprintf(stderr, "bbutil_init_egl failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}

	//Initialize application logic
	if (EXIT_SUCCESS != initialize()) {
		fprintf(stderr, "initialize failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}

	//Signal BPS library that navigator and screen events will be requested
	if (BPS_SUCCESS != screen_request_events(screen_cxt)) {
		fprintf(stderr, "screen_request_events failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}

	if (BPS_SUCCESS != navigator_request_events(0)) {
		fprintf(stderr, "navigator_request_events failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}

	//Signal BPS library that navigator orientation is to be locked
	if (BPS_SUCCESS != navigator_rotation_lock(true)) {
		fprintf(stderr, "navigator_rotation_lock failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}
	if (BPS_SUCCESS != navigator_set_orientation(0,NULL)) {
		fprintf(stderr, "navigator_set_orientation failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}
	if (BPS_SUCCESS != navigator_set_window_angle(0)) {
		fprintf(stderr, "navigator_set_orientation failed\n");
		bbutil_terminate();
		screen_destroy_context(screen_cxt);
		return 0;
	}

	inicia_juego();

	//habilita el teclado
	virtualkeyboard_request_events(0);

	//buscar controladores conectados
	discoverControllers(Controladores, &screen_cxt);

    cargar_configuracion(&invertir_acc,&usar_gamepad,&mando);

	while (!exit_application) {
		do{
			//Request and process BPS next available event
			event = NULL;
			rc = bps_get_event(&event, 0);
			assert(rc == BPS_SUCCESS);

			if (event) {
				domain = bps_event_get_domain(event);

				if (domain == screen_get_domain()) {
					handleScreenEvent(event);
				} else if (domain == navigator_get_domain()){
					switch (bps_event_get_code(event)){
						case NAVIGATOR_EXIT : 			exit_application = 1;
											  	  	  	break;

						case NAVIGATOR_WINDOW_STATE : 	if(!(NAVIGATOR_WINDOW_FULLSCREEN == navigator_event_get_window_state(event))){
															if(!(NAVIGATOR_KEYBOARD_CLOSED == navigator_event_get_keyboard_state(event))){
																virtualkeyboard_hide();
															}
														}else if(INTERFAZ_RECORD == interfaz_actual()){
															virtualkeyboard_show();
														}
														break;

						case NAVIGATOR_SWIPE_DOWN :		if(INTERFAZ_JUEGO == interfaz_actual()){
															do{
																render();
																usleep(misc_obtener_retardo_us_60fps());
															}while(!interfaz_cambiar(INTERFAZ_OPCIONES));
														}
														break;

						default : 			  			break;
					}
				} else if ((domain == sensor_get_domain()) && !usar_gamepad) {
					handleSensorEvent(event);
				}
			}
		}while(event != NULL);

		//actualización de oportunidades
		if(pausa_gamepad>0){
			pausa_gamepad--;
		}


		if(INTERFAZ_JUEGO == interfaz_actual()){
			if(usar_gamepad) actuar_controles();

			scorenew = bola_actualizar(&bola,plataforma,surface_width,surface_height);
			if(scorenew > plataforma->crd[5]){
				score = score + scorenew;
			}

			if(scorenew>0){
				alSourceStop(source[0]);
				alSourcef(source[0],AL_PITCH,(float)bola.crd[0]/300);
				alSourcef(source[0],AL_GAIN,((float)bola.crd[0]+100)/100);
				alSourcePlay(source[0]);
			}

			if(score > highscore){
				highscore = score;
			}

			//Se ha saltado sobre una plataforma mayor que la primera
			if(scorenew > P_TAM_Y){
				fraccion=((float)scorenew-P_TAM_Y)/10;
				for(i=0;i<10;i++){
					plataformas_desplazar(&plataforma,fraccion);
					plataformas_destruir(&plataforma);
					plataformas_posicionar(plataforma, score,surface_width,surface_height);
					bola_actualizar(&bola,plataforma,surface_width,surface_height);
					render();
				}
				act_bckgnd();
				scorenew=0;
			}
			interfaz_actualizar(score,highscore,l_hs,pausa_gamepad);

			//El jugador ha perdido
			if(scorenew == -1){
				lowscore = p_lowscore(l_hs);
				if ((lowscore == NULL) || ((lowscore != NULL) && (score > lowscore->score))){
					virtualkeyboard_change_options(VIRTUALKEYBOARD_LAYOUT_DEFAULT,VIRTUALKEYBOARD_ENTER_DEFAULT);
					virtualkeyboard_show();
					do{
						render();
						usleep(misc_obtener_retardo_us_60fps());
					}while(!interfaz_cambiar(INTERFAZ_RECORD));

				}else if((lowscore != NULL) && (score <= lowscore->score)){
					interfaz_cambiar(INTERFAZ_PIERDE);
					render();
					sleep(2);
					interfaz_cambiar(INTERFAZ_JUEGO);
					inicia_juego();
				}
			}
		}else if((INTERFAZ_RECORD == interfaz_actual()) && (hsi==4)){
			add_nuevo_hs(&l_hs,interfaz_obtener_inicial(),score);
			hsi=3;
			virtualkeyboard_hide();
			do{
				render();
			}while(!interfaz_cambiar(INTERFAZ_JUEGO));
			inicia_juego();
		}

		render();
		usleep(misc_obtener_retardo_us_60fps());
	}

	//DESCONECTAR SISTEMA DE SONIDO
	alDeleteSources(NUM_AL_SOURCES,source);
	alDeleteBuffers(NUM_AL_BUFFERS,buffer);
	alutExit();

	//GUARDAR CAMBIOS HIGHSCORE
	listahs_a_fichero(l_hs,HS_PATH);
	destruye_listahs(&l_hs);

	//GUARDAR CONFIGURACION
	guardar_configuracion(invertir_acc,usar_gamepad,mando);
	//Stop requesting events from libscreen
	screen_stop_events(screen_cxt);

	//Shut down BPS library for this process
	bps_shutdown();

	bbutil_destroy_font(font13);
	bbutil_destroy_font(font20);
	bbutil_destroy_font(font32);

	//Use utility code to terminate EGL setup
	bbutil_terminate();

	//Destroy libscreen context
	screen_destroy_context(screen_cxt);
	return 0;
}

void actuar_controles(){
	if(mando.controlador != 0){
		if((mando.botonera[JUMP].estado & PULSADO) && (bola.dir_y == -1) && (pausa_gamepad == 0)){
			bola_nueva_oportunidad(&bola);
			pausa_gamepad = TIEMPO_OPORTUNIDAD;
			interfaz_actualizar(score, highscore, l_hs, pausa_gamepad);
		}
		if(mando.botonera[LEFT].estado & PULSADO){
			if(bola.vel_x < 2){
				bola.vel_x = 2;
			}
			bola.dir_x = -1;
			pulsado_izq=true;
			pulsado_der=false;
		}else if(mando.botonera[RIGHT].estado & PULSADO){
			if(bola.vel_x < 2){
				bola.vel_x = 2;
			}
			bola.dir_x = 1;
			pulsado_izq=false;
			pulsado_der=true;
		}else{
			pulsado_izq=false;
			pulsado_der=false;
		}

		if((mando.botonera[MENU].estado & PULSADO) && !(mando.botonera[BACK].estado & PULSADO)){
			do{
				render();
				usleep(misc_obtener_retardo_us_60fps());
			}while(!interfaz_cambiar(INTERFAZ_OPCIONES));
		}

		//control de mandos analogicos
		if(mando.controlador->analog0[0]>0){
			bola.dir_x = 1;
			bola.vel_x = mando.controlador->analog0[0]*10/128;
		}else if(mando.controlador->analog0[0]<0){
			bola.dir_x = -1;
			bola.vel_x = -(mando.controlador->analog0[0])*10/128;
		}
	}

	//Control de gamepad
	//Acelerar bola si se mantiene pulsado
	if((pulsado_izq || pulsado_der)&&(bola.vel_x < 9)){
		bola.vel_x *= 1.1;
	//Si no hay botones pulsados desacelerar hasta 0
	}else if (!(pulsado_izq||pulsado_der)){
		//Parar la bola cuando la velocidad sea inferior a 0.5
		if(bola.vel_x < 0.5){
			bola.vel_x = 0;
			bola.dir_x = 0;
		//Desacelerar
		}else{
			bola.vel_x /= 1.3;
		}
	}
}




static void handleScreenEvent(bps_event_t *event) {
	screen_event_t screen_event = screen_event_get_event(event);

	int screen_val;
	int coords[2];
	int buttons;
	int i;
	screen_device_t device;
	GameController* controller = NULL;
	int attached;
	int type;

	interfaz_t interfaz = interfaz_actual();;
	interfaz_zona_t interfaz_zona;

	screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screen_val);

	switch (screen_val) {
		case SCREEN_EVENT_GAMEPAD:
    	case SCREEN_EVENT_JOYSTICK:
				//Nuevo evento de dispositivo de juego
				SCREEN_API(screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_DEVICE, (void**)&device), "SCREEN_PROPERTY_DEVICE");

				for (i = 0; i < MAX_CONTROLLERS; ++i) {
					if (device == Controladores[i].handle) {
						controller = &Controladores[i];
						break;
					}
				}

				if (!controller) {
					break;
				}


				mando.controlador = controller;

				// Store the controller's new state.
				SCREEN_API(screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS, &controller->buttons), "SCREEN_PROPERTY_BUTTONS");

				if (controller->analogCount > 0) {
					SCREEN_API(screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ANALOG0, controller->analog0), "SCREEN_PROPERTY_ANALOG0");
				}

				if (controller->analogCount == 2) {
					SCREEN_API(screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ANALOG1, controller->analog1), "SCREEN_PROPERTY_ANALOG1");
				}

				//comprobar todos los botones existentes
				for(i=0;i<5;i++){
					if(controller->buttons & mando.botonera[i].boton){
						mando.botonera[i].estado |= PULSADO;
					}else{
						mando.botonera[i].estado &= ~PULSADO;
					}
				}

				if(INTERFAZ_OPCIONES == interfaz){
					//si el boton back no está oyendo, salir al juego
					if(usar_gamepad && (mando.botonera[BACK].estado & PULSADO & ~ESCUCHANDO) && !(mando.botonera[MENU].estado & PULSADO & ~ESCUCHANDO)){
						do{
							render();
							usleep(misc_obtener_retardo_us_60fps());
						}while(!interfaz_cambiar(INTERFAZ_JUEGO));
					}else{
						//comprobar botones oyentes y asociar
						for(i=0;i<5;i++){
							if(mando.botonera[i].estado & ESCUCHANDO){
								if(controller->buttons){
									mando.botonera[i].boton = controller->buttons;
									mando.botonera[i].estado = ASOCIADO;
									mando.controlador = controller;
								}else if(controller->analog0[0]){
									//mando.botonera[i].eje = max()
								}
							}
						}
					}


				}else if(INTERFAZ_JUEGO == interfaz){

					if(usar_gamepad){
						actuar_controles();
					}else if(controller->buttons && !(mando.botonera[MENU].estado & PULSADO) && !(mando.botonera[BACK].estado & PULSADO)){
						do{
							render();
							usleep(misc_obtener_retardo_us_60fps());
						}while(!interfaz_cambiar(INTERFAZ_OPCIONES));
					}

				}else if(INTERFAZ_RECORD == interfaz){
					if(pausa_gamepad == 0){
						pausa_gamepad = 10;
						if((mando.botonera[JUMP].estado & PULSADO) && !(mando.botonera[BACK].estado & PULSADO)){
							if(hsi<4) hsi++;
							pausa_gamepad = 20;
						}else if((mando.botonera[BACK].estado & PULSADO) && !(mando.botonera[JUMP].estado & PULSADO)){
							interfaz_registrar_inicial('_',hsi);
							if(hsi>0) hsi--;
							pausa_gamepad = 20;
						}else if(controller->buttons & SCREEN_DPAD_UP_GAME_BUTTON){
							if(interfaz_obtener_inicial()[hsi]=='_'){
								interfaz_registrar_inicial('Z',hsi);
							}else if(interfaz_obtener_inicial()[hsi]=='a'){
								interfaz_registrar_inicial('9',hsi);
							}else if(interfaz_obtener_inicial()[hsi]=='A'){
								interfaz_registrar_inicial('z',hsi);
							}else if(interfaz_obtener_inicial()[hsi]=='0'){
								interfaz_registrar_inicial('Z',hsi);
							}else{
								interfaz_registrar_inicial(interfaz_obtener_inicial()[hsi]-1,hsi);
							}
						}else if(controller->buttons & SCREEN_DPAD_DOWN_GAME_BUTTON){
							if(interfaz_obtener_inicial()[hsi]=='_'){
								interfaz_registrar_inicial('A',hsi);
							}else if(interfaz_obtener_inicial()[hsi]=='z'){
								interfaz_registrar_inicial('A',hsi);
							}else if(interfaz_obtener_inicial()[hsi]=='Z'){
								interfaz_registrar_inicial('0',hsi);
							}else if(interfaz_obtener_inicial()[hsi]=='9'){
								interfaz_registrar_inicial('a',hsi);
							}else{
								interfaz_registrar_inicial(interfaz_obtener_inicial()[hsi]+1,hsi);
							}
						}
					}
				}
				break;

    	case SCREEN_EVENT_DEVICE:
				//se ha conectado o desconectado un dispositivo

				SCREEN_API(screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_DEVICE, (void**)&device), "SCREEN_PROPERTY_DEVICE");
				SCREEN_API(screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ATTACHED, &attached), "SCREEN_PROPERTY_ATTACHED");

				if (attached) {
					SCREEN_API(screen_get_device_property_iv(device, SCREEN_PROPERTY_TYPE, &type), "SCREEN_PROPERTY_TYPE");
				}

				int i;
				if (attached && (type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK)) {
					for (i = 0; i < MAX_CONTROLLERS; ++i) {
						if (!Controladores[i].handle) {
							Controladores[i].handle = device;
							loadController(&Controladores[i]);
							break;
						}
					}

					if(INTERFAZ_JUEGO == interfaz){
						do{
							render();
							usleep(misc_obtener_retardo_us_60fps());
						}while(!interfaz_cambiar(INTERFAZ_OPCIONES));
					}

				} else {
					for (i = 0; i < MAX_CONTROLLERS; ++i) {
						if (device == Controladores[i].handle) {
							initController(&Controladores[i], i);
							break;
						}
					}
				}
				break;

		case SCREEN_EVENT_MTOUCH_TOUCH:
				screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_SOURCE_POSITION,(int*)&coords);
				interfaz_zona = interfaz_localizar_toque((int*)&coords);

				if(INTERFAZ_JUEGO == interfaz){
						if(INTERFAZ_ZONA_HIGHSCORE == interfaz_zona){
							do{
								render();
								usleep(misc_obtener_retardo_us_60fps());
							}while(!interfaz_cambiar(INTERFAZ_HIGHSCORES));
						}else if(!usar_gamepad && pausa_gamepad==0){ //toque imitando a pulsacion de gamepad, recupera bote
							bola_nueva_oportunidad(&bola);
							pausa_gamepad = TIEMPO_OPORTUNIDAD;
						}else if(usar_gamepad){
							do{
								render();
								usleep(misc_obtener_retardo_us_60fps());
							}while(!interfaz_cambiar(INTERFAZ_OPCIONES));
						}
				}else if(INTERFAZ_HIGHSCORES == interfaz){
					do{
						render();
						usleep(misc_obtener_retardo_us_60fps());
					}while(!interfaz_cambiar(INTERFAZ_JUEGO));

				}else if(INTERFAZ_OPCIONES == interfaz){
					interfaz_limpiar_escuchas();

					if(INTERFAZ_ZONA_JUEGO == interfaz_zona){
						do{
							render();
							usleep(misc_obtener_retardo_us_60fps());
						}while(!interfaz_cambiar(INTERFAZ_JUEGO));
						for(i=0;i<5;i++){
							mando.botonera[i].estado &= ~(ESCUCHANDO|PULSADO);
						}

					}else if(INTERFAZ_ZONA_CHECKBOX_ACC == interfaz_zona){
						invertir_acc = !invertir_acc;
					}else if(INTERFAZ_ZONA_CHECKBOX_JOY == interfaz_zona){
						usar_gamepad = !usar_gamepad;
					}else if(INTERFAZ_ZONA_LEFT == interfaz_zona){
						mando.botonera[LEFT].estado |= ESCUCHANDO;
					}else if(INTERFAZ_ZONA_RIGHT == interfaz_zona){
						mando.botonera[RIGHT].estado |= ESCUCHANDO;
					}else if(INTERFAZ_ZONA_JUMP == interfaz_zona){
						mando.botonera[JUMP].estado |= ESCUCHANDO;
					}else if(INTERFAZ_ZONA_MENU == interfaz_zona){
						mando.botonera[MENU].estado |= ESCUCHANDO;
					}else if(INTERFAZ_ZONA_BACK == interfaz_zona){
						mando.botonera[BACK].estado |= ESCUCHANDO;
					}else if(INTERFAZ_ZONA_EXIT == interfaz_zona){
						exit_application = 1;
					}
				}
				break;

		case SCREEN_EVENT_POINTER:
				screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS, &buttons);
				screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_SOURCE_POSITION,(int*)&coords);
				interfaz_zona = interfaz_localizar_toque((int*)&coords);
				if(buttons == SCREEN_LEFT_MOUSE_BUTTON){
					if(INTERFAZ_JUEGO == interfaz){
							if(INTERFAZ_ZONA_HIGHSCORE == interfaz_zona){
								do{
									render();
									usleep(misc_obtener_retardo_us_60fps());
								}while(!interfaz_cambiar(INTERFAZ_HIGHSCORES));
							}else if(!usar_gamepad && pausa_gamepad==0){ //toque imitando a pulsacion de gamepad, recupera bote
								bola_nueva_oportunidad(&bola);
								pausa_gamepad = TIEMPO_OPORTUNIDAD;
							}else if(usar_gamepad){
								do{
									render();
									usleep(misc_obtener_retardo_us_60fps());
								}while(!interfaz_cambiar(INTERFAZ_OPCIONES));
							}
					}else if(INTERFAZ_HIGHSCORES == interfaz){
						do{
							render();
							usleep(misc_obtener_retardo_us_60fps());
						}while(!interfaz_cambiar(INTERFAZ_JUEGO));

					}else if(INTERFAZ_OPCIONES == interfaz){
						interfaz_limpiar_escuchas();

						if(INTERFAZ_ZONA_JUEGO == interfaz_zona){
							do{
								render();
								usleep(misc_obtener_retardo_us_60fps());
							}while(!interfaz_cambiar(INTERFAZ_JUEGO));
							for(i=0;i<5;i++){
								mando.botonera[i].estado &= ~(ESCUCHANDO|PULSADO);
							}

						}else if(INTERFAZ_ZONA_CHECKBOX_ACC == interfaz_zona){
							invertir_acc = !invertir_acc;
						}else if(INTERFAZ_ZONA_CHECKBOX_JOY == interfaz_zona){
							usar_gamepad = !usar_gamepad;
						}else if(INTERFAZ_ZONA_LEFT == interfaz_zona){
							mando.botonera[LEFT].estado |= ESCUCHANDO;
						}else if(INTERFAZ_ZONA_RIGHT == interfaz_zona){
							mando.botonera[RIGHT].estado |= ESCUCHANDO;
						}else if(INTERFAZ_ZONA_JUMP == interfaz_zona){
							mando.botonera[JUMP].estado |= ESCUCHANDO;
						}else if(INTERFAZ_ZONA_MENU == interfaz_zona){
							mando.botonera[MENU].estado |= ESCUCHANDO;
						}else if(INTERFAZ_ZONA_BACK == interfaz_zona){
							mando.botonera[BACK].estado |= ESCUCHANDO;
						}else if(INTERFAZ_ZONA_EXIT == interfaz_zona){
							exit_application = 1;
						}
					}
				}
				break;

		case SCREEN_EVENT_MTOUCH_MOVE:
		case SCREEN_EVENT_MTOUCH_RELEASE:
				break;

		case SCREEN_EVENT_KEYBOARD:
				if(INTERFAZ_RECORD == interfaz_actual()){
					screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_KEY_FLAGS, &screen_val);
					if (screen_val & KEY_DOWN){
						screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_KEY_SYM, &screen_val);
						switch (screen_val){
							case KEYCODE_BACKSPACE: if((hsi>0)&&(hsi<=3)) {
														hsi--;
														interfaz_registrar_inicial('_',hsi);
													}
													break;
							case KEYCODE_RETURN: 	if((hsi>0)|| interfaz_inicial_valida()){
														interfaz_registrar_inicial('\0',3);
														hsi=4;
													}
													break;
							case KEYCODE_LEFT_SHIFT:break;
							case KEYCODE_RIGHT_SHIFT:break;
							default:				interfaz_registrar_inicial((char)screen_val,hsi);
													if(hsi<3) hsi++;
													break;

						}
					}
				}
				break;
	}
}

static void handleSensorEvent(bps_event_t *event) {
	if (SENSOR_AZIMUTH_PITCH_ROLL_READING == bps_event_get_code(event)) {
		float azimuth, pitch, roll;
		sensor_event_get_apr(event, &azimuth, &pitch, &roll);

		if(!invertir_acc){
			if(pitch > 0){
				bola.dir_x = -1;
				bola.vel_x = pitch/4;
			}else{
				bola.dir_x = 1;
				bola.vel_x = -pitch/4;
			}
		}else{
			if(pitch > 0){
				bola.dir_x = -1;
				bola.vel_x = -pitch/4;
			}else{
				bola.dir_x = 1;
				bola.vel_x = pitch/4;
			}
		}


	}
}


void inicia_juego(){
	score=0;
	scorenew=0;
	bola.vel_x=0;
	bola.dir_x=0;
	pausa_gamepad = 0;

	plataformas_inicializar(&plataforma,surface_width);
	plataformas_posicionar(plataforma, score,surface_width,surface_height);
	bola_posicionar(&bola,plataforma);
	fondo_posicionar();
	nubes_posicionar();

	interfaz_actualizar(score, highscore,l_hs,pausa_gamepad);

}

int initialize(){
	srand(time(NULL));

	//carga la lista de puntuaciones
	fichero_a_listahs(HS_PATH,&l_hs);
	if(l_hs!=NULL) highscore = l_hs->score;
	else highscore = 0;



	//CARGAR DISPLAY
	//Avergiguar la resolucion de la pantalla
	eglQuerySurface(egl_disp, egl_surf, EGL_WIDTH, &surface_width);
	eglQuerySurface(egl_disp, egl_surf, EGL_HEIGHT, &surface_height);

	EGLint err = eglGetError();
	if (err != 0x3000) {
		fprintf(stderr, "Unable to query EGL surface dimensions\n");
		return EXIT_FAILURE;
	}

	//establecer tamano de la pantalla
	glViewport(0, 0, surface_width, surface_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//el segundo parametro determina la vista correspondiente a la proporcion
	glOrthof(0.0f, (float)(surface_width)/(float)(surface_height),
			0.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScalef(1.0f / surface_height, 1.0f / surface_height, 1.0f);

	//fondo de color negro
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);



	if(EXIT_FAILURE == interfaz_inicializar(surface_width, surface_height, screen_cxt, MARGEN, TIEMPO_OPORTUNIDAD)) return EXIT_FAILURE;

	//CARGAR TEXTURAS
	bola_inicializar(&bola);
	plataformas_texturizar();
	fondo_inicializar();
	nubes_inicializar();

	interfaz_actualizar(0,highscore,l_hs,pausa_gamepad);


	//INICIAR EL SENSOR
	if (sensor_is_supported(SENSOR_TYPE_AZIMUTH_PITCH_ROLL)) {
		//Microseconds between sensor reads. This is the rate at which the
		//sensor data will be updated from hardware. The hardware update
		//rate is set below using sensor_set_rate.
		static const int SENSOR_RATE = 16000;

		//Initialize the sensor by setting the rates at which the
		//sensor values will be updated from hardware
		sensor_set_rate(SENSOR_TYPE_AZIMUTH_PITCH_ROLL, SENSOR_RATE);

		sensor_set_skip_duplicates(SENSOR_TYPE_AZIMUTH_PITCH_ROLL, true);
		sensor_request_events(SENSOR_TYPE_AZIMUTH_PITCH_ROLL);
	}

	//CARGAR SISTEMA DE SONIDO

	alutInit(NULL,NULL);
	buffer[0]=alutCreateBufferFromFile("app/native/data/colision.wav");
	alGenSources(NUM_AL_SOURCES,source);

	alSourcei(source[0], AL_BUFFER, buffer[0]);

	alEnable(AL_POSITION);


	return EXIT_SUCCESS;
}

void act_bckgnd(){
	fondo_actualizar(scorenew);
	nubes_actualizar_alto(scorenew);
}


void render() {
	nubes_actualizar_ancho();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	fondo_render();
	nubes_render();
	plataformas_render(plataforma);
	bola_render(&bola);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);

	interfaz_render();


	//Use utility code to update the screen
	bbutil_swap();
}
