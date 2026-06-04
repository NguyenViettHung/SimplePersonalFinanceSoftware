#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int ma_dm; // mã danh mục
    char ten_dm[50]; // tên danh mục
    int han_muc; // hạn mức theo phần trăm
} danhMuc;

danhMuc *mangDMThu = NULL;
danhMuc *mangDMChi = NULL;
int countDMThu = 0;
int countDMChi = 0;
// Chỉ cho phép kiểu số nguyên
int nhapSoNguyen() {
    int giaTri;
    int kiemTra;
    char kiTuThua;

    while (1) { 
        kiemTra = scanf("%d", &giaTri);

        if (kiemTra != 1) {
            // TH1: Nhập chữ cái hoặc ký tự đặc biệt ngay từ đầu
            printf("\nDu lieu khong phai la so! Vui long chi nhap so nguyen.\n");
            printf("Nhap lai: ");
            
            // Dọn dẹp bộ nhớ đệm
            while (getchar() != '\n'); 
        } else {
            // TH2: Bắt đầu bằng số nhưng dính chữ ở đuôi
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
    for (int i=0; i< countDMChi; i++){
        tong+=mangDMChi[i].han_muc;
    }
    return tong;
}

void inDanhSachChi() {
    printf("\n=== DANH SACH DANH MUC CHI (%d) ===\n", countDMChi);
    if (countDMChi == 0) {
        printf("Danh sach hien dang rong.\n");
        return;
    }
    
    int tong = 0;
    for (int i = 0; i < countDMChi; i++) {
        printf("Ma: %-3d | Ten: %-10s | Han muc: %d%%\n", 
               mangDMChi[i].ma_dm, mangDMChi[i].ten_dm, mangDMChi[i].han_muc);
        tong += mangDMChi[i].han_muc;
    }
    printf("-----------------------------------\n");
    printf("=> Tong han muc da phan bo: %d%%\n", tong);
    printf("=> Ngan sach con lai: %d%%\n", 100 - tong);
}

void themDanhMucChi() {
    // Kiểm tra ngân sách 
    int tongHienTai = tongHanMuc();
    int phanTramConLai = 100 - tongHienTai;
    
    if (phanTramConLai <= 0) {
        printf("\nNgan sach danh muc Chi da phan bo het 100%%!\n");
        return; 
    }

    printf("\n--- THEM DANH MUC CHI ---\n");
    printf("Ban dang con %d%% ngan sach chua phan bo.\n", phanTramConLai);
    
    danhMuc dmTemp;
    
    // Nhập và kiểm tra mã danh mục
    printf("Nhap ma danh muc (so nguyen): ");
    dmTemp.ma_dm = nhapSoNguyen();

    for (int i = 0; i < countDMChi; i++) {
        if (mangDMChi[i].ma_dm == dmTemp.ma_dm) {
            printf("Loi: Ma danh muc %d da ton tai!\n", dmTemp.ma_dm);
            return;
        }
    }

    // Nhập tên và phần trăm
    printf("Nhap ten danh muc (khong khoang trang): ");
    scanf("%49s", dmTemp.ten_dm);

    printf("Nhap han muc (%%): ");
    dmTemp.han_muc = nhapSoNguyen();
    if (dmTemp.han_muc <= 0) {
        printf("Loi: Han muc phai lon hon 0%%!\n");
        return;
    }
    if (dmTemp.han_muc > phanTramConLai) {
        printf("Loi: Vuot qua 100%% ngan sach! Toi da chi duoc %d%%.\n", phanTramConLai);
        return;
    }

    // Cấp phát mảng động và lưu dữ liệu
    countDMChi++;
    mangDMChi = (danhMuc*) realloc(mangDMChi, countDMChi * sizeof(danhMuc));
    mangDMChi[countDMChi - 1] = dmTemp;
    
    // In kết quả
    int phanTramCapNhat = 100 - tongHanMuc();
    printf("\n=> Da them danh muc '%s' thanh cong!\n", dmTemp.ten_dm);
    printf("=> NGAN SACH CHI CHUA PHAN BO CON LAI: %d%%\n", phanTramCapNhat);
}

void xoaDanhMucChi() {
    if (countDMChi == 0) {
        printf("\nDanh sach danh muc Chi hien dang rong!\n");
        return;
    }

    int maCanXoa;
    printf("\n--- XOA DANH MUC CHI ---\n");
    printf("Nhap ma danh muc can xoa: ");
    scanf("%d", &maCanXoa);

    // Tìm vị trí cần xóa
    int viTriXoa = -1;
    for (int i = 0; i < countDMChi; i++) {
        if (mangDMChi[i].ma_dm == maCanXoa) {
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
    strcpy(tenDaXoa, mangDMChi[viTriXoa].ten_dm);

    // Dồn mảng
    for (int i = viTriXoa; i < countDMChi - 1; i++) {
        mangDMChi[i] = mangDMChi[i + 1];
    }

    // Cắt bỏ ô nhớ thừa ở cuối mảng
    countDMChi--;
    if (countDMChi == 0) {
        free(mangDMChi);
        mangDMChi = NULL;
    } else {
        mangDMChi = (danhMuc*) realloc(mangDMChi, countDMChi * sizeof(danhMuc));
    }

    // In kết quả
    int phanTramCapNhat = 100 - tongHanMuc();
    printf("\n=> Da xoa danh muc '%s' thanh cong!\n", tenDaXoa);
    printf("=> NGAN SACH CHI CHUA PHAN BO CON LAI: %d%%\n", phanTramCapNhat);
}

void hoanDoiPhanTramChi() {
    // Kiểm tra điều kiện
    if (countDMChi < 2) {
        printf("\nCan co it nhat 2 danh muc Chi de thuc hien hoan doi!\n");
        return;
    }

    int ma1, ma2;
    printf("\n--- HOAN DOI %% DANH MUC CHI ---\n");
    printf("Nhap MA danh muc thu nhat: ");
    scanf("%d", &ma1);
    printf("Nhap MA danh muc thu hai: ");
    scanf("%d", &ma2);

    // 2 mã giống nhau
    if (ma1 == ma2) {
        printf("\nHai ma danh muc phai khac nhau!\n");
        return;
    }

    // Tìm vị trí của 2 danh mục trong mảng
    int viTri1 = -1, viTri2 = -1;
    for (int i = 0; i < countDMChi; i++) {
        if (mangDMChi[i].ma_dm == ma1) {
            viTri1 = i;
        }
        if (mangDMChi[i].ma_dm == ma2) {
            viTri2 = i;
        }
    }

    // Kiểm tra xem có tìm thấy cả 2 không
    if (viTri1 == -1 || viTri2 == -1) {
        printf("\nKhong tim thay mot hoac ca hai ma danh muc vua nhap. Vui long thu lai!\n");
        return;
    }

    // Lưu lại % cũ để in thông báo
    int cu1 = mangDMChi[viTri1].han_muc;
    int cu2 = mangDMChi[viTri2].han_muc;

    // Hoán đổi
    int temp = mangDMChi[viTri1].han_muc;
    mangDMChi[viTri1].han_muc = mangDMChi[viTri2].han_muc;
    mangDMChi[viTri2].han_muc = temp;

    // 4. In thông báo thành công
    printf("\n=> HOAN DOI THANH CONG!\n");
    printf(" Danh muc '%s' (Ma: %d): %d%% -> %d%%\n", 
           mangDMChi[viTri1].ten_dm, ma1, cu1, mangDMChi[viTri1].han_muc);
    printf(" Danh muc '%s' (Ma: %d): %d%% -> %d%%\n", 
           mangDMChi[viTri2].ten_dm, ma2, cu2, mangDMChi[viTri2].han_muc);
}

// int main() {
//     int luaChon;
    
//     do {
//         printf("\n========= MENU TEST DANH MUC CHI =========\n");
//         printf("1. Xem danh sach danh muc hien tai\n");
//         printf("2. Them mot danh muc chi\n");
//         printf("3. Xoa mot danh muc chi\n");
//         printf("4. Hoan doi %% giua 2 danh muc\n");
//         printf("0. Thoat\n");
//         printf("Nhap lua chon cua ban: ");
//         luaChon = nhapSoNguyen();

//         switch (luaChon) {
//             case 1:
//                 inDanhSachChi();
//                 break;
//             case 2:
//                 themDanhMucChi();
//                 break;
//             case 3:
//                 xoaDanhMucChi();
//                 break;
//             case 4:
//                 hoanDoiPhanTramChi();
//                 break;
//             case 0:
//                 printf("\nDang thoat chuong trinh...\n");
//                 // Giải phóng bộ nhớ trước khi tắt app để tránh memory leak
//                 if (mangDMChi != NULL) {
//                     free(mangDMChi);
//                     mangDMChi = NULL;
//                     printf("=> Da giai phong bo nho thanh cong.\n");
//                 }
//                 break;
//             default:
//                 printf("\nLoi: Lua chon khong hop le! Vui long nhap tu 0 den 4.\n");
//         }
//     } while (luaChon != 0);

//     return 0;
// }