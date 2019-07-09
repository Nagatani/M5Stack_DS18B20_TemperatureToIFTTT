#ifndef _ClockController_h_
#define _ClockController_h_
#include <time.h>

class ClockController {
  public:
    ClockController();
    tm getDateTime();
    void setupTimeZone();

  private:
};
#endif
