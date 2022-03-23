#ifndef _SlackWebhooks_h_
#define _SlackWebhooks_h_
#include "arduino.h"

class SlackWebhooks {
  public:
    SlackWebhooks();
    void sendToSlack(String);

  private:
};

#endif
