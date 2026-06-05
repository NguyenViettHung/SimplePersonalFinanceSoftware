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

void saveData() {
    FILE *file = fopen("danh_muc.txt", "w");
    if (file == NULL) {
        printf("\n[LỖI] Khong the tao hoac mo file 'danh_muc.txt' de ghi du lieu!\n");
        return;
    }

    // Ghi mảng danh mục thu (số 0 ở đầu)
    for (int i = 0; i < countDMThu; i++) {
        fprintf(file, "0,%d,%s,%d\n", 
                mangDMThu[i].ma_dm, 
                mangDMThu[i].ten_dm, 
                mangDMThu[i].han_muc);
    }

    // Ghi mảng danh mục chi (số 1 ở đầu)
    for (int i = 0; i < countDMChi; i++) {
        fprintf(file, "1,%d,%s,%d\n", 
                mangDMChi[i].ma_dm, 
                mangDMChi[i].ten_dm, 
                mangDMChi[i].han_muc);
    }

    fclose(file);
    printf("\n=> Da luu thanh cong %d danh muc Thu va %d danh muc Chi vao file!\n", countDMThu, countDMChi);
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
        danhMuc dmTemp;
        
        // Xóa ký tự xuống dòng '\n' ở cuối chuỗi nếu có
        line[strcspn(line, "\n")] = 0; 
        
        // Bỏ qua các dòng trống
        if (strlen(line) == 0) continue;

        // Tách chuỗi bằng dấu phẩy
        char *token = strtok(line, ",");
        if (token == NULL) continue;
        
        // Lấy thông tin cờ phân loại (0 = Thu, 1 = Chi)
        int loaiDM = atoi(token);

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
        if (loaiDM == 0) {
            countDMThu++;
            mangDMThu = (danhMuc*) realloc(mangDMThu, countDMThu * sizeof(danhMuc));
            mangDMThu[countDMThu - 1] = dmTemp;
        } 
        else if (loaiDM == 1) {
            countDMChi++;
            mangDMChi = (danhMuc*) realloc(mangDMChi, countDMChi * sizeof(danhMuc));
            mangDMChi[countDMChi - 1] = dmTemp;
        }
    }

    fclose(file);
    printf("=> Da tai du lieu thanh cong: %d Thu | %d Chi.\n", countDMThu, countDMChi);
}