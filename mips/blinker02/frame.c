//-------------------------------------------------------------------------
// VideoCore IV implementation of dwelch67's uart02 sample.
// See: https://github.com/dwelch67/raspberrypi/tree/master/uart02)
//-------------------------------------------------------------------------

START("loader.bin");

fillb(0x200, 0);

#include "translate.c"

END
