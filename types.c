#include "types.h"



Bool Bool_implies(Bool const self, Bool const other) {
    return self <= other;
}