/*
 *
 *  Created on: 28/04/2014
 *      Author: Gato
 *      Based on: https://github.com/blackberry/NDK-Samples/blob/master/Gamepad/main.c
 */

/*
* Copyright (c) 2011-2012 Research In Motion Limited.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#include "gamepad.h"

// This macro provides error checking for all calls to libscreen APIs.
int rc;
#define SCREEN_API(x, y) rc = x; \
    if (rc) fprintf(stderr, "\n%s in %s: %d", y, __FUNCTION__, errno)


void initController(GameController* controller, int player)
{
    // Initialize controller values.
    controller->handle = 0;
    controller->type = 0;
    controller->analogCount = 0;
    controller->buttonCount = 0;
    controller->buttons = 0;
    controller->analog0[0] = controller->analog0[1] = controller->analog0[2] = 0;
    controller->analog1[0] = controller->analog1[1] = controller->analog1[2] = 0;
    sprintf(controller->deviceString, "Player %d: No device detected.", player + 1);
}

void inicializar_mando(){
    mando.controlador = NULL;
    mando.botonera[LEFT].analogico=0;
    mando.botonera[LEFT].boton=0;
    mando.botonera[LEFT].eje=0;
    mando.botonera[LEFT].estado=NO_ASOCIADO;
    mando.botonera[RIGHT].analogico=0;
	mando.botonera[RIGHT].boton=0;
	mando.botonera[RIGHT].eje=0;
	mando.botonera[RIGHT].estado=NO_ASOCIADO;
	mando.botonera[JUMP].analogico=0;
	mando.botonera[JUMP].boton=0;
	mando.botonera[JUMP].eje=0;
	mando.botonera[JUMP].estado=NO_ASOCIADO;
	mando.botonera[MENU].analogico=0;
	mando.botonera[MENU].boton=0;
	mando.botonera[MENU].eje=0;
	mando.botonera[MENU].estado=NO_ASOCIADO;
}


void loadController(GameController* controller)
{
    // Query libscreen for information about this device.
    SCREEN_API(screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_TYPE, &controller->type), "SCREEN_PROPERTY_TYPE");
    SCREEN_API(screen_get_device_property_cv(controller->handle, SCREEN_PROPERTY_ID_STRING, sizeof(controller->id), controller->id), "SCREEN_PROPERTY_ID_STRING");
    SCREEN_API(screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_BUTTON_COUNT, &controller->buttonCount), "SCREEN_PROPERTY_BUTTON_COUNT");

    // Check for the existence of analog sticks.
    if (!screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG0, controller->analog0)) {
    	++controller->analogCount;
    }

    if (!screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG1, controller->analog1)) {
    	++controller->analogCount;
    }

    if (controller->type == SCREEN_EVENT_GAMEPAD) {
        sprintf(controller->deviceString, "Gamepad device ID: %s", controller->id);
    } else {
        sprintf(controller->deviceString, "Joystick device ID: %s", controller->id);
    }
}

void pollDevices(GameController* Controladores)
{
    int i;
    for (i = 0; i < MAX_CONTROLLERS; i++) {
        GameController* controller = &Controladores[i];

        if (controller->handle) {
            // Get the current state of a gamepad device.
            SCREEN_API(screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_BUTTONS, &controller->buttons), "SCREEN_PROPERTY_BUTTONS");

            if (controller->analogCount > 0) {
            	SCREEN_API(screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG0, controller->analog0), "SCREEN_PROPERTY_ANALOG0");
            }

            if (controller->analogCount == 2) {
            	SCREEN_API(screen_get_device_property_iv(controller->handle, SCREEN_PROPERTY_ANALOG1, controller->analog1), "SCREEN_PROPERTY_ANALOG1");
            }
        }
    }
}

void discoverControllers(GameController* Controladores, screen_context_t* ContextS)
{
    // Get an array of all available devices.
    int deviceCount;
    SCREEN_API(screen_get_context_property_iv((screen_context_t)*ContextS, SCREEN_PROPERTY_DEVICE_COUNT, &deviceCount), "SCREEN_PROPERTY_DEVICE_COUNT");
    screen_device_t* devices = (screen_device_t*)calloc(deviceCount, sizeof(screen_device_t));
    SCREEN_API(screen_get_context_property_pv((screen_context_t)*ContextS, SCREEN_PROPERTY_DEVICES, (void**)devices), "SCREEN_PROPERTY_DEVICES");

    // Scan the list for gamepad and joystick devices.
    int i;
    int controllerIndex = 0;
    for (i = 0; i < deviceCount; i++) {
        int type;
        SCREEN_API(screen_get_device_property_iv(devices[i], SCREEN_PROPERTY_TYPE, &type), "SCREEN_PROPERTY_TYPE");

        if (!rc && (type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK)) {
            // Assign this device to control Player 1 or Player 2.
            GameController* controller = &Controladores[controllerIndex];
            controller->handle = devices[i];
            loadController(controller);

            // We'll just use the first compatible devices we find.
            controllerIndex++;
            if (controllerIndex == MAX_CONTROLLERS) {
                break;
            }
        }
    }

    free(devices);
}


