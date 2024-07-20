#pragma once

#include <string>

namespace editor
{
  class ProfilePicture
  {
  public:
      static std::string getCachedFile();
      static std::string getOnlineFile(const std::string& pic_url);
  };
}
