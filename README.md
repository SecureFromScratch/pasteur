# pasteur
Parameterized strings: lazy concatenation of string literals with arbitrary argument types.

## Goal
Given the following variable definitions:
int level = 42;
std::string name = "Yariv";

#### Instead of:
std::string s = std::string("Welcome to level ") + std::to_string(level) + ", " + name;
OutputMsg(s);

#### Write:
auto s = "Welcome to level " + pstr::embed(level) + ", " + pstr::embed(name);
OutputMsg(s);

## What is this good for?
Using embeded arguments delays the arguments integration into the string. This allows the processor of the string to 
## Original Intention
Pasteur was originally intended as an infrastructure to be used for the representation of parameterized interpolated strings, similar to C#'s FormattedString or Python's PEP501 suggestion.
