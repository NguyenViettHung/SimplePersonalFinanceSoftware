#include "giao_dich.h"
#include "khai_bao.h"
#include "file_io.h"
#include "menu.h"
#include "ngan_sach.h"
#include "bao_cao.h"
#include "danh_muc.h"
#include <stdio.h>

// --- CÁC MENU CON ---

void menuGiaoDich() {
    int luaChon;
    while (1) {
        printf("\n--- QUAN LY GIAO DICH ---\n");
        printf("  1. Them giao dich\n");
        printf("  2. Xoa giao dich\n");
        printf("  3. Tim kiem giao dich theo ngay\n");
        printf("  0. Quay lai Menu Chinh\n");
        printf("Lua chon: ");
        
        if (scanf("%d", &luaChon) != 1) { xoaBuffer(); printf("[LOI] Vui long nhap so.\n"); continue; }
        xoaBuffer();

        switch (luaChon) {
            case 1: nhapVaThemGiaoDich(); break;
            case 2: nhapVaXoaGiaoDich(); break;
            case 3: nhapVaTimKiemGiaoDich(); break;
            case 0: return; // Thoát khỏi vòng lặp, quay về hàm gọi nó (menu chính)
            default: printf("[LOI] Lua chon khong hop le.\n"); break;
        }
    }
}

void menuDanhMuc() {
    int luaChon;
    while (1) {
        printf("\n--- QUAN LY DANH MUC ---\n");
        printf("  1. Xem danh sach danh muc\n");
        printf("  2. Them danh muc\n");
        printf("  3. Xoa danh muc\n");
        printf("  4. Gan %% han muc cho danh muc (Chi)\n");
        printf("  5. Hoan doi %% giua cac danh muc (Chi)\n");
        printf("  0. Quay lai Menu Chinh\n");
        printf("Lua chon: ");
        
        if (scanf("%d", &luaChon) != 1) { xoaBuffer(); printf("[LOI] Vui long nhap so.\n"); continue; }
        xoaBuffer();

        switch (luaChon) {
            case 1: xemDanhSachDanhMuc(); break;
            case 2: themDanhMuc(); break;
            case 3: xoaDanhMuc(); break;
            case 4: chiaPhanTramChi(); break;
            case 5: hoanDoiHanMucChi(); break;
            case 0: return;
            default: printf("[LOI] Lua chon khong hop le.\n"); break;
        }
    }
}

void menuBaoCao() {
    int luaChon;
    while (1) {
        printf("\n--- BAO CAO & THONG KE ---\n");
        printf("  1. Tinh tong thu/chi theo thoi gian/danh muc\n");
        printf("  2. Bao cao cac khoan vuot qua han muc chi\n");
        printf("  3. Tinh ti le chi tieu giua cac danh muc\n");
        printf("  0. Quay lai Menu Chinh\n");
        printf("Lua chon: ");
        
        if (scanf("%d", &luaChon) != 1) { xoaBuffer(); printf("[LOI] Vui long nhap so.\n"); continue; }
        xoaBuffer();

        switch (luaChon) {
            case 1: nhapVaTinhTongThuChi(); break;
            case 2: baoCaoVuotHanMuc(); break;
            case 3: nhapVaTinhTiLeChi(); break;
            case 0: return;
            default: printf("[LOI] Lua chon khong hop le.\n"); break;
        }
    }
}

// --- MENU CHÍNH VÀ ĐIỀU HƯỚNG ---

void inMenuChinh() {
    printf("\n========================================\n");
    printf("   QUAN LY CHI TIEU CA NHAN \n");
    printf("========================================\n");
    printf("  1. Quan ly Giao dich\n");
    printf("  2. Quan ly Danh muc\n");
    printf("  3. Bao cao & Thong ke\n");
    printf("  0. Thoat chuong trinh\n");
    printf("========================================\n");
    printf("Lua chon: ");
}

void dieuHuong() {
    int flag = 1;
    int luaChon;
    
    // Giữ nguyên logic kiểm tra ngân sách hiện tại khi vừa vào app
    kiemTraVaNhapNganSachHienTai(); 
    
    while (flag) {
        inMenuChinh();

        if (scanf("%d", &luaChon) != 1) {
            xoaBuffer();
            printf("[LOI] Vui long nhap so.\n");
            continue;
        }
        xoaBuffer();

        switch (luaChon) {
            case 1: menuGiaoDich(); break;
            case 2: menuDanhMuc(); break;
            case 3: menuBaoCao(); break;
            case 0:
                printf("\n=> Da luu du lieu. Hen gap lai!\n");
                flag = 0;
                break;
            default:
                printf("[LOI] Lua chon khong hop le. Vui long chon tu 0-4.\n");
                break;
        }
    }
}
