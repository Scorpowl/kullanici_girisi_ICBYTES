#include "icb_gui.h"

// Form bile�enlerinin tan�mlar�
int SLE_Ad, SLE_Email, LB_KayitSebebi, MLE_Aciklama, BTN_Kaydet, MLE_KayitListesi;

void ICGUI_Create() {
    ICG_MWSize(500, 400);
    ICG_MWTitle("Kullan�c� Kay�t Formu");
}
void KaydetFonksiyonu() {
    
}

void ICGUI_main() {
    ICG_Static(20, 20, 80, 25, "Ad & Soyad:");
    SLE_Ad = ICG_SLEditBorder(120, 20, 200, 25, "");

    ICG_Static(20, 60, 80, 25, "E-posta:");
    SLE_Email = ICG_SLEditBorder(120, 60, 200, 25, "");

   /* ICG_Static(20, 100, 100, 25, "Kay�t Sebebi:");
    LB_KayitSebebi = ICG_ListBox(120, 100, 200, 100, ListboxSecimFonk);
    ICG_AddToList(LB_KayitSebebi, "K�t�phane");
    ICG_AddToList(LB_KayitSebebi, "�niversite");
    ICG_AddToList(LB_KayitSebebi, "Hastane");*/

    ICG_Static(20, 210, 100, 25, "Ek A��klama:");
    MLE_Aciklama = ICG_MLEditSunken(120, 210, 200, 60, "", SCROLLBAR_V);

    BTN_Kaydet = ICG_Button(120, 280, 100, 30, "Kaydet", KaydetFonksiyonu);

    ICG_Static(20, 320, 100, 25, "Kay�tl� Kullan�c�lar:");
    MLE_KayitListesi = ICG_MLEditSunken(120, 320, 350, 60, "", SCROLLBAR_V);
}