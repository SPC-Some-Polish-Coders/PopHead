# PopHead terminal handling
-------------------------

## Categories
<details>
<summary>"Click to expand"</summary>
  
- [General usage in game](#general-usage-in-game)
- [Navigation keys in terminal](#navigation-keys-in-the-terminal)
- Commands
	- [Game commands](#game-commands)
	- [Logging commands](#logging-commands)
	- [Audio commands](#audio-commands)
	- [Collision Debug commands](#collision-debug-commands)
	- [Player location commands](#player-location-commands)
	- [Camera commands](#camera-commands)

</details>

## General usage in game
If you'd like to get access to the terminal, during your play, **press "Tab"** key. The console window will appear.
Then you can enter one of the commands listed below. These can be either many-argumented or with no arguments. 

#### General way of entering for **many-argumented** commands is following:
#### `<command> <argument> <possible argument> ...`
#### and for **no-argument** commands it's simply:
#### `<command>`


Confirm it by **pressing "Enter"**. You can keep entering commands or if you'd like to leave **press "Tab"** once again.
In fact, order of arguments is not important at all. It's crucial to include "basic" command name in the beggining and arguments might be mixed.

```
log into terminal not
log not into terminal
log terminal not into
```
Above examples **mean the same**!

## Navigation keys in the terminal
- Using **"Up"** arrow you recover last entered command
- Using **"Down"** arrow you delete so far entered characters

## Game commands
-------------------------
| **Command** | **Describe** |
| ----------: | ----------- |
| **`exit`**  | is **no-argument** command which closes the program |
| **`echo`** | is **single-argumented** command which prints out the message given by the user |
| **`help`** | is **single-argumented** command which prints out availible commands. Needs to be combined with a number of a page |
| **`history`** | is **no-argument** command which prints out ten last used commands |
| **`clear`** | is **no-argument** command which clears the output area in terminal |

#### Examples: 
```
echo hello world
exit
help 2
history
clear
```
## Logging commands
-------------------------
### **`log`** is **many-argumented** command which is generally associated with way of logging into console or file:

 ### **`into`** argument makes logs appear only in specified place
 
| **into arguments** | **Describe** |
| -----------: | ------------ |
| `file` | argument makes logs appear in file |
| `console` | argument makes logs appear in console	|
| `terminal` | argument makes logs appear in terminal	|
| `all` | argument makes logs appear in both places |
| `not` | argument needs to be combined with one of above. It makes whole command opposite - logs won't appear in specified place |

#### Examples: 
```
log into file
log into both not
log into console
```
### **`types`** argument makes logs appear only from certain types

| **types arguments** | **Describe** |
| -----------: | ------------ |
| `all` | argument sets logging from all types |
| `clear` | argument clears logging types - won't log from any of them |
| `<log type>` | argument sets logging from specified type - these are following: `error` `info` `warning` `user` |
			
#### Examples: 
```
log types all
log types error info
log types clear
log types warning
```
### **`modules`** argument makes logs appear only from certain modules

| **modules arguments** | **Describe** |
| -----------: | ------------ |
| `all` | argument sets logging from all types |
| `clear` | argument clears logging modules - won't log from any of them |
| `<module>` | argument sets logging from specified module - these are following: `audio` `base` `input` `logs` `physics` `renderer` `resources` `states` `utilities` `terminal` `world` `none` |
		
#### Examples:
```
log modules all
log modules base input renderer resources
log modules clear
log modules none
```
## Audio commands
-------------------------
### **`mute`** is **single-argumented** command which disables audio from specified module:
| **mute arguments** | **Describe** |
| -----------: | ------------ |
| `music` | argument mutes music |
| `sound` | argument mutes sounds |
| `all` | argument mutes both music and sounds |
	
### **`unmute`** is **single-argumented** command which enables audio from specified module:
| **unmute arguments** | **Describe** |
| -----------: | ------------ |
| `music` | argument unmutes music |
| `sound` | argument unmutes sounds |
| `all` | argument unmutes both music and sounds |

#### Examples: 
```
mute all
unmute sound
mute sound
```
### **`setvolume`** is **many or single-argumented** command which sets loudness of specified module:
| **setvolume arguments** | **Describe** |
| -----------: | ------------ |
| `music` | argument sets volume of music |
| `sound` | argument sets volume of sound	|
| `no argument` | if neither music nor sound is included, volume is set to both |
| `<value>`| argument a number from range `0-100` |

#### Examples: 
```
setvolume 76
setvolume music 5
setvolume sound 100
```

## Collision Debug commands
-------------------------
### **`collisiondebug`** is **many-argumented** command which is generally associated with displaying collision debugging features:

#### `turn` argument switches the mode of collision debug to turned off/on
| **turn arguments** | **Describe** |
| -----------: | ------------ |
| `off` | argument turns off the collision debug |
| `on` | argument turns on the collision debug |
#### `color` argument changes color set of the collision debug
| **color arguments** | **Describe** |
| -----------: | ------------ |
| `1` | argument sets first color set |
| `2` | argument sets second color set |
|  `3` | argument sets third color set |
#### `display`argument changes type of highlighted objects
| **display arguments** | **Describe** |
| -----------: | ------------ |
| `static` | argument highlights static objects |
| `kinematic` | argument highlights kinematic objects |
| `all` | argument highlights all objects |
	
#### Examples: 
```
collisiondebug turn on
collisiondebug color 3
collisiondebug display static
```
## Player location commands
-------------------------
### **`currentpos`** is **no-argument** command which prints out player's coordinates
### **`teleport`** is **many-argumented** command which relocate player to other coordinates
| **teleport arguments** | **Describe** |
| -----------: | ------------ |
| `<X coordinate>` | argument is a number which sets the player on X axis |
| `<Y coordinate>` | argument is a number which sets the player on Y axis |
| `<only one argument`> | if only one argument is given, second one implicitly gets its value |
	
#### Examples: 
```
teleport 260 1255
teleport 300
```

## Camera commands
-------------------------
### **`view`** is command which changes camera view size
| **view arguments** | **Describe** |
| -----------: | ------------ |
| `<width value>` | argument is a number which sets camera view width |
| `<height value>` | argument is a number which sets camera view height |
| `<only one argument>` | if only one argument is given, second one implicitly gets its value |
| `normal` | sets camera view size back to default value (640, 480); should be passed as a single argument |
| `chunkdebug` | makes chunks render as for default camera view size; has to be passed as third argument; |
#### Hint:
Remember that default aspect ratio in PopHead is 4x3 so consider that using this command. Rather use ``view 2000 1500`` then ``view 2000``.
#### Examples: 
```
view 2000 1500
view 1000
view normal
view 1000 750 chunkdebug
```
