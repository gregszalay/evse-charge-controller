#include "ControlPilot.h"

float CPClassifier::_13V = 3.2;
float CPClassifier::_11V = 2.75;
float CPClassifier::_10V = 2.4;
float CPClassifier::_8V = 1.9;
float CPClassifier::_7V = 1.7;
float CPClassifier::_5V = 1.2;
float CPClassifier::_4V = 0.955;
float CPClassifier::_2V = 0.467;
float CPClassifier::_1V = 0.227;
float CPClassifier::_neg_1V = 0.0;
float CPClassifier::_neg_11V = 2.7;
float CPClassifier::_neg_13V = 2.9;

bool CPClassifier::_12V(float cp_voltage)
{
    return cp_voltage >= _8V && cp_voltage <= _10V;
};
bool CPClassifier::_9V(float cp_voltage)
{
    return cp_voltage >= _8V && cp_voltage <= _10V;
}
bool CPClassifier::_6V(float cp_voltage)
{
    return cp_voltage >= _5V && cp_voltage <= _7V;
}
bool CPClassifier::_3V(float cp_voltage)
{
    return cp_voltage >= _2V && cp_voltage <= _4V;
}
bool CPClassifier::_0V(float cp_voltage)
{
    return cp_voltage >= _neg_1V && cp_voltage <= _1V;
}
bool CPClassifier::_neg_12V(float cp_voltage)
{
    return cp_voltage >= _neg_11V && cp_voltage <= _neg_13V;
}