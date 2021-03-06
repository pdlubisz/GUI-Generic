/*
  Copyright (C) krycha88

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "esp_wifi.h"

#include "SuplaDeviceGUI.h"
#include "SuplaConfigManager.h"

namespace Supla {
namespace GUI {
void begin() {

#ifdef DEBUG_MODE
  new Supla::Sensor::EspFreeHeap();
#endif

  Supla::ESPWifi *wifi = new Supla::ESPWifi(
    ConfigManager->get(KEY_WIFI_SSID)->getValue(),
    ConfigManager->get(KEY_WIFI_PASS)->getValue());

  wifi->enableBuffer(true);
  wifi->enableSSL(true);

  String supla_hostname = ConfigManager->get(KEY_HOST_NAME)->getValue();
  supla_hostname.replace(" ", "_");
  wifi->setHostName(supla_hostname.c_str());

  SuplaDevice.setName((char*)ConfigManager->get(KEY_HOST_NAME)->getValue());

  SuplaDevice.begin((char*)ConfigManager->get(KEY_SUPLA_GUID)->getValue(),      // Global Unique Identifier
                    (char*)ConfigManager->get(KEY_SUPLA_SERVER)->getValue(),    // SUPLA server address
                    (char*)ConfigManager->get(KEY_SUPLA_EMAIL)->getValue(),     // Email address used to login to Supla Cloud
                    (char*)ConfigManager->get(KEY_SUPLA_AUTHKEY)->getValue());  // Authorization key

  ConfigManager->showAllValue();
  WebServer->begin();
}

void addRelayButton(int pinRelay, int pinButton, bool highIsOn) {
  relay.push_back(new Supla::Control::Relay(pinRelay, highIsOn));
  button.push_back(new Supla::Control::Button(pinButton, true));
  int size = relay.size() - 1;

  if (ConfigManager->get(KEY_TYPE_BUTTON)->getValueElement(size)) {
    //BISTABILNY
    button[size]->addAction(Supla::TOGGLE, *relay[size], Supla::ON_CHANGE);
  } else {
    //MONOSTABILNY
    button[size]->addAction(Supla::TOGGLE, *relay[size], ConfigManager->get(KEY_MONOSTABLE_TRIGGER)->getValueInt());
  }
}

void addDS18B20MultiThermometer(int pinNumber) {
  for (int i = 0; i < ConfigManager->get(KEY_MULTI_MAX_DS18B20)->getValueInt(); ++i) {
    String ds_key = KEY_DS;
    ds_key += i;
    sensorDS.push_back(new DS18B20(pinNumber, ConfigManager->get(ds_key.c_str())->getValueBin(MAX_DS18B20_ADDRESS)));
    supla_log(LOG_DEBUG, "Index %d - address %s", i, ConfigManager->get(ds_key.c_str())->getValue());
  }
}

void addConfigESP(int pinNumberConfig, int pinLedConfig, int modeConfigButton, bool highIsOn) {
  ConfigESP->addConfigESP(pinNumberConfig, pinLedConfig, modeConfigButton, highIsOn);
}

void addRolleShutter(int pinRelayUp, int pinRelayDown, int pinButtonUp, int pinButtonDown, bool highIsOn) {
  RollerShutterRelay.push_back(new Supla::Control::RollerShutter(pinRelayUp, pinRelayDown, highIsOn));
  if(pinButtonUp != OFF_GPIO) RollerShutterButtonOpen.push_back(new Supla::Control::Button(pinButtonUp, true, true));
  if(pinButtonDown != OFF_GPIO) RollerShutterButtonClose.push_back(new Supla::Control::Button(pinButtonDown, true, true));
  int size = RollerShutterRelay.size() - 1;
  if(pinButtonUp != OFF_GPIO && pinButtonDown != OFF_GPIO){
      RollerShutterButtonOpen[size]->addAction(Supla::OPEN_OR_STOP, *RollerShutterRelay[size], Supla::ON_PRESS);
      RollerShutterButtonClose[size]->addAction(Supla::CLOSE_OR_STOP, *RollerShutterRelay[size], Supla::ON_PRESS);
  }
  else RollerShutterButtonOpen[size]->addAction(Supla::STEP_BY_STEP, *RollerShutterRelay[size], Supla::ON_PRESS);
}

std::vector <Supla::Control::Relay *> relay;
std::vector <Supla::Control::Button *> button;
std::vector <DS18B20 *> sensorDS;
std::vector <Supla::Control::RollerShutter *> RollerShutterRelay;
std::vector <Supla::Control::Button *> RollerShutterButtonOpen;
std::vector <Supla::Control::Button *> RollerShutterButtonClose;
}
}

SuplaConfigManager *ConfigManager = new SuplaConfigManager();
SuplaConfigESP *ConfigESP = new SuplaConfigESP();
SuplaWebServer *WebServer = new SuplaWebServer();
