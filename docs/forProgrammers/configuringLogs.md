# Logs configuration
Logs module is now configured by XML file 'config/logsConfig.ini'. The simplest, default configuration looks like this:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<handlers>
    <handler type="fileHandler">
        <filepaths all="">
        </filepaths>
        <levels all="">
        </levels>
    </handler>
</handlers>
```

The main tag 'handlers' contains all of the log handlers to create in Game. Handler is a receiver of log records. We create handlers in tags 'handler' and with obligatory type attribute, which stands for type of handler.

There are three types of handlers:
- fileHandler: writes logs into file (logs/log_\*date\*_\*time\*.txt)
- consoleHandler: writes logs into console
- terminalHandler: writes logs into game's terminal

##### Note: You can create at most only one instance of each type. In the near future it will be possible to create multiple files.

Every handler has its own filter to know which logs save and which not. The default filter doesn't allow any logs. To enable some of them (or even all), you need to enable their paths and levels.

'filepaths' and 'levels' tags are for enabling logs in current handler. You can enable all files or all levels at once by simple adding all="" attribute. To enable only some files or levels you need to use 'enable'/'disable' and 'level' tags. Example:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<handlers>
    <handler type="consoleHandler">
        <filepaths>
            <enable>Physics/CollisionDebug</enable>
            <enable>Scenes</enable>
            <enable>Renderer</enable>
            <disable>camera.cpp</disable>
        </filepaths>
        <levels>
            <level>Warning</level>
            <level>Error</level>
            <level>Critical</level>
        </levels>
    </handler>
</handlers>
```
Configuration above enables logs from files in Physics/CollisionDebug/, Scenes/ and Renderer/ directories, but without camera.cpp file. Please note that directories and files are case sensitive!

Possible levels to allow:
- Info
- Warning
- Error
- Critical

See [logging tutorial](loggingTutorial.md)

Example of using multiple handlers:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<handlers>
    <handler type="fileHandler">
        <filepaths all="">
        </filepaths>
        <levels all="">
        </levels>
    </handler>
    <handler type="consoleHandler">
        <filepaths>
            <enable>Physics</enable>
            <enable>Scenes</enable>
            <enable>Renderer</enable>
        </filepaths>
        <levels>
            <level>Warning</level>
            <level>Error</level>
            <level>Critical</level>
        </levels>
    </handler>
</handlers>
```
