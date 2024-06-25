
// This is the configuration file for the robot
// Fill all the required the parameters and
// rename this file as 'config.h' before use

#ifndef _ROBOT_CONFIG_H
#define _ROBOT_CONFIG_H

// MQTT Communication
#ifdef ENABLE_MQTT
#define MQTT_SERVER "webservices.ceykod.com"
// #define MQTT_SERVER "192.168.8.1"
#define MQTT_PORT 1883
#define MQTT_CLIENT "Robot"
// #define MQTT_CLIENT "mqtt_server"
#define MQTT_USERNAME "swarm_user"
#define MQTT_PASSWORD "swarm_usere15"
#endif

// ------------------------------------------------------------- WiFi Client API
#ifdef ENABLE_WIFI_CLIENT
#define HOST "http://192.168.8.1/";
#define PORT 8081
#endif

// ------------------------------------------------------------ WiFi Credentials
#define WIFI_SSID "Nokia C21"
#define WIFI_PASS "DanaNew4321"

#endif
