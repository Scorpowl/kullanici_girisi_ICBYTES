#include <direct.h>
#include "icb_gui.h"
#include "ic_media.h"
#include <fstream>
#include <ctime>
#include "icbytes.h"
#include <string>
#include <algorithm>



int adKutusu, soyadKutusu, telKutusu, emailKutusu, frmFoto;
ICBYTES resim;

struct Musteri {
    char ad[50];
    char soyad[50];
    char telefon[20];
    char email[50];
    char resimYolu[100];  // assets klas�r�ne kay�t edilen resmin yolu
};

void Kaydet() {
    // Girdi verileri ICBYTES olarak tan�mlan�r
    ICBYTES ad, soyad, telefon, email, resimYolu;

    // GUI�den veri al
    GetText(adKutusu, ad);
    GetText(soyadKutusu, soyad);
    GetText(telKutusu, telefon);
    GetText(emailKutusu, email);

    // Manuel bir foto ismi ver (gerekirse zamanla de�i�tir)
    resimYolu = "assets/foto.icb";

    // Foto�raf� kaydet
    ICDEVICE foto;
    if (CreateFileDevice(foto, "assets/foto.icb")) {
        long long yazilanAdres = WriteICBYTES(foto, resim, 0);

        if (yazilanAdres <= 0) {
            MessageBoxA(NULL, "Resim yaz�lamad�!", "HATA", MB_OK | MB_ICONERROR);
        }
        else {
            MessageBoxA(NULL, "Resim ba�ar�yla yaz�ld�.", "Bilgi", MB_OK);
        }

        CloseDevice(foto);
    }
    else {
        MessageBoxA(NULL, "Foto�raf dosyas� a��lamad�!", "Hata", MB_OK | MB_ICONERROR);
    }


    // Veritaban� dosyas�na kay�t ekle
    ICDEVICE veriDosyasi;
    if (CreateFileDevice(veriDosyasi, "veri.bin")) {
        AppendMatrixToFile(veriDosyasi, ad);
        AppendMatrixToFile(veriDosyasi, soyad);
        AppendMatrixToFile(veriDosyasi, telefon);
        AppendMatrixToFile(veriDosyasi, email);
        AppendMatrixToFile(veriDosyasi, resimYolu);
        CloseDevice(veriDosyasi);
        MessageBoxA(NULL, "Kay�t ba�ar�yla eklendi", "Bilgi", MB_OK);
    }
    else {
        MessageBoxA(NULL, "Veritaban� dosyas� olu�turulamad�", "Hata", MB_OK | MB_ICONERROR);
    }
}



void FotoYukle() {
    ICBYTES yol;
    ReadImage(OpenFileMenu(yol, "JPEG\0*.JPG\0BMP\0*.BMP\0"), resim);
    DisplayImage(frmFoto, resim);
}

void Listele() {
    ICDEVICE dosya;
    if (!CreateFileReader(dosya, "veri.bin")) {
        MessageBoxA(NULL, "veri.bin a��lamad�", "Hata", MB_OK | MB_ICONERROR);
        return;
    }

    ICBYTES ad, soyad, tel, email, foto;
    int MLE = ICG_MLEdit(20, 20, 400, 300, "Kay�tlar", SCROLLBAR_V);
    int sayac = 1;

    while (ReadIntoMatrix(dosya, ad)) {
        ReadIntoMatrix(dosya, soyad);
        ReadIntoMatrix(dosya, tel);
        ReadIntoMatrix(dosya, email);
        ReadIntoMatrix(dosya, foto);

        ICG_printf(MLE, "Kay�t #%d:\n", sayac++);
        Print(MLE, ad);
        Print(MLE, soyad);
        Print(MLE, tel);
        Print(MLE, email);
        Print(MLE, foto);
        ICG_printf(MLE, "------------------------\n");
    }

    CloseDevice(dosya);
}


void ICGUI_Create() {
    _mkdir("assets"); // klas�r yoksa olu�turur, varsa hi�bir �ey yapmaz
    ICG_MWSize(500, 450);
    ICG_MWTitle("M��teri Kay�t Sistemi");
}

void ICGUI_main() {
    adKutusu = ICG_SLEditSunken(20, 20, 300, 25, "Ad");
    soyadKutusu = ICG_SLEditSunken(20, 60, 300, 25, "Soyad");
    telKutusu = ICG_SLEditSunken(20, 100, 300, 25, "Telefon");
    emailKutusu = ICG_SLEditSunken(20, 140, 300, 25, "E-posta");

    ICG_Button(20, 180, 150, 30, "Foto�raf Y�kle", FotoYukle);
    frmFoto = ICG_FrameThick(200, 180, 100, 100);

    ICG_Button(20, 300, 150, 40, "Kaydet", Kaydet);
    ICG_Button(200, 300, 150, 40, "Listele", Listele);

}
