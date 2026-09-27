#include "CredentialsStorage.h"

const char *CredentialsStorage::NAMESPACE = "wifi_config";

bool CredentialsStorage::loadCredentials(String &ssid, String &password)
{
    Preferences prefs;
    if (!prefs.begin(NAMESPACE, true)) // Read-only mode
    {
        return false;
    }

    bool configured = prefs.getBool("configured", false);
    if (configured)
    {
        ssid = prefs.getString("ssid", "");
        password = prefs.getString("password", "");
    }
    prefs.end();
    return configured && ssid.length() > 0;
}

bool CredentialsStorage::saveCredentials(const String &ssid, const String &password)
{
    Preferences prefs;
    if (!prefs.begin(NAMESPACE, false)) // Read-write mode
    {
        return false;
    }

    size_t s1 = prefs.putString("ssid", ssid);
    size_t s2 = prefs.putString("password", password);
    bool s3 = prefs.putBool("configured", true);
    prefs.end();

    return (s1 > 0 && s3);
}

void CredentialsStorage::clearCredentials()
{
    Preferences prefs;
    if (prefs.begin(NAMESPACE, false))
    {
        prefs.clear();
        prefs.end();
    }
}

bool CredentialsStorage::hasCredentials()
{
    Preferences prefs;
    if (!prefs.begin(NAMESPACE, true))
    {
        return false;
    }
    bool configured = prefs.getBool("configured", false);
    prefs.end();
    return configured;
}
