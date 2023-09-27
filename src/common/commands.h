/*
 * commands.h
 *
 * Defines command related macros.
 *
 * These macros are common to all programs in the diorama project.
 */

#ifndef COMMAND_H
#define COMMAND_H

// Commands

#define CMD_LIGHTING    1
#define CMD_FEATURE     2
#define CMD_AMBIENT     3
#define CMD_PROGRAM     4
#define CMD_RESET       5

// Sub-commands

#define SUBCMD_LIGHTING_LIGHT       1
#define SUBCMD_LIGHTING_SECTION     2
#define SUBCMD_LIGHTING_ALL         3

#define SUBCMD_FEATURE_ACTIVATE     1
#define SUBCMD_FEATURE_RESET        2

#define SUBCMD_AMBIENT_SWITCH       1
#define SUBCMD_AMBIENT_BRIGHTNESS   2

#define SUBCMD_PROGRAM_RUN          1
#define SUBCMD_PROGRAM_STOP         2

#define SUBCMD_NULL                 0

// Special parameter values

#define CMDPARAM_LIGHTING_ALL_EVERYTHING  0x01
#define CMDPARAM_LIGHTING_ALL_NORMAL      0x02
#define CMDPARAM_LIGHTING_ALL_FLICKER     0x03

#define CMDPARAM_AMBIENT_SWITCH_OFF       0x00
#define CMDPARAM_AMBIENT_SWITCH_ON        0xFF

#define CMDPARAM_AMBIENT_BRIGHTNESS_UP    0x01 // +1
#define CMDPARAM_AMBIENT_BRIGHTNESS_DOWN  0xFF // -1

#define CMDPARAM_NULL                     0x00

// Command macros

#define MakeCommand(mainCmd, subCmd) \
    ((((mainCmd) & 0x0F) << 4) | ((subCmd) & 0x0F))
#define ExtractMainCommand(cmd) \
    ((cmd) >> 4)
#define ExtractSubCommand(cmd) \
    ((cmd) & 0x0F)

#define MakeSectionLightParam(sectionId, lightId) \
    ((((lightId) & 0x0F) << 4) | ((sectionId) & 0x0F))
#define ExtractLightParam(param) \
    ((param) >> 4)
#define ExtractSectionParam(param) \
    ((param) & 0x0F)

// Parameter IDs

#define LIGHTING_STORE_SECION_ID        0
#define LIGHTING_FORGE_SECTION_ID       1
#define LIGHTING_OFFICE_SECTION_ID      2  // TBC: awaiting construction
#define LIGHTING_WORKSHOP_SECTION_ID    3  // TBC: awaiting construction
#define LIGHTING_LEANTO_SECTION_ID      4  // TBC: awaiting construction

#define LIGHTING_FLICKER_LIGHT_1_ID     0xF
#define LIGHTING_FLICKER_LIGHT_2_ID     0xE
#define LIGHTING_FLICKER_LIGHT_BASE     LIGHTING_FLICKER_LIGHT_1_ID

// Number of valid IDs for certain command types
#define LIGHTING_SECTION_COUNT  5
#define FEATURE_COUNT           3
#define PROGRAM_COUNT           1

// Number of lights in each lighting section
#define LIGHTING_STORE_NORMAL_LIGHT_COUNT       2
#define LIGHTING_STORE_FLICKER_LIGHT_COUNT      1
#define LIGHTING_FORGE_NORMAL_LIGHT_COUNT       4
#define LIGHTING_FORGE_FLICKER_LIGHT_COUNT      2 // Count TBC
#define LIGHTING_OFFICE_NORMAL_LIGHT_COUNT      1 // TBC: awaiting construction
#define LIGHTING_OFFICE_FLICKER_LIGHT_COUNT     1 // TBC: awaiting construction
#define LIGHTING_WORKSHOP_NORMAL_LIGHT_COUNT    2 // TBC: awaiting construction
#define LIGHTING_WORKSHOP_FLICKER_LIGHT_COUNT   1 // TBC: awaiting construction
#define LIGHTING_LEANTO_NORMAL_LIGHT_COUNT      1 // TBC: awaiting construction
#define LIGHTING_LEANTO_FLICKER_LIGHT_COUNT     0 // TBC: awaiting construction

// Map of lighting sections to number of normal lights in section
byte lightingNormalLightsInSection[LIGHTING_SECTION_COUNT] = {
    LIGHTING_STORE_NORMAL_LIGHT_COUNT,
    LIGHTING_FORGE_NORMAL_LIGHT_COUNT,
    LIGHTING_OFFICE_NORMAL_LIGHT_COUNT,
    LIGHTING_WORKSHOP_NORMAL_LIGHT_COUNT,
    LIGHTING_LEANTO_NORMAL_LIGHT_COUNT
};

// Map of lighting sections to number of flicker lights in section
byte lightingFlickerLightsInSection[LIGHTING_SECTION_COUNT] = {
    LIGHTING_STORE_FLICKER_LIGHT_COUNT,
    LIGHTING_FORGE_FLICKER_LIGHT_COUNT,
    LIGHTING_OFFICE_FLICKER_LIGHT_COUNT,
    LIGHTING_WORKSHOP_FLICKER_LIGHT_COUNT,
    LIGHTING_LEANTO_FLICKER_LIGHT_COUNT
};

#endif
