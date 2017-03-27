//
// Created by larryhsiao on 3/26/17.
//

#include <gtest/gtest.h>
#include <upnp.h>
#include <cstring>

#define SUIT_NAME Descovery

TEST(SUIT_NAME, RegisterClient) {
    UpnpClient_Handle clientHandle = -1;
    UpnpInit(NULL, NULL);
    int resultCode = UpnpRegisterClient([](Upnp_EventType EventType, void *Event, void *Cookie) -> int {
        return 0;
    }, &clientHandle, &clientHandle);
    UpnpFinish();
    ASSERT_EQ(UPNP_E_SUCCESS, resultCode);
}

TEST(SUIT_NAME, UnregisterClient) {
    UpnpClient_Handle clientHandle = -1;
    UpnpInit(NULL, NULL);
    UpnpRegisterClient([](Upnp_EventType EventType, void *Event, void *Cookie) -> int {
        return 0;
    }, &clientHandle, &clientHandle);
    int resultCode = UpnpUnRegisterClient(clientHandle);
    UpnpFinish();
    ASSERT_EQ(UPNP_E_SUCCESS, resultCode);
}

TEST(SUIT_NAME, DescoveryEvent_DeviceFoundNoError) {
    UpnpClient_Handle clientHandle = -1;
    UpnpInit(NULL, NULL);
    UpnpRegisterClient([](Upnp_EventType EventType, void *Event, void *Cookie) -> int {
        switch (EventType) {
            case UPNP_DISCOVERY_SEARCH_RESULT: {
                struct Upnp_Discovery *discoveryEvent = (struct Upnp_Discovery *) Event;
                if (discoveryEvent->ErrCode != UPNP_E_SUCCESS) {
                    throw std::runtime_error("Discover result failed");
                }
                break;
            }
            default: {
                throw std::runtime_error("Invalid event");
            }
        }
        return 0;
    }, &clientHandle, &clientHandle);
    UpnpSearchAsync(clientHandle, 1, "ssdp:all", NULL);
    UpnpFinish();
    SUCCEED();
}

TEST(SUIT_NAME, DescoveryEvent_DownloadDescription) {
    UpnpClient_Handle clientHandle = -1;
    UpnpInit(NULL, NULL);
    UpnpRegisterClient([](Upnp_EventType EventType, void *Event, void *Cookie) -> int {
        switch (EventType) {
            case UPNP_DISCOVERY_SEARCH_RESULT: {
                struct Upnp_Discovery *discoveryEvent = (struct Upnp_Discovery *) Event;
                IXML_Document *DescDoc = NULL;
                int resultCode = UpnpDownloadXmlDoc(discoveryEvent->Location, &DescDoc);
                if (resultCode != UPNP_E_SUCCESS) {
                    throw std::runtime_error("Download description xml failed");
                }
                if (DescDoc) {
                    ixmlDocument_free(DescDoc);
                }
                break;
            }
            default: {
                throw std::runtime_error("Invalid event");
            }
        }
        return 0;
    }, &clientHandle, &clientHandle);
    UpnpSearchAsync(clientHandle, 1, "ssdp:all", NULL);
    UpnpFinish();
    SUCCEED();
}

TEST(SUIT_NAME, DescoveryEvent_SearchSpecificDevice) {
    UpnpClient_Handle clientHandle = -1;
    UpnpInit(NULL, NULL);
    UpnpRegisterClient([](Upnp_EventType EventType, void *Event, void *Cookie) -> int {
        switch (EventType) {
            case UPNP_DISCOVERY_SEARCH_RESULT: {
                struct Upnp_Discovery *discoveryEvent = (struct Upnp_Discovery *) Event;
                if (std::strcmp(discoveryEvent->DeviceType,"urn:schemas-upnp-org:device:WANDevice:1") != 0){
                    throw std::runtime_error("found device not expected.");
                }
                break;
            }
            default: {
                throw std::runtime_error("Invalid event");
            }
        }
        return 0;
    }, &clientHandle, &clientHandle);
    UpnpSearchAsync(clientHandle, 1, "urn:schemas-upnp-org:device:WANDevice:1", NULL);
    UpnpFinish();
}

TEST(SUIT_NAME, 002) {
    UpnpClient_Handle clientHandle = -1;
    UpnpInit(NULL, NULL);
    UpnpRegisterClient([](Upnp_EventType EventType, void *Event, void *Cookie) -> int {
        switch (EventType) {
            case UPNP_DISCOVERY_SEARCH_RESULT: {
                struct Upnp_Discovery *discoveryEvent = (struct Upnp_Discovery *) Event;
                int ret;
                if (discoveryEvent->ErrCode != UPNP_E_SUCCESS) {
                    std::cout << "Error in Discovery Callback -- %d" << std::to_string(discoveryEvent->ErrCode)
                              << std::endl;
                }
                IXML_Document *DescDoc = NULL;
                ret = UpnpDownloadXmlDoc(discoveryEvent->Location, &DescDoc);
                if (ret != UPNP_E_SUCCESS) {
//                    std::cout << "Error obtaining device description from %s -- error = %d" << discoveryEvent->Location
//                              << std::endl;
                } else {
//                    std::cout << "Device found " << std::endl;
//                    std::cout << discoveryEvent->Location << std::endl;
//                    std::cout << discoveryEvent->DeviceType << std::endl;
                }
                if (DescDoc) {
                    ixmlDocument_free(DescDoc);
                }
                break;
            }
            default: {
                return 1;
            }
        }
        return 0;
    }, &clientHandle, &clientHandle);
    UpnpSearchAsync(clientHandle, 1, "ssdp:all", NULL);
    UpnpFinish();
}

