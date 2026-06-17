#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "khai_bao.h"
#include "file_io.h"
#include "giao_dich.h"

extern DanhMuc *mang_dm_thu;
extern DanhMuc *mang_dm_chi;
extern int so_luong_giao_dich_thu;
extern int so_luong_giao_dich_chi;
extern int so_luong_dm_chi;
extern int so_luong_dm_thu;

void baoCaoVuotHanMuc() {
    if (so_luong_dm_chi == 0) {
        printf("\nKhong co danh muc nao!\n");
        return;
    }

    printf("\n=== BAO CAO VUOT HAN MUC ===\n");
    printf("%-5s | %-15s | %-12s | %-12s | %-10s\n",
           "Ma", "Ten", "Han muc goc", "Da chi", "Vuot");
    printf("--------------------------------------------------------------\n");

    int co_vuot = 0;

    for (int i = 0; i < so_luong_dm_chi; i++) {
        if (mang_dm_chi[i].han_muc_tien < 0) {
            int da_chi = mang_dm_chi[i].han_muc_tien_goc 
                       - mang_dm_chi[i].han_muc_tien;

            printf("%-5d | %-15s | %-12d | %-12d | %d VND\n",
                   mang_dm_chi[i].ma_dm,
                   mang_dm_chi[i].ten_dm,
                   mang_dm_chi[i].han_muc_tien_goc,
                   da_chi,
                   -mang_dm_chi[i].han_muc_tien);

            co_vuot = 1;
        }
    }

    if (!co_vuot) {
        printf("Khong co danh muc nao vuot han muc!\n");
    }

    printf("--------------------------------------------------------------\n");
}