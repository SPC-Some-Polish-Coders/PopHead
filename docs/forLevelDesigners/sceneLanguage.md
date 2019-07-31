# Scene Parser
In order to load a map in our game, we use XML files which contain detailed data associated with given location. 
Then our simple parser buids a game scene based on what was described in the .xml.

In every such a document we include resources needed for the proper display so textures, background music as well 
as a map are listed there. We don't put sound resources there because it's done in soundData.xml (for now!). 

> If you're interested how the parser works, just check these links below!

> [sceneParser.hpp](https://github.com/SPC-Some-Polish-Coders/PopHead/blob/master/src/Scenes/sceneParser.hpp)

> [sceneParser.cpp](https://github.com/SPC-Some-Polish-Coders/PopHead/blob/master/src/Scenes/sceneParser.cpp)

# Tutorial

Best explanations are followed by some examples so here's the one:

## Example scene file

```xml
<?xml version="1.0" encoding="utf-8"?>
<scene>
  <loading>
    <textures>
      <resource respath="map/FULL_DESERT_TILESET_WIP.png"/>
      <resource respath="characters/vaultManSheet.png"/>
      <resource respath="characters/vaultMan.png"/>
      <resource respath="characters/zombie.png"/>
    </textures>
  </loading>
  <music>
    <theme filename="explorationTheme.ogg"/>
  </music>
  <root>
    <map name="map" filepath="maps/verySmallMap.tmx"/>
	  <entrance name="desertMap" filepath="scenes/desertScene.xml" positionX="0" positionY="40" width="20" height="20"/>
    <player texturepath="textures/characters/vaultManSheet.png"/>
    <group name="npc">
      <npcTest positionX="30" positionY="30"/>
    </group>
    <group name="enemies">
      <zombie positionX="50" positionY="50"/>
    </group>
  </root>
</scene>
```

## Scene element

We start writing our document with adding a root element called `<scene>...</scene>`.
It's just a container which holds other elements and from which we start parsing.

#### Every scene element should contain following elements: `<loading>`, `<music>` and `<root>`!

```xml
<scene>
  <loading>
     <!--Some content-->
  </loading>
  <music>
     <!--Some content-->
  </music>
  <root>
     <!--Some content-->
  </root>
</scene>
```

Now we can go to describing detailed elements.

## Loading element

This element contains all the graphics resources that are compulsory for the given location to load correctly.

#### Every loading element should contain following elements: `<textures>`! (in the future probably there will be more)

```xml
<scene>
  <loading>
     <textures>
        <resource respath="pathRelative/toThe/ResourcesDir.png"
     </textures>
  </loading>
  <music></music>
  <root></root>
</scene>
```

As you may noticed in the textures we put a resource element which only atribute is `respath` and in which
we include a direct path to the file. Remember that the path needs to be relative to the resources folder! 
So if the whole path is `G:/PopHead/resources/characters/vaultManSheet.png`, in `respath` you just type "characters/vaultManSheet.png"

## Music element

This element contains music which is supposed to be played in given location.

#### Music element contains only one element called `<theme>`.

```xml
<scene>
  <loading></loading>
  <music>
    <theme filename="explorationTheme.ogg"/>
  </music>
  <root></root>
</scene>
```

As its atribute which is called `filename`, we pass a path to the .ogg file which this time is relative to the resources/music folder!
So analogic - whole path: `G:/PopHead/resources/music/explorationTheme.ogg` -> "explorationTheme.ogg"

## Root element

It's the most developed one but it doesn't mean it's difficult. Here you provide all the game objects that should appear in  the given location

#### Every root element should contain following elements: `<map>`, `<entrance>`, `<player>`, `<group>`

```xml
<scene>
  <loading></loading>
  <music></music>
  <root>
    <map name="map" filepath="maps/verySmallMap.tmx"/>
	  <entrance name="desertMap" filepath="scenes/desertScene.xml" positionX="0" positionY="40" width="20" height="20"/>
    <player texturepath="textures/characters/vaultManSheet.png"/>
    <group name="npc"> <!--Some content--> </group>
    <group name="enemy"> <!--Some content--> </group>
  </root>
</scene>
```

