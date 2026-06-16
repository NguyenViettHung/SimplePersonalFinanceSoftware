// Tạo, xóa danh mục
// Quản lý % cho các danh mục (gồm chia %, hoán đổi %)
// Tính tiền cho các danh mục dựa vào % được chia và ngân sách

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "khai_bao.h"
#include "danh_muc.h"
DanhMuc *mang_dm_thu = NULL;
DanhMuc *mang_dm_chi = NULL;
int count_dm_thu = 0;
int count_dm_chi = 0;
// Chỉ cho phép kiểu số nguyên
// Đã gồm cả scanf để nhập
int nhapSoNguyen() {
    int giaTri;
    int kiemTra;
    char kiTuThua;

    while (1) { 
        kiemTra = scanf("%d", &giaTri);

        if (kiemTra != 1) {
            // Nhập chữ cái hoặc ký tự đặc biệt ngay từ đầu
            printf("\nDu lieu khong phai la so! Vui long chi nhap so nguyen.\n");
            printf("Nhap lai: ");
            
            // Dọn dẹp bộ nhớ đệm
            while (getchar() != '\n'); 
        } else {
            // Bắt đầu bằng số nhưng dính chữ ở đuôi
            scanf("%c", &kiTuThua); // Đọc ngay ký tự liền sau con số vừa nhập
            
            if (kiTuThua == '\n') {
                // Ký tự liền sau là nút Enter -> OK
                return giaTri; 
            } else {
                printf("\nDu lieu khong phai la so nguyen!\n");
                printf("Vui long nhap mot so nguyen duy nhat: ");
                
                // Dọn dẹp nốt phần đuôi thừa trong bộ đệm
                while (kiTuThua != '\n' && kiTuThua != EOF) {
                    kiTuThua = getchar();
                }
            }
        }
    }
}
int tongHanMuc(){
    int tong=0;
    for (int i=0; i< count_dm_chi; i++){
        tong+=mang_dm_chi[i].han_muc;
    }
    return tong;
}

void inDanhSachChi() {
    printf("\n=== DANH SACH DANH MUC CHI (%d) ===\n", count_dm_chi);
    if (count_dm_chi == 0) {
        printf("Danh sach hien dang rong.\n");
        return;
    }
    
    int tong = 0;
    for (int i = 0; i < count_dm_chi; i++) {
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
    DanhMuc dmTemp;

    // Nhập và kiểm tra mã danh mục
    printf("Nhap ma danh muc (so nguyen): ");
    dmTemp.ma_dm = nhapSoNguyen();

    for (int i = 0; i < count_dm_chi; i++) {
        if (mang_dm_chi[i].ma_dm == dmTemp.ma_dm) {
            printf("Loi: Ma danh muc %d da ton tai!\n", dmTemp.ma_dm);
            return;
        }
    }

    // Nhập tên danh mục
    printf("Nhap ten danh muc (khong khoang trang): ");
    scanf("%49s", dmTemp.ten_dm);

    // Hạn mức mặc định = 0, chưa phân bổ
    dmTemp.han_muc = 0;

    // Cấp phát mảng động
    count_dm_chi++;
    mang_dm_chi = (DanhMuc*) realloc(mang_dm_chi, count_dm_chi * sizeof(DanhMuc));
    mang_dm_chi[count_dm_chi - 1] = dmTemp;

    printf("\n=> Da them danh muc '%s' (Ma: %d) thanh cong!\n", dmTemp.ten_dm, dmTemp.ma_dm);
    printf("=> Han muc hien tai: 0%% (chua phan bo). Dung chuc nang 5 de phan bo ngan sach.\n");
}

// Chia % ngân sách cho một danh mục đã có
void chiaPhanTramChi() {
    if (count_dm_chi == 0) {
        printf("\nDanh sach danh muc Chi hien dang rong! Them danh muc truoc.\n");
        return;
    }

    // Hiển thị danh sách để người dùng chọn
    inDanhSachChi();

    int phanTramConLai = 100 - tongHanMuc();
    if (phanTramConLai <= 0) {
        printf("\nNgan sach danh muc Chi da phan bo het 100%%!\n");
        return;
    }

    printf("\n--- PHAN BO NGAN SACH CHO DANH MUC CHI ---\n");
    printf("Ban dang con %d%% ngan sach chua phan bo.\n", phanTramConLai);

    // Nhập mã danh mục cần gán %
    printf("Nhap ma danh muc can phan bo: ");
    int maCanGan = nhapSoNguyen();

    // Tìm vị trí trong mảng
    int viTri = -1;
    for (int i = 0; i < count_dm_chi; i++) {
        if (mang_dm_chi[i].ma_dm == maCanGan) {
            viTri = i;
            break;
        }
    }

    if (viTri == -1) {
        printf("Loi: Khong tim thay danh muc co ma %d!\n", maCanGan);
        return;
    }

    printf("Danh muc: '%s' | Han muc hien tai: %d%%\n",
           mang_dm_chi[viTri].ten_dm, mang_dm_chi[viTri].han_muc);

    // Tính lại ngân sách còn lại nếu cộng thêm phần cũ của danh mục này vào
    // (vì đang ghi đè, không cộng thêm)
    int nganSachKhaDung = phanTramConLai + mang_dm_chi[viTri].han_muc;
    printf("Han muc toi da co the dat: %d%%\n", nganSachKhaDung);

    // Nhập % mới
    printf("Nhap han muc moi (%%): ");
    int hanMucMoi = nhapSoNguyen();

    if (hanMucMoi <= 0) {
        printf("Loi: Han muc phai lon hon 0%%!\n");
        return;
    }
    if (hanMucMoi > nganSachKhaDung) {
        printf("Loi: Vuot qua ngan sach kha dung! Toi da nhap toi da %d%%.\n", nganSachKhaDung);
        return;
    }

    // Cập nhật
    int hanMucCu = mang_dm_chi[viTri].han_muc;
    mang_dm_chi[viTri].han_muc = hanMucMoi;

    int phanTramCapNhat = 100 - tongHanMuc();
    printf("\n=> Da cap nhat danh muc '%s': %d%% -> %d%%\n",
           mang_dm_chi[viTri].ten_dm, hanMucCu, hanMucMoi);
    printf("=> NGAN SACH CHI CHUA PHAN BO CON LAI: %d%%\n", phanTramCapNhat);
}

void xoaDanhMucChi() {
    if (count_dm_chi == 0) {
        printf("\nDanh sach danh muc Chi hien dang rong!\n");
        return;
    }

    int maCanXoa;
    printf("\n--- XOA DANH MUC CHI ---\n");
    printf("Nhap ma danh muc can xoa: ");
    scanf("%d", &maCanXoa);

    // Tìm vị trí cần xóa
    int viTriXoa = -1;
    for (int i = 0; i < count_dm_chi; i++) {
        if (mang_dm_chi[i].ma_dm == maCanXoa) {
            viTriXoa = i;
            break;
        }
    }

    if (viTriXoa == -1) {
        printf("Loi: Khong tim thay danh muc co ma da nhap!\n");
        return;
    }

    // Lưu lại tên để hiển thị thông báo
    char tenDaXoa[50];
    strcpy(tenDaXoa, mang_dm_chi[viTriXoa].ten_dm);

    // Dồn mảng (O(n))
    for (int i = viTriXoa; i < count_dm_chi - 1; i++) {
        mang_dm_chi[i] = mang_dm_chi[i + 1];
    }

    // Cắt bỏ ô nhớ thừa ở cuối mảng
    count_dm_chi--;
    if (count_dm_chi == 0) {
        free(mang_dm_chi);
        mang_dm_chi = NULL;
    } else {
        mang_dm_chi = (DanhMuc*) realloc(mang_dm_chi, count_dm_chi * sizeof(DanhMuc));
    }

    // In kết quả
    int phanTramCapNhat = 100 - tongHanMuc();
    printf("\n=> Da xoa danh muc '%s' thanh cong!\n", tenDaXoa);
    printf("=> NGAN SACH CHI CHUA PHAN BO CON LAI: %d%%\n", phanTramCapNhat);
}

void hoanDoiPhanTramChi() {
    // Kiểm tra điều kiện
    if (count_dm_chi < 2) {
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
    int viTri1 = -1, viTri2 = -1;
    for (int i = 0; i < count_dm_chi; i++) {
        if (mang_dm_chi[i].ma_dm == ma1) {
            viTri1 = i;
        }
        if (mang_dm_chi[i].ma_dm == ma2) {
            viTri2 = i;
        }
    }

    // Kiểm tra xem có tìm thấy cả 2 không
    if (viTri1 == -1 || viTri2 == -1) {
        printf("\nKhong tim thay mot hoac ca hai ma danh muc vua nhap. Vui long thu lai!\n");
        return;
    }

    // Lưu lại % cũ để in thông báo
    int cu1 = mang_dm_chi[viTri1].han_muc;
    int cu2 = mang_dm_chi[viTri2].han_muc;

    // Hoán đổi
    int temp = mang_dm_chi[viTri1].han_muc;
    mang_dm_chi[viTri1].han_muc = mang_dm_chi[viTri2].han_muc;
    mang_dm_chi[viTri2].han_muc = temp;

    // In thông báo thành công
    printf("\n=> HOAN DOI THANH CONG!\n");
    printf(" Danh muc '%s' (Ma: %d): %d%% -> %d%%\n", 
           mang_dm_chi[viTri1].ten_dm, ma1, cu1, mang_dm_chi[viTri1].han_muc);
    printf(" Danh muc '%s' (Ma: %d): %d%% -> %d%%\n", 
           mang_dm_chi[viTri2].ten_dm, ma2, cu2, mang_dm_chi[viTri2].han_muc);
}
void tinhTienDanhMucChi(NganSach ns) {
    if (count_dm_chi == 0) {
        printf("\nDanh sach danh muc Chi hien dang rong!\n");
        return;
    }

    printf("\n=== PHAN BO NGAN SACH: %s ===\n", ns.ma_ns);
    printf("Tong ngan sach: %d VND\n", ns.so_tien_ns);
    printf("---------------------------------------------------\n");

    int tongDaPhanBo = 0;

    for (int i = 0; i < count_dm_chi; i++) {
        int soTienDM;

        if (i == count_dm_chi - 1) {
            // Danh mục cuối: lấy phần còn lại để tránh mất tiền do làm tròn
            soTienDM = ns.so_tien_ns - tongDaPhanBo;
        } else {
            // Các danh mục trước: làm tròn xuống bình thường
            soTienDM = ns.so_tien_ns * mang_dm_chi[i].han_muc / 100;
            tongDaPhanBo += soTienDM;
        }

        printf("Ma: %-3d | Ten: %-15s | Han muc: %3d%% | So tien: %d VND\n",
               mang_dm_chi[i].ma_dm,
               mang_dm_chi[i].ten_dm,
               mang_dm_chi[i].han_muc,
               soTienDM);
    }

    printf("---------------------------------------------------\n");
    printf("=> Tong da phan bo: %d VND\n", ns.so_tien_ns);
}
