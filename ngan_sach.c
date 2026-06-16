#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "khai_bao.h"
 
struct NganSach *mang_ngan_sach = NULL;
int so_luong_ngan_sach = 0;
int suc_chua_ngan_sach = 0; // mang ngan sach

void khoiTaoNganSach(int khoi_tao_suc_chua) {
    if (khoi_tao_suc_chua <= 0) khoi_tao_suc_chua = 12;
    
    if (mang_ngan_sach == NULL) {
        mang_ngan_sach = (struct NganSach *)malloc(khoi_tao_suc_chua * sizeof(struct NganSach));
        suc_chua_ngan_sach = khoi_tao_suc_chua;
        so_luong_ngan_sach = 0;
    }
}
 
void giaiPhongNganSach() {
    free(mang_ngan_sach);
    mang_ngan_sach = NULL;
    so_luong_ngan_sach = 0;
    suc_chua_ngan_sach = 0;
}

// ============================================
// Hàm in danh sách ngân sách (kiểm tra)
// ============================================
void inDanhSachNganSach() {
    printf("\n========== DANH SACH NGAN SACH ==========\n");
    
    if (so_luong_ngan_sach == 0) {
        printf("Khong co danh sach ngan sach.\n");
        printf("=========================================\n\n");
        return;
    }
    
    printf("%-15s %-20s %-10s %-10s\n",
           "Ma NS", "So tien (VND)", "Thang", "Nam");
    printf("%-15s %-20s %-10s %-10s\n",
           "----------", "-------------------", "-----", "-----");
    
    for (int i = 0; i < so_luong_ngan_sach; i++) {
        printf("%-15s %-20d %-10d %-10d\n",
               mang_ngan_sach[i].ma_ns,
               mang_ngan_sach[i].so_tien_ns,
               mang_ngan_sach[i].thang,
               mang_ngan_sach[i].nam);
    }
    
    printf("=========================================\n\n");
}
 

// =====================================================
// TIM NGAN SACH THEO THANG/NAM
// Tra ve con tro den NganSach neu tim thay, NULL neu khong
// =====================================================
NganSach *timNganSach(int thang, int nam) {
    for (int i = 0; i < so_luong_ngan_sach; i++) {
        if (mang_ngan_sach[i].thang == thang && mang_ngan_sach[i].nam == nam)
            return &mang_ngan_sach[i];
    }
    return NULL;
}
 
// =====================================================
// NHAP NGAN SACH
// Logic:
//   - Lay thang/nam hien tai tu time.h
//   - Neu thang nay chua co ngan sach -> yeu cau nhap
//   - Neu da co -> hoi co muon cap nhat khong
//   - so_tien_ns = 0 duoc coi la chua co ngan sach
// =====================================================
void nhapNganSach() {
    // -- 1. Lay thang/nam hien tai --
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int thang_ht = t->tm_mon + 1;   // tm_mon: 0-11
    int nam_ht   = t->tm_year + 1900;
 
    printf("\n========== NGAN SACH THANG %02d/%04d ==========\n", thang_ht, nam_ht);
 
    // -- 2. Kiem tra da co ngan sach thang nay chua --
    NganSach *ns_ht = timNganSach(thang_ht, nam_ht);
 
    if (ns_ht != NULL && ns_ht->so_tien_ns > 0) {
        printf("Ngan sach hien tai: %d VND\n", ns_ht->so_tien_ns);
        printf("Ban co muon cap nhat ngan sach thang nay khong? (y/n): ");
        char lua_chon[4];
        if (fgets(lua_chon, sizeof(lua_chon), stdin) == NULL) return;
        if (lua_chon[0] != 'y' && lua_chon[0] != 'Y') {
            printf("Giu nguyen ngan sach: %d VND\n", ns_ht->so_tien_ns);
            return;
        }
    } else {
        printf("Chua co ngan sach cho thang %02d/%04d.\n", thang_ht, nam_ht);
        printf("Vui long nhap ngan sach truoc khi thuc hien cac chuc nang khac.\n");
    }
 
    // -- 3. Nhap so tien ngan sach --
    int so_tien_moi = 0;
    printf("Nhap so tien ngan sach (VND, > 0): ");
    while (scanf("%d", &so_tien_moi) != 1 || so_tien_moi <= 0) {
        printf("Loi: so tien phai lon hon 0. Nhap lai: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n'); // xoa buffer
 
    // -- 4. Cap nhat hoac them moi --
    if (ns_ht != NULL) {
        // Cap nhat ngan sach cu
        ns_ht->so_tien_ns = so_tien_moi;
        printf("Da cap nhat ngan sach thang %02d/%04d: %d VND\n",
               thang_ht, nam_ht, so_tien_moi);
    } else {
        // Them ngan sach moi
        so_luong_ngan_sach++;
        mang_ngan_sach = (NganSach*) realloc(mang_ngan_sach, so_luong_ngan_sach * sizeof(NganSach));
 
        NganSach ns_moi;
        sprintf(ns_moi.ma_ns, "NS%04d%02d", nam_ht, thang_ht);
        ns_moi.so_tien_ns = so_tien_moi;
        ns_moi.thang      = thang_ht;
        ns_moi.nam        = nam_ht;
 
        mang_ngan_sach[so_luong_ngan_sach - 1] = ns_moi;
        printf("Da tao ngan sach %s: %d VND\n", ns_moi.ma_ns, so_tien_moi);
    }
}

void kiemTraVaNhapNganSachHienTai() {
    time_t now = time(NULL);
	struct tm *tm_ht = localtime(&now);
	int thang_ht = tm_ht->tm_mon + 1;
	int nam_ht   = tm_ht->tm_year + 1900;
	NganSach *ns_ht = timNganSach(thang_ht, nam_ht);
	if (ns_ht == NULL || ns_ht->so_tien_ns == 0) {
		printf("\nChua co ngan sach thang %02d/%04d. Vui long nhap truoc khi tiep tuc.\n",
		       thang_ht, nam_ht);
		nhapNganSach();
	}
}