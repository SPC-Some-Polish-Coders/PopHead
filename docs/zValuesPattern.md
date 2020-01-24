# Z values / layers pattern:

We should stick to this pattern of layers when we assign Z component to entity. Remember that we rendering is more efficient when we have less different Z values.

#### Pattern:
- 200 and less - map layers 
- 170 and less - dead characters
- 100 and less - standing objects, characters
- 20 and less - GUI
