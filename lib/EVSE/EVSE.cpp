#include "EVSE.h"

ControlPilotSettings_t evse_cp_settings{
    .PWM_FREQ = 1000,
    .PWM_DEFAULT_DUTY_CYCLE = 100,
    .PWM_RESOLUTION = 8,
    .PWM_CHANNEL = 0,
    .PWM_OUT_PIN = 16,
    .PWM_OUT_PIN_2 = 17,
    .CP_READ_EXT_TRIG_PIN = 34,
    .CP_POS_ADC_PIN = 39,
    .CP_NEG_ADC_PIN = 33,
};

ProximityPilotSettings_t evse_pp_settings{
    .PP_PIN = 32,
};

EVSE::EVSE(int id) : cp(evse_cp_settings), pp(), cont(5)
{
    // Make sure the contactor is in an off state to start with
    cont.Off();

    /**
     * @brief Set up CP state transition callbacks
     *
     */

    // Called on B -> C state transition:
    this->cp.B_C = [this]()
    {
        if (!this->charging_allowed)
        {
            Serial.println("Not turning contactor on. Chaging is not enabled");
            return;
        }
        Serial.println("Switching contactor ON");
        this->cont.On();
    };
    // Called on C -> B state transition:
    this->cp.C_B = [this]()
    { Serial.println("Contactor OFF"); this->cont.Off(); };
}