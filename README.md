# Caffeine Math

[![Build Status](https://travis-ci.org/PhilCK/caffeine-math.svg?branch=master)](https://travis-ci.org/PhilCK/caffeine-math)

This is a header only math library that built up in my hobby projects. I've extracted it and put it up here for anybody who wants it use it. Its a no-nonsece floating point math library. I'm porting it over in chunks as I write tests just to double check its working correctly.


## Support For
floating point vector types (2,3 and 4).
~~floating point matrix types (3x3 and 4x4).~~ (currently testing before porting over)
~~floating point quaternion type.~~ (currently testing before porting over)
general math operations.

Some math function maybe missing I tend to add them as required. If something is missing feel free to submit a pull request or give me a poke.


## Where are the operators
I don't use them because it can create a lot of ambigioty. `vec3 * vec3` is the componet multiply or cross product. Cross product you say, then what about `vec4 * vec4`? But what about things like `vec + vec * vec`? Well I think that looks like balls, operators aren't hard add, but really breaking it down usually makes it clearer anyway.


## SIMD Status
There is no SIMD support currently, I have been working on it a little, but its not a priority for me right now because I have no math bottlenecks. However the library has been designed around a SIMD implimentation so when it comes there should be minimal fuss.


## License
MIT