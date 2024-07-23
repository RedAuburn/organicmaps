#pragma once

#include <string>

namespace editor
{
  class ProfilePicture
  {
  private:
      static std::string extractHashFromOsmUrl(const std::string& pic_url);
  public:
      static std::string getCachedFile();
      static std::string getOnlineFile(const std::string& pic_url);
  };
}
