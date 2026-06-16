#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "khai_bao.h"
#include "file_io.h"
#include "giao_dich.h"

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
        gd_temp.ma_dm, atoi(strtok(NULL, ","));
        strcpy(gd_temp.ma_ns,    strtok(NULL, ","));
        strcpy(gd_temp.ghi_chu,  strtok(NULL, ","));
        if (gd_temp.loai_gd == 0){
            so_luong_thu++;
            mang_thu = (struct GiaoDich*) realloc(mang_thu, so_luong_thu * sizeof(struct GiaoDich));
            mang_thu[so_luong_thu - 1] =gd_temp;
        }
        else if (gd_temp.loai_gd == 1){
            so_luong_chi++;
            mang_chi = (struct GiaoDich*) realloc(mang_chi, so_luong_chi * sizeof(struct GiaoDich));
            mang_chi[so_luong_chi - 1] = gd_temp;
        }
    }
    fclose(file);

    if (so_luong_thu > 1) insertionSortGiaoDich (mang_thu, so_luong_thu);
    if (so_luong_chi > 1) insertionSortGiaoDich (mang_chi, so_luong_chi);
    printf("Đã đọc và sắp xếp file giao dịch thành công\n");
}

// Hàm ghi file
void ghiFileGiaoDich(){
    FILE *file = fopen("giao_dich.txt", "w");
    if (file == NULL){
        printf ("Lỗi mở file ghi đè\n");
        return;
    }

    // Ghi mảng thu
    for (int i = 0; i < so_luong_thu; i++){
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
    for (int i = 0; i < so_luong_chi; i++){
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
    printf("Đã lưu %d giao dịch thu và %d giao dịch chi\n", so_luong_thu, so_luong_chi);
}
