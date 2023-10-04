# Infrared Command Reference

The IR controller interprets commands issued via the IR handset and converts them data that is transmitted to the main diorama microcontroller over I²C.

There are five types of IR command:

1. Model Lighting Commands
2. Feature Commands
3. Ambient Lighting Commands
4. Program Commands
5. Reset Command

There are sub-commands and additional daa that is associated with most of the main command types.

## Handset Usage

The handset key presses for the commands are as follows:

| Command Type | Command | 1st key | 2nd key |
|:-------------|:--------|:--------|:--------|
| 1 | Toggle given light in given section on & off | `1`...`9` (Section code) | `1`...`9` (Normal light code) _or_ `#` (Flicker light code) |
| 1 | Toggle all lights in given section on & off| `1`...`9` (Section code) | `0` |
| 1 | Toggle all lights on and off | `0` | `OK` or `0` |
| 1 | Toggle all normal lights on and off | `0` | `*` |
| 1 | Toggle all flicker lights on and off | `0` | `#` |
| 2 | Toggle a given dual state feature or activate a given single state feature | `*` | `1`...`9` (Feature code) |
| 2 | Reset all dual state features | `*` | `0` |
| 3 | Switch ambient lighting on to full brightness | `RIGHT-ARROW` | N/a |
| 3 | Switch ambient lighting off | `LEFT-ARROW` | N/a |
| 3 | Brighten ambient lighting a little | `UP-ARROW` | N/a |
| 3 | Dim ambient lighting a little | `DOWN-ARROW` | N/a |
| 4 | Run given program | `#` | `1`...`9` (Program code) |
| 4 | Stop current program | `#` | `0` |
| 5 | Master reset | `OK` | N/a |

## Command Data

Each command is transmitted from the IR controller over I²C as a pair of bytes. The first byte stores the command ID in its high nybble and the sub-command in its low nybble. The second byte contains data, the format of which is command dependent.

The following table lists the commands and the format of the two data bytes that are transmitted over I²C:

| Command | Main Command ID | Sub Command ID | Parameter |
|:--------|:----------------|:---------------|:----------|
| Toggle given light in given section on & off | 1 | 1 | High nybble: 0..8, 0xE, 0xF (Light ID), Low nybble: 0..8 (Section ID) |
| Toggle all lights in given section on & off | 1 | 2 | 0..8 (Section ID) |
| Toggle all normal lights on and off | 1 | 3 | 1 |
| Toggle all flicker lights on and off | 1 | 3 | 2 |
| Toggle all lights on and off | 1 | 3 | 3 |
| Toggle a given dual state feature or activate a given single state feature | 2 | 1 | 0..8 (Feature ID) |
| Reset all dual state features | 2 | 2 | _N/a_ |
| Switch ambient lighting on to full brightness | 3 | 1 | 0xFF |
| Switch ambient lighting off | 3 | 1 | 0 |
| Brighten ambient lighting a little | 3 | 2 | 1 |
| Dim ambient lighting a little | 3 | 2 | 0xFF |
| Run given program | 4 | 1 | 0..8 (Program ID) |
| Stop current program | 4 | 2 | _N/a_ |
| Master reset | 5 | _N/a_ | _N/a_ |

Notes:

1. Section, normal light, feature and program codes that use handset keys `1` to `9` map onto an ID that is one less than the numeral on the handset key, e.g key `4` represents command ID 3. The and `#` flicker light key represents command code 0xF.
2. The parameter of the command to turn all lights off is calculated by ORing together the parameters of the commands to turn off normal lights and to turn off flicker lights. I.e. 0b01 | 0b10 = 0b11 (1 | 2 = 3).
3. Where _N/a_ appears in the table it means that the value is ignored and can be anything. Zero is usually used, but don't assume that it is.
