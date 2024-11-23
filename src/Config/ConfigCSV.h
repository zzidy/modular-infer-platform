#ifndef CONFIGCSV_H
#define CONFIGCSV_H

#include "ConfigBase.h"

namespace Config {

class ConfigCSV : public ConfigBase {
 private:
  /* data */
 public:
  ConfigCSV(std::string sName, std::string sPath);
  ~ConfigCSV();
};

}  // namespace Config

#endif