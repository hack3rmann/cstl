#include "types.h"



bool bool_implies(bool const self, bool const other) {
    return self <= other;
}