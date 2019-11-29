# Z values / layers pattern:

We should stick to this pattern of layers when we assign Z component to entity. Remember that we rendering is more efficient when we have less different Z values.

#### Pattern:
- 200 and less - map layers 
- 100 and less - interactive objects (characters, enemies, vehicles)
- 10 and less - GUI