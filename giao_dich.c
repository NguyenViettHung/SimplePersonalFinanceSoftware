// phần giao dịch thu/chi gồm khai báo, đọc file và ghi đè
#include <stdio.h>
#include <stdlib.h> // Chứa hàm realloc, atoi
#include <string.h> // Chứa hàm strcpy, strtok, strcspn
// Định nghĩa struct
// Cấu trúc dữ liệu cho giao dịch
struct GiaoDich{
    char ma_gd[15]; // mã giao dịch
    int so_tien; // số tiền tương ứng với giao dịch
    int ngay; // ngày giao dịch
    int thang; // tháng giao dịch
    int nam; // năm giao dịch
    int loai_gd; // loại giao dịch: 0 - thu, 1 - chi
    char danh_muc[30]; // loại danh mục
    char ghi_chu[200]; // ghi chú
    char ma_ns[15]; // mã ngân sách (FK)
    char ma_tk[15]; // mã tài khoản (FK)
};

// Khai báo mảng động và biến đếm
struct GiaoDich* mangThu = NULL;
struct GiaoDich* mangChi = NULL;
int soLuongThu = 0;
int soLuongChi = 0;

// Hàm so sánh ngày (trả về số âm nếu ngày a xảy ra trước ngày b và ngược lại)
int soSanhNgay (struct GiaoDich a, struct GiaoDich b){
    if (a.nam != b.nam) return a.nam - b .nam;
    if (a.thang != b.thang) return a.thang - b.thang;
    return a.ngay - b.ngay;
    // Hàm này không nhận diện được số ngày trong thực tế, ví dụ nhập a là ngày 20/4/2025 và b là ngày 31/4/2026 thì vẫn chạy
    // => lỗ hổng nếu người dùng nhập sai
}// mục đích sử dụng cho quick sort

// Hàm hoán vị (sử dụng sau khi đã so sánh ngày => sắp xếp ngày theo đúng trật tự)
void hoanVi (struct GiaoDich* a, struct GiaoDich* b){
    struct GiaoDich temp = *a;
    *a = *b;
    *b = temp;
}

// Quick Sort (dùng mid pivot)
void quickSortGiaoDich (struct GiaoDich arr[], int left, int right){
    if (left>= right) return;

    // chọn mid pivot
    struct GiaoDich pivot = arr[ left + (right - left)/2];
    int i = left;
    int j = right;
    while (i <= j){
        while (soSanhNgay (arr[i], pivot) < 0){ // giao dịch đúng vị trí (trước mốc pivot)
            i++;
        }
        while (soSanhNgay (arr[j], pivot) > 0){ // giao dịch đúng vị trí (sau mốc pivot)
            j--;
        }
        if (i <= j){
            hoanVi(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }
    if (left <j){
        quickSortGiaoDich (arr, left, j);
    }
    if (i < right){
        quickSortGiaoDich (arr, i, right);
    }
}

// Hàm đọc file
void docFileGiaoDich(){
    FILE *file = fopen ("giao_dich.txt", "r");
    if (file == NULL){
        printf ("Chưa có file giao dịch, sẽ tạo mới khi ghi.\n");
        return;
    }

    char line[1000]; 
    while (fgets (line, sizeof(line), file)){ // lấy từng dòng trong file cho vào line
        struct GiaoDich gdTemp;
        line[strcspn(line, "\n")] = 0; // xóa ký tự xuống dòng (\n)
        char *token = strtok(line, ","); // strtok = string token
        if (token == NULL) continue;
        strcpy(gdTemp.ma_gd, token); // sao chép nội dung token vào ma_gd
        gdTemp.so_tien = atoi(strtok(NULL,  ","));
        gdTemp.ngay = atoi(strtok(NULL,  ",")); //atoi = ASCII to interger, ép chuỗi biến thành số nguyên
        gdTemp.thang = atoi(strtok(NULL,  ","));
        gdTemp.nam = atoi(strtok(NULL, ","));
        gdTemp.loai_gd = atoi(strtok(NULL, ","));
        strcpy(gdTemp.ma_ns,    strtok(NULL, ","));
        strcpy(gdTemp.danh_muc, strtok(NULL, ","));
        strcpy(gdTemp.ma_tk,    strtok(NULL, ","));
        strcpy(gdTemp.ghi_chu,  strtok(NULL, ","));
        if (gdTemp.loai_gd == 0){
            soLuongThu++;
            mangThu = (struct GiaoDich*) realloc(mangThu, soLuongThu * sizeof(struct GiaoDich));
            mangThu[soLuongThu - 1] =gdTemp;
        }
        else if (gdTemp.loai_gd == 1){
            soLuongChi++;
            mangChi = (struct GiaoDich*) realloc(mangChi, soLuongChi * sizeof(struct GiaoDich));
            mangChi[soLuongChi - 1] = gdTemp;
        }
    }
    fclose(file);

    if (soLuongThu > 1) quickSortGiaoDich (mangThu, 0, soLuongThu - 1);
    if (soLuongChi > 1) quickSortGiaoDich (mangChi, 0, soLuongChi - 1);
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
    for (int i = 0; i < soLuongThu; i++){
        fprintf(file, "%s,%d,%d,%d,%d,%d,%s,%s,%s,%s\n",
                mangThu[i].ma_gd, 
                mangThu[i].so_tien,
                mangThu[i].ngay, mangThu[i].thang, mangThu[i].nam,
                mangThu[i].loai_gd,
                mangThu[i].ma_ns,
                mangThu[i].danh_muc,
                mangThu[i].ma_tk,      
                mangThu[i].ghi_chu); 
    }

    // Ghi mảng chi
    for (int i = 0; i < soLuongChi; i++){
        fprintf(file, "%s,%d,%d,%d,%d,%d,%s,%s,%s,%s\n",
                mangChi[i].ma_gd, 
                mangChi[i].so_tien,
                mangChi[i].ngay, mangChi[i].thang, mangChi[i].nam,
                mangChi[i].loai_gd,
                mangChi[i].ma_ns,
                mangChi[i].danh_muc,
                mangChi[i].ma_tk,
                mangChi[i].ghi_chu);
    }     
           
    fclose(file);
    printf("Đã lưu %d giao dịch thu và %d giao dịch chi\n", soLuongThu, soLuongChi);
}

// Hàm giải phóng bộ nhớ
void giaiPhongGiaoDich() {
    free(mangThu);
    free(mangChi);
    mangThu = NULL;
    mangChi = NULL;
    soLuongThu = 0;
    soLuongChi = 0;
}