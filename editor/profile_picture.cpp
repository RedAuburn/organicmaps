#include "editor/profile_picture.hpp"

#include "platform/remote_file.hpp"
#include "platform/platform.hpp"
#include "platform/settings.hpp"

// Example pic_url: https://www.openstreetmap.org/rails/active_storage/representations/redirect/eyJfcmFpbHMiOnsiZGF0YSI6MzM1MjcxMDUsInB1ciI6ImJsb2JfaWQifX0=--957a2a99111c97d4b9f4181003c3e69713a221c6/eyJfcmFpbHMiOnsiZGF0YSI6eyJmb3JtYXQiOiJwbmciLCJyZXNpemVfdG9fbGltaXQiOlsxMDAsMTAwXX0sInB1ciI6InZhcmlhdGlvbiJ9fQ==--473f0190a9e741bb16565db85fe650d7b0a9ee69/cat.png
// Example hash: 473f0190a9e741bb16565db85fe650d7b0a9ee69

namespace editor
{
  const Platform & platform = GetPlatform();
  const std::string image_path = platform.WritablePathForFile(PROFILEPICTURE_FILENAME);

  std::string ProfilePicture::extractHashFromOsmUrl(const std::string& pic_url)
  {
    // Get image hash (hash start always includes '=--' but is usually '==--')
    const int hash_start = static_cast<int>(pic_url.rfind("=--") + 3);
    const int length = static_cast<int>(pic_url.rfind('/') - hash_start);
    return pic_url.substr(hash_start, length);
  }

  std::string ProfilePicture::getCachedFile()
  {
    bool image_exists = Platform::IsFileExistsByFullPath(image_path);

    if(image_exists)
      return image_path;
    else
      return {};
  }

  std::string ProfilePicture::getOnlineFile(const std::string& pic_url)
  {
    // See editor/server_api.cpp
    if(pic_url != "none")
    {
      // Get new hash
      std::string new_hash = extractHashFromOsmUrl(pic_url);
      // Get cached hash
      std::string current_hash;
      settings::StringStorage::Instance().GetValue(PROFILEPICTURE_HASH_TAG, current_hash);

      // Download new image
      if (new_hash != current_hash)
      {
        settings::StringStorage::Instance().SetValue(PROFILEPICTURE_HASH_TAG, std::move(new_hash));
        platform::RemoteFile remoteFile(pic_url);
        platform::RemoteFile::StartDownloadingHandler startHandler;
        platform::RemoteFile::ResultHandler resultHandler;
        remoteFile.DownloadAsync(image_path, std::move(startHandler), std::move(resultHandler));
      }
    }
    else
    {
      // User has removed profile picture online
      settings::StringStorage::Instance().DeleteKeyAndValue(PROFILEPICTURE_HASH_TAG);
      Platform::RemoveFileIfExists(image_path);
      return {};
    }
    return getCachedFile();
  }
}
