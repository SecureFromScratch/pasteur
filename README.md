# pasteur
Parameterized strings: lazy concatenation of string literals with arbitrary argument types.

## Goal
Given the following variable definitions:
int level = 42;
std::string name = "Yariv";

#### Instead of:
std::string s = std::string("Welcome to level ") + std::to_string(level) + ", " + pstr:embed(name);
OutputMsg(s);

#### Write:
auto s = "Welcome to level " + pstr:embed(level) + ", " + pstr:embed(name);
OutputMsg(s);

## Original Intention
Pasteur was originally intended as an infrastructure to be used for the representation of parameterized interpolated strings, similar to C#'s FormattedString or Python's PEP501 suggestion.
