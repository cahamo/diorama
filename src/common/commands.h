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

#define CMDPARAM_LIGHTING_ALL_NORMAL      0x01
#define CMDPARAM_LIGHTING_ALL_FLICKER     0x02
#define CMDPARAM_LIGHTING_ALL_EVERYTHING  (CMDPARAM_LIGHTING_ALL_NORMAL | CMDPARAM_LIGHTING_ALL_FLICKER)

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
#define LIGHTING_OFFICE_SECTION_ID      2
#define LIGHTING_WORKSHOP_SECTION_ID    3
#define LIGHTING_LEANTO_SECTION_ID      4

#define LIGHTING_FLICKER_LIGHT_ID       0x0F

#define LIGHTING_INVALID_LIGHT_ID       0xFF

// Number of valid IDs for certain command types
#define LIGHTING_SECTION_COUNT  5
#define FEATURE_COUNT           3       // TODO: correct number required - arbitrary value for testing
#define PROGRAM_COUNT           1       // TODO: correct number required - arbitrary value for testing

// Number of normal lights in each lighting section
#define LIGHTING_STORE_NORMAL_LIGHT_COUNT       2
#define LIGHTING_FORGE_NORMAL_LIGHT_COUNT       4
#define LIGHTING_OFFICE_NORMAL_LIGHT_COUNT      1
#define LIGHTING_WORKSHOP_NORMAL_LIGHT_COUNT    2
#define LIGHTING_LEANTO_NORMAL_LIGHT_COUNT      1

// Map of lighting sections to number of normal lights in section
byte lightingNormalLightsInSection[LIGHTING_SECTION_COUNT] = {
    LIGHTING_STORE_NORMAL_LIGHT_COUNT,
    LIGHTING_FORGE_NORMAL_LIGHT_COUNT,
    LIGHTING_OFFICE_NORMAL_LIGHT_COUNT,
    LIGHTING_WORKSHOP_NORMAL_LIGHT_COUNT,
    LIGHTING_LEANTO_NORMAL_LIGHT_COUNT
};

// Flags indicating whether section has a flicker light
// -- there can only be zero or one flicker light per section
#define LIGHTING_STORE_HAS_FLICKER_LIGHT        true
#define LIGHTING_FORGE_HAS_FLICKER_LIGHT        true
#define LIGHTING_OFFICE_HAS_FLICKER_LIGHT       true
#define LIGHTING_WORKSHOP_HAS_FLICKER_LIGHT     true
#define LIGHTING_LEANTO_HAS_FLICKER_LIGHT       true

// Map of showing which sections have flicker lights
bool lightingHasFlickerLightInSection[LIGHTING_SECTION_COUNT] = {
    LIGHTING_STORE_HAS_FLICKER_LIGHT,
    LIGHTING_FORGE_HAS_FLICKER_LIGHT,
    LIGHTING_OFFICE_HAS_FLICKER_LIGHT,
    LIGHTING_WORKSHOP_HAS_FLICKER_LIGHT,
    LIGHTING_LEANTO_HAS_FLICKER_LIGHT
};

#endif
