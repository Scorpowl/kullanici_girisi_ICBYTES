#include "icb_gui.h"
#include "icbytes.h"
#include "ic_media.h"
#include <string>
#include <vector>
using namespace std;

int sle_isim, sle_soyisim, sle_tc, sle_okulno, sle_bolum, foto_goster;

struct VERI_KAYDI {
    ICDEVICE index_dosya, veri_dosya, foto_dosya;
    ICBYTES index, isim, soyisim, tc, okulno, bolum, foto;
    unsigned secili_kayit = 1;
};

unsigned IndexAra(ICBYTES& index, unsigned long long* map) {
    for (unsigned i = 1; i <= index.Y(); i++) {
        if (index.O(1, i) == map[0] && index.O(2, i) == map[1]) return i;
    }
    return 0xffffffff;
}

void Girdi(void* p) {
    VERI_KAYDI& vk = *((VERI_KAYDI*)p);
    GetText(sle_isim, vk.isim);
    GetText(sle_soyisim, vk.soyisim);
    GetText(sle_tc, vk.tc);
    GetText(sle_okulno, vk.okulno);
    GetText(sle_bolum, vk.bolum);

    unsigned long long* map = KeyMapTR(vk.isim, 2);
    long long sonindex = vk.index.Y();

    if (vk.index.O(1, sonindex) != 0) {
        sonindex++;
        ResizeMatrix(vk.index, 3, sonindex);
    }
    else {
        unsigned varmi = IndexAra(vk.index, map);
        if (varmi != 0xffffffff) {
            ICG_printf("BU KAYIT VAR!\n");
            return;
        }
    }

    vk.index.O(1, sonindex) = map[0];
    vk.index.O(2, sonindex) = map[1];
    vk.index.O(3, sonindex) = GetFileLength(vk.veri_dosya);

    long long yeniAdres = WriteICBYTES(vk.veri_dosya, vk.isim, vk.index.O(3, sonindex));
    WriteICBYTES(vk.veri_dosya, vk.soyisim, yeniAdres);
    WriteICBYTES(vk.veri_dosya, vk.tc, yeniAdres);
    WriteICBYTES(vk.veri_dosya, vk.okulno, yeniAdres);
    WriteICBYTES(vk.veri_dosya, vk.bolum, yeniAdres);


    vk.index.O(3, sonindex) = yeniAdres;

    if (vk.foto.X() > 0 && vk.foto.Y() > 0) {
        long long foto_adres = GetFileLength(vk.foto_dosya);
        WriteICBYTES(vk.foto_dosya, vk.foto, foto_adres);
        vk.index.O(4, sonindex) = foto_adres;
    }

}

void Ara(void* p) {
    VERI_KAYDI& vk = *((VERI_KAYDI*)p);
    GetText(sle_isim, vk.isim);
    unsigned long long* map = KeyMapTR(vk.isim, 2);
    unsigned hangisi = IndexAra(vk.index, map);
    if (hangisi != 0xffffffff) {
        long long addr = vk.index.O(3, hangisi);
        ReadICBYTES(vk.veri_dosya, vk.isim, addr);
        addr = ReadICBYTES(vk.veri_dosya, vk.soyisim, addr);
        addr = ReadICBYTES(vk.veri_dosya, vk.tc, addr);
        addr = ReadICBYTES(vk.veri_dosya, vk.okulno, addr);
        addr = ReadICBYTES(vk.veri_dosya, vk.bolum, addr);


        ICG_SetWindowText(sle_isim, &vk.isim.C(1));
        ICG_SetWindowText(sle_soyisim, &vk.soyisim.C(1));
        ICG_SetWindowText(sle_tc, &vk.tc.C(1));
        ICG_SetWindowText(sle_okulno, &vk.okulno.C(1));
        ICG_SetWindowText(sle_bolum, &vk.bolum.C(1));

        ReadICBYTES(vk.foto_dosya, vk.foto, vk.index.O(4, vk.secili_kayit));
        DisplayImage(foto_goster, vk.foto);
    }
    else {
        ICG_printf("KAYIT BULUNAMADI!\n");
    }
}

void Onceki(void* p) {
    VERI_KAYDI& vk = *((VERI_KAYDI*)p);
    if (vk.index.Y() < 1) return;
    if (vk.secili_kayit <= 1) vk.secili_kayit = 1;
    else vk.secili_kayit--;

    long long addr = vk.index.O(3, vk.secili_kayit);
    addr = ReadICBYTES(vk.veri_dosya, vk.isim, addr);
    addr = ReadICBYTES(vk.veri_dosya, vk.soyisim, addr);
    addr = ReadICBYTES(vk.veri_dosya, vk.tc, addr);
    addr = ReadICBYTES(vk.veri_dosya, vk.okulno, addr);
    addr = ReadICBYTES(vk.veri_dosya, vk.bolum, addr);


    ICG_SetWindowText(sle_isim, &vk.isim.C(1));
    ICG_SetWindowText(sle_soyisim, &vk.soyisim.C(1));
    ICG_SetWindowText(sle_tc, &vk.tc.C(1));
    ICG_SetWindowText(sle_okulno, &vk.okulno.C(1));
    ICG_SetWindowText(sle_bolum, &vk.bolum.C(1));

    ReadICBYTES(vk.foto_dosya, vk.foto, vk.index.O(4, vk.secili_kayit));
    DisplayImage(foto_goster, vk.foto);
}

void Sonraki(void* p) {
    VERI_KAYDI& vk = *((VERI_KAYDI*)p);
    if (vk.index.Y() < 1) return;
    if (vk.secili_kayit >= vk.index.Y()) vk.secili_kayit = vk.index.Y();
    else vk.secili_kayit++;

    long long addr = vk.index.O(3, vk.secili_kayit);
    addr = ReadICBYTES(vk.veri_dosya, vk.isim, addr);
    addr = ReadICBYTES(vk.veri_dosya, vk.soyisim, addr);
    addr = ReadICBYTES(vk.veri_dosya, vk.tc, addr);
    addr = ReadICBYTES(vk.veri_dosya, vk.okulno, addr);
    addr = ReadICBYTES(vk.veri_dosya, vk.bolum, addr);



    ICG_SetWindowText(sle_isim, &vk.isim.C(1));
    ICG_SetWindowText(sle_soyisim, &vk.soyisim.C(1));
    ICG_SetWindowText(sle_tc, &vk.tc.C(1));
    ICG_SetWindowText(sle_okulno, &vk.okulno.C(1));
    ICG_SetWindowText(sle_bolum, &vk.bolum.C(1));

    ReadICBYTES(vk.foto_dosya, vk.foto, vk.index.O(4, vk.secili_kayit));
    DisplayImage(foto_goster, vk.foto);
}

void FotoSec(void* p) {
    VERI_KAYDI& vk = *((VERI_KAYDI*)p);
    ICBYTES yol;
    char* dosya = OpenFileMenu(yol, "*.jpg;*.bmp");
    if (dosya && ReadImage(dosya, vk.foto)) {
        ICBYTES goster;
        CreateImage(goster, 100, 100, ICB_C24);
        ToRGB24(vk.foto, goster);
        DisplayImage(foto_goster, goster);
    }
}

void Baslama(VERI_KAYDI& v) {
    CreateMatrix(v.index, 4, 1, ICB_ULONGLONG);
    v.index = 0;
    CreateFileDevice(v.index_dosya, "index.bin");
    CreateFileDevice(v.veri_dosya, "veri.bin");
    CreateFileDevice(v.foto_dosya, "foto.bin");
    ReadICBYTES(v.index_dosya, v.index, 0);
}

void ExitFonksiyonu(void* p) {
    VERI_KAYDI& v = *((VERI_KAYDI*)p);
    if (v.index.O(1, 1) != 0)
        WriteICBYTES(v.index_dosya, v.index, 0);
    CloseDevice(v.index_dosya);
    CloseDevice(v.veri_dosya);
    CloseDevice(v.foto_dosya);
}

void ICGUI_Create() {
    ICG_MWTitle("Veri Kayýt Sistemi");
    ICG_MWSize(640, 480);
}

void ICGUI_main() {
    static VERI_KAYDI kayit;

    ICG_Button(10, 10, 100, 25, "KAYDET", Girdi, &kayit);
    ICG_Button(120, 10, 100, 25, "ARA", Ara, &kayit);
    ICG_Button(230, 10, 100, 25, "<< ÖNCEKÝ", Onceki, &kayit);
    ICG_Button(340, 10, 100, 25, "SONRAKÝ >>", Sonraki, &kayit);
    ICG_Button(450, 10, 150, 25, "FOTOÐRAF SEÇ", FotoSec, &kayit);

    ICG_Static(10, 35, 100, 15, "Ýsim:");
    sle_isim = ICG_SLEditSunken(10, 50, 200, 20, "");

    ICG_Static(10, 65, 100, 15, "Soyisim:");
    sle_soyisim = ICG_SLEditSunken(10, 80, 200, 20, "");

    ICG_Static(10, 95, 100, 15, "TC Kimlik No:");
    sle_tc = ICG_SLEditSunken(10, 110, 200, 20, "");

    ICG_Static(10, 125, 100, 15, "Okul Numarasý:");
    sle_okulno = ICG_SLEditSunken(10, 140, 200, 20, "");

    ICG_Static(10, 155, 100, 15, "Bölüm:");
    sle_bolum = ICG_SLEditSunken(10, 170, 200, 20, "");


    foto_goster = ICG_StaticSunken(300, 50, 100, 100, "");

    ICG_SetOnExit(ExitFonksiyonu, &kayit);
    Baslama(kayit);
}
