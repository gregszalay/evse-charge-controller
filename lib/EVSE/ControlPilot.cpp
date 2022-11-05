#include "ControlPilot.h"

float CPClassifier::_12V_TOP = 0.0;
float CPClassifier::_12V_BOTTOM = 0.0;
float CPClassifier::_9V_TOP = 3.3;
float CPClassifier::_9V_BOTTOM = 3.0;
float CPClassifier::_6V_TOP = 2.9;
float CPClassifier::_6V_BOTTOM = 1.0;
float CPClassifier::_3V_TOP = 0.0;
float CPClassifier::_3V_BOTTOM = 0.0;
float CPClassifier::_0V_TOP = 0.0;
float CPClassifier::_0V_BOTTOM = 0.0;
float CPClassifier::_neg_12V_TOP = 0.0;
float CPClassifier::_neg_12V_BOTTOM = 0.0;

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