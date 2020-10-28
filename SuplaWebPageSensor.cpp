#include "SuplaWebPageSensor.h"

#include "SuplaDeviceGUI.h"
#include "SuplaWebServer.h"

SuplaWebPageSensor *WebPageSensor = new SuplaWebPageSensor();

SuplaWebPageSensor::SuplaWebPageSensor() {
}

void SuplaWebPageSensor::createWebPageSensor() {
  String path;
  path += PATH_START;
  path += PATH_SENSOR;
  WebServer->httpServer.on(path, std::bind(&SuplaWebPageSensor::handleSensor, this));
  path = PATH_START;
  path += PATH_SAVE_SENSOR;
  WebServer->httpServer.on(path, std::bind(&SuplaWebPageSensor::handleSensorSave, this));

#ifdef SUPLA_DS18B20
  path = PATH_START;
  path += PATH_MULTI_DS;
  WebServer->httpServer.on(path, std::bind(&SuplaWebPageSensor::handleSearchDS, this));
  path = PATH_START;
  path += PATH_SAVE_MULTI_DS;
  WebServer->httpServer.on(path, std::bind(&SuplaWebPageSensor::handleDSSave, this));
#endif
}

void SuplaWebPageSensor::handleSensor() {
  if (ConfigESP->configModeESP == NORMAL_MODE) {
    if (!WebServer->httpServer.authenticate(WebServer->www_username, WebServer->www_password))
      return WebServer->httpServer.requestAuthentication();
  }
  WebServer->sendContent(supla_webpage_sensor(0));
}

void SuplaWebPageSensor::handleSensorSave() {
  //  Serial.println(F("HTTP_POST - metoda handleSensorSave"));

  if (ConfigESP->configModeESP == NORMAL_MODE) {
    if (!WebServer->httpServer.authenticate(WebServer->www_username, WebServer->www_password))
      return WebServer->httpServer.requestAuthentication();
  }

  String key, input;
  uint8_t nr, current_value, last_value;

#ifdef SUPLA_DHT11
  last_value = ConfigManager->get(KEY_MAX_DHT11)->getValueInt();
  current_value = WebServer->httpServer.arg(INPUT_MAX_DHT11).toInt();

  if (last_value > 0) {
    for (nr = 1; nr <= last_value; nr++) {
      input = INPUT_DHT11_GPIO;
      input += nr;
      key = GPIO;
      key += WebServer->httpServer.arg(input).toInt();
      if (WebServer->httpServer.arg(input).toInt() != OFF_GPIO) {
        key = GPIO;
        key += WebServer->httpServer.arg(input).toInt();
        if (ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_OFF ||
            (ConfigESP->getGpio(nr, FUNCTION_DHT11) == WebServer->httpServer.arg(input).toInt() &&
             ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_DHT11)) {
          ConfigManager->setElement(key.c_str(), NR, nr);
          ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_DHT11);
        }
        else {
          WebServer->sendContent(supla_webpage_sensor(6));
          return;
        }
      }
      if (ConfigESP->getGpio(nr, FUNCTION_DHT11) != WebServer->httpServer.arg(input).toInt() ||
          WebServer->httpServer.arg(input).toInt() == OFF_GPIO || ConfigManager->get(key.c_str())->getElement(NR).toInt() > current_value) {
        key = GPIO;
        key += ConfigESP->getGpio(nr, FUNCTION_DHT11);
        ConfigManager->setElement(key.c_str(), NR, 0);
        ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_OFF);
        ConfigManager->setElement(key.c_str(), LEVEL, 0);
        ConfigManager->setElement(key.c_str(), MEMORY, 0);
      }
    }
  }
  if (strcmp(WebServer->httpServer.arg(INPUT_MAX_DHT11).c_str(), "") != 0) {
    ConfigManager->set(KEY_MAX_DHT11, WebServer->httpServer.arg(INPUT_MAX_DHT11).c_str());
  }
  ConfigESP->sort(FUNCTION_DHT11);
#endif

#ifdef SUPLA_DHT22
  last_value = ConfigManager->get(KEY_MAX_DHT22)->getValueInt();
  current_value = WebServer->httpServer.arg(INPUT_MAX_DHT22).toInt();

  if (last_value > 0) {
    for (nr = 1; nr <= last_value; nr++) {
      input = INPUT_DHT22_GPIO;
      input += nr;
      key = GPIO;
      key += WebServer->httpServer.arg(input).toInt();
      if (WebServer->httpServer.arg(input).toInt() != OFF_GPIO) {
        key = GPIO;
        key += WebServer->httpServer.arg(input).toInt();
        if (ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_OFF ||
            (ConfigESP->getGpio(nr, FUNCTION_DHT22) == WebServer->httpServer.arg(input).toInt() &&
             ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_DHT22)) {
          ConfigManager->setElement(key.c_str(), NR, nr);
          ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_DHT22);
        }
        else {
          WebServer->sendContent(supla_webpage_sensor(6));
          return;
        }
      }
      if (ConfigESP->getGpio(nr, FUNCTION_DHT22) != WebServer->httpServer.arg(input).toInt() ||
          WebServer->httpServer.arg(input).toInt() == OFF_GPIO || ConfigManager->get(key.c_str())->getElement(NR).toInt() > current_value) {
        key = GPIO;
        key += ConfigESP->getGpio(nr, FUNCTION_DHT22);
        ConfigManager->setElement(key.c_str(), NR, 0);
        ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_OFF);
        ConfigManager->setElement(key.c_str(), LEVEL, 0);
        ConfigManager->setElement(key.c_str(), MEMORY, 0);
      }
    }
  }
  if (strcmp(WebServer->httpServer.arg(INPUT_MAX_DHT22).c_str(), "") != 0) {
    ConfigManager->set(KEY_MAX_DHT22, WebServer->httpServer.arg(INPUT_MAX_DHT22).c_str());
  }
  ConfigESP->sort(FUNCTION_DHT22);
#endif

#ifdef SUPLA_BME280 || SUPLA_SHT30
  input = INPUT_SDA_GPIO;
  key = GPIO;
  key += WebServer->httpServer.arg(input).toInt();
  if (WebServer->httpServer.arg(input).toInt() != OFF_GPIO) {
    key = GPIO;
    key += WebServer->httpServer.arg(input).toInt();
    if (ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_OFF ||
        (ConfigESP->getGpio(1, FUNCTION_SDA) == WebServer->httpServer.arg(input).toInt() &&
         ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_SDA)) {
      ConfigManager->setElement(key.c_str(), NR, 1);
      ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_SDA);
    }
    else {
      WebServer->sendContent(supla_webpage_sensor(6));
      return;
    }
  }
  if (ConfigESP->getGpio(1, FUNCTION_SDA) != WebServer->httpServer.arg(input).toInt() || WebServer->httpServer.arg(input).toInt() == OFF_GPIO) {
    key = GPIO;
    key += ConfigESP->getGpio(1, FUNCTION_SDA);
    ConfigManager->setElement(key.c_str(), NR, 0);
    ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_OFF);
    ConfigManager->setElement(key.c_str(), LEVEL, 0);
    ConfigManager->setElement(key.c_str(), MEMORY, 0);
  }
  ConfigESP->sort(FUNCTION_SDA);

  input = INPUT_SCL_GPIO;
  key = GPIO;
  key += WebServer->httpServer.arg(input).toInt();
  if (WebServer->httpServer.arg(input).toInt() != OFF_GPIO) {
    key = GPIO;
    key += WebServer->httpServer.arg(input).toInt();
    if (ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_OFF ||
        (ConfigESP->getGpio(1, FUNCTION_SCL) == WebServer->httpServer.arg(input).toInt() &&
         ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_SCL)) {
      ConfigManager->setElement(key.c_str(), NR, 1);
      ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_SCL);
    }
    else {
      WebServer->sendContent(supla_webpage_sensor(6));
      return;
    }
  }
  if (ConfigESP->getGpio(1, FUNCTION_SCL) != WebServer->httpServer.arg(input).toInt() || WebServer->httpServer.arg(input).toInt() == OFF_GPIO) {
    key = GPIO;
    key += ConfigESP->getGpio(1, FUNCTION_SCL);
    ConfigManager->setElement(key.c_str(), NR, 0);
    ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_OFF);
    ConfigManager->setElement(key.c_str(), LEVEL, 0);
    ConfigManager->setElement(key.c_str(), MEMORY, 0);
  }
  ConfigESP->sort(FUNCTION_SCL);
#endif

#ifdef SUPLA_BME280
  key = KEY_ACTIVE_SENSOR;
  input = INPUT_BME280;
  if (strcmp(WebServer->httpServer.arg(input).c_str(), "") != 0) {
    ConfigManager->setElement(KEY_ACTIVE_SENSOR, SENSOR_BME280, WebServer->httpServer.arg(input).toInt());
  }

  key = KEY_ALTITUDE_BME280;
  input = INPUT_ALTITUDE_BME280;
  if (strcmp(WebServer->httpServer.arg(INPUT_ALTITUDE_BME280).c_str(), "") != 0) {
    ConfigManager->set(key.c_str(), WebServer->httpServer.arg(input).c_str());
  }
#endif

#ifdef SUPLA_SHT30
  key = KEY_ACTIVE_SENSOR;
  input = INPUT_SHT30;
  if (strcmp(WebServer->httpServer.arg(input).c_str(), "") != 0) {
    ConfigManager->setElement(KEY_ACTIVE_SENSOR, SENSOR_SHT30, WebServer->httpServer.arg(input).toInt());
  }
#endif

#ifdef SUPLA_SI7021
  key = KEY_ACTIVE_SENSOR;
  input = INPUT_SI7021;
  if (strcmp(WebServer->httpServer.arg(input).c_str(), "") != 0) {
    ConfigManager->setElement(KEY_ACTIVE_SENSOR, SENSOR_SI7021, WebServer->httpServer.arg(input).toInt());
  }
#endif

#ifdef SUPLA_HC_SR04
  input = INPUT_TRIG_GPIO;
  key = GPIO;
  key += WebServer->httpServer.arg(input).toInt();
  if (WebServer->httpServer.arg(input).toInt() != OFF_GPIO) {
    key = GPIO;
    key += WebServer->httpServer.arg(input).toInt();
    if (ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_OFF ||
        (ConfigESP->getGpio(1, FUNCTION_TRIG) == WebServer->httpServer.arg(input).toInt() &&
         ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_TRIG)) {
      ConfigManager->setElement(key.c_str(), NR, 1);
      ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_TRIG);
    }
    else {
      WebServer->sendContent(supla_webpage_sensor(6));
      return;
    }
  }
  if (ConfigESP->getGpio(1, FUNCTION_TRIG) != WebServer->httpServer.arg(input).toInt() || WebServer->httpServer.arg(input).toInt() == OFF_GPIO) {
    key = GPIO;
    key += ConfigESP->getGpio(1, FUNCTION_TRIG);
    ConfigManager->setElement(key.c_str(), NR, 0);
    ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_OFF);
    ConfigManager->setElement(key.c_str(), LEVEL, 0);
    ConfigManager->setElement(key.c_str(), MEMORY, 0);
  }
  ConfigESP->sort(FUNCTION_TRIG);

  input = INPUT_ECHO_GPIO;
  key = GPIO;
  key += WebServer->httpServer.arg(input).toInt();
  if (WebServer->httpServer.arg(input).toInt() != OFF_GPIO) {
    key = GPIO;
    key += WebServer->httpServer.arg(input).toInt();
    if (ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_OFF ||
        (ConfigESP->getGpio(1, FUNCTION_ECHO) == WebServer->httpServer.arg(input).toInt() &&
         ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_ECHO)) {
      ConfigManager->setElement(key.c_str(), NR, 1);
      ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_ECHO);
    }
    else {
      WebServer->sendContent(supla_webpage_sensor(6));
      return;
    }
  }
  if (ConfigESP->getGpio(1, FUNCTION_ECHO) != WebServer->httpServer.arg(input).toInt() || WebServer->httpServer.arg(input).toInt() == OFF_GPIO) {
    key = GPIO;
    key += ConfigESP->getGpio(1, FUNCTION_ECHO);
    ConfigManager->setElement(key.c_str(), NR, 0);
    ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_OFF);
    ConfigManager->setElement(key.c_str(), LEVEL, 0);
    ConfigManager->setElement(key.c_str(), MEMORY, 0);
  }
  ConfigESP->sort(FUNCTION_ECHO);
#endif

#ifdef SUPLA_DS18B20
  input = INPUT_MULTI_DS_GPIO;
  key = GPIO;
  key += WebServer->httpServer.arg(input).toInt();
  if (WebServer->httpServer.arg(input).toInt() != OFF_GPIO) {
    key = GPIO;
    key += WebServer->httpServer.arg(input).toInt();
    if (ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_OFF ||
        (ConfigESP->getGpio(1, FUNCTION_DS18B20) == WebServer->httpServer.arg(input).toInt() &&
         ConfigManager->get(key.c_str())->getElement(FUNCTION).toInt() == FUNCTION_DS18B20)) {
      ConfigManager->setElement(key.c_str(), NR, 1);
      ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_DS18B20);
    }
    else {
      WebServer->sendContent(supla_webpage_sensor(6));
      return;
    }
  }
  if (ConfigESP->getGpio(1, FUNCTION_DS18B20) != WebServer->httpServer.arg(input).toInt() || WebServer->httpServer.arg(input).toInt() == OFF_GPIO) {
    key = GPIO;
    key += ConfigESP->getGpio(1, FUNCTION_DS18B20);
    ConfigManager->setElement(key.c_str(), NR, 0);
    ConfigManager->setElement(key.c_str(), FUNCTION, FUNCTION_OFF);
    ConfigManager->setElement(key.c_str(), LEVEL, 0);
    ConfigManager->setElement(key.c_str(), MEMORY, 0);
  }
  if (strcmp(WebServer->httpServer.arg(INPUT_MAX_DS18B20).c_str(), "") > 0) {
    ConfigManager->set(KEY_MULTI_MAX_DS18B20, WebServer->httpServer.arg(INPUT_MAX_DS18B20).c_str());
  }
  ConfigESP->sort(FUNCTION_DS18B20);
#endif

  switch (ConfigManager->save()) {
    case E_CONFIG_OK:
      WebServer->sendContent(supla_webpage_sensor(1));
      // WebServer->rebootESP();
      break;
    case E_CONFIG_FILE_OPEN:
      //      Serial.println(F("E_CONFIG_FILE_OPEN: Couldn't open file"));
      WebServer->sendContent(supla_webpage_sensor(2));
      break;
  }
}

String SuplaWebPageSensor::supla_webpage_sensor(int save) {
  uint8_t nr, suported, selected;
  String page, key;

  page += WebServer->SuplaMetas();
  page += WebServer->SuplaStyle();
  page += WebServer->SuplaSaveResult(save);
  page += F("</div>");
  page += WebServer->SuplaJavaScript(PATH_SENSOR);
  page += F("<div class='s'>");
  //  page += WebServer->SuplaLogo();
  page += WebServer->SuplaSummary();
  page += F("<form method='post' action='");
  page += PATH_SAVE_SENSOR;
  page += F("'>");

#ifdef SUPLA_DHT11
  page += F("<div class='w'><h3>Ustawienie GPIO dla DHT11</h3>");
  page += F("<i><label>ILOŚĆ</label><input name='");
  page += INPUT_MAX_DHT11;
  page += F("' type='number' placeholder='0' step='1' min='0' max='");
  page += ConfigESP->countFreeGpio(FUNCTION_DHT11);
  page += F("' value='");
  page += String(ConfigManager->get(KEY_MAX_DHT11)->getValue());
  page += F("'></i>");
  for (nr = 1; nr <= ConfigManager->get(KEY_MAX_DHT11)->getValueInt(); nr++) {
    page += F("<i><label>");
    page += nr;
    page += F(". DHT11</label><select name='");
    page += INPUT_DHT11_GPIO;
    page += nr;
    page += F("'>");
    selected = ConfigESP->getGpio(nr, FUNCTION_DHT11);
    for (suported = 0; suported < 18; suported++) {
      if (ConfigESP->checkBusyGpio(suported, FUNCTION_DHT11) == false || selected == suported) {
        page += F("<option value='");
        page += suported;
        if (selected == suported) {
          page += F("' selected>");
        }
        else {
          page += F("'>");
        }
        page += (WebServer->Supported_Gpio[suported]);
      }
    }
    page += F("</select></i>");
  }
  page += F("</div>");
#endif

#ifdef SUPLA_DHT22
  page += F("<div class='w'><h3>Ustawienie GPIO dla DHT22</h3>");
  page += F("<i><label>ILOŚĆ</label><input name='");
  page += INPUT_MAX_DHT22;
  page += F("' type='number' placeholder='0' step='1' min='0' max='");
  page += ConfigESP->countFreeGpio(FUNCTION_DHT22);
  page += F("' value='");
  page += String(ConfigManager->get(KEY_MAX_DHT22)->getValue());
  page += F("'></i>");
  for (nr = 1; nr <= ConfigManager->get(KEY_MAX_DHT22)->getValueInt(); nr++) {
    page += F("<i><label>");
    page += nr;
    page += F(". DHT22</label><select name='");
    page += INPUT_DHT22_GPIO;
    page += nr;
    page += F("'>");
    selected = ConfigESP->getGpio(nr, FUNCTION_DHT22);
    for (suported = 0; suported < 18; suported++) {
      if (ConfigESP->checkBusyGpio(suported, FUNCTION_DHT22) == false || selected == suported) {
        page += F("<option value='");
        page += suported;
        if (selected == suported) {
          page += F("' selected>");
        }
        else {
          page += F("'>");
        }
        page += (WebServer->Supported_Gpio[suported]);
      }
    }
    page += F("</select></i>");
  }
  page += F("</div>");
#endif

#ifdef SUPLA_DS18B20
  page += F("<div class='w'><h3>Ustawienie GPIO dla Multi DS18B20</h3>");
  page += F("<i><label>ILOŚĆ</label><input name='");
  page += INPUT_MAX_DS18B20;
  page += F("' type='number' placeholder='1' step='1' min='0' max='");
  page += MAX_DS18B20;
  page += F("' value='");
  page += String(ConfigManager->get(KEY_MULTI_MAX_DS18B20)->getValue());
  page += F("'></i>");
  page += F("<i>");
  if (ConfigManager->get(KEY_MULTI_MAX_DS18B20)->getValueInt() > 1) {
    page += F("<label>");
    if (ConfigESP->getGpio(1, FUNCTION_DS18B20) != OFF_GPIO) {
      page += F("<a href='");
      page += PATH_START;
      page += PATH_MULTI_DS;
      page += F("'>");
    }
    page += F("MULTI DS18B20 ");
    if (ConfigESP->getGpio(1, FUNCTION_DS18B20) != OFF_GPIO) {
      page += WebServer->SuplaIconEdit();
      page += F("</a>");
    }
    page += F("</label>");
  }
  page += F("<select name='");
  page += INPUT_MULTI_DS_GPIO;
  page += F("'>");
  selected = ConfigESP->getGpio(1, FUNCTION_DS18B20);
  for (suported = 0; suported < 18; suported++) {
    if (ConfigESP->checkBusyGpio(suported, FUNCTION_DS18B20) == false || selected == suported) {
      page += F("<option value='");
      page += suported;
      if (selected == suported) {
        page += F("' selected>");
      }
      else {
        page += F("'>");
      }
      page += (WebServer->Supported_Gpio[suported]);
    }
  }
  page += F("</select></i>");

  /* if (ConfigManager->get(KEY_MULTI_MAX_DS18B20)->getValueInt() > 1) {
     page += F("<button type='submit'formaction='");
     page += PATH_START;
     page += PATH_MULTI_DS;
     page += F("'>Ustawienia DS18B20</button>");
    }*/

  page += F("</div>");
#endif

#if defined(SUPLA_BME280) || defined(SUPLA_SHT30) || defined(SUPLA_SI7021)
  page += F("<div class='w'><h3>Ustawienie GPIO dla i2c</h3>");
  page += F("<i><label>");
  page += F("SDA</label><select name='");
  page += INPUT_SDA_GPIO;
  page += F("'>");
  selected = ConfigESP->getGpio(1, FUNCTION_SDA);
  for (suported = 0; suported < 18; suported++) {
    if (ConfigESP->checkBusyGpio(suported, FUNCTION_SDA) == false || selected == suported) {
      page += F("<option value='");
      page += suported;
      if (selected == suported) {
        page += F("' selected>");
      }
      else {
        page += F("'>");
      }
      page += (WebServer->Supported_Gpio[suported]);
    }
  }
  page += F("</select></i>");
  page += F("<i><label>");
  page += F("SCL</label><select name='");
  page += INPUT_SCL_GPIO;
  page += F("'>");
  selected = ConfigESP->getGpio(1, FUNCTION_SCL);
  for (suported = 0; suported < 18; suported++) {
    if (ConfigESP->checkBusyGpio(suported, FUNCTION_SCL) == false || selected == suported) {
      page += F("<option value='");
      page += suported;
      if (selected == suported) {
        page += F("' selected>");
      }
      else {
        page += F("'>");
      }
      page += (WebServer->Supported_Gpio[suported]);
    }
  }
  page += F("</select></i>");
  
  if (ConfigESP->getGpio(1, FUNCTION_SDA) != OFF_GPIO && ConfigESP->getGpio(1, FUNCTION_SCL) != OFF_GPIO) {
#ifdef SUPLA_BME280
    page += F("<i style='border-bottom:none !important;'><label>");
    page += F("BME280 adres</label><select name='");
    page += INPUT_BME280;
    page += F("'>");

    selected = ConfigManager->get(KEY_ACTIVE_SENSOR)->getElement(SENSOR_BME280).toInt();
    for (suported = 0; suported < sizeof(SupportedBme280) / sizeof(char *); suported++) {
      page += F("<option value='");
      page += suported;
      if (selected == suported) {
        page += F("' selected>");
      }
      else {
        page += F("'>");
      }
      page += (SupportedBme280[suported]);
    }
    page += F("</select></i>");
    page += F("<i><input name='");
    page += INPUT_ALTITUDE_BME280;
    page += F("' value='");
    page += ConfigManager->get(KEY_ALTITUDE_BME280)->getValue();
    page += F("' ");
    page += F("><label>Wysokość m n.p.m.</label></i>");
#endif

#ifdef SUPLA_SHT30
    page += F("<i><label>");
    page += F("SHT30</label><select name='");
    page += INPUT_SHT30;
    page += F("'>");

    selected = ConfigManager->get(KEY_ACTIVE_SENSOR)->getElement(SENSOR_SHT30).toInt();
    for (suported = 0; suported < sizeof(SupportedSHT30) / sizeof(char *); suported++) {
      page += F("<option value='");
      page += suported;
      if (selected == suported) {
        page += F("' selected>");
      }
      else {
        page += F("'>");
      }
      page += (SupportedSHT30[suported]);
    }
    page += F("</select></i>");
#endif

#ifdef SUPLA_SI7021
    page += F("<i><label>");
    page += F("Si7021</label><select name='");
    page += INPUT_SI7021;
    page += F("'>");

    selected = ConfigManager->get(KEY_ACTIVE_SENSOR)->getElement(SENSOR_SI7021).toInt();
    for (suported = 0; suported < sizeof(SupportedSensorActivity) / sizeof(char *); suported++) {
      page += F("<option value='");
      page += suported;
      if (selected == suported) {
        page += F("' selected>");
      }
      else {
        page += F("'>");
      }
      page += (SupportedSensorActivity[suported]);
    }
    page += F("</select></i>");
#endif
  }
  page += F("</div>");
#endif

#ifdef SUPLA_HC_SR04
  page += F("<div class='w'><h3>Ustawienie GPIO dla HC-SR04</h3>");
  page += F("<i><label>");
  page += F("TRIG</label><select name='");
  page += INPUT_TRIG_GPIO;
  page += F("'>");
  selected = ConfigESP->getGpio(1, FUNCTION_TRIG);
  for (suported = 0; suported < 18; suported++) {
    if (ConfigESP->checkBusyGpio(suported, FUNCTION_TRIG) == false || selected == suported) {
      page += F("<option value='");
      page += suported;
      if (selected == suported) {
        page += F("' selected>");
      }
      else {
        page += F("'>");
      }
      page += (WebServer->Supported_Gpio[suported]);
    }
  }
  page += F("</select></i>");
  page += F("<i><label>");
  page += F("ECHO</label><select name='");
  page += INPUT_ECHO_GPIO;
  page += F("'>");
  selected = ConfigESP->getGpio(1, FUNCTION_ECHO);
  for (suported = 0; suported < 18; suported++) {
    if (ConfigESP->checkBusyGpio(suported, FUNCTION_ECHO) == false || selected == suported) {
      page += F("<option value='");
      page += suported;
      if (selected == suported) {
        page += F("' selected>");
      }
      else {
        page += F("'>");
      }
      page += (WebServer->Supported_Gpio[suported]);
    }
  }
  page += F("</select></i>");
  page += F("</div>");
#endif

  page += F("<button type='submit'>Zapisz</button></form>");
  page += F("<br>");

  page += F("<a href='");
  page += PATH_START;
  page += PATH_DEVICE_SETTINGS;
  page += F("'><button>Powrót</button></a></div>");
  return page;
}

#ifdef SUPLA_DS18B20
void SuplaWebPageSensor::handleSearchDS() {
  if (ConfigESP->configModeESP == NORMAL_MODE) {
    if (!WebServer->httpServer.authenticate(WebServer->www_username, WebServer->www_password))
      return WebServer->httpServer.requestAuthentication();
  }
  WebServer->sendContent(supla_webpage_search(0));
}

void SuplaWebPageSensor::handleDSSave() {
  if (ConfigESP->configModeESP == NORMAL_MODE) {
    if (!WebServer->httpServer.authenticate(WebServer->www_username, WebServer->www_password))
      return WebServer->httpServer.requestAuthentication();
  }
  for (uint8_t i = 0; i < ConfigManager->get(KEY_MULTI_MAX_DS18B20)->getValueInt(); i++) {
    String ds_key = KEY_DS;
    String ds_name_key = KEY_DS_NAME;
    ds_key += i;
    ds_name_key += i;

    String ds = F("dschlid");
    String ds_name = F("dsnameid");
    ds += i;
    ds_name += i;

    ConfigManager->set(ds_key.c_str(), WebServer->httpServer.arg(ds).c_str());
    ConfigManager->set(ds_name_key.c_str(), WebServer->httpServer.arg(ds_name).c_str());

    Supla::GUI::sensorDS[i]->setDeviceAddress(ConfigManager->get(ds_key.c_str())->getValueBin(MAX_DS18B20_ADDRESS));
  }

  switch (ConfigManager->save()) {
    case E_CONFIG_OK:
      // Serial.println(F("E_CONFIG_OK: Config save"));
      WebServer->sendContent(supla_webpage_search(1));
      // WebServer->rebootESP();
      break;
    case E_CONFIG_FILE_OPEN:
      //      Serial.println(F("E_CONFIG_FILE_OPEN: Couldn't open file"));
      WebServer->sendContent(supla_webpage_search(2));
      break;
  }
}

String SuplaWebPageSensor::supla_webpage_search(int save) {
  String content = "";
  uint8_t count = 0;
  uint8_t pin = ConfigESP->getGpio(1, FUNCTION_DS18B20);

  OneWire ow(pin);
  DallasTemperature sensors;
  DeviceAddress address;
  char strAddr[64];
  uint8_t i;

  content += WebServer->SuplaMetas();
  content += WebServer->SuplaStyle();
  //  content += WebServer->SuplaFavicon();
  content += WebServer->SuplaSaveResult(save);
  content += WebServer->SuplaJavaScript(PATH_MULTI_DS);
  content += F("<div class='s'>");
  //  content += WebServer->SuplaLogo();
  content += WebServer->SuplaSummary();
  content += F("<center>");
  if (ConfigESP->getGpio(1, FUNCTION_DS18B20) < OFF_GPIO || !Supla::GUI::sensorDS.empty()) {
    content += F("<form method='post' action='");
    content += PATH_SAVE_MULTI_DS;
    content += F("'>");
    this->showDS18B20(content);
    content += F("<button type='submit'>Zapisz</button></form>");
    content += F("<br>");
  }
  content += F("<form method='post' action='");
  content += PATH_SAVE_MULTI_DS;
  content += F("'>");
  content += F("<div class='w'>");
  content += F("<h3>Znalezione DS18b20</h3>");
  sensors.setOneWire(&ow);
  sensors.begin();
  if (sensors.isParasitePowerMode()) {
    supla_log(LOG_DEBUG, "OneWire(pin %d) Parasite power is ON", pin);
  }
  else {
    supla_log(LOG_DEBUG, "OneWire(pin %d) Parasite power is OFF", pin);
  }
  // report parasite power requirements
  for (i = 0; i < sensors.getDeviceCount(); i++) {
    if (!sensors.getAddress(address, i)) {
      supla_log(LOG_DEBUG, "Unable to find address for Device %d", i);
    }
    else {
      sprintf(strAddr, "%02X%02X%02X%02X%02X%02X%02X%02X", address[0], address[1], address[2], address[3], address[4], address[5], address[6],
              address[7]);
      supla_log(LOG_DEBUG, "Index %d - address %s", i, strAddr);

      content += F("<i><input name='dschlid");
      content += count;

      content += F("' value='");
      content += String(strAddr);
      content += F("' maxlength=");
      content += MAX_DS18B20_ADDRESS_HEX;
      content += F(" readonly><label></i>");

      count++;
    }
    delay(0);
  }

  if (count == 0) {
    content += F("<i><label>brak podłączonych czujników</label></i>");
  }
  content += F("</div>");
  content += F("</center>");
  content += F("<button type='submit'>Zapisz znalezione DS18b20</button></form>");
  content += F("<br><br>");
  content += F("<a href='");
  content += PATH_START;
  content += PATH_SENSOR;
  content += F("'><button>Powrót</button></a></div>");
  //  content += WebServer->SuplaCopyrightBar();

  return content;
}

void SuplaWebPageSensor::showDS18B20(String &content, bool readonly) {
  if (ConfigESP->getGpio(1, FUNCTION_DS18B20) < OFF_GPIO || !Supla::GUI::sensorDS.empty()) {
    content += F("<div class='w'>");
    content += F("<h3>Temperatura</h3>");
    for (uint8_t i = 0; i < ConfigManager->get(KEY_MULTI_MAX_DS18B20)->getValueInt(); i++) {
      String ds_key = KEY_DS;
      String ds_name_key = KEY_DS_NAME;
      ds_key += i;
      ds_name_key += i;

      double temp = Supla::GUI::sensorDS[i]->getValue();
      content += F("<i style='border-bottom:none !important;'><input name='dsnameid");
      content += i;
      content += F("' value='");
      content += String(ConfigManager->get(ds_name_key.c_str())->getValue());
      content += F("' maxlength=");
      content += MAX_DS18B20_NAME;
      if (readonly) {
        content += F(" readonly");
      }
      content += F("><label>");
      content += F("Nazwa ");
      content += i + 1;
      content += F("</label></i>");
      content += F("<i><input name='dschlid");
      content += i;
      content += F("' value='");
      content += String(ConfigManager->get(ds_key.c_str())->getValue());
      content += F("' maxlength=");
      content += MAX_DS18B20_ADDRESS_HEX;
      if (readonly) {
        content += F(" readonly");
      }
      content += F("><label>");
      if (temp != -275) {
        content += temp;
      }
      else {
        content += F("--.--");
      }
      content += F(" <b>&deg;C</b> ");
      content += F("</label></i>");
      delay(0);
    }
    content += F("</div>");
  }
}
#endif
