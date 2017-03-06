# Mac OS X Keylogger

This keylogger is a research tool for my thesis about keystroke biometrics. It's not meant to be malicious. It logs keyUp, keyDown and flagCHANGE events paired with a second (since 1970) and a nanosecond timestamp - I don't know why I didnt combine the two (simply concatenatinf the to gives you nanoseconds since 1970), but you can easily do that when processing the data.

Output looks like this:
```bash
1488838235,789626000,25,keyUP
1488838235,957501000,49,keyDOWN
1488838236,53511000,49,keyUP
1488838236,301612000,55,flagCHANGE
1488838236,397406000,9,keyDOWN
``` 


## Usage

Start by cloning the repository and running the proper make commands, shown below. By default, the application installs to `/usr/local/bin/keylogger`, which can easily be changed in the [`Makefile`](https://github.com/leoneckert/keylogger/blob/master/Makefile). `make install` may require root access.

```bash
$ git clone https://github.com/leoneckert/keylogger && cd keylogger
$ make && make install
```

The keylogger should now be running, check with:

```bash
$ tail -f /var/log/keystroke.log
```

The application by default logs to `/var/log/keystroke.log`, which may require root access depending on your system's permissions. You can change this in [`keylogger.h`](https://github.com/leoneckert/keylogger/blob/master/keylogger.h#L12) if necessary.

If it is not running yet try to simply type

```bash
$ keylogger
```
or
```bash
$ keylogger &
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

Additonal, I'd run:

```bash
$ ps aux | grep keylogger
```
which should give you the process id, and then run
```bash
$ sudo kill -9 [process id]
```
to kill the process. 
To Check if the logger is still running or actually quit, run:
```bash
$ tail -f /var/log/keystroke.log
```
and see if it's still logging.

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

## Contributing

Feel free to fork the project and submit a pull request with your changes!