#ifndef CREDENTIALS_STORAGE_H
#define CREDENTIALS_STORAGE_H

#include <Arduino.h>
#include <Preferences.h>

class CredentialsStorage
{
private:
    static const char *NAMESPACE;

public:
    static bool loadCredentials(String &ssid, String &password);
    static bool saveCredentials(const String &ssid, const String &password);
    static void clearCredentials();
    static bool hasCredentials();
};

#endif // CREDENTIALS_STORAGE_H
