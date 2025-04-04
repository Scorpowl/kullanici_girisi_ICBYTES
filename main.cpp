#include <direct.h>
#include "icb_gui.h"
#include "ic_media.h"
#include <fstream>
#include <ctime>
#include "icbytes.h"
#include <string>
#include <algorithm>
#include <io.h>

int adKutusu, soyadKutusu, telKutusu, emailKutusu, frmFoto;
ICBYTES resim;

struct Musteri {
    char ad[50];
    char soyad[50];
    char telefon[20];
    char email[50];
    char resimYolu[100];  // assets klasörüne kayýt edilen resmin yolu
};

void Kaydet() {
    // Girdi verileri ICBYTES olarak tanýmlanýr
    ICBYTES ad, soyad, telefon, email, resimYolu;

    // GUI’den veri al
    GetText(adKutusu, ad);
    GetText(soyadKutusu, soyad);
    GetText(telKutusu, telefon);
    GetText(emailKutusu, email);

    // Manuel bir foto ismi ver (gerekirse zamanla deðiþtir)
    resimYolu = "assets/foto.icb";

    

    // Fotoðrafý kaydet
    ICDEVICE foto;
    if (CreateFileDevice(foto, "assets/foto.icb")) {
        long long yazilan = WriteICBYTES(foto, resim, 0);
        CloseDevice(foto);

        if (yazilan <= 0)
            MessageBoxA(NULL, "Fotoðraf yazýlamadý!", "Hata", MB_OK | MB_ICONERROR);
    }
    else {
        MessageBoxA(NULL, "Fotoðraf dosyasý oluþturulamadý!", "Hata", MB_OK | MB_ICONERROR);
        return;
    }

    // Veritabaný dosyasýna kayýt ekle
    ICDEVICE veriDosyasi;
    if (CreateFileDevice(veriDosyasi, "veri.bin")) {
        AppendMatrixToFile(veriDosyasi, ad);
        AppendMatrixToFile(veriDosyasi, soyad);
        AppendMatrixToFile(veriDosyasi, telefon);
        AppendMatrixToFile(veriDosyasi, email);
        AppendMatrixToFile(veriDosyasi, resimYolu);
        CloseDevice(veriDosyasi);
        MessageBoxA(NULL, "Kayýt baþarýyla eklendi", "Bilgi", MB_OK);
    }
    else {
        MessageBoxA(NULL, "Veritabaný dosyasý oluþturulamadý", "Hata", MB_OK | MB_ICONERROR);
    }
}

void FotoYukle() {
    ICBYTES yol;
    ReadImage(OpenFileMenu(yol, "JPEG\0*.JPG\0BMP\0*.BMP\0"), resim);
    DisplayImage(frmFoto, resim);
}

void ListePenceresi() {
    
    // Liste alaný
    int MLE = ICG_MLEdit(20, 20, 550, 320, "Kayýtlar", SCROLLBAR_V);
    ICG_SetWindowText(MLE, "");

    // Dosyayý oku
    ICDEVICE dosya;
    if (!CreateFileReader(dosya, "veri.bin")) {
        ICG_printf(MLE, "veri.bin açýlamadý!\n");
        return;
    }

    ICBYTES ad, soyad, tel, email, foto;
    int sayac = 1;

    while (ReadIntoMatrix(dosya, ad)) {
        ReadIntoMatrix(dosya, soyad);
        ReadIntoMatrix(dosya, tel);
        ReadIntoMatrix(dosya, email);
        ReadIntoMatrix(dosya, foto);

        ICG_printf(MLE, "Kayýt #%d:\n", sayac++);
        Print(MLE, ad);
        Print(MLE, soyad);
        Print(MLE, tel);
        Print(MLE, email);
        Print(MLE, foto);
        ICG_printf(MLE, "-----------------------------\n");
    }

    CloseDevice(dosya);

}

void Listele() {
    ICDEVICE dosya;
    if (!CreateFileReader(dosya, "veri.bin")) {
        MessageBoxA(NULL, "veri.bin açýlamadý", "Hata", MB_OK | MB_ICONERROR);
        return;
    }

    ICBYTES ad, soyad, tel, email, foto;
    int MLE = ICG_MLEdit(20, 20, 440, 300, "Kayýtlar", SCROLLBAR_V);
    ICG_SetWindowText(MLE, "");
    int sayac = 1;

    while (ReadIntoMatrix(dosya, ad)) {
        ReadIntoMatrix(dosya, soyad);
        ReadIntoMatrix(dosya, tel);
        ReadIntoMatrix(dosya, email);
        ReadIntoMatrix(dosya, foto);

        ICG_printf(MLE, "Kayýt #%d:\n", sayac++);
        Print(MLE, ad);
        Print(MLE, soyad);
        Print(MLE, tel);
        Print(MLE, email);
        Print(MLE, foto);
        ICG_printf(MLE, "-----------------------------\n");
    }

    CloseDevice(dosya);
}


void ICGUI_Create() {
    if (_access("assets", 0) == -1) {
        _mkdir("assets");
    }
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
    ICG_Button(200, 300, 150, 40, "Listele", Listele);
    ICG_Button(200, 350, 150, 40, "Kayýtlarý Göster", ListePenceresi);


}
