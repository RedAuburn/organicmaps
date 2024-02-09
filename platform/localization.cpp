#include "platform/localization.hpp"
#include "platform/get_text_by_id.hpp"
#include "platform/measurement_utils.hpp"
#include "platform/settings.hpp"

#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>

namespace platform
{
namespace
{
enum class MeasurementType
{
  Distance = 0,
  Speed,
  Altitude
};

const LocalizedUnits & GetLocalizedUnits(measurement_utils::Units units, MeasurementType measurementType)
{
  static LocalizedUnits UnitsLengthImperial = {GetLocalizedString("ft"), GetLocalizedString("mi")};
  static LocalizedUnits UnitsLengthMetric = {GetLocalizedString("m"), GetLocalizedString("km")};

  static LocalizedUnits UnitsSpeedImperial = {GetLocalizedString("ft"), GetLocalizedString("miles_per_hour")};
  static LocalizedUnits UnitsSpeedMetric = {GetLocalizedString("m"), GetLocalizedString("kilometers_per_hour")};

  switch (measurementType)
  {
  case MeasurementType::Distance:
  case MeasurementType::Altitude:
    switch (units)
    {
    case measurement_utils::Units::Imperial: return UnitsLengthImperial;
    case measurement_utils::Units::Metric: return UnitsLengthMetric;
    }
    break;
  case MeasurementType::Speed:
    switch (units)
    {
    case measurement_utils::Units::Imperial: return UnitsSpeedImperial;
    case measurement_utils::Units::Metric: return UnitsSpeedMetric;
    }
  }
  UNREACHABLE();
}

std::string parseLocalizationString(std::string locale, std::string key)
{
  std::string buildPath;
  buildPath = filesystem::exists("/res/values/") ? "true" : "False";

  std::ifstream stringsFile("android/app/src/main/res/values-hi/strings.xml");
  std::stringstream buffer;
  buffer << stringsFile.rdbuf();
  return buildPath;
}

}  // namespace

LocalizedUnits GetLocalizedDistanceUnits()
{
  return GetLocalizedUnits(measurement_utils::GetMeasurementUnits(), MeasurementType::Distance);
}

LocalizedUnits GetLocalizedAltitudeUnits()
{
  return GetLocalizedUnits(measurement_utils::GetMeasurementUnits(), MeasurementType::Altitude);
}

const std::string & GetLocalizedSpeedUnits(measurement_utils::Units units)
{
  return GetLocalizedUnits(units, MeasurementType::Speed).m_high;
}

std::string GetLocalizedSpeedUnits()
{
  return GetLocalizedSpeedUnits(measurement_utils::GetMeasurementUnits());
}

//old stuff

std::string GetLocalizedTypeName(std::string const & poiType)
{
  return parseLocalizationString("en", "hello");
  //return poiType;
}

std::string GetLocalizedBrandName(std::string const & brand)
{
  return "wowlocalizedbrandname";
}

std::string GetLocalizedString(std::string const & key)
{
  return languages::GetCurrentTwine(key);
  return "wowlocalizedstring";
}

std::string GetCurrencySymbol(std::string const & currencyCode)
{
  return "wowcurrencysymbol";
}

std::string GetLocalizedMyPositionBookmarkName()
{
  return "wowlocalizedpositionbookmarkname";
}

}  // namespace platform
