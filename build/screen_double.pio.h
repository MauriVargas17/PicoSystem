// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------------- //
// screen_double //
// ------------- //

#define screen_double_wrap_target 0
#define screen_double_wrap 18

static const uint16_t screen_double_program_instructions[] = {
            //     .wrap_target
    0x80a0, //  0: pull   block                      
    0x6030, //  1: out    x, 16                      
    0x6050, //  2: out    y, 16                      
    0xa0e1, //  3: mov    osr, x                     
    0x6074, //  4: out    null, 20                   
    0x7001, //  5: out    pins, 1         side 0     
    0x18e5, //  6: jmp    !osre, 5        side 1     
    0xa0e1, //  7: mov    osr, x                     
    0x6074, //  8: out    null, 20                   
    0x7001, //  9: out    pins, 1         side 0     
    0x18e9, // 10: jmp    !osre, 9        side 1     
    0xa0e2, // 11: mov    osr, y                     
    0x6074, // 12: out    null, 20                   
    0x7001, // 13: out    pins, 1         side 0     
    0x18ed, // 14: jmp    !osre, 13       side 1     
    0xa0e2, // 15: mov    osr, y                     
    0x6074, // 16: out    null, 20                   
    0x7001, // 17: out    pins, 1         side 0     
    0x18f1, // 18: jmp    !osre, 17       side 1     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program screen_double_program = {
    .instructions = screen_double_program_instructions,
    .length = 19,
    .origin = -1,
};

static inline pio_sm_config screen_double_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + screen_double_wrap_target, offset + screen_double_wrap);
    sm_config_set_sideset(&c, 2, true, false);
    return c;
}
#endif

