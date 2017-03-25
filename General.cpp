#include <gtest/gtest.h>
#include <upnp.h>

#define SUIT_TEST UPNP

TEST(SUIT_TEST, Initial) {
    int initialResultCode = UpnpInit(NULL, 0);
    UpnpFinish();
    ASSERT_EQ(UPNP_E_SUCCESS, initialResultCode);
}

TEST(SUIT_TEST, Finish) {
    UpnpInit(NULL, 0);
    int finalizeResultCode = UpnpFinish();
    ASSERT_EQ(UPNP_E_SUCCESS, finalizeResultCode);
}

//TEST(SUIT_TEST, ControlPoint){
//    int rc;
//    ithread_t cmdloop_thread;
//    int code;
//    rc = TvCtrlPointStart(linux_print, NULL, 0);
//    if (rc != TV_SUCCESS) {
//        SampleUtil_Print("Error starting UPnP TV Control Point\n");
//        ASSERT_EQ(UPNP_E_SUCCESS, rc);
//    }
//    /* start a command loop thread */
//    code = ithread_create(&cmdloop_thread, NULL, TvCtrlPointCommandLoop, NULL);
//    if (code !=  0) {
//        FAIL();
//    }
//    rc = TvCtrlPointStop();
//
//    ASSERT_EQ(UPNP_E_SUCCESS, rc);
//}