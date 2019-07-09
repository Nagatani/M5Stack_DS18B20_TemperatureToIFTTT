#ifndef _IFTTTWebhooks_h_
#define _IFTTTWebhooks_h_
#include "arduino.h"

class IFTTTWebhooks {
  public:
    IFTTTWebhooks();
    void sendToIFTTT(String, String, String, String);

  private:
};

#endif
