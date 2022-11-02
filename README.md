## Circuit

https://tinyurl.com/2okg2vls
https://tinyurl.com/22xwktd6
https://tinyurl.com/29v7sfvd
https://tinyurl.com/259mykdw

https://tinyurl.com/2dm7u7qt
https://tinyurl.com/24lqx3d3

https://tinyurl.com/233tq6cn

## GPIO INPUT/OUTPUT

Below is a representation of a physical ESP module in a top-down view.

> You can use the "My Macro" column to note down what macro you use for that pin in your app
> I've added some examples for the macros

| Notes           | My Macro | GPIO      | #   | XXX | #   | GPIO      | My Macro      | Notes              |
| --------------- | -------- | --------- | --- | --- | --- | --------- | ------------- | ------------------ |
| **3V3**         | X        | **_3V3_** | 19  | XXX | 19  | **_GND_** | X             | **GND**            |
| **RESET**       | X        | **_EN_**  | 18  | XXX | 18  | **_23_**  |               |                    |
| **INPUT_ONLY**  |          | **_36_**  | 17  | XXX | 17  | **_22_**  |               |                    |
| **INPUT_ONLY**  | ADC_PIN  | **_39_**  | 16  | XXX | 16  | **_1_**   |               | **USB_PROG_DEBUG** |
| **INPUT_ONLY**  | TRIG_PIN | **_34_**  | 15  | XXX | 15  | **_3_**   |               | **USB_PROG_DEBUG** |
| **INPUT_ONLY**  |          | **_35_**  | 14  | XXX | 14  | **_21_**  |               |                    |
|                 |          | **_32_**  | 13  | XXX | 13  | **_GND_** | X             | **GND**            |
|                 |          | **_33_**  | 12  | XXX | 12  | **_19_**  |               |                    |
|                 |          | **_25_**  | 11  | XXX | 11  | **_18_**  |               |                    |
|                 |          | **_26_**  | 10  | XXX | 10  | **_5_**   | CONTACTOR_PIN | **PWM_AT_BOOT**    |
|                 |          | **_27_**  | 9   | XXX | 9   | **_17_**  | PWM_OUT_PIN_2 |                    |
| **PWM_AT_BOOT** |          | **_14_**  | 8   | XXX | 8   | **_16_**  | PWM_OUT_PIN   |                    |
|                 |          | **_12_**  | 7   | XXX | 7   | **_4_**   |               |                    |
| **GND**         | X        | **_GND_** | 6   | XXX | 6   | **_0_**   | !             | **BOOT_H_PROG_L**  |
|                 |          | **_13_**  | 5   | XXX | 5   | **_2_**   |               |                    |
| **FORBIDDEN**   |          | **_9_**   | 4   | XXX | 4   | **_15_**  |               | **PWM_AT_BOOT**    |
| **FORBIDDEN**   |          | **_10_**  | 3   | XXX | 3   | **_8_**   |               | **FORBIDDEN**      |
| **FORBIDDEN**   |          | **_11_**  | 2   | XXX | 2   | **_7_**   |               | **FORBIDDEN**      |
|                 |          | **_5V_**  | 1   | XXX | 1   | **_6_**   |               | **FORBIDDEN**      |

## GPIO notes

| MACRO         | USE                                                                                                    |
| ------------- | ------------------------------------------------------------------------------------------------------ |
| ADC_PIN       | Used to measure CP voltage. Connected to CP (via voltage divider and voltage follower).                |
| TRIG_PIN      | Used to trigger the ADC of the ADC_PIN. Triggers the CP_ADC_ISR interrupt. Connected to PWM_OUT_PIN_2. |
| PWM_OUT_PIN   | Source of the CP PWM. 1kHz, variable duty cycle.                                                       |
| PWM_OUT_PIN_2 | Source of the CP PWM. Identical to the PWM_OUT_PIN signal. Used to trigger the ADC.                    |
| CONTACTOR_PIN | Starts/stops the charging.                                                                             |
