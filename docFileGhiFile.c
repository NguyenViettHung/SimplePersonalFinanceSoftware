#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int ma_dm; // mã danh mục
    char ten_dm[50]; // tên danh mục
    int han_muc; // hạn mức theo phần trăm
} DanhMuc;

DanhMuc *mang_dm_thu = NULL;
DanhMuc *mang_dm_chi = NULL;
int so_luong_dm_thu = 0;
int so_luong_dm_chi = 0;

void saveData() {
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

void loadData() {
    FILE *file = fopen("danh_muc.txt", "r");
    if (file == NULL) {
        // Nếu file chưa tồn tại
        printf("=> Chua co file 'danh_muc.txt'. He thong se tao moi khi ban luu du lieu.\n");
        return;
    }

    char line[200];
    
    // Đọc từng dòng cho đến khi hết file
    while (fgets(line, sizeof(line), file)) {
        DanhMuc dm_temp;
        
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
        dm_temp.ma_dm = atoi(token);

        // Lấy Tên danh mục
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(dm_temp.ten_dm, token);

        // Lấy Hạn mức %
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        dm_temp.han_muc = atoi(token);

        // Đưa dữ liệu vào đúng mảng dựa trên cờ phân loại
        if (loai_dm == 0) {
            so_luong_dm_thu++;
            mang_dm_thu = (DanhMuc*) realloc(mang_dm_thu, so_luong_dm_thu * sizeof(DanhMuc));
            mang_dm_thu[so_luong_dm_thu - 1] = dm_temp;
        } 
        else if (loai_dm == 1) {
            so_luong_dm_chi++;
            mang_dm_chi = (DanhMuc*) realloc(mang_dm_chi, so_luong_dm_chi * sizeof(DanhMuc));
            mang_dm_chi[so_luong_dm_chi - 1] = dm_temp;
        }
    }

    fclose(file);
    printf("=> Da tai du lieu thanh cong: %d Thu | %d Chi.\n", so_luong_dm_thu, so_luong_dm_chi);
}
