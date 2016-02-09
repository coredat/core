# Code Standards

Slightly stricter than the `code_styles` document. Every game I've done has had an architectural or technical goal. This one is tobe data orientated and to live life without (or as little as possible) STL.


### No Allocations

The game engine has pre-allocated memory, use that.


### Templates

Shun Templates, they slow compile times down.


### Smart Pointers

Shared objects aren't a thing, please don't use them unless absolutely required.


### STL in General

Shun STL when ever possible, templates, and do they solve our issues?
