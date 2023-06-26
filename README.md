# pasteur

A parameterized strings library.

## What are Parameterized Strings?

A *secure* alternative for string concatenation, using lazy concatenation of string literals with arbitrary argument types.
By delaying the concatentaion of arguments as strings, and preserving their original type and value, arguments can be encoded and sanitized at *output*.

### What is this good for?

Using embeded arguments delays the arguments integration into the string to the point of actual usage.
This allows for writing common, unified *secure* interfaces that solve many of the most common security problems and avoid common input handling anti-patterns.

### Which antipatterns are avoided?

String-concatenation
Input Sanitization

### Which security problems can be eliminated?

OS Command injection
SQL injection
NoSQL injection
Log nl/cr injection 
Sensitive information disclosure

## Quick Usage Example

Given the following variable definitions:
int level = 42;
std::string name = "Yariv";

#### Instead of:

std::string s1 = std::string("Welcome to level ") + std::to_string(level) + ", " + name;
OutputMsg(s1);

std::stringstring s2;
s2 << "Welcome to level " << level << ", " << name;
OutputMsg(s2.str());

#### Write:

auto s = "Welcome to level " + pstr::embed(level) + ", " + pstr::embed(name);
OutputMsg(s);

**or**

auto s = pstr::concat / "Welcome to level " + level + ", " + name;
OutputMsg(s);

## Original Intention
Pasteur was originally intended as an infrastructure to be used for the representation of parameterized interpolated strings, similar to C#'s FormattedString or Python's PEP501 suggestion.
