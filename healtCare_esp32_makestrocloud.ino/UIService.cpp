//
// Created by Andri Yadi on 8/5/16.
//

#include "UIService.h"
#include "images.h"



int bpmValue_ = 0;
unsigned long lastUpdateDisplayMilis = 0;

/*
void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);

    char formattedTemperature[10];
    char formattedHumidity[10];

    //display->drawString(0, 0, "Wed, 31 Aug 2016");
    //display->drawString(0, 0, ("Published: " + String(currentSensorValues.counter)));
    display->drawString(0, 0, statusInfo);

    // temperature
    display->setFont(ArialMT_Plain_24);
    dtostrf(currentSensorValues.T2, 4, 1, formattedTemperature);
    dtostrf(currentSensorValues.H2, 4, 1, formattedHumidity);
    display->drawString(0, 24, String(formattedTemperature) + "°C");

    // humidity
    display->setFont(ArialMT_Plain_16);
    display->drawString(0, 48, String(formattedHumidity) + "%");
    display->setTextAlignment(TEXT_ALIGN_LEFT);

}
*/

//void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
//    // draw an xbm image.
//    // Please note that everything that should be transitioned
//    // needs to be drawn relative to x and y
//
//    //display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
//    display->setTextAlignment(TEXT_ALIGN_CENTER);
//    display->setFont(ArialMT_Plain_10);
//    display->drawString(110 + x, y, "Temp");
//    // display->drawStringMaxWidth(90 + x, y+12, 24, wunderground.getWeatherText());
//
////    display->setFont(ArialMT_Plain_10);
////    String weatherIcon = wunderground.getTodayIcon();
////    // int weatherIconWidth = display->getStringWidth(weatherIcon);
////    display->drawString(110 + x, 20 + y, weatherIcon);
//
//    display->setFont(ArialMT_Plain_16);
//
//    char formattedTemperature[10];
//    dtostrf(currentSensorValues.T1, 4, 1, formattedTemperature);
//    String temp = String(formattedTemperature) + "°";
//
//    display->drawString(106 + x, 44 + y,temp);
//    int tempWidth = display->getStringWidth(temp);
//}

//void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
//    // draw an xbm image.
//    // Please note that everything that should be transitioned
//    // needs to be drawn relative to x and y
//
//    //display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
//    display->setTextAlignment(TEXT_ALIGN_CENTER);
//    display->setFont(ArialMT_Plain_10);
//    display->drawString(110 + x, y, "Acc");
//    // display->drawStringMaxWidth(90 + x, y+12, 24, wunderground.getWeatherText());
//
////    display->setFont(ArialMT_Plain_10);
////    String weatherIcon = wunderground.getTodayIcon();
////    // int weatherIconWidth = display->getStringWidth(weatherIcon);
////    display->drawString(110 + x, 20 + y, weatherIcon);
//
//    char formattedX[8];
//    char formattedY[8];
//    char formattedZ[8];
//
//    dtostrf(currentSensorValues.X, 4, 1, formattedX);
//    dtostrf(currentSensorValues.Y, 4, 1, formattedY);
//    dtostrf(currentSensorValues.Z, 4, 1, formattedZ);
//
//    display->setTextAlignment(TEXT_ALIGN_RIGHT);
//    display->setFont(ArialMT_Plain_10);
//    display->drawString(118 + x, 18 + y, "X:" + String(formattedX));
//    display->setFont(ArialMT_Plain_10);
//    display->drawString(118 + x, 31 + y, "Y:" + String(formattedY));
//    display->setFont(ArialMT_Plain_10);
//    display->drawString(118 + x, 44 + y, "Z:" + String(formattedZ));
//}

//void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
//    // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
//    // Besides the default fonts there will be a program to convert TrueType fonts into this format
////    display->setTextAlignment(TEXT_ALIGN_LEFT);
////    display->setFont(ArialMT_Plain_10);
////    display->drawString(0 + x, 10 + y, "Arial 10");
////
////    display->setFont(ArialMT_Plain_16);
////    display->drawString(0 + x, 20 + y, "Arial 16");
////
////    display->setFont(ArialMT_Plain_24);
////    display->drawString(0 + x, 34 + y, "Arial 24");
//
//
//    char formattedPressure[10];
//    char formattedLux[15];
//    char formattedHeading[6];
//
//    dtostrf(currentSensorValues.P, 4, 1, formattedPressure);
//    dtostrf(currentSensorValues.lux, 4, 1, formattedLux);
//    dtostrf(currentSensorValues.magHeading, 3, 1, formattedHeading);
//
//    display->setTextAlignment(TEXT_ALIGN_RIGHT);
//    display->setFont(ArialMT_Plain_10);
////    display->drawString(110 + x, 0 + y, String(formattedPressure));
////    display->setFont(ArialMT_Plain_10);
////    display->drawString(110 + x, 22 + y, String(formattedLux));
////    display->setFont(ArialMT_Plain_16);
////    display->drawString(110 + x, 44 + y, String(currentSensorValues.gas));
//
//    display->drawString(118 + x, 2 + y, "P:" + String(formattedPressure));
//    display->setFont(ArialMT_Plain_10);
//    display->drawString(118 + x, 17 + y, "L:" + String(formattedLux));
//    display->setFont(ArialMT_Plain_10);
//    display->drawString(118 + x, 32 + y, "G:" + String(currentSensorValues.gas));
//    display->setFont(ArialMT_Plain_10);
//    display->drawString(118 + x, 47 + y, "C:" + String(formattedHeading));
//}

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
    //display->drawXbm(10, (DISPLAY_HEIGHT-heart_height)/2, heart_width, heart_height, heart_bits);
}


void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    // draw an xbm image.
    // Please note that everything that should be transitioned
    // needs to be drawn relative to x and y

    //display->clear();

//    if (millis() - lastUpdateDisplayMilis < 1000) {
//        return;
//    }

    lastUpdateDisplayMilis = millis();
    display->drawXbm(10, (DISPLAY_HEIGHT-heart_height)/2, heart_width, heart_height, heart_bits);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_24);
    display->drawString(x + 10 + heart_width + 20, y + (DISPLAY_HEIGHT-28)/2, String(bpmValue_));
    //display->drawString(x + 10 + heart_width + 20, y + (DISPLAY_HEIGHT-28), String(PedoValue_));

}


// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { drawFrame1 };
// how many frames are there?
int frameCount = 1;

// Overlays are statically drawn on top of a frame eg. a clock
//OverlayCallback overlays[] = { msOverlay, msOverlay2 };
//int overlaysCount = 2;P

UIService::UIService() {

}

UIService::~UIService() {
    if (display != NULL) {
        delete display;
        display = NULL;
    }

    if (ui != NULL) {
        delete ui;
        ui = NULL;
    }
}

void UIService::begin() {

    display = new SSD1306(0x3C, 21,22);
    ui = new OLEDDisplayUi(display);

    ui->setTargetFPS(60);

//    ui->disableIndicator();

    // Customize the active and inactive symbol
    ui->setActiveSymbol(activeSymbol);
    ui->setInactiveSymbol(inactiveSymbol);
    ui->setIndicatorPosition(BOTTOM);
//
//    // Defines where the first frame is located in the bar.
//    ui->setIndicatorDirection(LEFT_RIGHT);
//
//    // You can change the transition that is used
//    // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
//    ui->setFrameAnimation(SLIDE_UP);

    //ui->disableAutoTransition();

    ui->setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
    ui->setFrameAnimation(SLIDE_DOWN);

    // Add frames
    ui->setFrames(frames, frameCount);

    // Add overlays
  //  ui->setOverlays(overlays, overlaysCount);

    // Initialising the UI will init the display too.
    ui->init();

    display->flipScreenVertically();
}

void UIService::loop() {

//    if ((millis() - lastUpdateDisplayMilis > (SENSOR_QUERY_INTERVAL/2)) && ui->getUiState()->frameState == FIXED) {
//
//        lastUpdateDisplayMilis = millis();
//        currentSensorValues = svc.getLastSensorValues();
//        //Serial.print("G: ");
//        //Serial.println(currentSensorValues.gas);
//
//
//    }

    int remainingTimeBudget = ui->update();

    if (remainingTimeBudget > 0) {
        // You can do some work here
        // Don't do stuff if you are below your
        // time budget.

        delay(remainingTimeBudget);
    }
}

void UIService::setBPMValue(int bpm) {
    bpmValue_ = bpm;
    //Serial.print("[String] BPM: "); Serial.println(info);
    //ui->update();
}

