# GUI Designing
In our application gui elements are saved in gui.xml files. All files are in scenes/gui. There is an example file:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<gui>
    <interface name="game_pause_menu">
        <widget name="buttonArea" centerPos="0.5" size="0.4" color="transparent">
            <textWidget name="playButton" text="Play" texturePath="gui/button.png" color="green" centerPos="0.5,0.3" fontName="joystixMonospace.ttf" textColor="rgb(100, 100, 255)" onButtonPressed="replaceScene:scenes/desert.xml">
            </textWidget>
            <widget name="exitButton" texturePath="gui/button.png" color="red" bottomCenterPos="0.5,0.8">
            </widget>
        </widget>
    </interface>
</gui>
```
In this tutorial all elements of gui that can be modified by xml file will be described in detail.

There are now 3 types of gui elements:
- interface
- widget
- textWidget
- sliderWidget

## Interfaces
Interface has always size of the whole screen and contains widgets.

## Attributes
##### Note about 'vector values': they can be in from "valueX,valueY" or "valueBoth"; example:
```xml
<widget origin="0.5" position="0.5,0.3">
```
- Interface attributes
  - hide - when value is "true" the interface will be hidden initially
- Widget attributes (apply also to derived classes like TextWidget and SliderWidget):
  - hide - when value is "true" the widget will be hidden initially
  - name - it's a 'must-be' attribute, every widget on its gui tree level has to have different name
  - texturePath - path to load texture displayed in widget
  - color - sets color which is multiplied with texture (see [Color-properties](#color-properties))
  - velocity - sets widget constant velocity (used in credits)
  - size - (vector value) sets size 
  - centerPos - (vector value) sets widget position according to widget's center
  - topCenterPos - (vector value) sets widget position according to widget's top side center 
  - bottomCenterPos - (vector value) sets widget position according to widget's bottom side center
  - rightCenterPos - (vector value) sets widget position according to widget's right side center 
  - leftCenterPos - (vector value) sets widget position according to widget's left side center 
  - topLeftPos - (vector value) sets widget position according to widget's top left corner 
  - topRightPos - (vector value) sets widget position according to widget's top right corner
  - bottomLeftPos - (vector value) sets widget position according to widget's bottom left corner
  - bottomRightPos - (vector value) sets widget position according to widget's bottom right corner
  - onButtonPressed - creates action that is performed when user clicks mouse over a widget (see [Actions](#widgets-actions))
  - onButtonReleased - creates action that is performed when user releases mouse over a widget (see [Actions](#widgets-actions))
  - onButtonUpdate - creates action that is performed on every gui update (see [Actions](#widgets-actions))
  - template - loads properties from source template
- TextWidget attributes:
  - text - just text to display
  - fontName - name of font file
  - textAligment - values can be "center", "left" or "right", "center" is default value
  - fontSize - size of font of text
  - textColor - color of text (see [Color-properties](#color-properties))
- SliderWidget attributes
  - iconTexturePath - path to load texture displayed as slider icon
  - icon size (vector value)
  - init value - can be set to floating point number or to some variable (for example "get:SoundVolume")
  - min value - can be set to floating point number or to some variable (for example "get:SoundVolume")
  - max value - can be set to floating point number or to some variable (for example "get:SoundVolume")

### Positioning and size
poitions and sizes are in widget's normalized space which means that point (0, 0) is top left corner of parent widget and (1, 1) is bottom right corner of parent widget. In widget has no parent (which means that its parent node in xml is interface) then the widget normalized space corresponds to screen size.

### Color properties
color properties can take 3 formats:
- rgb(v1, v2, v3)
- rgba(v1, v2, v3, v4)
- predefined colors:
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
- load last save
- set - for example "set:MusicVolume" sets music volume from slider widget value
- closeGame - closes the game, doesn't take any parameters
- hideGuiInterface - hides gui interface, takes interface name
- showGuiInterface - shows gui interface, takes interface name
- setGamePause - takes true or false

### Widget templates
You can specify widget template in your gui.xml file like this:
```xml
<gui>
  <widgetTemplate name="redText" fontName="consola.ttf" fontSize="50" textColor="red" />
  <interface name="buttons">
    <widget name="button" template="redText" text="story" color="blue" centerPos="0.5, 0.3" size="0.3, 0.15" />
    <widget name="button" template="redText" text="menu" color="yellow" centerPos="0.5, 0.7" size="0.3, 0.15" />
  </interface>
</gui>
```
It is equivalent to:

```xml
<gui>
  <interface name="buttons">
    <widget name="button" fontName="consola.ttf" fontSize="50" textColor="red" text="story" color="blue" centerPos="0.5, 0.3" size="0.3, 0.15" />
    <widget name="button" fontName="consola.ttf" fontSize="50" textColor="red" text="menu" color="yellow" centerPos="0.5, 0.7" size="0.3, 0.15" />
  </interface>
</gui>
```

### Live gui editing
To edit gui efficently you should use terminal command ``rguilive`` and maybe ``rguilivefreq``
