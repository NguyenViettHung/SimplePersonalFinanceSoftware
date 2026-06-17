#include "giao_dich.h"
#include "khai_bao.h"
#include "file_io.h"
#include "menu.h"
#include "ngan_sach.h"
#include "bao_cao.h"
#include "danh_muc.h"

void inMenu() {
    printf("\n");
    printf("========================================\n");
    printf("   QUAN LY CHI TIEU CA NHAN \n");
    printf("========================================\n");
    printf("  1. Them giao dich\n");
    printf("  2. Xoa giao dich\n");
    printf("  3. Tim kiem giao dich theo ngay\n");
    printf("  4. Tinh tong thu/chi theo ngay/thang/nam/loai danh muc\n");
    printf("  5. Bao cao cac khoan vuot qua han muc chi\n");
    printf("  6. Tinh ti le chi tieu giua cac danh muc\n");
    printf("  0. Thoat\n");
    printf("========================================\n");
    printf("Lua chon: ");
}

void dieuHuong() {
    int flag = 1;
    int luaChon;
    /* --- Vong lap chinh --- */
    while (flag) {
        inMenu();

        if (scanf("%d", &luaChon) != 1) {
            xoaBuffer();
            printf("[LOI] Vui long nhap so.\n");
            continue;
        }
        xoaBuffer();

        switch (luaChon) {
        case 1:
            nhapVaThemGiaoDich();
            break;
        case 2:
            nhapVaXoaGiaoDich();
            break;
        case 3:
            nhapVaTimKiemGiaoDich();
            break;
        case 4:
            nhapVaTinhTongThuChi();
            break;
        case 5:
            baoCaoVuotHanMuc();
            break;
        case 6:
            
            break;
        case 0:
            flag = 0;
            break;
        default:
            printf("[LOI] Lua chon khong hop le. Vui long chon lai (0-8).\n");
            break;
        }
    }
}