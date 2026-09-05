//#ifndef SRC_HTTP_HTTPCLIENT_H_
//#define SRC_HTTP_HTTPCLIENT_H_

#include <mutex>
#include <string>

#include "Curl.h"
#include "../Settings.h"
//#include "../sql/ParameterDB.h"
#include "HttpStatusCodeHandler.h"

class HttpClient
{
public:
  HttpClient(CSettings* settings);
  ~HttpClient();
//  std::string HttpGetCached(const std::string& url, time_t cacheDuration, int &statusCode);
  std::string HttpGet(const std::string& url, int &statusCode);
  std::string HttpDelete(const std::string& url, int &statusCode);
  std::string HttpPost(const std::string& url, const std::string& postData, int &statusCode);
//  bool GetDeviceFromSerial();
  bool RefreshToken();
  bool RefreshSSToken();
  bool RefreshGenericToken();
  void ClearSession();
  void SetApi(const std::string& api);
  void SetSupportApi(const std::string& supportApi);
  std::string GetUUID();
   void SetStatusCodeHandler(HttpStatusCodeHandler* statusCodeHandler) {
    m_statusCodeHandler = statusCodeHandler;
  }
private:
  std::string HttpRequest(const std::string& action, const std::string& url, const std::string& postData, int &statusCode);
  std::string HttpRequestToCurl(Curl &curl, const std::string& action, const std::string& url, const std::string& postData, int &statusCode);
  // Picks the token this URL authenticates with and puts the matching auth
  // headers on the request, returning that token (empty when it falls back to
  // Basic auth) so the 401 path can tell a genuinely stale token from one
  // another thread has already replaced. Caller must hold m_authMutex: the
  // tokens live in the settings, which the refresh paths write to.
  std::string ApplyAuthHeadersLocked(Curl& curl, const std::string& url, std::string& authMode);
  // Refreshes whichever of the three tokens the given URL uses.
  // Caller must hold m_authMutex.
  bool RefreshTokenForUrlLocked(const std::string& url);
  std::string GenerateUUID();
  // The EPG prefetch has several requests in flight at once, so everything
  // touching the shared token state has to be serialised -- otherwise two
  // threads hitting a 401 together both refresh, and the second refresh uses
  // the refresh token the first one has already consumed.
  std::mutex m_authMutex;
  std::string m_uuid;
  std::string m_api;
  std::string m_supportApi;
  int m_platform;
  /*
  std::string client_id;
  std::string client_secret;
  */
  CSettings* m_settings;
  HttpStatusCodeHandler *m_statusCodeHandler = nullptr;
};

//#endif /* SRC_HTTP_HTTPCLIENT_H_ */
