#include "ControlPilot.h"

volatile uint64_t ControlPilot::iter = 0;
volatile uint16_t ControlPilot::cp_pos_raw = 0;
volatile uint16_t ControlPilot::cp_neg_raw = 0;

bool CPClassifier::_12V(float cp_voltage)
{
    return cp_voltage > _9V_BOTTOM && cp_voltage < _9V_TOP;
};
bool CPClassifier::_9V(float cp_voltage)
{
    return cp_voltage > _9V_BOTTOM && cp_voltage < _9V_TOP;
}
bool CPClassifier::_6V(float cp_voltage)
{
    return cp_voltage > _6V_BOTTOM && cp_voltage < _6V_TOP;
}
bool CPClassifier::_3V(float cp_voltage)
{
    return cp_voltage > _3V_BOTTOM && cp_voltage < _3V_TOP;
}
bool CPClassifier::_0V(float cp_voltage)
{
    return cp_voltage > _0V_BOTTOM && cp_voltage < _0V_TOP;
}
bool CPClassifier::_neg_12V(float cp_voltage)
{
    return cp_voltage > _neg_12V_BOTTOM && cp_voltage < _neg_12V_TOP;
}