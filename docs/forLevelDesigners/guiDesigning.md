# GUI Designing
In our application gui elements are saved in xml file. All files are in resources/scenes/gui. There is an example file:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<gui>
    <interface name="game_pause_menu">
        <widget name="buttonArea" contentPath="GuiTestContent/gui4.png" origin="0.5">
            <textWidget name="button1" contentPath="GuiTestContent/gui1.png" origin="0.5" position="0.5,0.3" fontPath="GuiTestContent/testFont.ttf" textPosition="0.4,0.3" color="green" onButtonPressed="replaceScene:scenes/desert.xml">Run Game
            </textWidget>
            <widget name="button2" contentPath="GuiTestContent/gui2.png" origin="0.5" position="0.5,0.7">
            </widget>
        </widget>
    </interface>
</gui>
```
In this tutorial all elements of gui that can be modified by xml file will be described in detail.

There are now 3 types of gui elements:
- widget
- interface
- textWidget

The first one is in code a base class for the rest. Very detailed description of these elements and their attributes can be found in [Gui tutorial](../forProgrammers/guiTutorial.md).

## Attributes
##### Note about 'double values': they can be in from "valueX,valueY" or "valueBoth"; example:
```xml
<widget origin="0.5" position="0.5,0.3">
```
- Widget attributes (apply also to derived classes like Interface and TextWidget):
  - name - it's a 'must-be' attribute
  - contentPath - path to texture displayed in widget
  - alpha - sets alpha channel
  - origin - double value
  - position - double value
  - scale - double value
  - onButtonPressed - creates action that is performed when user clicks mouse over a widget (see [Actions](#widgets-actions))
  - onButtonReleased - creates action that is performed when user releases mouse over a widget (see [Actions](#widgets-actions))
  - onButtonUpdate - creates action that is performed on every gui update (see [Actions](#widgets-actions))
- TextWidget attributes:
  - text - just text to display
  - fontPath - path to font for text
  - textPosition - double value
  - characterSize - size of text
  - textOrigin - double value
  - scaleText - double value
  - textAlpha - alpha channel for text
  - color - color of text, for now supported only:
    - black
	- white
	- red
	- green
	- blue
	- yellow
	- magenta
	- cyan
	- transparent

### Widgets actions
Actions are written in form "actionName:parameter". Example: 
```xml
<widget onButtonPressed="replaceScene:scenes/desert.xml" ...></widget>
```
Actions:
- replaceScene - changes current scene in Game, takes scene file path
- closeGame - closes the game, doesn't take any parameters
