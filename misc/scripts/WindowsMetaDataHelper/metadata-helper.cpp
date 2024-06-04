#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "metadata-helper.h"
#include <memory>
#include <vector>
#include <algorithm>
#include <iterator>
#include <regex>
#include <iostream>
#include <sstream>
#include <functional>
#include <map>
#include <memory>

#include <Windows.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

#ifdef _MSC_VER
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

struct device_id {
    std::string pid;
    std::string vid;
};
inline bool equal(const std::string &a, const std::string &b) {
    return strcasecmp(a.c_str(), b.c_str()) == 0;
}
inline bool operator==(const device_id &a, const device_id &b) {
    return equal(a.pid, b.pid) && equal(a.vid, b.vid);
}

static int stringHex_to_int(std::string str) {
    int nRet  = 0;
    int count = 1;
    if(!str.empty()) {
        auto iter = str.end() - 1;
        while(true) {
            int nNum = 0;
            if(*iter >= '0' && *iter <= '9') {
                nNum = *iter - '0';
            }
            else if(*iter >= 'A' && *iter <= 'F') {
                nNum = *iter - 'A' + 10;
            }
            else if(*iter >= 'a' && *iter <= 'f') {
                nNum = *iter - 'a' + 10;
            }
            else {
                break;
            }
            nRet += nNum * count;
            count *= 16;

            if(iter == str.begin()) {
                break;
            }
            else {
                iter--;
            }
        }
    }
    return nRet;
}

static bool parse_device_id(const std::string &id, device_id *res) {
    static const std::regex                         regex("vid_([0-9a-f]+)&pid_([0-9a-f]+)&mi_([0-9]+)#[0-9a-f]&([0-9a-f]+)", std::regex_constants::icase);
    std::match_results<std::string::const_iterator> match;
    if(std::regex_search(id, match, regex) && match.size() > 4) {
        res->vid = match[1];
        res->pid = match[2];
        return true;
    }
    return false;
}

static void foreach_device_path(const std::vector<device_id> &devices, std::function<void(const device_id &, std::wstring)> action) {
    std::map<std::string, std::vector<device_id>> guid_to_devices;
    for(auto &&d: devices) {
        guid_to_devices["e5323777-f976-4f5b-9b55-b94699c46e44"].push_back(d);
        guid_to_devices["65E8773D-8F56-11D0-A3B9-00A0C9223196"].push_back(d);
    }
    for(auto &&kvp: guid_to_devices) {
        auto              guid = kvp.first;
        std::stringstream ss;
        ss << "SYSTEM\\CurrentControlSet\\Control\\DeviceClasses\\{" << guid << "}";
        auto         s = ss.str();
        std::wstring prefix(s.begin(), s.end());
        HKEY         key;
        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, prefix.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &key) == ERROR_SUCCESS) {
            // Don't forget to release in the end:
            std::shared_ptr<void> raii(key, RegCloseKey);
            TCHAR                 achClass[MAX_PATH] = TEXT("");                    // buffer for class name
            DWORD                 cchClassName       = MAX_PATH;                    // size of class string
            DWORD                 cSubKeys           = 0;                           // number of subkeys
            DWORD                 cbMaxSubKey;                                      // longest subkey size
            DWORD                 cchMaxClass;                                      // longest class string
            DWORD                 cValues;                                          // number of values for key
            DWORD                 cchMaxValue;                                      // longest value name
            DWORD                 cbMaxValueData;                                   // longest value data
            DWORD                 cbSecurityDescriptor;                             // size of security descriptor
            FILETIME              ftLastWriteTime;                                  // last write time
            DWORD                 retCode = RegQueryInfoKey(key,                    // key handle
                                                            achClass,               // buffer for class name
                                                            &cchClassName,          // size of class string
                                                            NULL,                   // reserved
                                                            &cSubKeys,              // number of subkeys
                                                            &cbMaxSubKey,           // longest subkey size
                                                            &cchMaxClass,           // longest class string
                                                            &cValues,               // number of values for this key
                                                            &cchMaxValue,           // longest value name
                                                            &cbMaxValueData,        // longest value data
                                                            &cbSecurityDescriptor,  // security descriptor
                                                            &ftLastWriteTime);      // last write time
            for(auto i = 0ul; i < cSubKeys; i++) {
                TCHAR achKey[MAX_KEY_LENGTH];
                DWORD cbName = MAX_KEY_LENGTH;
                retCode      = RegEnumKeyEx(key, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
                if(retCode == ERROR_SUCCESS) {
                    std::wstring suffix = achKey;
                    device_id    rdid;
                    std::string  id = std::string(suffix.begin(), suffix.end());
                    if(parse_device_id(id, &rdid)) {
                        for(auto &&did: kvp.second) {
                            if(stringHex_to_int(rdid.pid) == std::atoi(did.pid.c_str()) && stringHex_to_int(rdid.vid) == std::atoi(did.vid.c_str())) {
                                std::wstringstream ss;
                                ss << prefix << "\\" << suffix << "\\#GLOBAL\\Device Parameters";
                                auto path = ss.str();
                                action(rdid, path);
                            }
                        }
                    }
                }
            }
        }
    }
}
bool windows_metadata_helper::is_running_as_admin() {
    BOOL                     result      = FALSE;
    PSID                     admin_group = NULL;
    SID_IDENTIFIER_AUTHORITY ntauthority = SECURITY_NT_AUTHORITY;
    if(!AllocateAndInitializeSid(&ntauthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &admin_group)) {
        return false;
    }
    std::shared_ptr<void> raii(admin_group, FreeSid);
    if(!CheckTokenMembership(NULL, admin_group, &result)) {
        return false;
    }
    return result;
}

bool windows_metadata_helper::is_enabled(std::string pid, std::string vid) {
    bool      res = true;
    device_id did{ pid, vid };
    foreach_device_path({ did }, [&res, did](const device_id &, std::wstring path) {
        HKEY key;
        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, path.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &key) == ERROR_SUCCESS) {
            std::shared_ptr<void> raii(key, RegCloseKey);
            bool                  found = true;
            DWORD                 len   = sizeof(DWORD);
            std::wstringstream    ss;
            ss << L"MetadataBufferSizeInKB0";
            std::wstring metadatakey            = ss.str();
            DWORD        MetadataBufferSizeInKB = 0;
            RegQueryValueEx(key, metadatakey.c_str(), NULL, NULL, (LPBYTE)(&MetadataBufferSizeInKB), &len);
            found = found && MetadataBufferSizeInKB;
            res &= found;
        }
    });
    return res;
}
void windows_metadata_helper::enable_metadata(std::string pid, std::string vid) {
    if(is_running_as_admin()) {
        std::vector<device_id> dids{ { pid, vid } };
        bool                   failure = false;
        foreach_device_path(dids, [&failure](const device_id &did, std::wstring path) {
            HKEY key;
            if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, path.c_str(), 0, KEY_WRITE | KEY_WOW64_64KEY, &key) == ERROR_SUCCESS) {
                std::shared_ptr<void> raii(key, RegCloseKey);
                bool                  found = true;
                DWORD                 len   = sizeof(DWORD);
                std::wstringstream    ss;
                bool                  success     = true;
                const int             max_pin_num = 3;
                for(int i = 0; i < max_pin_num; i++) {
                    ss.str(L"");
                    ss << L"MetadataBufferSizeInKB" << i;
                    std::wstring metadatakey            = ss.str();
                    DWORD        MetadataBufferSizeInKB = 5;
                    if(RegSetValueEx(key, metadatakey.c_str(), 0, REG_DWORD, (const BYTE *)&MetadataBufferSizeInKB, sizeof(DWORD)) != ERROR_SUCCESS) {
                        success = false;
                        continue;
                    }
                }
            }
        });
    }
}