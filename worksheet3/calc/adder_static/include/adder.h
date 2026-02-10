// begin=adder.h ============
// The following two lines prevent adder.h from being included
// more than once in any source (.cpp) file. If this were to happen
// it would cause problems in the compile process but it is difficult to
// prevent manually in large projects. These #ifndef, #define and #endif
// commands form an "include guard" and are types of compiler directive.
// The include guard contains an ID for this file "MATHSLIB_ADDER_H", this is
// arbitrary but must be unique within the project.

#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

// prototype for our function
int add(int a, int b);

#endif
// end=adder.h
