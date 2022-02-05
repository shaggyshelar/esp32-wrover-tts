| Supported Targets | ESP32-Wroover-8 MB Flash & 8MB PSRAM |
| ----------------- | ----- |
# ESP32 Offline Text To Speech Example

The interactive shell implemented in this example provides command to dynamically enter text which you want to convert to speech.

## How to use example

### Hardware Required

* A development board with ESP32 SoC (e.g., ESP32-DevKitC, ESP-WROVER-KIT, etc.) with 8MB flash and 8MB PSRAM
* Adafruit I2S 3W Class D Amplifier Breakout - MAX98357A
* A USB cable for power supply and programming

The following table describes the pins we use by default


| pin name| function | gpio_num | MAX98357 pin |
|:---:|:---:|:---:|:---:|
| WS  |word select| GPIO_NUM_5 | LRC |
| SCK |continuous serial clock| GPIO_NUM_4 | BCLK |
| DO |data out| GPIO_NUM_18 | DIN |

GAIN, SD pins of MAX98357 are not connected

### Configure the project

```
idf.py menuconfig
```

* `Serial flasher config > Flash size > 8 MB` as necessary
* `Partition Table > Partition Table > Custom partition table CSV` as necessary

### Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py build
idf.py -p PORT flash monitor
```

(Replace PORT with the name of the serial port to use.)

(To exit the serial monitor, type ``Ctrl-]``.)

See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.

## Example Output

Enter the `help` command get a full list of all available commands. The following is a sample session of the Console Example where a variety of commands provided by the Console Example are used. Note that GPIO15 is connected to GND to remove the boot log output. 

```
Type 'help' to get the list of commands.
Use UP/DOWN arrows to navigate through command history.
Press TAB when typing command name to auto-complete.
[esp32]> help
free
  Get the current size of free heap memory

heap
  Get minimum size of free heap memory that was available during program execu
  tion

version
  Get version of chip and SDK

restart
  Software reset of the chip

tasks
  Get information about running tasks

deep_sleep  [-t <t>] [--io=<n>] [--io_level=<0|1>]
  Enter deep sleep mode. Two wakeup modes are supported: timer and GPIO. If no
  wakeup option is specified, will sleep indefinitely.
  -t, --time=<t>  Wake up time, ms
      --io=<n>  If specified, wakeup using GPIO with given number
  --io_level=<0|1>  GPIO level to trigger wakeup

light_sleep  [-t <t>] [--io=<n>]... [--io_level=<0|1>]...
  Enter light sleep mode. Two wakeup modes are supported: timer and GPIO. Mult
  iple GPIO pins can be specified using pairs of 'io' and 'io_level' arguments
  . Will also wake up on UART input.
  -t, --time=<t>  Wake up time, ms
      --io=<n>  If specified, wakeup using GPIO with given number
  --io_level=<0|1>  GPIO level to trigger wakeup

tts  <tts_text>
  Convert text to speech
    <tts_text>  Text to covert to speech

help
  Print the list of registered commands


[esp32]> free
257200
[esp32]> tts "This is text to speech example"
[esp32]> restart
I (205639) restart: Restarting
I (616) heap_init: Initializing. RAM available for dynamic allocation:
I (617) heap_init: At 3FFAE2A0 len 00001D60 (7 KiB): DRAM
I (623) heap_init: At 3FFB7EA0 len 00028160 (160 KiB): DRAM
I (642) heap_init: At 3FFE0440 len 00003BC0 (14 KiB): D/IRAM
I (661) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (681) heap_init: At 40093EA8 len 0000C158 (48 KiB): IRAM

This is an example of ESP-IDF console component.
Type 'help' to get the list of commands.
Use UP/DOWN arrows to navigate through command history.
Press TAB when typing command name to auto-complete.
[esp32]> 

```

## Troubleshooting

### Line Endings

The line endings in the Console Example are configured to match particular serial monitors. Therefore, if the following log output appears, consider using a different serial monitor (e.g. Putty for Windows) or modify the example's [UART configuration](#Configuring-UART-and-VFS).

```
This is an example of ESP-IDF console component.
Type 'help' to get the list of commands.
Use UP/DOWN arrows to navigate through command history.
Press TAB when typing command name to auto-complete.
Your terminal application does not support escape sequences.
Line editing and history features are disabled.
On Windows, try using Putty instead.
esp32>
```
