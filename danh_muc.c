// Tạo, xóa danh mục
// Quản lý % cho các danh mục (gồm chia %, hoán đổi %)
// Tính tiền cho các danh mục dựa vào % được chia và ngân sách

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "khai_bao.h"
#include "danh_muc.h"
#include "giao_dich.h"
DanhMuc *mang_dm_thu = NULL;
DanhMuc *mang_dm_chi = NULL;
int so_luong_dm_thu = 0;
int so_luong_dm_chi = 0;
// Chỉ cho phép kiểu số nguyên
// Đã gồm cả scanf để nhập
int nhapSoNguyen() {
    int gia_tri;
    int check;
    char ki_tu_thua;

    while (1) { 
        check = scanf("%d", &gia_tri);

        if (check != 1) {
            // Nhập chữ cái hoặc ký tự đặc biệt ngay từ đầu
            printf("\nDu lieu khong phai la so! Vui long chi nhap so nguyen.\n");
            printf("Nhap lai: ");
            
            // Dọn dẹp bộ nhớ đệm
            while (getchar() != '\n'); 
        } else {
            // Bắt đầu bằng số nhưng dính chữ ở đuôi
            scanf("%c", &ki_tu_thua); // Đọc ngay ký tự liền sau con số vừa nhập
            
            if (ki_tu_thua == '\n') {
                // Ký tự liền sau là nút Enter -> OK
                return gia_tri;
            } else {
                printf("\nDu lieu khong phai la so nguyen!\n");
                printf("Vui long nhap mot so nguyen duy nhat: ");
                
                // Dọn dẹp nốt phần đuôi thừa trong bộ đệm
                while (ki_tu_thua != '\n' && ki_tu_thua != EOF) {
                    ki_tu_thua = getchar();
                }
            }
        }
    }
}
int tongHanMuc(){
    int tong=0;
    for (int i=0; i< so_luong_dm_chi; i++){
        tong+=mang_dm_chi[i].han_muc;
    }
    return tong;
}

void inTenDanhMuc() {
    printf("\n=== DANH SACH DANH MUC CHI ===\n");
    if (so_luong_dm_chi == 0) {
        printf("Danh sach hien dang rong.\n");
        return;
    }

    printf("%-5s | %-15s\n", "Ma", "Ten");
    printf("------------------------\n");

    for (int i = 0; i < so_luong_dm_chi; i++) {
        printf("%-5d | %-15s\n",
               mang_dm_chi[i].ma_dm,
               mang_dm_chi[i].ten_dm);
    }

    printf("------------------------\n");
}

void inDanhSachChi() {
    printf("\n=== DANH SACH DANH MUC CHI (%d) ===\n", so_luong_dm_chi);
    if (so_luong_dm_chi == 0) {
        printf("Danh sach hien dang rong.\n");
        return;
    }
    
    int tong = 0;
    for (int i = 0; i < so_luong_dm_chi; i++) {
        printf("Ma: %-3d | Ten: %-10s | Han muc: %d%%\n", 
               mang_dm_chi[i].ma_dm, mang_dm_chi[i].ten_dm, mang_dm_chi[i].han_muc);
        tong += mang_dm_chi[i].han_muc;
    }
    printf("-----------------------------------\n");
    printf("=> Tong han muc da phan bo: %d%%\n", tong);
    printf("=> Ngan sach con lai: %d%%\n", 100 - tong);
}

// Thêm danh mục (mã + tên), han_muc mặc định = 0
void themDanhMucChi() {
    printf("\n--- THEM DANH MUC CHI ---\n");
    DanhMuc dm_temp;

    // Nhập và kiểm tra mã danh mục
    printf("Nhap ma danh muc (so nguyen): ");
    dm_temp.ma_dm = nhapSoNguyen();

    for (int i = 0; i < so_luong_dm_chi; i++) {
        if (mang_dm_chi[i].ma_dm == dm_temp.ma_dm) {
            printf("Loi: Ma danh muc %d da ton tai!\n", dm_temp.ma_dm);
            return;
        }
    }

    // Nhập tên danh mục
    printf("Nhap ten danh muc (khong khoang trang): ");
    scanf("%49s", dm_temp.ten_dm);

    // Hạn mức mặc định = 0, chưa phân bổ
    dm_temp.han_muc = 0;

    // Cấp phát mảng động
    so_luong_dm_chi++;
    mang_dm_chi = (DanhMuc*) realloc(mang_dm_chi, so_luong_dm_chi * sizeof(DanhMuc));
    mang_dm_chi[so_luong_dm_chi - 1] = dm_temp;

    printf("\n=> Da them danh muc '%s' (Ma: %d) thanh cong!\n", dm_temp.ten_dm, dm_temp.ma_dm);
    printf("=> Han muc hien tai: 0%% (chua phan bo). Dung chuc nang 5 de phan bo ngan sach.\n");
}

// Chia % ngân sách cho một danh mục đã có
void chiaPhanTramChi() {
    if (so_luong_dm_chi == 0) {
        printf("\nDanh sach danh muc Chi hien dang rong! Them danh muc truoc.\n");
        return;
    }

    // Hiển thị danh sách để người dùng chọn
    inDanhSachChi();

    int phan_tram_con_lai = 100 - tongHanMuc();
    if (phan_tram_con_lai <= 0) {
        printf("\nNgan sach danh muc Chi da phan bo het 100%%!\n");
        return;
    }

    printf("\n--- PHAN BO NGAN SACH CHO DANH MUC CHI ---\n");
    printf("Ban dang con %d%% ngan sach chua phan bo.\n", phan_tram_con_lai);

    // Nhập mã danh mục cần gán %
    printf("Nhap ma danh muc can phan bo: ");
    int ma_can_gan = nhapSoNguyen();

    // Tìm vị trí trong mảng
    int vi_tri = -1;
    for (int i = 0; i < so_luong_dm_chi; i++) {
        if (mang_dm_chi[i].ma_dm == ma_can_gan) {
            vi_tri = i;
            break;
        }
    }

    if (vi_tri == -1) {
        printf("Loi: Khong tim thay danh muc co ma %d!\n", ma_can_gan);
        return;
    }

    printf("Danh muc: '%s' | Han muc hien tai: %d%%\n",
           mang_dm_chi[vi_tri].ten_dm, mang_dm_chi[vi_tri].han_muc);

    // Tính lại ngân sách còn lại nếu cộng thêm phần cũ của danh mục này vào
    // vì đang ghi đè, không cộng thêm
    int ngan_sach_kha_dung = phan_tram_con_lai + mang_dm_chi[vi_tri].han_muc;
    printf("Han muc toi da co the dat: %d%%\n", ngan_sach_kha_dung);

    // Nhập % mới
    printf("Nhap han muc moi (%%): ");
    int han_muc_moi = nhapSoNguyen();

    if (han_muc_moi <= 0) {
        printf("Loi: Han muc phai lon hon 0%%!\n");
        return;
    }
    if (han_muc_moi > ngan_sach_kha_dung) {
        printf("Loi: Vuot qua ngan sach kha dung! Toi da nhap toi da %d%%.\n", ngan_sach_kha_dung);
        return;
    }

    // Cập nhật
    int han_muc_cu = mang_dm_chi[vi_tri].han_muc;
    mang_dm_chi[vi_tri].han_muc = han_muc_moi;

    int phanTramCapNhat = 100 - tongHanMuc();
    printf("\n=> Da cap nhat danh muc '%s': %d%% -> %d%%\n",
           mang_dm_chi[vi_tri].ten_dm, han_muc_cu, han_muc_moi);
    printf("=> NGAN SACH CHI CHUA PHAN BO CON LAI: %d%%\n", phanTramCapNhat);
}

void xoaDanhMucChi() {
    if (so_luong_dm_chi == 0) {
        printf("\nDanh sach danh muc Chi hien dang rong!\n");
        return;
    }

    int ma_can_xoa;
    printf("\n--- XOA DANH MUC CHI ---\n");
    printf("Nhap ma danh muc can xoa: ");
    scanf("%d", &ma_can_xoa);

    // Tìm vị trí cần xóa
    int vi_tri_xoa = -1;
    for (int i = 0; i < so_luong_dm_chi; i++) {
        if (mang_dm_chi[i].ma_dm == ma_can_xoa) {
            vi_tri_xoa = i;
            break;
        }
    }

    if (vi_tri_xoa == -1) {
        printf("Loi: Khong tim thay danh muc co ma da nhap!\n");
        return;
    }

    // Lưu lại tên để hiển thị thông báo
    char ten_da_xoa[50];
    strcpy(ten_da_xoa, mang_dm_chi[vi_tri_xoa].ten_dm);

    // Dồn mảng (O(n))
    for (int i = vi_tri_xoa; i < so_luong_dm_chi - 1; i++) {
        mang_dm_chi[i] = mang_dm_chi[i + 1];
    }

    // Cắt bỏ ô nhớ thừa ở cuối mảng
    so_luong_dm_chi--;
    if (so_luong_dm_chi == 0) {
        free(mang_dm_chi);
        mang_dm_chi = NULL;
    } else {
        mang_dm_chi = (DanhMuc*) realloc(mang_dm_chi, so_luong_dm_chi * sizeof(DanhMuc));
    }

    // In kết quả
    int phanTramCapNhat = 100 - tongHanMuc();
    printf("\n=> Da xoa danh muc '%s' thanh cong!\n", ten_da_xoa);
    printf("=> NGAN SACH CHI CHUA PHAN BO CON LAI: %d%%\n", phanTramCapNhat);
}

void hoanDoiPhanTramChi() {
    // Kiểm tra điều kiện
    if (so_luong_dm_chi < 2) {
        printf("\nCan co it nhat 2 danh muc Chi de thuc hien hoan doi!\n");
        return;
    }

    int ma1, ma2;
    printf("\n--- HOAN DOI %% DANH MUC CHI ---\n");
    printf("Nhap ma danh muc thu nhat: ");
    scanf("%d", &ma1);
    printf("Nhap ma danh muc thu hai: ");
    scanf("%d", &ma2);

    // 2 mã giống nhau
    if (ma1 == ma2) {
        printf("\nHai ma danh muc phai khac nhau!\n");
        return;
    }

    // Tìm vị trí của 2 danh mục trong mảng
    int vi_tri1 = -1, vi_tri2 = -1;
    for (int i = 0; i < so_luong_dm_chi; i++) {
        if (mang_dm_chi[i].ma_dm == ma1) {
            vi_tri1 = i;
        }
        if (mang_dm_chi[i].ma_dm == ma2) {
            vi_tri2 = i;
        }
    }

    // Kiểm tra xem có tìm thấy cả 2 không
    if (vi_tri1 == -1 || vi_tri2 == -1) {
        printf("\nKhong tim thay mot hoac ca hai ma danh muc vua nhap. Vui long thu lai!\n");
        return;
    }

    // Lưu lại % cũ để in thông báo
    int cu1 = mang_dm_chi[vi_tri1].han_muc;
    int cu2 = mang_dm_chi[vi_tri2].han_muc;

    // Hoán đổi
    int temp = mang_dm_chi[vi_tri1].han_muc;
    mang_dm_chi[vi_tri1].han_muc = mang_dm_chi[vi_tri2].han_muc;
    mang_dm_chi[vi_tri2].han_muc = temp;

    // In thông báo thành công
    printf("\n=> HOAN DOI THANH CONG!\n");
    printf(" Danh muc '%s' (Ma: %d): %d%% -> %d%%\n", 
           mang_dm_chi[vi_tri1].ten_dm, ma1, cu1, mang_dm_chi[vi_tri1].han_muc);
    printf(" Danh muc '%s' (Ma: %d): %d%% -> %d%%\n", 
           mang_dm_chi[vi_tri2].ten_dm, ma2, cu2, mang_dm_chi[vi_tri2].han_muc);
}
void tinhTienDanhMucChi(NganSach ns) {
    if (so_luong_dm_chi == 0) {
        printf("\nDanh sach danh muc Chi hien dang rong!\n");
        return;
    }

    printf("\n=== PHAN BO NGAN SACH: %s ===\n", ns.ma_ns);
    printf("Tong ngan sach: %d VND\n", ns.so_tien_ns);
    printf("---------------------------------------------------\n");

    int tong_da_phan_bo = 0;

    for (int i = 0; i < so_luong_dm_chi; i++) {
        int so_tien_dm;

        if (i == so_luong_dm_chi - 1) {
            // Danh mục cuối: lấy phần còn lại để tránh mất tiền do làm tròn
            so_tien_dm = ns.so_tien_ns - tong_da_phan_bo;
        } else {
            // Các danh mục trước: làm tròn xuống bình thường
            so_tien_dm = ns.so_tien_ns * mang_dm_chi[i].han_muc / 100;
            tong_da_phan_bo += so_tien_dm;
        }

        printf("Ma: %-3d | Ten: %-15s | Han muc: %3d%% | So tien: %d VND\n",
               mang_dm_chi[i].ma_dm,
               mang_dm_chi[i].ten_dm,
               mang_dm_chi[i].han_muc,
               so_tien_dm);
    }

    printf("---------------------------------------------------\n");
    printf("=> Tong da phan bo: %d VND\n", ns.so_tien_ns);
}

// Tính % tỉ lệ chi của từng danh mục so với tổng đã chi
// Lọc theo tháng/năm của ngân sách truyền vào (chỉ tính % của 1 tháng nhất định, tháng sau là reset)
void tinhTiLeChi(NganSach ns) {
    if (so_luong_dm_chi == 0 || so_luong_chi == 0) {
        printf("\nKhong co du lieu de tinh ti le!\n");
        return;
    }

    int *so_tien_dm = (int*) calloc(so_luong_dm_chi, sizeof(int));
    if (so_tien_dm == NULL) {
        printf("\nLoi cap phat bo nho!\n");
        return;
    }

    int tong_da_chi = 0;

    // Duyệt mang_chi
    for (int i = 0; i < so_luong_chi; i++) {
        if (mang_chi[i].thang != ns.thang) continue;
        if (mang_chi[i].nam   != ns.nam)   continue;

        // Gom tiền vào đúng vị trí danh mục
        for (int j = 0; j < so_luong_dm_chi; j++) {
            if (mang_dm_chi[j].ma_dm == mang_chi[i].ma_dm) {
                so_tien_dm[j] += mang_chi[i].so_tien_gd;
                tong_da_chi   += mang_chi[i].so_tien_gd;
                break;
            }
        }
    }

    if (tong_da_chi == 0) {
        printf("\nChua co giao dich Chi nao trong thang %d/%d!\n", ns.thang, ns.nam);
        free(so_tien_dm);
        return;
    }

    printf("\n=== TI LE CHI GIUA CAC DANH MUC [%d/%d] ===\n", ns.thang, ns.nam);
    printf("Tong da chi: %d VND\n", tong_da_chi);
    printf("--------------------------------------------------------------\n");

    for (int i = 0; i < so_luong_dm_chi; i++) {
        float tiLe = (float)so_tien_dm[i] / tong_da_chi * 100.0f;
        printf("ID: %-3d | Ten: %-15s | Da chi: %-10d VND | Ti le: %.2f%%\n",
               mang_dm_chi[i].ma_dm,
               mang_dm_chi[i].ten_dm,
               so_tien_dm[i],
               tiLe);
    }

    printf("--------------------------------------------------------------\n");
    printf("=> Tong: %d VND | 100.00%%\n", tong_da_chi);

    free(so_tien_dm);
}

void capNhatHanMucTien(NganSach ns) {
    for (int i = 0; i < so_luong_dm_chi; i++) {
        int tien = ns.so_tien_ns * mang_dm_chi[i].han_muc / 100;
        mang_dm_chi[i].han_muc_tien_goc = tien;
        mang_dm_chi[i].han_muc_tien     = tien;  // bắt đầu = gốc
    }
}

void truHanMucVaCanhBao(int ma_dm, int so_tien) {
    int vi_tri = -1;
    for (int i = 0; i < so_luong_dm_chi; i++) {
        if (mang_dm_chi[i].ma_dm == ma_dm) {
            vi_tri = i;
            break;
        }
    }
    if (vi_tri == -1) return;

    mang_dm_chi[vi_tri].han_muc_tien -= so_tien;

    int con_lai = mang_dm_chi[vi_tri].han_muc_tien;
    int goc     = mang_dm_chi[vi_tri].han_muc_tien_goc;

    if (con_lai <= 0) {
        printf("\n!!! CANH BAO: '%s' da VUOT HAN MUC!\n",
               mang_dm_chi[vi_tri].ten_dm);
        printf("    Han muc goc : %d VND\n", goc);
        printf("    Vuot        : %d VND\n", -con_lai);
    } else if (goc > 0 && con_lai <= goc * 20 / 100) {
        printf("\n!!! CANH BAO: '%s' da su dung >= 80%% han muc!\n",
               mang_dm_chi[vi_tri].ten_dm);
        printf("    Han muc goc : %d VND\n", goc);
        printf("    Con lai     : %d VND (%.1f%%)\n",
               con_lai, (float)con_lai / goc * 100);
    }
}