#include <jni.h>

#include "app/organicmaps/core/jni_helper.hpp"
#include "app/organicmaps/Framework.hpp"

#include "base/logging.hpp"
#include "base/string_utils.hpp"
#include "base/timer.hpp"

#include "editor/osm_auth.hpp"
#include "editor/server_api.hpp"

#include "platform/remote_file.hpp"

namespace
{
using namespace osm;
using namespace jni;

bool LoadOsmUserPreferences(std::string const & oauthToken, UserPreferences & outPrefs)
{
  try
  {
    ServerApi06 const api(OsmOAuth::ServerAuth(oauthToken));
    outPrefs = api.GetUserPreferences();
    return true;
  }
  catch (std::exception const & ex)
  {
    LOG(LWARNING, ("Can't load user preferences from server: ", ex.what()));
  }
  return false;
}
}  // namespace

extern "C"
{

JNIEXPORT jstring JNICALL
Java_app_organicmaps_editor_OsmOAuth_nativeAuthWithPassword(JNIEnv * env, jclass clazz,
                                                                jstring login, jstring password)
{
  OsmOAuth auth = OsmOAuth::ServerAuth();
  try
  {
    if (auth.AuthorizePassword(ToNativeString(env, login), ToNativeString(env, password)))
      return ToJavaString(env, auth.GetAuthToken());
    LOG(LWARNING, ("nativeAuthWithPassword: invalid login or password."));
  }
  catch (std::exception const & ex)
  {
    LOG(LWARNING, ("nativeAuthWithPassword error ", ex.what()));
  }
  return nullptr;
}

JNIEXPORT jstring JNICALL
Java_app_organicmaps_editor_OsmOAuth_nativeGetOsmUsername(JNIEnv * env, jclass, jstring oauthToken)
{
  UserPreferences prefs;
  if (LoadOsmUserPreferences(jni::ToNativeString(env, oauthToken), prefs))
    return jni::ToJavaString(env, prefs.m_displayName);
  return nullptr;
}

JNIEXPORT jint JNICALL
Java_app_organicmaps_editor_OsmOAuth_nativeGetOsmChangesetsCount(JNIEnv * env, jclass, jstring oauthToken)
{
  UserPreferences prefs;
  if (LoadOsmUserPreferences(jni::ToNativeString(env, oauthToken), prefs))
    return prefs.m_changesets;
  return -1;
}

JNIEXPORT jstring JNICALL
Java_app_organicmaps_editor_OsmOAuth_nativeGetOsmProfilePicture(JNIEnv * env, jclass, jstring oauthToken, jstring appDir)
{
  UserPreferences prefs;
  std::string imageName = jni::ToNativeString(env, appDir);
  if (LoadOsmUserPreferences(jni::ToNativeString(env, oauthToken), prefs))
  {
    //leave it to platform to interpret filetype
    std::string filePath = imageName + "/profile_picture";
    //looks like downloader can't handle 302 redirects used by osm api
    //std::string imageUrl = prefs.m_imageUrl;
    std::string imageUrl = "https://avatars.githubusercontent.com/u/26939824?s=400&u=fea34225120af76242ece8e8df62f3ad9a47e886&v=4";

    platform::RemoteFile remoteImage = platform::RemoteFile(imageUrl, {}, {}, true);
    platform::RemoteFile::StartDownloadingHandler startDownloadingHandler;
    auto result = remoteImage.Download(filePath);

        LOG(LWARNING,("Loading profile picture",result.m_url));
        if (result.m_status == platform::RemoteFile::Status::Ok)
          return jni::ToJavaString(env, filePath);
        else
          LOG(LWARNING,("Unable to load image: response code", result.m_httpCode));
    }
  return nullptr;
}


JNIEXPORT jstring JNICALL
Java_app_organicmaps_editor_OsmOAuth_nativeGetHistoryUrl(JNIEnv * env, jclass, jstring user)
{
  return jni::ToJavaString(env, OsmOAuth::ServerAuth().GetHistoryURL(jni::ToNativeString(env, user)));
}
} // extern "C"
