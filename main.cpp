#include "icb_gui.h"
#include "icbytes.h"
//#include "icbimage.h"

int NAME, EMAIL, PHONE, REASON, PHOTO_FRAME;
ICBYTES photo;

void ICGUI_Create() {
    ICG_MWSize(600, 400);
    ICG_MWTitle("Customer Registration");
}

void CapturePhoto() {
    ICDEVICE cam;
    CreateDXCam(cam, 0);
    CaptureImage(cam, photo);
    DisplayImage(PHOTO_FRAME, photo);
    CloseDevice(cam);
}

void RegisterUser() {
    ICBYTES name, email, phone, reason;
    GetText(NAME, name);
    GetText(EMAIL, email);
    GetText(PHONE, phone);
    GetText(REASON, reason);

    KeyMap userData;
    SetKey(userData, "Name", name);
    SetKey(userData, "Email", email);
    SetKey(userData, "Phone", phone);
    SetKey(userData, "Reason", reason);
    SetKey(userData, "Photo", photo);

    SaveKeyMap("users.kdb", userData, APPEND_MODE);
    ICG_printf(NAME, "User Registered Successfully!");
}

void ShowUsers() {
    KeyMap userData;
    LoadKeyMap("users.kdb", userData);

    ICBYTES name, email;
    for (int i = 0; i < 3; i++) {
        GetKey(userData, i, "Name", name);
        GetKey(userData, i, "Email", email);
        ICG_printf(NAME, name + " - " + email);
    }
}

void ICGUI_main() {
    ICG_Static(10, 10, 100, 25, "Name:");
    NAME = ICG_SLEditSunken(120, 10, 200, 25, "");

    ICG_Static(10, 50, 100, 25, "Email:");
    EMAIL = ICG_SLEditSunken(120, 50, 200, 25, "");

    ICG_Static(10, 90, 100, 25, "Phone:");
    PHONE = ICG_SLEditSunken(120, 90, 200, 25, "");

    ICG_Static(10, 130, 100, 25, "Reason:");
    REASON = ICG_SLEditSunken(120, 130, 200, 25, "");

    PHOTO_FRAME = ICG_FrameMedium(350, 10, 200, 200);
    ICG_Button(10, 170, 150, 30, "Capture Photo", CapturePhoto);
    ICG_Button(10, 220, 150, 30, "Register", RegisterUser);
    ICG_Button(10, 270, 150, 30, "Show Users", ShowUsers);
} 