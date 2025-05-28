#include "one.glsl"
In the scene weve been using so far there are a few spots where z-fighting can be noticed. The
containers were placed at the exact height of the floor which means the bottom plane of the container
is coplanar with the floor plane. The depth values of both planes are then the same so the resulting
depth test has no way of figuring out which is the right one.