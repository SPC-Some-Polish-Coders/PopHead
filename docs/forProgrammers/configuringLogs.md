# Logs configuration
Logs module is now configured by XML file 'config/logsConfig.ini'. The simplest, default configuration looks like this:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<handlers>
    <handler type="fileHandler">
        <modules all="">
        </modules>
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

Every handler has its own filter to know which logs save and which not. The default filter doesn't allow any logs. To enable some of them (or even all), you need to enable their modules and levels.
##### Note: For now you can filter logs only by their module in which they were reported and their log level. There is also an idea to enable filterring by file's name, but it's not necessary now.

'modules' and 'levels' tags are for enabling logs in current handler. You can enable all modules or all levels at once by simple adding all="" attribute. To enable only some modules or levels you need to use 'module' and 'level' tags. Example:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<handlers>
    <handler type="consoleHandler">
        <modules>
            <module>Physics</module>
            <module>Scenes</module>
            <module>Renderer</module>
        </modules>
        <levels>
            <level>Warning</level>
            <level>Error</level>
            <level>Critical</level>
        </levels>
    </handler>
</handlers>
```
Possible levels to allow:
- Info
- Warning
- Error
- Critical

See [logging tutorial](loggingTutorial.md)

The names of modules are created by batch script in config/modules.txt file.

Example of using multiple handlers:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<handlers>
    <handler type="fileHandler">
        <modules all="">
        </modules>
        <levels all="">
        </levels>
    </handler>
    <handler type="consoleHandler">
        <modules>
            <module>Physics</module>
            <module>Scenes</module>
            <module>Renderer</module>
        </modules>
        <levels>
            <level>Warning</level>
            <level>Error</level>
            <level>Critical</level>
        </levels>
    </handler>
</handlers>
```
