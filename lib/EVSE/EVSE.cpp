#include "EVSE.h"

void EVSE::setupCallbacks(){
    /**
     * @brief Set up CP state transition callbacks
     * TODO: implement callbacks & relating actions for all state transitions
     */

    cp.callbacks.A_B = [this]()
    {
        Serial.println("EV connected!");
        status.isEVConnected = 1;
    };

    cp.callbacks.B_A = [this]()
    {
        Serial.println("EV disconnected!");
        status.isEVConnected = 0;
    };

    cp.callbacks.B_C = [this]()
    {
        Serial.println("EV requested charging.");
        if (!this->charging_allowed)
        {
            Serial.println("Not turning contactor on. Chaging is not enabled");
            return;
        }
        Serial.println("Switching contactor ON");
        this->cont.On();
        status.isCharging = 1;
    };

    cp.callbacks.B_D = [this]()
    {
        Serial.println("**Ventillation required**");
        (cp.callbacks.B_C)(); // Use the same callback as B=>C
    };

    cp.callbacks.C_B = [this]()
    {
        Serial.println("EV stopped charging.");
        Serial.println("Contactor OFF");
        this->cont.Off();
        status.isCharging = 0;
    };

    cp.callbacks.D_B = [this]()
    {
        Serial.println("**Ventillation required**");
        (cp.callbacks.C_B)(); // Use the same callback as C=>B
    };

    cp.callbacks.A_E = [this]()
    {
        Serial.println("**Error: short circuit detected.");
        status.error = 1;
    };

    cp.callbacks.B_E = [this]()
    {
        Serial.println("**Error: short circuit detected.");
        status.error = 1;
    };

    cp.callbacks.C_E = [this]()
    {
        Serial.println("**Error: short circuit detected.");
        status.error = 1;
    };

    cp.callbacks.D_E = [this]()
    {
        Serial.println("**Error: short circuit detected.");
        status.error = 1;
    };

    cp.callbacks.E_A = [this]()
    {
        Serial.println("Recovered from short circuit error.");
        status.error = 0;
    };

    cp.callbacks.E_B = [this]()
    {
        Serial.println("Recovered from short circuit error.");
        status.error = 0;
    };

    cp.callbacks.E_C = [this]()
    {
        Serial.println("Recovered from short circuit error.");
        status.error = 0;
    };

    cp.callbacks.E_D = [this]()
    {
        Serial.println("Recovered from short circuit error.");
        status.error = 0;
    };
}