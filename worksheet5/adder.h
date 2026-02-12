// b e g i n = adder.h ============

// The following two lines prevent adder.h from being included
// more than once in any source (.cpp) file. If this were to happen
// it would cause problems in the compile process but it is difficult to
// prevent manually in large projects. These #ifndef, #define and #endif
// commands form an "include guard" and are types of compiler directive.
// The include guard contains an ID for this file "MATHSLIB_ADDER_H", this is
// arbitrary but must be unique within the project.
#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

// We need to prefix our function names with an additional
// keyword which is different depending on the operating
// system we are using, and whether we are using or creating the
// library.
// The variables "maths_EXPORTS" must be defined at build time if
// we are building the library, but not if we are using it.
#if defined(_WIN32) || defined(WIN32)
#if defined(maths_STATIC)
// dont add any keywords if building a static library
#define MATHSLIB_API
#else
#if defined(maths_EXPORTS)
#define MATHSLIB_API __declspec(dllexport)
#else
#define MATHSLIB_API __declspec(dllimport)
#endif
#endif
#else
// MATHSLIB_API is defined as nothing if we're not on Windows
#define MATHSLIB_API
#endif

/**
 * @file
 * This file contains the declarations of all exported functions in the maths library.
 */

 /**
  * @brief Adding function
  * This function adds two numbers.
  *
  * @param a The first number.
  * @param b The second number.
  * @return Sum of a and b.
  */

// The above will include one of:
// __declspec(dllexport)
// __declspec(dllimport)
// before declarations. This is a Microsoft specific
// extension to C/C++

// prototype for the function including additional keyword
MATHSLIB_API int add(int a, int b);

#endif

// end = adder.h ============
