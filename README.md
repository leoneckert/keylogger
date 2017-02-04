# Mac OS X Keylogger

This repository holds the code for a simple and easy to use keylogger for Mac OS X. It is not meant to be malicious, and is written as a proof of concept. There is not a lot of information on keyloggers or implementing them on Mac OS X, and most of the ones I've seen do not work as indicated. This project aims to be a simple implementation on how it can be accomplished on OS X.


## Usage

Start by cloning the repository and running the proper make commands, shown below. By default, the application installs to `/usr/local/bin/keylogger`, which can easily be changed in the [`Makefile`](https://github.com/leoneckert/keylogger/blob/master/Makefile). `make install` may require root access.

```bash
$ git clone https://github.com/leoneckert/keylogger && cd keylogger
$ make && make install
```

The application by default logs to `/var/log/keystroke.log`, which may require root access depending on your system's permissions. You can change this in [`keylogger.h`](https://github.com/leoneckert/keylogger/blob/master/keylogger.h#L12) if necessary.

```bash
$ keylogger
Logging to: /var/log/keystroke.log
```

If you'd like the application to run on startup, run the `startup` make target:

```bash
$ sudo make startup
```

## Uninstallation

You can completely remove the application from your system (including the startup daemon) by running the following command (logs will not be deleted):

```bash
$ sudo make uninstall
```

### Optional Parameters

You can pass in two optional parameters to the program. The `clear` option will clear the logs at the default location. Any other argument passed in will be used as the path to the log file for that process. See below:

```bash
# Clear the logfile.
$ keylogger clear
Logfile cleared.

# Specify a logfile location.
$ keylogger ~/logfile.txt
Logging to: /Users/Casey/logfile.txt
```

## Timestamps

Output is formatted as [seconds],[nanoseconds],[key] i.e. "1445569623,952880000,[cmd]"

## Modification for Macbook Keyboard and fix repetition

Two changes have been made here. One is, I commented out specific keys because they are not on common keyboards. Generally the logger should log out for as many as possible keys to adapt to different keyboards. For my project however I am ultimately turning the data into a graphic and the empty slots of keys that are never pressed are not needed in there. 
Secondly, some keys were always written twice to the log file. 
The following keys:

keyCode 54: Key: "[cmd]"
keyCode 55: Key: "[cmd]";
keyCode 56: Key: "[shift]"
keyCode 60: Key: "[shift]"
keyCode 58: Key: "[alt]"
keyCode 61: Key: "[alt]"
keyCode 59: Key: "[ctrl]"




to change that I added some variables. I added a boolean printIt and called the convertKeyCode function outside of the actual print method. Like this:
```bash
const char *convertedKey = convertKeyCode(keyCode);
if(printIt){
    fprintf(logfile, "%lu,%lu,%s\n",ts.tv_sec,ts.tv_nsec,convertedKey);
    fflush(logfile);
} 
```
`printIt` is true by default, so normally things get printed / logged. However whenever a "critical key" (thos that were printed twice) is pressed the following happens:
```bash
case 61: 
    critKey = true; //all these lines are to prevent this key from being printed twice
    lastKeyCode = (int) keyCode; 
    return "[alt]"; 
```
we set `critKey` to true, signifying "watch out, the last key was one of those that for some reason triggers the keystroke event twice" and then we know that the next time, the keystroke is both Triggered AND the key pressed is the same as that critical keystroke, we dont want to print it. The problem was for it to be triggered once on keydown and once on keyup. `shift + K` for example would print [shift], k, [shift]. with my solution it just prints [shift], k.

Ill put some more thought into this explanation soon, I am aware its not quite clear and a bit messy. For me, fixing it was important because the project I am working on requires it.



## Contributing

Feel free to fork the project and submit a pull request with your changes!