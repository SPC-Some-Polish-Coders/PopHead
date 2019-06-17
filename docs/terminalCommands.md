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

## Navigation keys in the terminal
- Using **"Up"** arrow you recover last entered command
- Using **"Down"** arrow you delete so far entered characters

## Game commands
-------------------------
| **Command** | **Describe** |
| ----------: | ----------- |
| **`exit`**  | is **no-argument** command which closes the program |
| **`echo`** | is **single-argumented** command which prints out the message given by the user |

#### Examples: 
```
echo hello world
exit
```
## Logging commands
-------------------------
**`log`** is **many-argumented** command which is generally associated with way of logging into console or file:
- **`into`** argument makes logs appear only in specified place
	- `file` argument makes logs appear in file
	- `console` argument makes logs appear in console
	- `both` argument makes logs appear in both places
	- `not` argument makes whole command opposite - logs won't appear in specified place
	
#### Examples: 
```
log into file
log into both not
log into console
```
- **`types`** argument makes logs appear only from certain types
	- `all` argument sets logging from all types
	- `clear` argument clears logging types - won't log from any of them
	- `<name>` argument sets logging from specified type - these are following:
		- `error` `info` `warning` `user`
			
	#### Examples: 
	```
	log types all
	log types error info
	log types clear
	log types warning
	```
- **`modules`** argument makes logs appear only from certain modules
	- `all` argument sets logging from all types
	- `clear` argument clears logging modules - won't log from any of them
	- `<name>` argument sets logging from specified module - these are following:
		- `audio` `base` `input` `logs` `physics` `renderer` `resources` `states` `utilities` `terminal` `world` `none`
		
#### Examples:
```
log modules all
log modules base input renderer resources
log modules clear
log modules none
```
## Audio commands
-------------------------
**`mute`** is **single-argumented** command which disables audio from specified module:
- `music` argument mutes music
- `sound` argument mutes sounds
- `all` argument mutes both music and sounds
	
**`unmute`** is **single-argumented** command which enables audio from specified module:
- `music` argument unmutes music
- `sound`argument unmutes sounds
- `all` argument unmutes both music and sounds

#### Examples: 
```
mute all
unmute sound
mute sound
```
**`setvolume`** is **many or single-argumented** command which sets loudness of specified module:
- `music` argument sets volume of music
- `sound` argument sets volume of sound
- if neither music nor sound is included, volume is set to both
- `<value>` argument a number from range `0-100`

#### Examples: 
```
setvolume 76
setvolume music 5
setvolume sound 100
```

## Collision Debug commands
-------------------------
**`collisiondebug`** is **many-argumented** command which is generally associated with displaying collision debugging features:
- `turn` argument switches the mode of collision debug to turned off/on
	- `off` argument turns off the collision debug
	- `on` argument turns on the collision debug
- `color` argument changes color set of the collision debug
	- `1` argument sets first color set
	- `2` argument sets second color set
	- `3` argument sets third color set
- `display`argument changes type of highlighted objects
	- `static` argument highlights static objects
	- `kinematic` argument highlights kinematic objects
	- `all` argument highlights all objects
	
#### Examples: 
```
collisiondebug off
collisiondebug color 3
collisiondebug display static
```
## Player location commands
-------------------------
**`currentpos`** is **no-argument** command which prints out player's coordinates
**`teleport`** is **many-argumented** command which relocate player to other coordinates
	- `<X coordinate>` argument is a number which sets the player on X axis
	- `<Y coordinate>` argument is a number which sets the player on Y axis
	- if only one argument is given, second one implicitly gets its value
	
#### Examples: 
```
teleport 260 1255
teleport 300
```
