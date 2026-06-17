#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "khai_bao.h"
#include "file_io.h"
#include "giao_dich.h"

extern struct GiaoDich* mang_thu;
extern struct GiaoDich* mang_chi;
extern struct NganSach *mang_ngan_sach;
extern DanhMuc *mang_dm_thu;
extern DanhMuc *mang_dm_chi;
extern int so_luong_giao_dich_thu;
extern int so_luong_giao_dich_chi;
extern int count_dm_chi;
extern int count_dm_thu;
extern int so_luong_ngan_sach;
extern int suc_chua_ngan_sach;
// Hàm đọc file
void docFileGiaoDich(){
    FILE *file = fopen ("giao_dich.txt", "r");
    if (file == NULL){
        printf ("Chưa có file giao dịch, sẽ tạo mới khi ghi.\n");
        return;
    }

    char line[1000];
    while (fgets (line, sizeof(line), file)){ 
        struct GiaoDich gd_temp;
        line[strcspn(line, "\n")] = 0;
        char *token = strtok(line, ",");
        if (token == NULL) continue;
        strcpy(gd_temp.ma_gd, token);
        gd_temp.so_tien_gd= atoi(strtok(NULL,  ","));
        gd_temp.ngay = atoi(strtok(NULL,  ",")); 
        gd_temp.thang = atoi(strtok(NULL,  ","));
        gd_temp.nam = atoi(strtok(NULL, ","));
        gd_temp.loai_gd = atoi(strtok(NULL, ","));
        gd_temp.ma_dm = atoi(strtok(NULL, ","));
        strcpy(gd_temp.ma_ns,    strtok(NULL, ","));
        strcpy(gd_temp.ghi_chu,  strtok(NULL, ","));
        if (gd_temp.loai_gd == 0){
            so_luong_giao_dich_thu++;
            mang_thu = (struct GiaoDich*) realloc(mang_thu, so_luong_giao_dich_thu * sizeof(struct GiaoDich));
            mang_thu[so_luong_giao_dich_thu - 1] =gd_temp;
        }
        else if (gd_temp.loai_gd == 1){
            so_luong_giao_dich_chi++;
            mang_chi = (struct GiaoDich*) realloc(mang_chi, so_luong_giao_dich_chi * sizeof(struct GiaoDich));
            mang_chi[so_luong_giao_dich_chi - 1] = gd_temp;
        }
    }
    fclose(file);

    if (so_luong_giao_dich_thu > 1) insertionSortGiaoDich (mang_thu, so_luong_giao_dich_thu);
    if (so_luong_giao_dich_chi > 1) insertionSortGiaoDich (mang_chi, so_luong_giao_dich_chi);
}

void ghiFileGiaoDich(){
    FILE *file = fopen("giao_dich.txt", "w");
    if (file == NULL){
        printf ("Lỗi mở file ghi đè\n");
        return;
    }

    // Ghi mảng thu
    for (int i = 0; i < so_luong_giao_dich_thu; i++){
        fprintf(file, "%s, %d, %d, %d, %d, %d, %s, %d, %s\n",
                mang_thu[i].ma_gd, 
                mang_thu[i].so_tien_gd,
                mang_thu[i].ngay, mang_thu[i].thang, mang_thu[i].nam,
                mang_thu[i].loai_gd,
                mang_thu[i].ghi_chu,
                mang_thu[i].ma_ns,
                mang_thu[i].ma_dm); 
    }

    // Ghi mảng chi
    for (int i = 0; i < so_luong_giao_dich_chi; i++){
        fprintf(file, "%s, %d, %d, %d, %d, %d, %s, %d, %s\n",
                mang_chi[i].ma_gd, 
                mang_chi[i].so_tien_gd,
                mang_chi[i].ngay, mang_chi[i].thang, mang_chi[i].nam,
                mang_chi[i].loai_gd,
                mang_thu[i].ghi_chu,
                mang_chi[i].ma_ns,
                mang_chi[i].ma_dm);
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
    for (int i = 0; i < count_dm_thu; i++) {
        fprintf(file, "0,%d,%s,%d\n", 
                mang_dm_thu[i].ma_dm, 
                mang_dm_thu[i].ten_dm, 
                mang_dm_thu[i].han_muc);
    }

    // Ghi mảng danh mục chi (số 1 ở đầu)
    for (int i = 0; i < count_dm_chi; i++) {
        fprintf(file, "1,%d,%s,%d\n", 
                mang_dm_chi[i].ma_dm, 
                mang_dm_chi[i].ten_dm, 
                mang_dm_chi[i].han_muc);
    }

    fclose(file);
    printf("\n=> Da luu thanh cong %d danh muc Thu va %d danh muc Chi vao file!\n", count_dm_thu, count_dm_chi);
}

void docFileDanhMuc() {
    FILE *file = fopen("danh_muc.txt", "r");
    if (file == NULL) {
        // Nếu file chưa tồn tại
        printf("=> Chua co file 'danh_muc.txt'. He thong se tao moi khi ban luu du lieu.\n");
        return;
    }

    char line[200];
    
    // Đọc từng dòng cho đến khi hết file
    while (fgets(line, sizeof(line), file)) {
        DanhMuc dmTemp;
        
        // Xóa ký tự xuống dòng '\n' ở cuối chuỗi nếu có
        line[strcspn(line, "\n")] = 0; 
        
        // Bỏ qua các dòng trống
        if (strlen(line) == 0) continue;

        // Tách chuỗi bằng dấu phẩy
        char *token = strtok(line, ",");
        if (token == NULL) continue;
        
        // Lấy thông tin cờ phân loại (0 = Thu, 1 = Chi)
        int loai_dm = atoi(token);

        // Lấy Mã danh mục
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        dmTemp.ma_dm = atoi(token);

        // Lấy Tên danh mục
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(dmTemp.ten_dm, token);

        // Lấy Hạn mức %
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        dmTemp.han_muc = atoi(token);

        // Đưa dữ liệu vào đúng mảng dựa trên cờ phân loại
        if (loai_dm == 0) {
            count_dm_thu++;
            mang_dm_thu = (DanhMuc*) realloc(mang_dm_thu, count_dm_thu * sizeof(DanhMuc));
            mang_dm_thu[count_dm_thu - 1] = dmTemp;
        } 
        else if (loai_dm == 1) {
            count_dm_chi++;
            mang_dm_chi = (DanhMuc*) realloc(mang_dm_chi, count_dm_chi * sizeof(DanhMuc));
            mang_dm_chi[count_dm_chi - 1] = dmTemp;
        }
    }

    fclose(file);
    printf("=> Da tai du lieu thanh cong: %d Thu | %d Chi.\n", count_dm_thu, count_dm_chi);
}

//Hàm đọc file ngan_sach.txt
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
    
    fclose(file);
}
 
//Hàm ghi file ngan_sach.txt
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
    docFileGiaoDich();
    docFileDanhMuc();
    docFileNganSach();
}

void luuDuLieu() {
    ghiFileGiaoDich();
    ghiFileDanhMuc();
    ghiFileNganSach();
}