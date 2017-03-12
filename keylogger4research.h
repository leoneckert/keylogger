//  _              _
// | | _____ _   _| | ___   __ _  __ _  ___ _ __
// | |/ / _ \ | | | |/ _ \ / _` |/ _` |/ _ \ '__|
// |   <  __/ |_| | | (_) | (_| | (_| |  __/ |
// |_|\_\___|\__, |_|\___/ \__, |\__, |\___|_|
//           |___/         |___/ |___/
//
//  _  _                                       _
// | || |    _ __ ___  ___  ___  __ _ _ __ ___| |__
// | || |_  | '__/ _ \/ __|/ _ \/ _` | '__/ __| '_ \
// |__   _| | | |  __/\__ \  __/ (_| | | | (__| | | |
//    |_|   |_|  \___||___/\___|\__,_|_|  \___|_| |_|
//
//https://github.com/caseyscarborough/keylogger
#ifndef __KEYLOGGER_H__
#define __KEYLOGGER_H__

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
//
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
//
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif


// https://developer.apple.com/library/mac/documentation/Carbon/Reference/QuartzEventServicesRef/Reference/reference.html

CGEventRef CGEventCallback(CGEventTapProxy, CGEventType, CGEventRef, void*);
const char *convertKeyCode(int);

//void print_current_time_with_ms();

#endif
