// Tạo, xóa danh mục
// Quản lý % cho các danh mục (gồm chia %, hoán đổi %)
// Tính tiền cho các danh mục dựa vào % được chia và ngân sách

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "khai_bao.h"
#include "danh_muc.h"
#include "giao_dich.h"

extern int so_luong_giao_dich_chi;

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

// Thêm danh mục (mã + tên), han_muc mặc định = 0
void themDanhMuc() {
    printf("\n--- THEM DANH MUC MOI ---\n");
    DanhMuc dm_temp;

    // --- BẮT ĐẦU ĐOẠN THÊM MỚI: Chọn loại danh mục ---
    int loai_dm;
    printf("Chon loai danh muc (0 - Thu, 1 - Chi, hoac 2 de huy): ");
    while (1) {
        if (scanf("%d", &loai_dm) != 1) {
            printf("Loi: Vui long nhap so. Nhap lai (0/1/2): ");
            xoaBuffer();
            continue;
        }
        xoaBuffer();
        
        if (loai_dm == 2) {
            printf("\n=> Da huy thao tac them danh muc. Quay lai Menu!\n");
            return;
        }
        if (loai_dm == 0 || loai_dm == 1) {
            break; // Hợp lệ thì thoát vòng lặp đi tiếp
        }
        printf("Loi: Chi duoc nhap 0, 1 hoac 2. Nhap lai: ");
    }
    
    // Nhập và kiểm tra mã danh mục
    int ma_moi;
    printf("\nNhap ma danh muc moi (Hoac nhap 0 de huy): ");
    
    while (1) {
        if (scanf("%d", &ma_moi) != 1) {
            printf("Loi: Vui long nhap mot so nguyen. Nhap lai (Hoac 0 de huy): ");
            xoaBuffer();
            continue;
        }
        xoaBuffer();

        // ---- XU LY LENH HUY ----
        if (ma_moi == 0) {
            printf("\n=> Da huy thao tac them danh muc. Quay lai Menu!\n");
            return; // Thoat luon khoi ham themDanhMucChi
        }

        if (ma_moi < 0) {
            printf("Loi: Ma danh muc phai lon hon 0. Nhap lai: ");
            continue;
        }

        // ---- KIEM TRA TRUNG LAP ----
        int bi_trung = 0;
        
        if (loai_dm == 0) { // Kiểm tra bên mảng Thu
            for (int i = 0; i < so_luong_dm_thu; i++) {
                if (mang_dm_thu[i].ma_dm == ma_moi) {
                    bi_trung = 1;
                    break;
                }
            }
        } else { // Kiểm tra bên mảng Chi
            for (int i = 0; i < so_luong_dm_chi; i++) {
                if (mang_dm_chi[i].ma_dm == ma_moi) {
                    bi_trung = 1;
                    break;
                }
            }
        }

        // Xu ly ket qua kiem tra
        if (bi_trung == 1) {
            printf("[LOI] Ma danh muc '%d' da ton tai!\n", ma_moi);
            printf("Vui long nhap mot ma khac chua duoc su dung (Hoac 0 de huy): ");
        } else {
            break; // Ma hop le (chua ton tai), thoat vong lap de tiep tuc nhap ten
        }
    }
    dm_temp.ma_dm = ma_moi;
    

    // Nhập tên danh mục
    printf("Nhap ten danh muc (khong khoang trang): ");
    scanf("%49s", dm_temp.ten_dm);

    // Hạn mức mặc định = 0, chưa phân bổ
    dm_temp.han_muc = 0;

    // Cấp phát mảng động
    if (loai_dm == 0) {
        so_luong_dm_thu++;
        mang_dm_thu = (DanhMuc*) realloc(mang_dm_thu, so_luong_dm_thu * sizeof(DanhMuc));
        mang_dm_thu[so_luong_dm_thu - 1] = dm_temp;
        printf("\n=> Da them danh muc THU '%s' (Ma: %d) thanh cong!\n", dm_temp.ten_dm, dm_temp.ma_dm);
    } else {
        so_luong_dm_chi++;
        mang_dm_chi = (DanhMuc*) realloc(mang_dm_chi, so_luong_dm_chi * sizeof(DanhMuc));
        mang_dm_chi[so_luong_dm_chi - 1] = dm_temp;
        printf("\n=> Da them danh muc CHI '%s' (Ma: %d) thanh cong!\n", dm_temp.ten_dm, dm_temp.ma_dm);
        printf("=> Han muc hien tai: 0%% (chua phan bo). Dung chuc nang 5 de phan bo ngan sach.\n");
    }
}

// Chia % ngân sách cho một danh mục đã có
void chiaPhanTramChi() {
    if (so_luong_dm_chi == 0) {
        printf("\nDanh sach danh muc Chi hien dang rong! Them danh muc truoc.\n");
        return;
    }

    // Hiển thị danh sách để người dùng chọn
    printf("\n=== DANH SACH DANH MUC CHI ===\n");
    printf("%-5s | %-15s | %-5s\n", "Ma", "Ten", "Han muc (%)");
    printf("----------------------------------\n");
    for (int i = 0; i < so_luong_dm_chi; i++) {
        printf("%-5d | %-15s | %d%%\n", mang_dm_chi[i].ma_dm, mang_dm_chi[i].ten_dm, mang_dm_chi[i].han_muc);
    }
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

void xemDanhSachDanhMuc() {
    printf("\n--- XEM DANH SACH DANH MUC ---\n");
    int loai_dm;
    printf("Chon danh sach can xem (0 - Thu, 1 - Chi, 2 - Tat ca, hoac 3 de huy): ");
    
    while (1) {
        if (scanf("%d", &loai_dm) != 1) {
            printf("Loi: Vui long nhap so (0/1/2/3). Nhap lai: ");
            xoaBuffer();
            continue;
        }
        xoaBuffer();
        if (loai_dm >= 0 && loai_dm <= 3) break;
        printf("Loi: Chi nhap 0, 1, 2 hoac 3. Nhap lai: ");
    }

    if (loai_dm == 3) {
        printf("\n=> Da huy thao tac xem.\n");
        return;
    }

    // In danh sach THU
    if (loai_dm == 0 || loai_dm == 2) {
        printf("\n=== DANH SACH DANH MUC THU ===\n");
        if (so_luong_dm_thu == 0) {
            printf("Danh sach thu hien dang rong.\n");
        } else {
            printf("%-5s | %-15s\n", "Ma", "Ten");
            printf("------------------------\n");
            for (int i = 0; i < so_luong_dm_thu; i++) {
                printf("%-5d | %-15s\n", mang_dm_thu[i].ma_dm, mang_dm_thu[i].ten_dm);
            }
            printf("------------------------\n");
        }
    }

    // In danh sach CHI
    if (loai_dm == 1 || loai_dm == 2) {
        printf("\n=== DANH SACH DANH MUC CHI ===\n");
        if (so_luong_dm_chi == 0) {
            printf("Danh sach chi hien dang rong.\n");
        } else {
            printf("%-5s | %-15s | %-5s\n", "Ma", "Ten", "Han muc (%)");
            printf("----------------------------------\n");
            for (int i = 0; i < so_luong_dm_chi; i++) {
                printf("%-5d | %-15s | %d%%\n", mang_dm_chi[i].ma_dm, mang_dm_chi[i].ten_dm, mang_dm_chi[i].han_muc);
            }
            printf("----------------------------------\n");
        }
    }
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
// Lọc theo tháng/năm của ngân sách truyền vào (tính % của 1 tháng/năm nhất định)
void tinhTiLeChi(NganSach ns) {
    if (so_luong_dm_chi == 0 || so_luong_giao_dich_chi == 0) {
        printf("\nKhong co du lieu de tinh ti le!\n");
        return;
    }

    int *so_tien_dm = (int*) calloc(so_luong_dm_chi, sizeof(int));
    if (so_tien_dm == NULL) {
        printf("\nLoi cap phat bo nho!\n");
        return;
    }

    int tong_da_chi = 0;

    // Lọc giao dịch chi theo khoảng thời gian (tháng hoặc năm)
    for (int i = 0; i < so_luong_giao_dich_chi; i++) {
        if (mang_chi[i].loai_gd != 1) {
            continue;
        }
        if (ns.thang != -1 && mang_chi[i].thang != ns.thang) {
            continue;
        }
        if (ns.nam != -1 && mang_chi[i].nam != ns.nam) {
            continue;
        }

        for (int j = 0; j < so_luong_dm_chi; j++) {
            if (mang_dm_chi[j].ma_dm == mang_chi[i].ma_dm) {
                so_tien_dm[j] += mang_chi[i].so_tien_gd;
                tong_da_chi += mang_chi[i].so_tien_gd;
                break;
            }
        }
    }

    if (tong_da_chi == 0) {
        if (ns.thang != -1) {
            printf("\nChua co giao dich Chi nao trong thang %d/%d!\n", ns.thang, ns.nam);
        } else {
            printf("\nChua co giao dich Chi nao trong nam %d!\n", ns.nam);
        }
        free(so_tien_dm);
        return;
    }

    if (ns.thang != -1) {
        printf("\n=== TI LE CHI GIUA CAC DANH MUC [%d/%d] ===\n", ns.thang, ns.nam);
    } else {
        printf("\n=== TI LE CHI GIUA CAC DANH MUC [%d] ===\n", ns.nam);
    }
    printf("Tong da chi: %d VND\n", tong_da_chi);
    printf("--------------------------------------------------------------\n");

    for (int i = 0; i < so_luong_dm_chi; i++) {
        double tiLe = tong_da_chi > 0
            ? (double)so_tien_dm[i] / tong_da_chi * 100.0
            : 0.0;    //Trường hợp tổng đã chi = 0
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
//Lam theo case 6 trong menu
void nhapVaTinhTiLeChi() {
    NganSach ns;
    int lua_chon;

    printf("\n--- TINH TI LE CHI GIUA CAC DANH MUC ---\n");
    printf("Chon khoang thoi gian:\n");
    printf("  1. Theo thang\n");
    printf("  2. Theo nam\n");

    // Nhập và kiểm tra lựa chọn
    while (1) {
        lua_chon = nhapSoNguyen();
        if (lua_chon == 1 || lua_chon == 2) {
            break;
        }
        printf("Loi: Vui long nhap 1 hoac 2: ");
    }

    if (lua_chon == 1) {
        printf("Thang (1-12): ");
        while (1) {
            ns.thang = nhapSoNguyen();
            if (ns.thang >= 1 && ns.thang <= 12) {
                break;
            }
            printf("Loi: Thang phai trong khoang 1-12. Nhap lai: ");
        }
    } else {
        ns.thang = -1;
    }

    // Xử lý nhập năm
    printf("Nam (>= 1900): ");
    while (1) {
        ns.nam = nhapSoNguyen();
        if (ns.nam >= 1900) {
            break;
        }
        printf("Loi: Nam phai >= 1900. Nhap lai: ");
    }

    if (ns.thang != -1) {
        snprintf(ns.ma_ns, sizeof(ns.ma_ns), "NS%04d%02d", ns.nam, ns.thang);
    } else {
        snprintf(ns.ma_ns, sizeof(ns.ma_ns), "NS%04d", ns.nam);
    }
    ns.so_tien_ns = 0;

    tinhTiLeChi(ns);
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


void xoaDanhMuc() {
    printf("\n--- XOA DANH MUC ---\n");
    int loai_dm;
    printf("Chon loai danh muc can xoa (0 - Thu, 1 - Chi, hoac 2 de huy): ");
    
    while (1) {
        if (scanf("%d", &loai_dm) != 1) {
            printf("Loi: Vui long nhap so (0/1/2). Nhap lai: ");
            xoaBuffer();
            continue;
        }
        xoaBuffer();
        if (loai_dm >= 0 && loai_dm <= 2) break;
        printf("Loi: Chi nhap 0, 1 hoac 2. Nhap lai: ");
    }

    if (loai_dm == 2) {
        printf("\n=> Da huy thao tac xoa danh muc.\n");
        return;
    }

    int ma_xoa;
    printf("Nhap ma danh muc can xoa (Hoac 0 de huy): ");
    while (scanf("%d", &ma_xoa) != 1) {
        printf("Loi: Vui long nhap mot so. Nhap lai: ");
        xoaBuffer();
    }
    xoaBuffer();

    if (ma_xoa == 0) {
        printf("\n=> Da huy thao tac xoa.\n");
        return;
    }

    int vi_tri = -1;
    // Phep xoa cho mang THU
    if (loai_dm == 0) {
        for (int i = 0; i < so_luong_dm_thu; i++) {
            if (mang_dm_thu[i].ma_dm == ma_xoa) { vi_tri = i; break; }
        }
        if (vi_tri == -1) {
            printf("[LOI] Khong tim thay ma danh muc THU '%d'!\n", ma_xoa);
        } else {
            for (int i = vi_tri; i < so_luong_dm_thu - 1; i++) {
                mang_dm_thu[i] = mang_dm_thu[i + 1]; // Dich trai de xoa
            }
            so_luong_dm_thu--;
            printf("=> Da xoa thanh cong danh muc THU ma '%d'!\n", ma_xoa);
        }
    } 
    // Phep xoa cho mang CHI
    else {
        for (int i = 0; i < so_luong_dm_chi; i++) {
            if (mang_dm_chi[i].ma_dm == ma_xoa) { vi_tri = i; break; }
        }
        if (vi_tri == -1) {
            printf("[LOI] Khong tim thay ma danh muc CHI '%d'!\n", ma_xoa);
        } else {
            for (int i = vi_tri; i < so_luong_dm_chi - 1; i++) {
                mang_dm_chi[i] = mang_dm_chi[i + 1]; // Dich trai de xoa
            }
            so_luong_dm_chi--;
            printf("=> Da xoa thanh cong danh muc CHI ma '%d'!\n", ma_xoa);
        }
    }
}
