#pragma once

namespace Axton
{
#define AX_FLAG_OPERATORS(Enum) inline Enum operator |(Enum lhs, Enum rhs) { return static_cast<Enum>(int(lhs) | int(rhs)); }\
inline bool operator &(Enum lhs, Enum rhs) { return static_cast<bool>(int(lhs) & int(rhs)); }
}