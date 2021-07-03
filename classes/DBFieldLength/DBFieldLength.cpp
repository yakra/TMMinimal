#include "DBFieldLength.h"

// constants
const size_t DBFieldLength::label = 26;
const size_t DBFieldLength::regionCode = 8;
const size_t DBFieldLength::regionName = 48;
const size_t DBFieldLength::regiontype = 32;
const size_t DBFieldLength::root = 32;
// sums of other constants
const size_t DBFieldLength::dcErrValue = DBFieldLength::root + DBFieldLength::label + 1;;
