//
// Created by larryhsiao on 3/26/17.
//

#include <gtest/gtest.h>
#include <upnp.h>

#define SUIT_NAME Descovery

TEST(SUIT_NAME, 01) {
    UpnpClient_Handle clientHandle = -1;
    UpnpInit(NULL, NULL);
    UpnpRegisterClient([](Upnp_EventType EventType, void *Event, void *Cookie) -> int {
        switch (EventType) {
            case UPNP_DISCOVERY_SEARCH_RESULT: {
                struct Upnp_Discovery* discoveryEvent = (struct Upnp_Discovery *) Event;
                int ret;
                if (discoveryEvent->ErrCode != UPNP_E_SUCCESS) {
                    std::cout << "Error in Discovery Callback -- %d" << std::to_string(discoveryEvent->ErrCode) << std::endl;
                }
                IXML_Document *DescDoc = NULL;
                ret = UpnpDownloadXmlDoc(discoveryEvent->Location, &DescDoc);
                if (ret != UPNP_E_SUCCESS) {
                    std::cout << "Error obtaining device description from %s -- error = %d" << discoveryEvent->Location
                              << std::endl;
                } else {
                    std::cout << "Device found " << std::endl;
                    std::cout << discoveryEvent->Location << std::endl;
                    std::cout << discoveryEvent->DeviceType << std::endl;
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

