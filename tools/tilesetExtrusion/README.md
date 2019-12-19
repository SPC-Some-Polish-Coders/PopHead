### Why we have to extrude our tileset
This way we avoid graphical artifact called texture bleeding which looks like ugly lines between tiles in tilemap.

### Frame extruder tool
Tile extruding tool "Frame extruder" was made by lgibson02 <br>
Here is the repo of it: https://github.com/lgibson02/FrameExtruder

### Extruding PopHead tileset tutorial
We have to use it after each change in tileset.png texture. <br>
First you have to open the FrameExtruder-v1.0.exe <br>
Don't change the default parameters. <br>
Just click ``select manualy`` and close the program. <br>
Later run afterExtruding.bat.

### Tileset files
- In tileset.png we store not extruded texture which we edit manually and draw new graphics. We also use tileset.png as tileset texture in Tiled.
- In extrudedTileset.png we store extruded texture which we use in the actual game to render tilemap. 