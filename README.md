## GPIO INPUT/OUTPUT

Below is a representation of a physical ESP module in a top-down view.

> You can use the "My Macro" column to note down what macro you use for that pin in your app
> I've added some examples for the macros

| Notes           | My Macro | GPIO                | #   | XXX | #   | GPIO                | My Macro      | Notes              |
| --------------- | -------- | ------------------- | --- | --- | --- | ------------------- | ------------- | ------------------ |
| **3V3**         | X        | \_\_ **_3V3_** \_\_ | 19  | XXX | 19  | \_\_ **_GND_** \_\_ | X             | **GND**            |
| **RESET**       | X        | \_\_ **_EN_** \_\_  | 18  | XXX | 18  | \_\_ **_23_** \_\_  |               |                    |
| **INPUT_ONLY**  | ADC_PIN  | \_\_ **_36_** \_\_  | 17  | XXX | 17  | \_\_ **_22_** \_\_  |               |                    |
| **INPUT_ONLY**  | TRIG_PIN | \_\_ **_39_** \_\_  | 16  | XXX | 16  | \_\_ **_1_** \_\_   |               | **USB_PROG_DEBUG** |
| **INPUT_ONLY**  |          | \_\_ **_34_** \_\_  | 15  | XXX | 15  | \_\_ **_3_** \_\_   |               | **USB_PROG_DEBUG** |
| **INPUT_ONLY**  |          | \_\_ **_35_** \_\_  | 14  | XXX | 14  | \_\_ **_21_** \_\_  |               |                    |
|                 |          | \_\_ **_32_** \_\_  | 13  | XXX | 13  | \_\_ **_GND_** \_\_ | X             | **GND**            |
|                 |          | \_\_ **_33_** \_\_  | 12  | XXX | 12  | \_\_ **_19_** \_\_  |               |                    |
|                 |          | \_\_ **_25_** \_\_  | 11  | XXX | 11  | \_\_ **_18_** \_\_  |               |                    |
|                 |          | \_\_ **_26_** \_\_  | 10  | XXX | 10  | \_\_ **_5_** \_\_   | CONTACTOR_PIN | **PWM_AT_BOOT**    |
|                 |          | \_\_ **_27_** \_\_  | 9   | XXX | 9   | \_\_ **_17_** \_\_  | PWM_OUT_PIN_2 |                    |
| **PWM_AT_BOOT** |          | \_\_ **_14_** \_\_  | 8   | XXX | 8   | \_\_ **_16_** \_\_  | PWM_OUT_PIN   |                    |
|                 |          | \_\_ **_12_** \_\_  | 7   | XXX | 7   | \_\_ **_4_** \_\_   |               |                    |
| **GND**         | X        | \_\_ **_GND_** \_\_ | 6   | XXX | 6   | \_\_ **_0_** \_\_   | !             | **BOOT_H_PROG_L**  |
|                 |          | \_\_ **_13_** \_\_  | 5   | XXX | 5   | \_\_ **_2_** \_\_   |               |                    |
| **FORBIDDEN**   |          | \_\_ **_9_** \_\_   | 4   | XXX | 4   | \_\_ **_15_** \_\_  |               | **PWM_AT_BOOT**    |
| **FORBIDDEN**   |          | \_\_ **_10_** \_\_  | 3   | XXX | 3   | \_\_ **_8_** \_\_   |               | **FORBIDDEN**      |
| **FORBIDDEN**   |          | \_\_ **_11_** \_\_  | 2   | XXX | 2   | \_\_ **_7_** \_\_   |               | **FORBIDDEN**      |
|                 |          | \_\_ **_5V_** \_\_  | 1   | XXX | 1   | \_\_ **_6_** \_\_   |               | **FORBIDDEN**      |

## GPIO notes

| MACRO         | USE                                                                                                    |
| ------------- | ------------------------------------------------------------------------------------------------------ |
| ADC_PIN       | Used to measure CP voltage. Connected to CP (via voltage divider and voltage follower).                |
| TRIG_PIN      | Used to trigger the ADC of the ADC_PIN. Triggers the CP_ADC_ISR interrupt. Connected to PWM_OUT_PIN_2. |
| PWM_OUT_PIN   | Source of the CP PWM. 1kHz, variable duty cycle.                                                       |
| PWM_OUT_PIN_2 | Source of the CP PWM. Identical to the PWM_OUT_PIN signal. Used to trigger the ADC.                    |
| CONTACTOR_PIN | Starts/stops the charging.                                                                             |
