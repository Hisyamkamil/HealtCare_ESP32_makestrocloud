//
// Created by Andri Yadi on 8/5/16.
//

#ifndef ENVIROSENSE_UISERVICE_H
#define ENVIROSENSE_UISERVICE_H

#include <SSD1306.h>
#include <OLEDDisplayUi.h>
#include "images.h"

class UIService {
public:
    UIService();
    ~UIService();
    void begin();
    void loop();
    void setBPMValue(int bpm);
    

private:
    SSD1306 *display = NULL;
    OLEDDisplayUi *ui = NULL;

    //unsigned long lastUpdateDisplayMilis = 0;
};


#endif //ENVIROSENSE_UISERVICE_H
