/**
 * @brief    Swarm Robot Motor Controller Library
 * @author   Nuwan Jaliyagoda
 * @version  2.0.0
 * @url      N/A
 *
 * ------------------------------------------------------------------------------
 */

#include "robot_encoder.h"
#include "config/pins.h"

RobotEncoder *RobotEncoder::encoders[MAX_ESP32_ENCODERS] = {NULL, NULL, NULL,
                                                            NULL,
                                                            NULL, NULL, NULL, NULL};

bool RobotEncoder::attachedInterrupt = false;
pcnt_isr_handle_t RobotEncoder::user_isr_handle = NULL;

RobotEncoder::RobotEncoder()
{
    attached = false;
    aPinNumber = (gpio_num_t)0;
    bPinNumber = (gpio_num_t)0;
    working = false;
    direction = false;
    unit = (pcnt_unit_t)0; // -1 *
}

RobotEncoder::~RobotEncoder()
{
}

/* Decode what PCNT's unit originated an interrupt
 * and pass this information together with the event type
 * the main program using a queue.
 */
static void IRAM_ATTR pcnt_example_intr_handler(void *arg)
{
    RobotEncoder *ptr;

    uint32_t intr_status = PCNT.int_st.val;
    int i;

    for (i = 0; i < PCNT_UNIT_MAX; i++)
    {
        if (intr_status & (BIT(i)))
        {
            ptr = RobotEncoder::encoders[i];
            /* Save the PCNT event type that caused an interrupt
             to pass it to the main program */

            int status = 0;
            if (PCNT.status_unit[i].h_lim_lat)
            {
                status = ptr->r_enc_config.counter_h_lim;
            }
            if (PCNT.status_unit[i].l_lim_lat)
            {
                status = ptr->r_enc_config.counter_l_lim;
            }
            // pcnt_counter_clear(ptr->unit);
            PCNT.int_clr.val = BIT(i); // clear the interrupt
            ptr->count = status + ptr->count;
        }
    }
}

void RobotEncoder::attach(int a, int b, boolean fq)
{
    if (attached)
    {
        Serial.println("All ready attached, FAIL!");
        return;
    }
    int index = 0;
    for (; index < MAX_ESP32_ENCODERS; index++)
    {
        if (RobotEncoder::encoders[index] == NULL)
        {
            encoders[index] = this;
            break;
        }
    }
    if (index == MAX_ESP32_ENCODERS)
    {
        Serial.println("Too many encoders, FAIL!");
        return;
    }

    // Set data now that pin attach checks are done
    fullQuad = fq;
    unit = (pcnt_unit_t)index;
    this->aPinNumber = (gpio_num_t)a;
    this->bPinNumber = (gpio_num_t)b;

    // Set up the IO state of hte pin
    gpio_pad_select_gpio(aPinNumber);
    gpio_pad_select_gpio(bPinNumber);
    gpio_set_direction(aPinNumber, GPIO_MODE_INPUT);
    gpio_set_direction(bPinNumber, GPIO_MODE_INPUT);
    gpio_pulldown_en(aPinNumber);
    gpio_pulldown_en(bPinNumber);

    // Set up encoder PCNT configuration
    r_enc_config.pulse_gpio_num = aPinNumber; // Rotary Encoder Chan A
    r_enc_config.ctrl_gpio_num = bPinNumber;  // Rotary Encoder Chan B

    r_enc_config.unit = unit;
    r_enc_config.channel = PCNT_CHANNEL_0;

    r_enc_config.pos_mode = fullQuad ? PCNT_COUNT_DEC : PCNT_COUNT_DIS; // Count Only On Rising-Edges
    r_enc_config.neg_mode = PCNT_COUNT_INC;                             // Discard Falling-Edge

    r_enc_config.lctrl_mode = PCNT_MODE_KEEP;    // Rising A on HIGH B = CW Step
    r_enc_config.hctrl_mode = PCNT_MODE_REVERSE; // Rising A on LOW B = CCW Step

    r_enc_config.counter_h_lim = INT16_MAX;
    r_enc_config.counter_l_lim = INT16_MIN;

    pcnt_unit_config(&r_enc_config);

    // Filter out bounces and noise
    pcnt_set_filter_value(unit, 250); // Filter Runt Pulses
    pcnt_filter_enable(unit);

    /* Enable events on  maximum and minimum limit values */
    pcnt_event_enable(unit, PCNT_EVT_H_LIM);
    pcnt_event_enable(unit, PCNT_EVT_L_LIM);

    pcnt_counter_pause(unit); // Initial PCNT init
    pcnt_counter_clear(unit);
    /* Register ISR handler and enable interrupts for PCNT unit */
    if (RobotEncoder::attachedInterrupt == false)
    {
        RobotEncoder::attachedInterrupt = true;
        esp_err_t er = pcnt_isr_register(pcnt_example_intr_handler, (void *)NULL, (int)0,
                                         (pcnt_isr_handle_t *)&RobotEncoder::user_isr_handle);
        if (er != ESP_OK)
        {
            Serial.println("Encoder wrap interupt failed");
        }
    }
    pcnt_intr_enable(unit);
    pcnt_counter_resume(unit);
}

void RobotEncoder::attachHalfQuad(int aPintNumber, int bPinNumber)
{
    attach(aPintNumber, bPinNumber, true);
}
void RobotEncoder::attachSingleEdge(int aPintNumber, int bPinNumber)
{
    attach(aPintNumber, bPinNumber, false);
}

void RobotEncoder::setCount(int32_t value)
{
    count = value - getCountRaw();
}
int32_t RobotEncoder::getCountRaw()
{
    int16_t c;
    pcnt_get_counter_value(unit, &c);
    return c;
}
int32_t RobotEncoder::getCount()
{
    return getCountRaw() + count;
}

int32_t RobotEncoder::clearCount()
{
    count = 0;
    return pcnt_counter_clear(unit);
}

int32_t RobotEncoder::pauseCount()
{
    return pcnt_counter_pause(unit);
}

int32_t RobotEncoder::resumeCount()
{
    return pcnt_counter_resume(unit);
}