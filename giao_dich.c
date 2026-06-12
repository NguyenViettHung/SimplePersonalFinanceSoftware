// phần giao dịch thu/chi gồm khai báo, đọc file và ghi đè
// Tìm kiếm giao dịch theo ngày, theo danh mục
// Tính toán số dư (luôn cập nhật)
// Tính toán tổng thu/chi theo ngày, tháng, năm, loại

// CÓ THỜI GIAN THÌ XEM LẠI CÁCH SẮP XẾP CODE

#include <stdio.h>
#include <stdlib.h> // Chứa hàm realloc, atoi
#include <string.h> // Chứa hàm strcpy, strtok, strcspn

#include "khai_bao.h" 
#include "giao_dich.h"

struct GiaoDich* mangThu = NULL;
struct GiaoDich* mangChi = NULL;
int soLuongThu = 0;
int soLuongChi = 0;

int g_so_du = 0; // biến toàn cục lưu số dư hiện tại, sẽ được cập nhật sau mỗi giao dịch


// Hàm hoán vị (sử dụng sau khi đã so sánh ngày => sắp xếp ngày theo đúng trật tự)
void hoanVi (struct GiaoDich* a, struct GiaoDich* b){
    struct GiaoDich temp = *a;
    *a = *b;
    *b = temp;
}


// 1. SẮP XẾP GIAO DỊCH
// Mảng số lượng coi như đã sắp xếp tốt (tăng dần theo ngày)
// Chỉ để chèn thêm giao dịch mới vào đúng vị trí

    // Hàm so sánh ngày (trả về số âm nếu ngày a xảy ra trước ngày b và ngược lại)
int soSanhNgay (struct GiaoDich a, struct GiaoDich b){
    if (a.nam != b.nam) return a.nam - b .nam;
    if (a.thang != b.thang) return a.thang - b.thang;
    return a.ngay - b.ngay;
    // Hàm này không nhận diện được số ngày trong thực tế, ví dụ nhập a là ngày 20/4/2025 và b là ngày 31/4/2026 thì vẫn chạy
    // => lỗ hổng nếu người dùng nhập sai
}// mục đích sử dụng cho quick sort

void insertionSortGiaoDich (struct GiaoDich* mang, int soLuong){
    struct GiaoDich gdMoi = mang[soLuong - 1]; 
    int i = soLuong - 2; // để so sánh phần tử từ kế cuối trở về
    while (i >= 0&& soSanhNgay(mang[i], gdMoi) > 0){
        mang[i + 1] = mang[i];
        i--;
    }
    mang[i + 1] = gdMoi; //chèn giao dịch mới vào dúng vị trí cần sắp xếp
}


// 2. TÌM KIẾM GIAO DỊCH

// Kiểm tra giao dịch có nằm trong khoảng ngày được chọn không
static int ngayTrongKhoang (struct GiaoDich *gd, int ngay_bd, int thang_bd, int nam_bd, int ngay_kt, int thang_kt, int nam_kt){
    //ĐỔI NGÀY, THÁNG, NĂM THÀNH SỐ NGUYÊN THEO Ý VIỆT ĐỂ SO SÁNH
    int ngayGD = gd -> nam * 10000 + gd -> thang *100 + gd -> ngay;
    int ngayBatDau = (nam_bd != -1)?(nam_bd * 10000 + thang_bd * 100 + ngay_bd):0; // nếu ngày bắt đầu không được nhập thì gán giá trị 0
    int ngayKetThuc = (nam_kt != -1)?(nam_kt * 10000 + thang_kt * 100 + ngay_kt):99999999; // nếu ngày kết thúc không được nhập thì gán giá trị lớn nhất có thể

    if (nam_bd == -1 && thang_bd == -1 && ngay_bd == -1){//không nhập ngày bắt đầu
        ngayBatDau = 0; 
    }
    if (nam_kt == -1 && thang_kt == -1 && ngay_kt == -1){ // không nhập ngày kết thúc
        ngayKetThuc = 99999999; // gán giá trị lớn nhất có thể
    }
    
    return (ngayGD >= ngayBatDau) && (ngayGD <= ngayKetThuc);
}
 
// Tìm kiến nhị phân vị trí bắt đầu trong mảng đã sắp xếp
static int timViTriBatDau (struct GiaoDich mang[], int soLuong, int ngayBatDau){
    int trai = 0, phai = soLuong - 1;
    while (trai < phai){
        int giua = trai + (phai - trai) / 2;
        int ngayG = mang[giua].nam * 10000 + mang[giua].thang * 100 + mang[giua].ngay;
        if (ngayG < ngayBatDau) trai = giua + 1;
        else phai = giua;
    } 
    return trai;
}

int timKiemGiaoDich (struct GiaoDich mang[], int soLuong, int ngay_bd, int thang_bd, int nam_bd, int ngay_kt, int thang_kt, int nam_kt, int ma_dm_loc, int mangKetQua[]){ // ma_dm_loc là mã danh mục cần lọc
    int soKetQua = 0;
    int batDau = 0;

    // Nếu có ngày bắt đầu
    if (ngay_bd != -1 || thang_bd != -1 || nam_bd != -1){
        int ngayBatDau = (nam_bd != -1 ? nam_bd : 0) * 10000 + (thang_bd != -1 ? thang_bd : 1) * 100 + (ngay_bd != -1 ? ngay_bd : 1);
        batDau = timViTriBatDau (mang, soLuong, ngayBatDau);
    }

    // Duyệt tuyến tính từ vị trí bắt đầu đến khi hết mảng hoặc vượt quá ngày kết thúc
    for (int i = batDau; i < soLuong; i++){
        if (ngay_kt != -1){
            int ngayGD = mang[i].nam * 10000 + mang[i].thang * 100 + mang[i].ngay;
            int ngayKetThuc = nam_kt * 10000 + (thang_kt != -1 ? thang_kt : 12) * 100 + (ngay_kt != -1 ? ngay_kt : 31);
            if (ngayGD > ngayKetThuc) break;
        }

    // Kiểm tra khoảng ngày đầy đủ
    if (!ngayTrongKhoang(&mang[i], ngay_bd, thang_bd, nam_bd, ngay_kt, thang_kt, nam_kt)) continue;


    // Kiểm tra danh mục
    if (ma_dm_loc != -1 && mang[i].ma_dm != ma_dm_loc) continue;

    // Nếu đủ điều kiện thì ghi nhận vị trí
    mangKetQua [soKetQua++] = i;
}
return soKetQua;
}

// In kết quả ra màn hình
void inKetQuaTimKiem (struct GiaoDich mang[], int mangKetQua[], int soKetQua){
    if (soKetQua == 0){
        printf("Không tìm thấy giao dịch nào phù hợp với tiêu chí đã chọn.\n");
        return;
    }
    printf("\n%-10s %-12s %-15s %-12s %s\n", "Ma GD", "Ngay", "So tien", "Danh muc", "Ghi chu");
    for (int i = 0; i < soKetQua; i++) {
        struct GiaoDich *gd = &mang[mangKetQua[i]];
        printf("%-10s %02d/%02d/%04d  %-15d %-12d %s\n", gd -> ma_gd, gd -> ngay, gd -> thang, gd -> nam, gd -> so_tien_gd, gd -> ma_dm, gd -> ghi_chu);
    }
    printf("Tong: %d giao dich\n\n", soKetQua);
}


// 3. SỐ DƯ

// Tính lại số dư sau mỗi giao dịch KHÔNG LÀM MẤT DỮ LIỆU KHI KHỞI ĐỘNG LẠI
void tinhLaiSoDu(int so_tien_ns_hien_tai){
    int so_du = so_tien_ns_hien_tai;

    for (int i = 0; i < soLuongThu; i++){
        so_du += mangThu[i].so_tien_gd;
    }

    for (int i = 0; i < soLuongChi; i++){
        so_du -= mangChi[i].so_tien_gd;
    }
    g_so_du = so_du;

    if (g_so_du < 0){
        printf("Số dư hiện tại âm: %d VND\n", g_so_du);
    }
}

// Cập nhật số dư sau khi thêm/xóa giao dịch
void capNhatSoDu (int delta){
    g_so_du+= delta;
    if (g_so_du < 0)
    printf ("Số dư hiện tại âm: %d VND\n", g_so_du);
}


// 4. TÍNH TOÁN TỔNG THU/CHI THEO NGÀY, THÁNG, NĂM, LOẠI DANH MỤC

// Tính tổng thu chi
void tinhTongThuChi (int loai_gd_loc, int ngay_loc, int thang_loc, int nam_loc, int ma_dm_loc, int *tongThu, int *tongChi){
    *tongThu = 0;
    *tongChi = 0;

    // Duyệt mangThu
    if (loai_gd_loc == 0 || loai_gd_loc == 2){
        for (int i = 0; i < soLuongThu; i++){
            if (ngay_loc != -1 && mangThu[i].ngay != ngay_loc) continue;
            if (thang_loc != -1 && mangThu[i].thang != thang_loc) continue;
            if (nam_loc != -1 && mangThu[i].nam != nam_loc) continue;
            if (ma_dm_loc != -1 && mangThu[i].ma_dm != ma_dm_loc) continue;
            *tongThu += mangThu[i].so_tien_gd;
        }
    }

    // Duyệt mangChi
    if (loai_gd_loc == 1 || loai_gd_loc == 2){
        for (int i = 0; i < soLuongChi; i++){
            if (ngay_loc != -1 && mangChi[i].ngay != ngay_loc) continue;
            if (thang_loc != -1 && mangChi[i].thang != thang_loc) continue;
            if (nam_loc != -1 && mangChi[i].nam != nam_loc) continue;
            if (ma_dm_loc != -1 && mangChi[i].ma_dm != ma_dm_loc) continue;
            *tongChi += mangChi[i].so_tien_gd;
        }
    }
}

    
// Hàm đọc file
//[ĐÁNH DẤU CHUYỂN ĐI -> file_io.c]
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
        gdTemp.so_tien_gd= atoi(strtok(NULL,  ","));
        gdTemp.ngay = atoi(strtok(NULL,  ",")); //atoi = ASCII to interger, ép chuỗi biến thành số nguyên
        gdTemp.thang = atoi(strtok(NULL,  ","));
        gdTemp.nam = atoi(strtok(NULL, ","));
        gdTemp.loai_gd = atoi(strtok(NULL, ","));
        gdTemp.ma_dm, atoi(strtok(NULL, ","));
        strcpy(gdTemp.ma_ns,    strtok(NULL, ","));
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

    if (soLuongThu > 1) insertionSortGiaoDich (mangThu, soLuongThu);
    if (soLuongChi > 1) insertionSortGiaoDich (mangChi, soLuongChi);
    printf("Đã đọc và sắp xếp file giao dịch thành công\n");
}

// Hàm ghi file
//[ĐÁNH DẤU CHUYỂN ĐI -> file_io.c]
void ghiFileGiaoDich(){
    FILE *file = fopen("giao_dich.txt", "w");
    if (file == NULL){
        printf ("Lỗi mở file ghi đè\n");
        return;
    }

    // Ghi mảng thu
    for (int i = 0; i < soLuongThu; i++){
        fprintf(file, "%s, %d, %d, %d, %d, %d, %s, %d, %s\n",
                mangThu[i].ma_gd, 
                mangThu[i].so_tien_gd,
                mangThu[i].ngay, mangThu[i].thang, mangThu[i].nam,
                mangThu[i].loai_gd,
                mangThu[i].ma_ns,
                mangThu[i].ma_dm, 
                mangThu[i].ghi_chu); 
    }

    // Ghi mảng chi
    for (int i = 0; i < soLuongChi; i++){
        fprintf(file, "%s, %d, %d, %d, %d, %d, %s, %d, %s\n",
                mangChi[i].ma_gd, 
                mangChi[i].so_tien_gd,
                mangChi[i].ngay, mangChi[i].thang, mangChi[i].nam,
                mangChi[i].loai_gd,
                mangChi[i].ma_ns,
                mangChi[i].ma_dm, 
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
