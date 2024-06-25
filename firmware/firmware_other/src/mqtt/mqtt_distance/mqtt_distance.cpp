#include "mqtt_distance.h"

uint8_t dist_lock;
uint16_t dist_virt;

#ifdef ENABLE_MQTT

void mqtt_distance_handle(char *msg)
{
#ifdef ENABLE_VIRT_READINGS
        // Serial.printf("distance update from server: %d -> %d \n", atoi(g[3]), atoi(msg));

        dist_lock = 0;
    dist_virt = atoi(msg);

#endif
}

int distance_read()
{
    uint16_t dist_final;

    // Get physical sensor reading
    uint16_t dist_phy = 0;
    for (int i = 0; i < 3; i++)
        dist_phy += distance.getDistanceInt();
    dist_phy = (dist_phy / 3) / 10; // in cm

    // This is to start the time measurement
    int startT = millis();

#ifdef ENABLE_VIRT_READINGS
    // Publish: sensor/distance
    //      { "id":0, "dist":0 }
    sprintf(tempString1, "%s/%s", CHANNEL, TOPIC_DISTANCE_REQ_TO_SERVER);
    sprintf(tempString2, "{\"id\":\"%d\",\"dist\":\"%d\"}", mqtt_robot_id, dist_phy);

    dist_lock = 1;
    dist_virt = -1;

    mqtt_publish(tempString1, tempString2, false);
    mqtt_wait(&dist_lock);

    if (dist_lock == 0)
    {
        // have both sensor readings
        dist_final = (dist_phy < dist_virt) ? dist_phy : dist_virt;
    }
    else
    {
        // only physical readings received
        dist_final = dist_phy;
    }
#else
    // No virtual sensor support
    dist_final = dist_phy;
#endif

    // This is to stop the time measurement and log it ------
    int stopT = millis();
    char tmp[16];
    sprintf(tmp, "%d", (stopT - startT));
    // mqtt_log(tmp);
    // ------------------------------------------------------

    // Serial.printf("Distance: phy: %d virt: %d | final: %d\n\n", dist_phy, dist_virt, dist_final);

    // int distReadings[4] = {0, 0, 0, 0};
    // distReadings[0] = getDistanceVal(lox0, 0);
    // distReadings[1] = getDistanceVal(lox1, 1);
    // distReadings[2] = getDistanceVal(lox2, 2);
    // distReadings[3] = getDistanceVal(lox3, 3);

    char tmp2[64];
    // sprintf(tmp2, "[%d,%d,%d,%d]", distReadings[0], distReadings[1], distReadings[2], distReadings[3]);
    sprintf(tmp2, "[%d,%d,%d,%d]", getDistanceVal(lox0, 0), getDistanceVal(lox1, 1), getDistanceVal(lox2, 2), getDistanceVal(lox3, 3));
    mqtt_log(tmp2);
    Serial.printf("Distances: [L,F,R,B] = %s\n", tmp2);

    // display_clear();
    // display_print(0, dist_phy);
    // display_print(1, dist_virt);

    return dist_final;
}

#endif
