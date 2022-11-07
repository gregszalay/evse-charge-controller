#include "EVSE.h"

ControlPilotSettings_t evse_cp_settings{
    // TODO: put in global macros, 1 ESP can only handle 1 EVSE anyway
    .PWM_FREQ = 1000,
    .PWM_DEFAULT_DUTY_CYCLE = 100,
    .PWM_RESOLUTION = 8,
    .PWM_CHANNEL = 0};

EVSE::EVSE(int id) : cp(evse_cp_settings), pp(), cont(5)
{
    // Make sure the contactor is in an off state to start with
    cont.Off();

    /**
     * @brief Set up CP state transition callbacks
     * TODO: implement callbacks & relating actions for all state transitions
     */

    this->cp.A_B = [this]()
    {
        Serial.println("EV connected!");
    };

    this->cp.B_A = [this]()
    {
        Serial.println("EV disconnected!");
    };

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

    this->cp.C_B = [this]()
    { Serial.println("Contactor OFF"); this->cont.Off(); };
}