#include "icb_gui.h"
#include "ic_media.h"

int adKutusu, soyadKutusu, telKutusu, emailKutusu, frmFoto;
ICBYTES resim;

struct Musteri {
    char ad[50];
    char soyad[50];
    char telefon[20];
    char email[50];
    char resimYolu[100];  // assets klasörüne kayýt edilen resmin yolu
};


void FotoYukle() {
    ICBYTES yol;
    ReadImage(OpenFileMenu(yol, "JPEG\0*.JPG\0BMP\0*.BMP\0"), resim);
    DisplayImage(frmFoto, resim);
}

void Kaydet() {
    // Buraya kayýt verilerini toplayýp veritabanýna yazacaðýz
    // Örn: GetText(adKutusu, veri.ad) gibi
}

void ICGUI_Create() {
    ICG_MWSize(500, 450);
    ICG_MWTitle("Müþteri Kayýt Sistemi");
}

void ICGUI_main() {
    adKutusu = ICG_SLEditSunken(20, 20, 300, 25, "Ad");
    soyadKutusu = ICG_SLEditSunken(20, 60, 300, 25, "Soyad");
    telKutusu = ICG_SLEditSunken(20, 100, 300, 25, "Telefon");
    emailKutusu = ICG_SLEditSunken(20, 140, 300, 25, "E-posta");

    ICG_Button(20, 180, 150, 30, "Fotoðraf Yükle", FotoYukle);
    frmFoto = ICG_FrameThick(200, 180, 100, 100);

    ICG_Button(20, 300, 150, 40, "Kaydet", Kaydet);
}
