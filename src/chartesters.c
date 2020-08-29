#include "chartesters.h"

bool isDigit(char c) { return (c >= '0' && c <= '9'); }
bool isCharLetter(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
bool isCharAlpha(char c) { return (isDigit(c) || isCharLetter(c) || c == '_'); }
bool isCharExpression(char c) { return (isDigit(c) || isCharAlpha(c) || c == '*'); }
