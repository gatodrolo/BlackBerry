/*
 * interfaz.c
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

#include "interfaz.h"

int interfaz_inicializar(int surf_w, int surf_h, screen_context_t s_cxt, int margen, int t_oportunidad){
	int dpi;
	//CARGAR FUENTE
	dpi = bbutil_calculate_dpi(s_cxt);
	font13 = bbutil_load_font("/usr/fonts/font_repository/monotype/arial.ttf", 6, dpi);
	font20 = bbutil_load_font("/usr/fonts/font_repository/monotype/arial.ttf", 8, dpi);
	font32 = bbutil_load_font("/usr/fonts/font_repository/monotype/arial.ttf", 10, dpi);

	//si alguna de las fuentes falla, devolver fallo general
	if (!(font13 && font20 && font32)) {
		return EXIT_FAILURE;
	}

	memcpy(gp_marco_esperacrd,(float[]){surf_w*0.25,surf_h-30, surf_w*0.25,surf_h-10, surf_w*0.75,surf_h-10, surf_w*0.75,surf_h-30},8*sizeof(float));

	/* Orden de los vertices:
	 * 	4   5    1   2    7   8
	 * 	  3        0		6
	 * 	¡¡cada vertice tiene dos coordenadas!!
	 */
	triangulos[0] = surf_w/2;
	triangulos[1] = surf_h-6.33;
	triangulos[2] = triangulos[0]-2.5;
	triangulos[3] = surf_h-2;
	triangulos[4] = triangulos[0]+2.5;
	triangulos[5] = triangulos[3];

	triangulos[6] = triangulos[0]-15;
	triangulos[7] = triangulos[1];
	triangulos[8] = triangulos[6]-2.5;
	triangulos[9] = triangulos[3];
	triangulos[10] = triangulos[6]+2.5;
	triangulos[11] = triangulos[3];

	triangulos[12] = triangulos[0]+15;
	triangulos[13] = triangulos[1];
	triangulos[14] = triangulos[12]-2.5;
	triangulos[15] = triangulos[3];
	triangulos[16] = triangulos[12]+2.5;
	triangulos[17] = triangulos[3];

	memcpy(opciones,(float[]){2,surf_h, 2,surf_h, surf_w-2,surf_h, surf_w-2,surf_h}, 8*sizeof(float));

	memcpy(asoc_left,(float[]){surf_w*0.5,surf_h*0.94, surf_w*0.5,surf_h*0.99, surf_w*0.59,surf_h*0.99, surf_w*0.59,surf_h*0.94},8*sizeof(float));
	memcpy(asoc_right,(float[]){surf_w*0.5,surf_h*0.87, surf_w*0.5,surf_h*0.92, surf_w*0.59,surf_h*0.92, surf_w*0.59,surf_h*0.87},8*sizeof(float));
	memcpy(asoc_jump,(float[]){surf_w*0.5,surf_h*0.80, surf_w*0.5,surf_h*0.85, surf_w*0.59,surf_h*0.85, surf_w*0.59,surf_h*0.80},8*sizeof(float));
	memcpy(asoc_menu,(float[]){surf_w*0.5,surf_h*0.73, surf_w*0.5,surf_h*0.78, surf_w*0.59,surf_h*0.78, surf_w*0.59,surf_h*0.73},8*sizeof(float));
	memcpy(asoc_back,(float[]){surf_w*0.5,surf_h*0.66, surf_w*0.5,surf_h*0.71, surf_w*0.59,surf_h*0.71, surf_w*0.59,surf_h*0.66},8*sizeof(float));
	memcpy(boton_exit,(float[]){surf_w*0.81,surf_h*0.94, surf_w*0.81,surf_h*0.99, surf_w*0.99,surf_h*0.99, surf_w*0.99,surf_h*0.94},8*sizeof(float));

	invertir_acc = 0;
	usar_gamepad = 0;


	interfaz_en_uso = INTERFAZ_JUEGO;

	surface_width = surf_w;
	surface_height = surf_h;
	margen_pantalla = margen;
	tiempo_oportunidad = t_oportunidad;

	animacion_completada = 1;

	return EXIT_SUCCESS;
}

void interfaz_actualizar(int punt, int max_punt, TlistaHS *lista, int progr_pausa){
	float textH_width, textH_height;

	puntuacion = punt;
	maxima_puntuacion = max_punt;
	lista_puntuaciones = lista;

	sprintf(cadena,"Highscore: %d",maxima_puntuacion);
	bbutil_measure_text(font13, cadena, &textH_width, &textH_height);

	gp_esperacrd[0] = (surface_width/2)-surface_width*0.25*(tiempo_oportunidad-(float)progr_pausa)/tiempo_oportunidad;
	gp_esperacrd[1] = surface_height-30;
	gp_esperacrd[2] = (surface_width/2)+surface_width*0.25*(tiempo_oportunidad-(float)progr_pausa)/tiempo_oportunidad;
	gp_esperacrd[3] = gp_esperacrd[1];
	gp_esperacrd[4] = gp_esperacrd[0];
	gp_esperacrd[5] = surface_height-10;
	gp_esperacrd[6] = gp_esperacrd[2];
	gp_esperacrd[7] = gp_esperacrd[5];

	recth_crd[0] = 2.0f;
	recth_crd[1] = (surface_height-textH_height)-13;
	recth_crd[2] = 2.0f;
	recth_crd[3] = surface_height-1;
	recth_crd[4] = textH_width+margen_pantalla+2;
	recth_crd[5] = surface_height-1;
	recth_crd[6] = textH_width+margen_pantalla+2;
	recth_crd[7] = (surface_height-textH_height)-13;
}

int interfaz_cambiar(interfaz_t interfaz_destino){
	animacion_completada = 0;
	int tiempo_animacion = 0;
	float textH_width, textH_height;
	float fraccionH1, fraccionW1, fraccionH2, fraccionW2;
	static int segmento = 0;
	static int progreso_animacion = 0;

	sprintf(cadena,"Highscore: %d",maxima_puntuacion);
	bbutil_measure_text(font13, cadena, &textH_width, &textH_height);

	switch (interfaz_destino){
		case INTERFAZ_OPCIONES : 	tiempo_animacion = 30;
									fraccionH1 = 1.0/tiempo_animacion;
									fraccionH2 = (surface_height*0.34)/tiempo_animacion;
									interfaz_en_uso = INTERFAZ_OPCIONES;
									if(progreso_animacion <= tiempo_animacion){
										progreso_animacion++;
										opciones[1] -= fraccionH2;
										opciones[3] -= fraccionH1;
										opciones[5] -= fraccionH1;
										opciones[7] -= fraccionH2;
									}else{
										progreso_animacion = 0;
										animacion_completada = 1;
									}
									break;
		case INTERFAZ_HIGHSCORES : 	tiempo_animacion = 30;
									fraccionH1=(surface_height-(textH_height-30))/tiempo_animacion;
									fraccionW1=((surface_width/2)-(textH_width+margen_pantalla+2))/tiempo_animacion;
									interfaz_en_uso = INTERFAZ_HIGHSCORES;
									if((segmento == 0) && (recth_crd[1] >= 15)){
										recth_crd[1] -= fraccionH1;
										recth_crd[4] += fraccionW1;
										recth_crd[7] -= fraccionH1;
										recth_crd[6] += fraccionW1;
									}else if(segmento == 0){
										segmento = 1;
									}

									if((segmento == 1) && (abs(recth_crd[0]-(surface_width-recth_crd[4]))>20)){
										recth_crd[0] += fraccionW1;
										recth_crd[2] += fraccionW1;
										recth_crd[4] += fraccionW1;
										recth_crd[6] += fraccionW1;
									}else if(segmento == 1){
										segmento = 0;
										animacion_completada=1;
									}
									break;

		case INTERFAZ_RECORD :		tiempo_animacion = 30;
									interfaz_en_uso = INTERFAZ_RECORD;
									fraccionH1=60/tiempo_animacion;
									fraccionH2=200/tiempo_animacion;
									fraccionW1=(surface_width/3)/tiempo_animacion;
									fraccionW2=((surface_width*2/3)-textH_width)/tiempo_animacion;
									if(progreso_animacion <= tiempo_animacion){
										progreso_animacion++;
										recth_crd[0] += fraccionW1;
										recth_crd[1] -= fraccionH2;
										recth_crd[2] = recth_crd[0];
										recth_crd[3] -= fraccionH1;
										recth_crd[4] += fraccionW2;
										recth_crd[5] = recth_crd[3];
										recth_crd[6] = recth_crd[4];
										recth_crd[7] = recth_crd[1];
									}else{
										progreso_animacion = 0;
										animacion_completada=1;
									}
									break;

		case INTERFAZ_PIERDE :		interfaz_en_uso = INTERFAZ_PIERDE;
									animacion_completada=1;
									break;

		case INTERFAZ_JUEGO :	switch (interfaz_en_uso) {
									case INTERFAZ_OPCIONES: tiempo_animacion = 30;
															fraccionH1 = 1.0/tiempo_animacion;
															fraccionH2 = (surface_height*0.34)/tiempo_animacion;
															if(progreso_animacion <= tiempo_animacion){
																progreso_animacion++;
																opciones[1] += fraccionH2;
																opciones[3] += fraccionH1;
																opciones[5] += fraccionH1;
																opciones[7] += fraccionH2;
															}else{
																progreso_animacion = 0;
																animacion_completada = 1;
																interfaz_en_uso = INTERFAZ_JUEGO;
															}
															break;

									case INTERFAZ_RECORD : 	tiempo_animacion = 30;
															fraccionH1=60/tiempo_animacion;
															fraccionH2=200/tiempo_animacion;
															fraccionW1=(surface_width/3)/tiempo_animacion;
															fraccionW2=((surface_width*2/3)-textH_width)/tiempo_animacion;
															if(progreso_animacion <= tiempo_animacion){
																progreso_animacion++;
																recth_crd[0] -= fraccionW1;
																recth_crd[1] += fraccionH2;
																recth_crd[2] = recth_crd[0];
																recth_crd[3] += fraccionH1;
																recth_crd[4] -= fraccionW2;
																recth_crd[5] = recth_crd[3];
																recth_crd[6] = recth_crd[4];
																recth_crd[7] = recth_crd[1];
															}else{
																interfaz_en_uso = INTERFAZ_JUEGO;
																animacion_completada=1;
																progreso_animacion = 0;
															}
															break;

									case INTERFAZ_PIERDE :	interfaz_en_uso = INTERFAZ_JUEGO;
															animacion_completada=1;
															break;

									case INTERFAZ_HIGHSCORES : tiempo_animacion = 30;
															fraccionH1=(surface_height-(textH_height-30))/tiempo_animacion;
															fraccionW1=((surface_width/2)-(textH_width+margen_pantalla+2))/tiempo_animacion;
															if((segmento == 0) && (recth_crd[0]>=6.0f)){
																recth_crd[0] -= fraccionW1;
																recth_crd[2] -= fraccionW1;
																recth_crd[4] -= fraccionW1;
																recth_crd[6] -= fraccionW1;
															}else if(segmento == 0){
																segmento = 1;
															}

															if((segmento == 1) && (recth_crd[1]<=((surface_height-textH_height)-30))){
																recth_crd[1] += fraccionH1;
																recth_crd[4] -= fraccionW1;
																recth_crd[7] += fraccionH1;
																recth_crd[6] -= fraccionW1;
															}else if(segmento == 1){
																interfaz_en_uso = INTERFAZ_JUEGO;
																animacion_completada=1;
																segmento = 0;
															}
															break;

									default:				break;
								}
								break; //FIN DE SWITCH DE CASE INTERFAZ_JUEGO

		default :			animacion_completada=1;
							break;

	}
	return animacion_completada;
}

/* interfaz_zona_t interfaz_localizar_toque(int* coord);
 * Se busca sobre cual de las zonas definidas ha pulsado el usuario.
 * Se ha añadido un margen de 30 pixeles para facilitar el toque sobre
 * pantallas táctiles.
 */
interfaz_zona_t interfaz_localizar_toque(int* coord){
	interfaz_zona_t zona_detectada = INTERFAZ_ZONA_INDEFINIDA;

	switch (interfaz_en_uso){
		case INTERFAZ_JUEGO :	if((coord[0] > recth_crd[0]) && (coord[0] < (recth_crd[4]+30)) &&
									((surface_height-coord[1]) > (recth_crd[1]-30)) && ((surface_height-coord[1]) < recth_crd[3])){
										zona_detectada = INTERFAZ_ZONA_HIGHSCORE;
								}
								break;

		case INTERFAZ_OPCIONES: if (coord[1] > surface_height*0.33){
									zona_detectada = INTERFAZ_ZONA_JUEGO;
								}else if((coord[0] > checkbox_accel[0]-30) && (coord[0] < checkbox_accel[4]+30) &&
										(surface_height-coord[1] > checkbox_accel[1]-30) && (surface_height-coord[1] < checkbox_accel[3]+30)){
									zona_detectada = INTERFAZ_ZONA_CHECKBOX_ACC;
								}else if((coord[0] > checkbox_control[0]-30) && (coord[0] < checkbox_control[4]+30) &&
										(surface_height-coord[1] > checkbox_control[1]-30) && (surface_height-coord[1] < checkbox_control[3]+30)){
									zona_detectada = INTERFAZ_ZONA_CHECKBOX_JOY;
								}else if((coord[0] > asoc_left[0]-30) && (coord[0] < asoc_left[4]+30) &&
										(surface_height-coord[1] > asoc_left[1]-10) && (surface_height-coord[1] < asoc_left[3]+10)){
									zona_detectada = INTERFAZ_ZONA_LEFT;
								}else if((coord[0] > asoc_right[0]-30) && (coord[0] < asoc_right[4]+30) &&
										(surface_height-coord[1] > asoc_right[1]-10) && (surface_height-coord[1] < asoc_right[3]+10)){
									zona_detectada = INTERFAZ_ZONA_RIGHT;
								}else if((coord[0] > asoc_jump[0]-30) && (coord[0] < asoc_jump[4]+30) &&
										(surface_height-coord[1] > asoc_jump[1]-10) && (surface_height-coord[1] < asoc_jump[3]+10)){
									zona_detectada = INTERFAZ_ZONA_JUMP;
								}else if((coord[0] > asoc_menu[0]-30) && (coord[0] < asoc_menu[4]+30) &&
										(surface_height-coord[1] > asoc_menu[1]-10) && (surface_height-coord[1] < asoc_menu[3]+10)){
									zona_detectada = INTERFAZ_ZONA_MENU;
								}else if((coord[0] > asoc_back[0]-30) && (coord[0] < asoc_back[4]+30) &&
										(surface_height-coord[1] > asoc_back[1]-10) && (surface_height-coord[1] < asoc_back[3]+10)){
									zona_detectada = INTERFAZ_ZONA_BACK;
								}else if((coord[0] > boton_exit[0]-30) && (coord[0] < boton_exit[4]+30) &&
										(surface_height-coord[1] > boton_exit[1]-10) && (surface_height-coord[1] < boton_exit[3]+10)){
									zona_detectada = INTERFAZ_ZONA_EXIT;
								}
								break;

		default : 				break;
	}

	return zona_detectada;
}

/* void interfaz_limpiar_escuchas();
 * Elimina los flags de escucha de las zonas de configuración del controlador.
 */
void interfaz_limpiar_escuchas(){
	int i;
	for(i=0;i<5;i++){
		mando.botonera[i].estado &= ~PULSADO;
	}
}

/* interfaz_t interfaz_actual()
 * Devuelve el la interfaz que está siendo mostrada en la pantalla
 */
interfaz_t interfaz_actual(){
	return interfaz_en_uso;
}


/* void interfaz_registrar_inicial(char valor, int index)
 * Introduce la inicial proporcionada en el hueco indicado del array ini[4]
 */
void interfaz_registrar_inicial(char valor, int index){
	ini[index] = valor;
}


/* char* interfaz_obtener_inicial()
 * Devuelve la dirección de memoria donde comienza el array ini[4]
 */
char* interfaz_obtener_inicial(){
	return ini;
}

/* int interfaz_inicial_valida()
 * Comprueba que el array es un nombre valido
 */
int interfaz_inicial_valida(){
	int salida = 0;
	if(ini[0] != '_') salida = 1;

	return salida;
}


/* void interfaz_render()
 * Renderiza los distintos tipos de interfaces
 */
void interfaz_render(){
	float pos_x, pos_y;
	static float text_width, text_height,textH_width, textH_height;

	//Colorear en blanco las letras
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	sprintf(cadena,"Score: %d",puntuacion);
	bbutil_measure_text(font13, cadena, &text_width, &text_height);
	pos_y = surface_height - 30;
	pos_x = (surface_width - text_width) - margen_pantalla;
	bbutil_render_text(font13, cadena, pos_x, pos_y);

	sprintf(cadena,"Highscore: %d",maxima_puntuacion);
	bbutil_measure_text(font13, cadena, &textH_width, &textH_height);
	bbutil_render_text(font13, cadena, margen_pantalla, pos_y);



	switch (interfaz_en_uso){
		case INTERFAZ_OPCIONES :   	glEnableClientState(GL_VERTEX_ARRAY);

									glPushMatrix();
									glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
									glVertexPointer(2, GL_FLOAT, 0, recth_crd);
									glDrawArrays(GL_LINE_LOOP, 0, 4);
									glPopMatrix();

									glPushMatrix();
									glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
									glVertexPointer(2, GL_FLOAT, 0, opciones); //Fondo negro para opciones
									glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
									glPopMatrix();

									glPushMatrix();
									glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
									glVertexPointer(2, GL_FLOAT, 0, opciones); //Marco blanco
									glDrawArrays(GL_LINE_LOOP, 0, 4);
									glPopMatrix();




									if(animacion_completada){
										sprintf(cadena,"Invert accelerometer axis:  ");
										bbutil_measure_text(font13, cadena, &text_width, &text_height);
										pos_y = surface_height*0.88 + text_height/2;
										pos_x = 30;
										bbutil_render_text(font13, cadena, pos_x, pos_y);

										pos_x += text_width;
										memcpy(checkbox_accel,(float[]){pos_x,pos_y-5,pos_x,pos_y+25,pos_x+30,pos_y+25,pos_x+30,pos_y-5},8*sizeof(float));;

										glPushMatrix();
										glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
										glVertexPointer(2, GL_FLOAT, 0, checkbox_accel);
										glDrawArrays(GL_LINE_LOOP, 0, 4);
										glPopMatrix();

										if(invertir_acc){
											//checkbox_tick se dibuja como GL_TRIANGLE_FAN desde el vertice inferior
											memcpy(checkbox_tick,(float[]){pos_x+13,pos_y-3, pos_x+1,pos_y+4, pos_x+4,pos_y+10, pos_x+13,pos_y+5,
																			pos_x+24,pos_y+20, pos_x+28,pos_y+16},12*sizeof(float));
											glPushMatrix();
											glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
											glVertexPointer(2, GL_FLOAT, 0, checkbox_tick);
											glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
											glPopMatrix();
										}

										sprintf(cadena,"Use controller:  ");
										bbutil_measure_text(font13, cadena, &text_width, &text_height);
										pos_y = surface_height*0.82 + text_height/2;
										pos_x = 30;
										bbutil_render_text(font13, cadena, pos_x, pos_y);

										pos_x += text_width;
										memcpy(checkbox_control,(float[]){pos_x,pos_y-5,pos_x,pos_y+25,pos_x+30,pos_y+25,pos_x+30,pos_y-5},8*sizeof(float));;

										glPushMatrix();
										glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
										glVertexPointer(2, GL_FLOAT, 0, checkbox_control);
										glDrawArrays(GL_LINE_LOOP, 0, 4);
										glPopMatrix();

										if(usar_gamepad){
											//checkbox_tick se dibuja como GL_TRIANGLE_FAN desde el vertice inferior
											memcpy(checkbox_tick,(float[]){pos_x+13,pos_y-3, pos_x+1,pos_y+4, pos_x+4,pos_y+10, pos_x+13,pos_y+5,
																			pos_x+24,pos_y+20, pos_x+28,pos_y+16},12*sizeof(float));
											glPushMatrix();
											glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
											glVertexPointer(2, GL_FLOAT, 0, checkbox_tick);
											glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
											glPopMatrix();

											//Dibujar botones de asociacion
										/*--------------------LEFT-------------------------*/
											glPushMatrix();
											if(mando.botonera[LEFT].estado & ESCUCHANDO){
												glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
											}else if(mando.botonera[LEFT].estado & PULSADO){
												glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
											}else if(mando.botonera[LEFT].estado & ASOCIADO){
												glColor4f(0.0f, 0.5f, 0.0f, 1.0f);
											}else{
												glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
											}
											glVertexPointer(2, GL_FLOAT, 0, asoc_left); //Fondo rojo para no asociado
											glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
											glPopMatrix();

											glPushMatrix();
											glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
											glVertexPointer(2, GL_FLOAT, 0, asoc_left); //Marco blanco
											glDrawArrays(GL_LINE_LOOP, 0, 4);
											glPopMatrix();

											sprintf(cadena,"LEFT");
											bbutil_measure_text(font13, cadena, &text_width, &text_height);
											pos_y = asoc_left[1] + ((asoc_left[3]-asoc_left[1])/2 - text_height/2);
											pos_x = asoc_left[0]+ ((asoc_left[4]-asoc_left[0])/2 - text_width/2);
											bbutil_render_text(font13, cadena, pos_x, pos_y);
										/*-------------------RIGHT--------------------------*/
											glPushMatrix();
											if(mando.botonera[RIGHT].estado & ESCUCHANDO){
												glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
											}else if(mando.botonera[RIGHT].estado & PULSADO){
												glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
											}else if(mando.botonera[RIGHT].estado & ASOCIADO){
												glColor4f(0.0f, 0.5f, 0.0f, 1.0f);
											}else{
												glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
											}
											glVertexPointer(2, GL_FLOAT, 0, asoc_right); //Fondo rojo para no asociado
											glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
											glPopMatrix();

											glPushMatrix();
											glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
											glVertexPointer(2, GL_FLOAT, 0, asoc_right); //Marco blanco
											glDrawArrays(GL_LINE_LOOP, 0, 4);
											glPopMatrix();

											sprintf(cadena,"RIGHT");
											bbutil_measure_text(font13, cadena, &text_width, &text_height);
											pos_y = asoc_right[1] + ((asoc_right[3]-asoc_right[1])/2 - text_height/2);
											pos_x = asoc_right[0]+ ((asoc_right[4]-asoc_right[0])/2 - text_width/2);
											bbutil_render_text(font13, cadena, pos_x, pos_y);
										/*--------------------JUMP-------------------------*/
											glPushMatrix();
											if(mando.botonera[JUMP].estado & ESCUCHANDO){
												glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
											}else if(mando.botonera[JUMP].estado & PULSADO){
												glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
											}else if(mando.botonera[JUMP].estado & ASOCIADO){
												glColor4f(0.0f, 0.5f, 0.0f, 1.0f);
											}else{
												glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
											}
											glVertexPointer(2, GL_FLOAT, 0, asoc_jump); //Fondo rojo para no asociado
											glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
											glPopMatrix();

											glPushMatrix();
											glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
											glVertexPointer(2, GL_FLOAT, 0, asoc_jump); //Marco blanco
											glDrawArrays(GL_LINE_LOOP, 0, 4);
											glPopMatrix();

											sprintf(cadena,"JUMP");
											bbutil_measure_text(font13, cadena, &text_width, &text_height);
											pos_y = asoc_jump[1] + ((asoc_jump[3]-asoc_jump[1])/2 - text_height/2);
											pos_x = asoc_jump[0]+ ((asoc_jump[4]-asoc_jump[0])/2 - text_width/2);
											bbutil_render_text(font13, cadena, pos_x, pos_y);
										/*-------------------MENU---------------------------*/
											glPushMatrix();
											if(mando.botonera[MENU].estado & ESCUCHANDO){
												glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
											}else if(mando.botonera[MENU].estado & PULSADO){
												glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
											}else if(mando.botonera[MENU].estado & ASOCIADO){
												glColor4f(0.0f, 0.5f, 0.0f, 1.0f);
											}else{
												glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
											}
											glVertexPointer(2, GL_FLOAT, 0, asoc_menu); //Fondo rojo para no asociado
											glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
											glPopMatrix();

											glPushMatrix();
											glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
											glVertexPointer(2, GL_FLOAT, 0, asoc_menu); //Marco blanco
											glDrawArrays(GL_LINE_LOOP, 0, 4);
											glPopMatrix();

											sprintf(cadena,"MENU");
											bbutil_measure_text(font13, cadena, &text_width, &text_height);
											pos_y = asoc_menu[1] + ((asoc_menu[3]-asoc_menu[1])/2 - text_height/2);
											pos_x = asoc_menu[0]+ ((asoc_menu[4]-asoc_menu[0])/2 - text_width/2);
											bbutil_render_text(font13, cadena, pos_x, pos_y);
											/*-------------------BACK---------------------------*/
											glPushMatrix();
											if(mando.botonera[BACK].estado & ESCUCHANDO){
												glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
											}else if(mando.botonera[BACK].estado & PULSADO){
												glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
											}else if(mando.botonera[BACK].estado & ASOCIADO){
												glColor4f(0.0f, 0.5f, 0.0f, 1.0f);
											}else{
												glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
											}
											glVertexPointer(2, GL_FLOAT, 0, asoc_back); //Fondo rojo para no asociado
											glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
											glPopMatrix();

											glPushMatrix();
											glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
											glVertexPointer(2, GL_FLOAT, 0, asoc_back); //Marco blanco
											glDrawArrays(GL_LINE_LOOP, 0, 4);
											glPopMatrix();

											sprintf(cadena,"BACK");
											bbutil_measure_text(font13, cadena, &text_width, &text_height);
											pos_y = asoc_back[1] + ((asoc_back[3]-asoc_back[1])/2 - text_height/2);
											pos_x = asoc_back[0]+ ((asoc_back[4]-asoc_back[0])/2 - text_width/2);
											bbutil_render_text(font13, cadena, pos_x, pos_y);
										}

										/*-------------------EXIT---------------------------*/
										glPushMatrix();
										glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
										glVertexPointer(2, GL_FLOAT, 0, boton_exit);
										glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
										glPopMatrix();

										glPushMatrix();
										glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
										glVertexPointer(2, GL_FLOAT, 0, boton_exit); //Marco blanco
										glDrawArrays(GL_LINE_LOOP, 0, 4);
										glPopMatrix();

										sprintf(cadena,"EXIT GAME");
										bbutil_measure_text(font13, cadena, &text_width, &text_height);
										pos_y = boton_exit[1] + ((boton_exit[3]-boton_exit[1])/2 - text_height/2);
										pos_x = boton_exit[0]+ ((boton_exit[4]-boton_exit[0])/2 - text_width/2);
										bbutil_render_text(font13, cadena, pos_x, pos_y);

									}

									glDisableClientState(GL_VERTEX_ARRAY);

									break;

		case INTERFAZ_HIGHSCORES :	glEnableClientState(GL_VERTEX_ARRAY);

									glPushMatrix();
									glColor4f(0.0f, 0.0f, 0.0f, 1.0f); //Interior opaco del rectangulo de puntuaciones
									glVertexPointer(2, GL_FLOAT, 0, recth_crd);
									glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
									glPopMatrix();

									glPushMatrix();
									glColor4f(1.0f, 1.0f, 1.0f, 1.0f);//Marco del rectangulo de puntuaciones
									glVertexPointer(2, GL_FLOAT, 0, recth_crd);
									glDrawArrays(GL_LINE_LOOP, 0, 4);
									glPopMatrix();

									glDisableClientState(GL_VERTEX_ARRAY);

									if(animacion_completada){
										glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

										bbutil_measure_text(font20,"HIGHSCORES",&text_width,&text_height);
								       	bbutil_render_text(font20, "HIGHSCORES", recth_crd[0]+(((recth_crd[6]-recth_crd[0])-text_width)/2), surface_height-40);

								       	int i = 1;
								    	TlistaHS * aux = lista_puntuaciones;
								    	while(aux != NULL){
								    		sprintf(cadena,"%d.       %s",i,aux->ini);
								    		bbutil_render_text(font20, cadena, recth_crd[0]+25, surface_height-((i*45)+90));
								    		sprintf(cadena,"%d",aux->score);
								    		bbutil_measure_text(font20,cadena,&text_width,&text_height);
								    		bbutil_render_text(font20, cadena, (recth_crd[4]-text_width)-20, surface_height-((i*45)+90));
								    		aux = aux->sig;
								    		i++;
								    	}
									}

									break;

		case INTERFAZ_PIERDE :		glEnableClientState(GL_VERTEX_ARRAY);

									glPushMatrix();
									glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //Marco estado gamepad
									glVertexPointer(2, GL_FLOAT, 0, gp_marco_esperacrd);
									glDrawArrays(GL_LINE_LOOP, 0, 4);
									glPopMatrix();

									glPushMatrix();
									glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
									glVertexPointer(2, GL_FLOAT, 0, gp_esperacrd);
									glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
									glPopMatrix();

									glPushMatrix();
									glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
									glVertexPointer(2, GL_FLOAT, 0, recth_crd);
									glDrawArrays(GL_LINE_LOOP, 0, 4);
									glPopMatrix();

									glDisableClientState(GL_VERTEX_ARRAY);

							    	bbutil_render_text(font32, "OUT!", surface_width/2-100, surface_height/2-50);

							    	break;

		case INTERFAZ_JUEGO :		glEnableClientState(GL_VERTEX_ARRAY);

									glPushMatrix();
									glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
									glVertexPointer(2, GL_FLOAT, 0, triangulos);
									glDrawArrays(GL_TRIANGLES, 0, 9);
									glPopMatrix();

									glPushMatrix();
									glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
									glVertexPointer(2, GL_FLOAT, 0, gp_esperacrd);
									glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
									glPopMatrix();

									glPushMatrix();
									glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //Marco estado gamepad
									glVertexPointer(2, GL_FLOAT, 0, gp_marco_esperacrd);
									glDrawArrays(GL_LINE_LOOP, 0, 4);
									glPopMatrix();

									glPushMatrix();
									glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
									glVertexPointer(2, GL_FLOAT, 0, recth_crd);
									glDrawArrays(GL_LINE_LOOP, 0, 4);
									glPopMatrix();

									glDisableClientState(GL_VERTEX_ARRAY);

									break;

		case INTERFAZ_RECORD :		glEnableClientState(GL_VERTEX_ARRAY);

									glPushMatrix();
									glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //Marco estado gamepad
									glVertexPointer(2, GL_FLOAT, 0, gp_marco_esperacrd);
									glDrawArrays(GL_LINE_LOOP, 0, 4);
									glPopMatrix();

									glPushMatrix();
									glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
									glVertexPointer(2, GL_FLOAT, 0, gp_esperacrd);
									glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
									glPopMatrix();

									glPushMatrix();
									glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
									glVertexPointer(2, GL_FLOAT, 0, recth_crd);
									glDrawArrays(GL_LINE_LOOP, 0, 4);
									glPopMatrix();

									glDisableClientState(GL_VERTEX_ARRAY);

							    	sprintf(cadena,"NEW HIGHSCORE: %d",puntuacion);
									bbutil_measure_text(font13,cadena,&text_width,&text_height);
							    	bbutil_render_text(font13, cadena, recth_crd[0]+(((recth_crd[6]-recth_crd[0])-text_width)/2), recth_crd[3]-35);
							    	sprintf(cadena,"%c       %c       %c",ini[0],ini[1],ini[2]);
									bbutil_measure_text(font20,cadena,&text_width,&text_height);
							    	bbutil_render_text(font20, cadena,recth_crd[0]+(((recth_crd[6]-recth_crd[0])-text_width)/2), recth_crd[1]+20);

									break;
	}

}

