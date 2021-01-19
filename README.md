# CHEX
A single header library to manage hexagonal coordinates

# INFO
This code is the C implementation of the famous  page :
https://www.redblobgames.com/grids/hexagons/
It is a self contained single header library with all the basic funtions needed.

## Notes
On the main implementation page the function hex_direction(...) have the following assert :
assert (0 <= direction && direction < 6);
i think the correct assert should be :
assert (0 >= direction && direction < 6);