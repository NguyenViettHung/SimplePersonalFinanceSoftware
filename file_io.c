#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "khai_bao.h"
#include "file_io.h"
#include "giao_dich.h"
#include "danh_muc.h"

extern struct GiaoDich* mang_thu;
extern struct GiaoDich* mang_chi;
extern struct NganSach *mang_ngan_sach;
extern DanhMuc *mang_dm_thu;
extern DanhMuc *mang_dm_chi;
extern int so_luong_giao_dich_thu;
extern int so_luong_giao_dich_chi;
extern int so_luong_dm_chi;
extern int so_luong_dm_thu;
extern int so_luong_ngan_sach;
extern int suc_chua_ngan_sach;
// Hàm đọc file
void docFileGiaoDich() {
    FILE *file = fopen("giao_dich.txt", "r");
    if (file == NULL) {
        printf("=> Chua co file 'giao_dich.txt'. He thong se tao moi khi ban luu du lieu.\n");
        return;
    }

    char line[1000];
    
    // Reset bộ đếm trước khi đọc để tránh lỗi khi nạp lại dữ liệu nhiều lần
    so_luong_giao_dich_thu = 0;
    so_luong_giao_dich_chi = 0;

    while (fgets(line, sizeof(line), file)) { 
        struct GiaoDich gd_temp;
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;

char *token = strtok(line, ",");
        if (token == NULL) continue;
        strcpy(gd_temp.ma_gd, token);
        
        token = strtok(NULL, ","); if (token) gd_temp.so_tien_gd = atoi(token);
        token = strtok(NULL, ","); if (token) gd_temp.ngay       = atoi(token); 
        token = strtok(NULL, ","); if (token) gd_temp.thang      = atoi(token);
        token = strtok(NULL, ","); if (token) gd_temp.nam        = atoi(token);
        token = strtok(NULL, ","); if (token) gd_temp.loai_gd    = atoi(token);
        token = strtok(NULL, ","); if (token) gd_temp.ma_dm      = atoi(token);
        
        // KIỂM TRA NULL TRƯỚC KHI STRCPY CHO CHUỖI
        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(gd_temp.ma_ns, token);
        } else {
            strcpy(gd_temp.ma_ns, ""); // Nếu rỗng thì gán chuỗi rỗng
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(gd_temp.ghi_chu, token);
        } else {
            strcpy(gd_temp.ghi_chu, ""); // Nếu ghi chú rỗng thì gán chuỗi rỗng để không bị Crash
        }
        //     BẮT ĐẦU BỘ LỌC
        // 1. Chặn số tiền âm hoặc bằng 0
        if (gd_temp.so_tien_gd <= 0) {
            printf("Canh bao Giao dich '%s': So tien (%d) khong hop le. Da bo qua!\n", gd_temp.ma_gd, gd_temp.so_tien_gd);
            continue;
        }

        // 2. Chặn mã danh mục dị thường (<= 0)
        if (gd_temp.ma_dm <= 0) {
            printf("Canh bao Giao dich '%s': Ma danh muc (%d) khong hop le. Da bo qua!\n", gd_temp.ma_gd, gd_temp.ma_dm);
            continue;
        }

        // 3. Chặn mã danh mục KHÔNG CÓ THỰC (ví dụ mã 9999)
        int ton_tai_dm = 0;
        if (gd_temp.loai_gd == 0) {
            for (int i = 0; i < so_luong_dm_thu; i++) {
                if (mang_dm_thu[i].ma_dm == gd_temp.ma_dm) { ton_tai_dm = 1; break; }
            }
        } else if (gd_temp.loai_gd == 1) {
            for (int i = 0; i < so_luong_dm_chi; i++) {
                if (mang_dm_chi[i].ma_dm == gd_temp.ma_dm) { ton_tai_dm = 1; break; }
            }
        } else {
            printf("Canh bao Giao dich '%s': Loai giao dich (%d) sai. Da bo qua!\n", gd_temp.ma_gd, gd_temp.loai_gd);
            continue;
        }
        if (ton_tai_dm == 0) {
            printf("Canh bao Giao dich '%s': Ma danh muc '%d' KHONG TON TAI trong he thong. Da bo qua!\n", gd_temp.ma_gd, gd_temp.ma_dm);
            continue;
        }
        // 4. Kiểm tra ngày/tháng/năm hợp lệ
        if (gd_temp.nam < 1900 || gd_temp.thang < 1 || gd_temp.thang > 12) {
            printf("[Canh bao Giao dich '%s': Thang/Nam (%d/%d) khong hop le. Da bo qua!\n", gd_temp.ma_gd, gd_temp.thang, gd_temp.nam);
            continue;
        }
        // KẾT THÚC KIỂM DUYỆT
        // Đưa dữ liệu sạch vào mảng
        if (gd_temp.loai_gd == 0){
            so_luong_giao_dich_thu++;
            mang_thu = (struct GiaoDich*) realloc(mang_thu, so_luong_giao_dich_thu * sizeof(struct GiaoDich));
            mang_thu[so_luong_giao_dich_thu - 1] = gd_temp;
        }
        else if (gd_temp.loai_gd == 1){
            so_luong_giao_dich_chi++;
            mang_chi = (struct GiaoDich*) realloc(mang_chi, so_luong_giao_dich_chi * sizeof(struct GiaoDich));
            mang_chi[so_luong_giao_dich_chi - 1] = gd_temp;
        }
    }
    fclose(file);

    // Sắp xếp lại giao dịch theo thời gian nếu cần
    if (so_luong_giao_dich_thu > 1) insertionSortGiaoDich(mang_thu, so_luong_giao_dich_thu);
    if (so_luong_giao_dich_chi > 1) insertionSortGiaoDich(mang_chi, so_luong_giao_dich_chi);
    
    printf("=> Da tai du lieu giao dich (Da don dep giao dich loi).\n");
}

void ghiFileGiaoDich(){
    FILE *file = fopen("giao_dich.txt", "w");
    if (file == NULL){
        printf ("Loi mo file ghi de\n");
        return;
    }

    // Ghi mảng thu
    for (int i = 0; i < so_luong_giao_dich_thu; i++){
        fprintf(file, "%s,%d,%d,%d,%d,%d,%d,%s,%s\n",
                mang_thu[i].ma_gd, 
                mang_thu[i].so_tien_gd,
                mang_thu[i].ngay, mang_thu[i].thang, mang_thu[i].nam,
                mang_thu[i].loai_gd,
                mang_thu[i].ma_dm,
                mang_thu[i].ma_ns,
                mang_thu[i].ghi_chu);
    }

    // Ghi mảng chi
    for (int i = 0; i < so_luong_giao_dich_chi; i++){
        fprintf(file, "%s,%d,%d,%d,%d,%d,%d,%s,%s\n",
                mang_chi[i].ma_gd, 
                mang_chi[i].so_tien_gd,
                mang_chi[i].ngay, mang_chi[i].thang, mang_chi[i].nam,
                mang_chi[i].loai_gd,
                mang_chi[i].ma_dm,
                mang_chi[i].ma_ns,
                mang_chi[i].ghi_chu);
    }     
           
    fclose(file);
}

void ghiFileDanhMuc() {
    FILE *file = fopen("danh_muc.txt", "w");
    if (file == NULL) {
        printf("\n[LỖI] Khong the tao hoac mo file 'danh_muc.txt' de ghi du lieu!\n");
        return;
    }

    // Ghi mảng danh mục thu (số 0 ở đầu)
    for (int i = 0; i < so_luong_dm_thu; i++) {
        fprintf(file, "0,%d,%s,%d\n", 
                mang_dm_thu[i].ma_dm, 
                mang_dm_thu[i].ten_dm, 
                mang_dm_thu[i].han_muc);
    }

    // Ghi mảng danh mục chi (số 1 ở đầu)
    for (int i = 0; i < so_luong_dm_chi; i++) {
        fprintf(file, "1,%d,%s,%d\n", 
                mang_dm_chi[i].ma_dm, 
                mang_dm_chi[i].ten_dm, 
                mang_dm_chi[i].han_muc);
    }

    fclose(file);
    printf("\n=> Da luu thanh cong %d danh muc Thu va %d danh muc Chi vao file!\n", so_luong_dm_thu, so_luong_dm_chi);
}

void docFileDanhMuc() {
    FILE *file = fopen("danh_muc.txt", "r");
    if (file == NULL) {
        printf("=> Chua co file 'danh_muc.txt'. He thong se tao moi khi ban luu du lieu.\n");
        return;
    }

    char line[200];
    int tong_han_muc_chi = 0; // Biến theo dõi tổng % để chặn việc vượt quá 100%
    
    // Reset số lượng trước khi đọc (Phòng trường hợp gọi hàm nạp dữ liệu nhiều lần)
    so_luong_dm_thu = 0;
    so_luong_dm_chi = 0;

    while (fgets(line, sizeof(line), file)) {
        DanhMuc dmTemp;
        
        line[strcspn(line, "\n")] = 0; 
        if (strlen(line) == 0) continue;

        char *token = strtok(line, ",");
        if (token == NULL) continue;
        int loai_dm = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        dmTemp.ma_dm = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(dmTemp.ten_dm, token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        dmTemp.han_muc = atoi(token);

        // BẮT ĐẦU BỘ LỌC KIỂM DUYỆT
        // Chặn Mã âm hoặc Mã = 0
        if (dmTemp.ma_dm <= 0) {
            printf("Canh bao Ma '%d' (Ten: %s) khong hop le. Da bo qua!\n", dmTemp.ma_dm, dmTemp.ten_dm);
            continue; // Hủy bỏ, không nạp dòng này vào mảng
        }

        // Chặn trùng Mã (Duplicate ID)
        int bi_trung = 0;
        if (loai_dm == 0) {
            for (int i = 0; i < so_luong_dm_thu; i++) {
                if (mang_dm_thu[i].ma_dm == dmTemp.ma_dm) { bi_trung = 1; break; }
            }
            dmTemp.han_muc_tien = -1;
            dmTemp.han_muc_tien_goc = -1;
        } else if (loai_dm == 1) {
            for (int i = 0; i < so_luong_dm_chi; i++) {
                if (mang_dm_chi[i].ma_dm == dmTemp.ma_dm) { bi_trung = 1; break; }
            }
            dmTemp.han_muc_tien = 0;
            dmTemp.han_muc_tien_goc = 0;
        }
        
        if (bi_trung) {
            printf("Canh bao Phat hien trung ma '%d' (Ten: %s). Da bo qua dong bi trung!\n", dmTemp.ma_dm, dmTemp.ten_dm);
            continue;
        }

        // Chặn % hạn mức dị thường (< 0% hoặc > 100%)
        if (dmTemp.han_muc < 0 || dmTemp.han_muc > 100) {
            printf("Canh bao Han muc %d%% cua ma '%d' khong hop le. Tu dong dat ve 0%%!\n", dmTemp.han_muc, dmTemp.ma_dm);
            dmTemp.han_muc = 0; 
            // Giữ lại danh mục nhưng tước bỏ % dị thường
        }

        // Chặn tổng % tất cả các danh mục Chi vượt 100%
        if (loai_dm == 1) {
            if (tong_han_muc_chi + dmTemp.han_muc > 100) {
                int han_muc_cho_phep = 100 - tong_han_muc_chi;
                printf("Canh bao Ma '%d' lam tong ngan sach vuot 100%%. Ep han muc tu %d%% xuong %d%%!\n", 
                       dmTemp.ma_dm, dmTemp.han_muc, han_muc_cho_phep);
                dmTemp.han_muc = han_muc_cho_phep;
            }
            tong_han_muc_chi += dmTemp.han_muc; // Cộng dồn phần trăm hiện tại
        }

        // KẾT THÚC KIỂM DUYỆT
        // Đưa dữ liệu sạch vào mảng
        if (loai_dm == 0) {
            so_luong_dm_thu++;
            mang_dm_thu = (DanhMuc*) realloc(mang_dm_thu, so_luong_dm_thu * sizeof(DanhMuc));
            mang_dm_thu[so_luong_dm_thu - 1] = dmTemp;
        } 
        else if (loai_dm == 1) {
            so_luong_dm_chi++;
            mang_dm_chi = (DanhMuc*) realloc(mang_dm_chi, so_luong_dm_chi * sizeof(DanhMuc));
            mang_dm_chi[so_luong_dm_chi - 1] = dmTemp;
        }
    }

    fclose(file);
    printf("=> Da tai du lieu danh muc (Da don dep danh muc loi).\n");
}

void docFileNganSach() {
    char *ten_file = "ngan_sach.txt";
    FILE *file = fopen(ten_file, "r");
    if (file == NULL) {
        printf("Thong bao: Chua co file %s, se tao moi khi ghi.\n", ten_file);
        return;
    }
    
    if (mang_ngan_sach == NULL) {
        khoiTaoNganSach(10);
    }
    
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        
        // Xóa ký tự newline
        line[strcspn(line, "\n")] = 0;
        
        // Bỏ qua dòng trống
        if (strlen(line) == 0) continue;
        
        struct NganSach ns_tam;
        memset(&ns_tam, 0, sizeof(struct NganSach));
        
        char *token;
        char line_copy[200];
        strcpy(line_copy, line);

        token = strtok(line_copy, ",");
        if (token == NULL) continue;
        strncpy(ns_tam.ma_ns, token, sizeof(ns_tam.ma_ns) - 1);
        
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        ns_tam.so_tien_ns = atoi(token);
        
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        ns_tam.thang = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        ns_tam.nam = atoi(token);

        if (strlen(ns_tam.ma_ns) == 0) {
            printf("Canh bao: Dong co ma_ns trong, bo qua.\n");
            continue;
        }

        if (ns_tam.so_tien_ns <= 0) {
            printf("Canh bao: %s co so tien <= 0, bo qua.\n", ns_tam.ma_ns);
            continue;
        }

        if (ns_tam.thang < 1 || ns_tam.thang > 12) {
            printf("Canh bao: %s co thang khong hop le, bo qua.\n", ns_tam.ma_ns);
            continue;
        }

        if (ns_tam.nam < 1900) {
            printf("Canh bao: %s co nam khong hop le, bo qua.\n", ns_tam.ma_ns);
            continue;
        }
        
        //Cấp phát lại nếu mảng đầy
        if (so_luong_ngan_sach >= suc_chua_ngan_sach) {
            suc_chua_ngan_sach += 12;
            struct NganSach *tmp = (struct NganSach *)realloc(
                mang_ngan_sach, suc_chua_ngan_sach * sizeof(struct NganSach));
            if (tmp == NULL) {
                printf("Loi: Khong the cap phat bo nho.\n");
                fclose(file);
                return;
            }
            mang_ngan_sach = tmp;
        }

        mang_ngan_sach[so_luong_ngan_sach] = ns_tam;
        so_luong_ngan_sach++;
    }
    printf("=> Da tai du lieu ngan sach (Da don dep ngan sach loi).\n");
    fclose(file);
}
 
void ghiFileNganSach() {
    char *ten_file = "ngan_sach.txt";
    FILE *file = fopen(ten_file, "w");
    if (file == NULL) {
        printf("Loi: Khong the mo file %s de ghi.\n", ten_file);
        return;
    }

    for (int i = 0; i < so_luong_ngan_sach; i++) {
        fprintf(file, "%s,%d,%d,%d\n",
                mang_ngan_sach[i].ma_ns,
                mang_ngan_sach[i].so_tien_ns,
                mang_ngan_sach[i].thang,
                mang_ngan_sach[i].nam);
    }
    
    fclose(file);
}


void napDuLieu() {
    docFileNganSach();
    docFileDanhMuc();
    docFileGiaoDich();
    capNhatHanMucTien();
}

void luuDuLieu() {
    ghiFileDanhMuc();
    ghiFileGiaoDich();
    ghiFileNganSach();
}
