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
int suc_chua_thu = 0;
int suc_chua_chi = 0;
int g_so_du = 0; // biến toàn cục lưu số dư hiện tại, sẽ được cập nhật sau mỗi giao dịch

void xoaBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

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
                mangThu[i].ghi_chu,
                mangThu[i].ma_ns,
                mangThu[i].ma_dm); 
    }

    // Ghi mảng chi
    for (int i = 0; i < soLuongChi; i++){
        fprintf(file, "%s, %d, %d, %d, %d, %d, %s, %d, %s\n",
                mangChi[i].ma_gd, 
                mangChi[i].so_tien_gd,
                mangChi[i].ngay, mangChi[i].thang, mangChi[i].nam,
                mangChi[i].loai_gd,
                mangThu[i].ghi_chu,
                mangChi[i].ma_ns,
                mangChi[i].ma_dm);
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
    suc_chua_thu = 0;
    suc_chua_chi = 0;
}

// ---------------------------------------------
// Ham sinh ma_gd tu tang (1, 2, 3...)
// Duyet 2 mang, lay ID lon nhat roi +1
// ---------------------------------------------
static int sinhMaGD() {
    int max = 0;
    for (int i = 0; i < so_luong_thu; i++) {
        int id = atoi(mang_thu[i].ma_gd);
        if (id > max) max = id;
    }
    for (int i = 0; i < so_luong_chi; i++) {
        int id = atoi(mang_chi[i].ma_gd);
        if (id > max) max = id;
    }
    return max + 1;
}
 
// ---------------------------------------------
// Ham them giao dich
// input : mang (qua bien global), cac truong du lieu
// output: khong (cap nhat truc tiep bien global)
//
// Logic:
//   1. Kiem tra input
//   2. Gan vao bien tam gd
//   3. TH mang day -> realloc them BUOC_CAP_PHAT phan tu
//   4. Tim vi tri chen tu cuoi mang (so sanh ngay)
//   5. Dich cac phan tu phia sau ra sau 1 o
//   6. Gan gd vao vi tri do
//   7. Cap nhat so du va han muc
// ---------------------------------------------
void nhapVaThemGiaoDich() {
    struct GiaoDich gd = nhapGiaoDich();
	themGiaoDich(gd.so_tien_gd,
                 gd.ngay, gd.thang, gd.nam,
                 gd.loai_gd,
                 gd.ghi_chu,
                 gd.ma_dm);
}

struct GiaoDich nhapGiaoDich() {
    struct GiaoDich gd;
    memset(&gd, 0, sizeof(struct GiaoDich));
    
    // -- 1. NH?P LO?I GIAO D?CH --
    printf("\n========== NHAP GIAO DICH ==========\n");
    printf("Loai giao dich:\n");
    printf("  0 - Thu (nhap tien)\n");
    printf("  1 - Chi (chi tien)\n");
    printf("Chon (0/1): ");
    
    int loai_temp;
    while (scanf("%d", &loai_temp) != 1 || (loai_temp != 0 && loai_temp != 1)) {
        printf("Loi: Vui long chon 0 hoac 1: ");
        xoaBuffer();
    }
    gd.loai_gd = loai_temp;
    xoaBuffer();
    
    // -- 2. NH?P S? TI?N --
    printf("\nSo tien giao dich (VND): ");
    while (scanf("%d", &gd.so_tien_gd) != 1 || gd.so_tien_gd <= 0) {
        printf("Loi: So tien phai lon hon 0. Nhap lai: ");
        xoaBuffer();
    }
    xoaBuffer();
    
    // -- 3. NH?P NG�Y TH�NG NAM --
    printf("\nNgay giao dich (1-31): ");
    while (scanf("%d", &gd.ngay) != 1 || gd.ngay < 1 || gd.ngay > 31) {
        printf("Loi: Ngay phai trong khoang 1-31. Nhap lai: ");
        xoaBuffer();
    }
    xoaBuffer();
    
    printf("Thang giao dich (1-12): ");
    while (scanf("%d", &gd.thang) != 1 || gd.thang < 1 || gd.thang > 12) {
        printf("Loi: Thang phai trong khoang 1-12. Nhap lai: ");
        xoaBuffer();
    }
    xoaBuffer();
    
    printf("Nam giao dich (>= 1900): ");
    while (scanf("%d", &gd.nam) != 1 || gd.nam < 1900) {
        printf("Loi: Nam phai >= 1900. Nhap lai: ");
        xoaBuffer();
    }
    xoaBuffer();
    
    // X�a newline ? cu?i
    int len = strlen(gd.ma_ns);
    if (len > 0 && gd.ma_ns[len - 1] == '\n') {
        gd.ma_ns[len - 1] = '\0';
    }
    
  
    
    // -- 4. NH?P M� DANH M?C --
    printf("Ma danh muc (> 0): ");
    while (scanf("%d", &gd.ma_dm) != 1 || gd.ma_dm <= 0) {
        printf("Loi: Ma danh muc phai > 0. Nhap lai: ");
        xoaBuffer();
    }
    xoaBuffer();
    
    // -- 5. NH?P GHI CH� (T�Y CH?N) --
    printf("Ghi chu (tuy chon, Enter de bo qua): ");
    fgets(gd.ghi_chu, sizeof(gd.ghi_chu), stdin);
    
    // X�a newline ? cu?i
    len = strlen(gd.ghi_chu);
    if (len > 0 && gd.ghi_chu[len - 1] == '\n') {
        gd.ghi_chu[len - 1] = '\0';
    }
    
    return gd;
}

void themGiaoDich(int so_tien_gd,
                  int ngay, int thang, int nam,
                  int loai_gd,
                  const char *ghi_chu,
                  int ma_dm)
{
    // -- 1. Kiem tra input ---------------------
     // -- 1. Kiem tra input ---------------------
    if (so_tien_gd <= 0) {
        printf("Loi: so tien phai lon hon 0.\n");
        return;
    }
    if (ngay < 1 || ngay > 31 || thang < 1 || thang > 12 || nam < 1900) {
        printf("Loi: ngay thang nam khong hop le.\n");
        return;
    }
    if (loai_gd != 0 && loai_gd != 1) {
        printf("Loi: loai giao dich khong hop le (0-thu, 1-chi).\n");
        return;
    }
    if (ma_dm <= 0) {
        printf("Loi: ma danh muc khong hop le.\n");
        return;
    }
 
    // -- 2. Gan vao bien tam -------------------
    struct GiaoDich gd;
    int id = sinhMaGD();
    snprintf(gd.ma_gd, sizeof(gd.ma_gd), "%d", id);
    gd.so_tien_gd = so_tien_gd;
    gd.ngay       = ngay;
    gd.thang      = thang;
    gd.nam        = nam;
    gd.loai_gd    = loai_gd;
    gd.ma_dm      = ma_dm;
    strncpy(gd.ghi_chu, ghi_chu ? ghi_chu : "", sizeof(gd.ghi_chu) - 1);
    gd.ghi_chu[sizeof(gd.ghi_chu) - 1] = '\0';
    snprintf(gd.ma_ns, 15, "NS%04d%02d", nam, thang);
 
    // -- 3. Xac dinh mang dich (dung con tro de tranh lap code) --
    struct GiaoDich **mang;
    int             *so_luong;
    int             *suc_chua;
 
    if (loai_gd == 0) {
        mang     = &mang_thu;
        so_luong = &so_luong_thu;
        suc_chua = &suc_chua_thu;
    } else {
        mang     = &mang_chi;
        so_luong = &so_luong_chi;
        suc_chua = &suc_chua_chi;
    }
 
    // -- 4. Mang day -> realloc them BUOC_CAP_PHAT o ----------
    if (*so_luong == *suc_chua) {
        int suc_chua_moi = *suc_chua + BUOC_CAP_PHAT;
        struct GiaoDich *tmp = (struct GiaoDich *)realloc(
            *mang, suc_chua_moi * sizeof(struct GiaoDich));
        if (tmp == NULL) {
            printf("Loi: khong the cap phat bo nho.\n");
            return;
        }
        *mang     = tmp;
        *suc_chua = suc_chua_moi;
    }
 
    // -- 5. Tim vi tri chen (duyet nguoc tu cuoi) --------------
    int vi_tri = *so_luong;
    for (int i = *so_luong - 1; i >= 0; i--) {
        if (soSanhNgay((*mang)[i], gd) <= 0) {
            vi_tri = i + 1;
            break;
        }
        vi_tri = i;
    }
 
    // -- 6. Dich cac phan tu [vi_tri..so_luong-1] ra sau 1 o --
    for (int i = *so_luong; i > vi_tri; i--) {
        (*mang)[i] = (*mang)[i - 1];
    }
 
    // -- 7. Chen gd vao vi tri tim duoc --------------------
    (*mang)[vi_tri] = gd;
    (*so_luong)++;
 
    // -- 8. Cap nhat so du va han muc -------------------------
    if (loai_gd == 0) {
        g_so_du += so_tien_gd;
    } else {
        g_so_du -= so_tien_gd;
    }//dang thieu cap nhat han muc
 
    printf("Them giao dich thanh cong! (Ma GD: %s, Vi tri: %d)\n",
           gd.ma_gd, vi_tri);
}

// =====================================================
// XOA GIAO DICH
// Input : ngay, thang, nam cua giao dich can xoa
// Flow  : 1. Tim & in tat ca GD trong ngay do
//         2. Nguoi dung chon ma GD muon xoa
//         3. Xoa khoi mang (dich trai), cap nhat so du
// =====================================================

void nhapVaXoaGiaoDich() {
    int ngay, thang, nam;
	inDanhSachGiaoDich();
	scanf("%d%d%d",&ngay,&thang,&nam);
	xoaBuffer();
	xoaGiaoDich(ngay,thang,nam);
}

void xoaGiaoDich(int ngay, int thang, int nam) {
 
    // -- 1. Xay dung ngay so sanh --
    int ngay_tim = nam * 10000 + thang * 100 + ngay;
 
    // -- 2. Thu thap tat ca GD trong ngay tu ca 2 mang --
    //    Luu chi so goc (loai, vi_tri) de xoa chinh xac
    typedef struct { int loai; int vi_tri; } vi_tri_gd;
    vi_tri_gd ds[1000];
    int dem = 0;
 
    // Duyet mang_thu
    for (int i = 0; i < so_luong_thu; i++) {
        int ngay_gd = mang_thu[i].nam * 10000 + mang_thu[i].thang * 100 + mang_thu[i].ngay;
        if (ngay_gd == ngay_tim) {
            ds[dem].loai    = 0;
            ds[dem].vi_tri  = i;
            dem++;
        }
        if (ngay_gd > ngay_tim) break; // mang da sap xep -> khoi som
    }
 
    // Duyet mang_chi
    for (int i = 0; i < so_luong_chi; i++) {
        int ngay_gd = mang_chi[i].nam * 10000 + mang_chi[i].thang * 100 + mang_chi[i].ngay;
        if (ngay_gd == ngay_tim) {
            ds[dem].loai    = 1;
            ds[dem].vi_tri  = i;
            dem++;
        }
        if (ngay_gd > ngay_tim) break;
    }
 
    // -- 3. Kiem tra co GD nao trong ngay khong --
    if (dem == 0) {
        printf("Khong co giao dich nao vao ngay %02d/%02d/%04d.\n", ngay, thang, nam);
        return;
    }
 
    // -- 4. In danh sach GD trong ngay --
    printf("\n=== GIAO DICH NGAY %02d/%02d/%04d ===\n", ngay, thang, nam);
    printf("%-6s %-8s %-10s %-5s %-15s %s\n",
           "Ma GD", "Loai", "So tien", "DM", "Ma NS", "Ghi chu");
    printf("----------------------------------------------------------\n");
 
    for (int i = 0; i < dem; i++) {
        struct GiaoDich *gd = (ds[i].loai == 0)
                              ? &mang_thu[ds[i].vi_tri]
                              : &mang_chi[ds[i].vi_tri];
        printf("%-6s %-8s %-10d %-5d %-15s %s\n",
               gd->ma_gd,
               (ds[i].loai == 0) ? "THU" : "CHI",
               gd->so_tien_gd,
               gd->ma_dm,
               gd->ma_ns,
               gd->ghi_chu);
    }
    printf("----------------------------------------------------------\n");
 
    // -- 5. Nguoi dung chon ma GD muon xoa --
    char ma_xoa[15];
    printf("Nhap ma giao dich muon xoa (hoac 0 de huy): ");
    if (fgets(ma_xoa, sizeof(ma_xoa), stdin) == NULL) return;
    ma_xoa[strcspn(ma_xoa, "\n")] = '\0';
 
    if (strcmp(ma_xoa, "0") == 0) {
        printf("Da huy xoa.\n");
        return;
    }
 
    // -- 6. Tim ma GD trong danh sach da loc --
    int loai_xoa = -1;
    int vi_tri_xoa = -1;
    for (int i = 0; i < dem; i++) {
        struct GiaoDich *gd = (ds[i].loai == 0)
                              ? &mang_thu[ds[i].vi_tri]
                              : &mang_chi[ds[i].vi_tri];
        if (strcmp(gd->ma_gd, ma_xoa) == 0) {
            loai_xoa    = ds[i].loai;
            vi_tri_xoa  = ds[i].vi_tri;
            break;
        }
    }
 
    if (vi_tri_xoa == -1) {
        printf("Khong tim thay ma giao dich \'%s\' trong ngay nay.\n", ma_xoa);
        return;
    }
 
    // -- 7. Lay thong tin GD truoc khi xoa (de cap nhat so du) --
    struct GiaoDich *mang_dich = (loai_xoa == 0) ? mang_thu : mang_chi;
    int             *so_luong  = (loai_xoa == 0) ? &so_luong_thu : &so_luong_chi;
    int so_tien_xoa = mang_dich[vi_tri_xoa].so_tien_gd;
 
    printf("Xac nhan xoa GD %s - %d VND - %s? (y/n): ",
           mang_dich[vi_tri_xoa].ma_gd,
           mang_dich[vi_tri_xoa].so_tien_gd,
           mang_dich[vi_tri_xoa].ghi_chu);
 
    char xac_nhan[4];
    if (fgets(xac_nhan, sizeof(xac_nhan), stdin) == NULL) return;
    if (xac_nhan[0] != 'y' && xac_nhan[0] != 'Y') {
        printf("Da huy xoa.\n");
        return;
    }
 
    // -- 8. Dich trai de xoa phan tu tai vi_tri_xoa --
    for (int i = vi_tri_xoa; i < *so_luong - 1; i++) {
        mang_dich[i] = mang_dich[i + 1];
    }
    (*so_luong)--;
 
    // -- 9. Cap nhat so du --
    if (loai_xoa == 0) {
        g_so_du -= so_tien_xoa; // xoa GD thu -> so du giam
    } else {
        g_so_du += so_tien_xoa; // xoa GD chi -> so du tang
    }
 
    printf("Da xoa giao dich %s thanh cong. So du hien tai: %d VND\n",
           ma_xoa, g_so_du);
}