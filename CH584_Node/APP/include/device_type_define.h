//
// Created by xtx on 25-3-22.
//

#ifndef DEVICE_TYPE_DEFINE_H
#define DEVICE_TYPE_DEFINE_H

/* Check if a valid device type is defined during compilation */
#if defined(DEVICE_TYPE_CENTER)
#define CURRENT_DEVICE_TYPE 0x01
#error "No CH592X is Center Node"
#elif defined(DEVICE_TYPE_USER)
#define CURRENT_DEVICE_TYPE 0x02
#elif defined(DEVICE_TYPE_A42)
#define CURRENT_DEVICE_TYPE 0x10
#elif defined(DEVICE_TYPE_A21)
#define CURRENT_DEVICE_TYPE 0x11
#elif defined(DEVICE_TYPE_B53)
#define CURRENT_DEVICE_TYPE 0x20
#else
#error "No valid device type defined. Use CMake with -DDEVICE_TYPE_CENTER, -DDEVICE_TYPE_USER, -DDEVICE_TYPE_A43, -DDEVICE_TYPE_A22, or -DDEVICE_TYPE_B55"
#endif

#endif //DEVICE_TYPE_DEFINE_H
