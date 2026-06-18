// phần giao dịch thu/chi gồm khai báo, đọc file và ghi đè
// Tìm kiếm giao dịch theo ngày, theo danh mục
// Tính toán số dư (luôn cập nhật)
// Tính toán tổng thu/chi theo ngày, tháng, năm, loại

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

#include "khai_bao.h" 
#include "giao_dich.h"
#include "danh_muc.h"

struct GiaoDich* mang_thu = NULL;
struct GiaoDich* mang_chi = NULL;
int so_luong_giao_dich_thu = 0;
int so_luong_giao_dich_chi = 0;
int suc_chua_thu = 0;
int suc_chua_chi = 0;
int g_so_du = 0;  

void xoaBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Hàm hoán vị
void hoanVi (struct GiaoDich* a, struct GiaoDich* b){
    struct GiaoDich temp = *a;
    *a = *b;
    *b = temp;
}


// 1. SẮP XẾP GIAO DỊCH
// Mảng số lượng coi như đã sắp xếp tốt (tăng dần theo ngày)
// Chỉ để chèn thêm giao dịch mới vào đúng vị trí

    // Hàm so sánh ngày
int soSanhNgay (struct GiaoDich a, struct GiaoDich b){
    if (a.nam != b.nam) return a.nam - b .nam;
    if (a.thang != b.thang) return a.thang - b.thang;
    return a.ngay - b.ngay;
}

void insertionSortGiaoDich (struct GiaoDich* mang, int so_luong){
    struct GiaoDich gd_moi = mang[so_luong - 1]; 
    int i = so_luong - 2;
    while (i >= 0&& soSanhNgay(mang[i], gd_moi) > 0){
        mang[i + 1] = mang[i];
        i--;
    }
    mang[i + 1] = gd_moi; 
}


// 2. TÌM KIẾM GIAO DỊCH

// Kiểm tra giao dịch có nằm trong khoảng ngày được chọn không
static int ngayTrongKhoang (struct GiaoDich *gd, int ngay_bd, int thang_bd, int nam_bd, int ngay_kt, int thang_kt, int nam_kt){
    int ngay_gd = gd -> nam * 10000 + gd -> thang *100 + gd -> ngay;
    int ngay_bat_dau = (nam_bd != -1)?(nam_bd * 10000 + thang_bd * 100 + ngay_bd):0; 
    int ngay_ket_thuc = (nam_kt != -1)?(nam_kt * 10000 + thang_kt * 100 + ngay_kt):99999999; 

    if (nam_bd == -1 && thang_bd == -1 && ngay_bd == -1){
        ngay_bat_dau = 0; 
    }
    if (nam_kt == -1 && thang_kt == -1 && ngay_kt == -1){
        ngay_ket_thuc = 99999999;
    }
    
    return (ngay_gd >= ngay_bat_dau) && (ngay_gd <= ngay_ket_thuc);
}
 
// Tìm kiến nhị phân vị trí bắt đầu trong mảng đã sắp xếp
static int timViTriBatDau (struct GiaoDich mang[], int so_luong, int ngay_bat_dau){
    int trai = 0, phai = so_luong - 1;
    while (trai < phai){
        int giua = trai + (phai - trai) / 2;
        int ngay_g = mang[giua].nam * 10000 + mang[giua].thang * 100 + mang[giua].ngay;
        if (ngay_g < ngay_bat_dau) trai = giua + 1;
        else phai = giua;
    } 
    return trai;
}

int timKiemGiaoDich (struct GiaoDich mang[], int so_luong, int ngay_bd, int thang_bd, int nam_bd, int ngay_kt, int thang_kt, int nam_kt, int ma_dm_loc, int mang_ket_qua[]){ // ma_dm_loc là mã danh mục cần lọc
    int so_ket_qua = 0;
    int bat_dau = 0;

    // Nếu có ngày bắt đầu
    if (ngay_bd != -1 || thang_bd != -1 || nam_bd != -1){
        int ngay_bat_dau = (nam_bd != -1 ? nam_bd : 0) * 10000 + (thang_bd != -1 ? thang_bd : 1) * 100 + (ngay_bd != -1 ? ngay_bd : 1);
        bat_dau = timViTriBatDau (mang, so_luong, ngay_bat_dau);
    }

    // Duyệt tuyến tính từ vị trí bắt đầu đến khi hết mảng hoặc vượt quá ngày kết thúc
    for (int i = bat_dau; i < so_luong; i++){
        if (ngay_kt != -1){
            int ngay_gd = mang[i].nam * 10000 + mang[i].thang * 100 + mang[i].ngay;
            int ngay_ket_thuc = nam_kt * 10000 + (thang_kt != -1 ? thang_kt : 12) * 100 + (ngay_kt != -1 ? ngay_kt : 31);
            if (ngay_gd > ngay_ket_thuc) break;
        }

    // Kiểm tra khoảng ngày đầy đủ
    if (!ngayTrongKhoang(&mang[i], ngay_bd, thang_bd, nam_bd, ngay_kt, thang_kt, nam_kt)) continue;

    // Kiểm tra danh mục
    if (ma_dm_loc != -1 && mang[i].ma_dm != ma_dm_loc) continue;

    // Nếu đủ điều kiện thì ghi nhận vị trí
    mang_ket_qua [so_ket_qua++] = i;
}
return so_ket_qua;
}

// In kết quả ra màn hình
void inKetQuaTimKiem (struct GiaoDich mang[], int mang_ket_qua[], int so_ket_qua){
    if (so_ket_qua == 0){
        printf("Khong tim thay giao dich phu hop voi tieu chi đa chon.\n");
        return;
    }
    printf("\n%-10s %-12s %-15s %-12s %s\n", "Ma giao dich", "Ngay", "So tien", "Danh muc", "Ghi chu");
    for (int i = 0; i < so_ket_qua; i++) {
        struct GiaoDich *gd = &mang[mang_ket_qua[i]];
        printf("%-10s %02d/%02d/%04d  %-15d %-12d %s\n", gd -> ma_gd, gd -> ngay, gd -> thang, gd -> nam, gd -> so_tien_gd, gd -> ma_dm, gd -> ghi_chu);
    }
    printf("Tong: %d giao dich\n\n", so_ket_qua);
}

// Hàm bọc cho chức năng tìm kiếm để nhúng vào Menu
void nhapVaTimKiemGiaoDich() {
    int ngay_bd, thang_bd, nam_bd;
    int ngay_kt, thang_kt, nam_kt;
    int ma_dm_loc;
    printf("Nhap ngay/thang/nam bat đau: ");
    scanf("%d%d%d", &ngay_bd, &thang_bd, &nam_bd);
    
    printf("Nhập ngày/tháng/năm kết thúc: ");
    scanf("%d%d%d", &ngay_kt, &thang_kt, &nam_kt);
    
    printf("Nhap ma danh muc can loc (Nhap -1 đe bo qua): ");
    scanf("%d", &ma_dm_loc);
    xoaBuffer();

    // Chuẩn bị mảng chứa kết quả và gọi hàm lõi
    int mang_ket_qua[1000];
    int so_ket_qua = timKiemGiaoDich(
        mang_thu, so_luong_giao_dich_thu, 
        ngay_bd, thang_bd, nam_bd, 
        ngay_kt, thang_kt, nam_kt, 
        ma_dm_loc, mang_ket_qua
    );
    
    inKetQuaTimKiem(mang_thu, mang_ket_qua, so_ket_qua);
}

// 3. SỐ DƯ

// Tính lại số dư sau mỗi giao dịch
void tinhLaiSoDu(int so_tien_ns_hien_tai){
    int so_du = so_tien_ns_hien_tai;

    for (int i = 0; i < so_luong_giao_dich_thu; i++){
        so_du += mang_thu[i].so_tien_gd;
    }

    for (int i = 0; i < so_luong_giao_dich_chi; i++){
        so_du -= mang_chi[i].so_tien_gd;
    }
    g_so_du = so_du;

    if (g_so_du < 0){
        printf("So du hien tai am: %d VND\n", g_so_du);
    }
}

// Cập nhật số dư sau khi thêm/xóa giao dịch
void capNhatSoDu (int delta){
    g_so_du+= delta;
    if (g_so_du < 0)
    printf ("So du hien tai am: %d VND\n", g_so_du);
}


// 4. TÍNH TOÁN TỔNG THU/CHI THEO NGÀY, THÁNG, NĂM, LOẠI DANH MỤC

// Tính tổng thu chi
void tinhTongThuChi (int loai_gd_loc, int ngay_loc, int thang_loc, int nam_loc, int ma_dm_loc, int *tong_thu, int *tong_chi){
    *tong_thu = 0;
    *tong_chi = 0;

    // Duyệt mang_thu
    if (loai_gd_loc == 0 || loai_gd_loc == 2){
        for (int i = 0; i < so_luong_giao_dich_thu; i++){
            if (ngay_loc != -1 && mang_thu[i].ngay != ngay_loc) continue;
            if (thang_loc != -1 && mang_thu[i].thang != thang_loc) continue;
            if (nam_loc != -1 && mang_thu[i].nam != nam_loc) continue;
            if (ma_dm_loc != -1 && mang_thu[i].ma_dm != ma_dm_loc) continue;
            *tong_thu += mang_thu[i].so_tien_gd;
        }
    }

    // Duyệt mang_chi
    if (loai_gd_loc == 1 || loai_gd_loc == 2){
        for (int i = 0; i < so_luong_giao_dich_chi; i++){
            if (ngay_loc != -1 && mang_chi[i].ngay != ngay_loc) continue;
            if (thang_loc != -1 && mang_chi[i].thang != thang_loc) continue;
            if (nam_loc != -1 && mang_chi[i].nam != nam_loc) continue;
            if (ma_dm_loc != -1 && mang_chi[i].ma_dm != ma_dm_loc) continue;
            *tong_chi += mang_chi[i].so_tien_gd;
        }
    }
}

// Hàm giải phóng bộ nhớ
void giaiPhongGiaoDich() {
    free(mang_thu);
    free(mang_chi);
    mang_thu = NULL;
    mang_chi = NULL;
    so_luong_giao_dich_thu = 0;
    so_luong_giao_dich_chi = 0;
    suc_chua_thu = 0;
    suc_chua_chi = 0;
}
// Hàm bọc chức năng 4
void nhapVaTinhTongThuChi() {
    int loai_gd_loc, ngay_loc, thang_loc, nam_loc, ma_dm_loc;
    int tong_thu = 0, tong_chi = 0;
    
    // 1. Nhập loại giao dịch cần thống kê
    printf("Chon loai giao dich (0: Thu, 1: Chi, 2: Tat ca): ");
    while (scanf("%d", &loai_gd_loc) != 1 || (loai_gd_loc < 0 || loai_gd_loc > 2)) {
        printf("Loi: Vui long nhap 0, 1 hoac 2: ");
        xoaBuffer();
    }
    xoaBuffer();

    // 2. Nhập các tiêu chí lọc thời gian và danh mục
    printf("Nhap ngay (Nhap -1 đe bo qua): ");
    scanf("%d", &ngay_loc);
    
    printf("Nhap thang (Nhap -1 đe bo qua): ");
    scanf("%d", &thang_loc);
    
    printf("Nhap nam (Nhap -1 đe bo qua): ");
    scanf("%d", &nam_loc);
    
    printf("Nhap ma danh muc (Nhap -1 đe bo qua): ");
    scanf("%d", &ma_dm_loc);
    xoaBuffer();

    tinhTongThuChi(loai_gd_loc, ngay_loc, thang_loc, nam_loc, ma_dm_loc, &tong_thu, &tong_chi);
	
    if (loai_gd_loc == 0 || loai_gd_loc == 2) {
        printf("Tong tien thu: %d VND\n", tong_thu);
    }
    if (loai_gd_loc == 1 || loai_gd_loc == 2) {
        printf("Tong tien chi: %d VND\n", tong_chi);
    }
    
    // Nếu chọn xem cả 2, in thêm dòng chênh lệch (Số dư trong khoảng thời gian đó)
    if (loai_gd_loc == 2) {
        printf("Chenh lech (thu - chi): %d VND\n", tong_thu - tong_chi);
}
}

// ---------------------------------------------
// Ham sinh ma_gd tu tang (1, 2, 3...)
// Duyet 2 mang, lay ID lon nhat roi +1
// ---------------------------------------------
static int sinhMaGD() {
    int max = 0;
    for (int i = 0; i < so_luong_giao_dich_thu; i++) {
        int id = atoi(mang_thu[i].ma_gd);
        if (id > max) max = id;
    }
    for (int i = 0; i < so_luong_giao_dich_chi; i++) {
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
    
    // -- 4. NH?P M� DANH M?C --
    inTenDanhMuc();
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
    int len = strlen(gd.ghi_chu);
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
        so_luong = &so_luong_giao_dich_thu;
        suc_chua = &suc_chua_thu;
    } else {
        mang     = &mang_chi;
        so_luong = &so_luong_giao_dich_chi;
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
        truHanMucVaCanhBao(ma_dm, so_tien_gd);
    }
 
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
    printf("Nhap thoi gian cua giao dich muon xoa");
    printf("\nNgay giao dich (1-31): ");
    while (scanf("%d", &ngay) != 1 || ngay < 1 || ngay > 31) {
        printf("Loi: Ngay phai trong khoang 1-31. Nhap lai: ");
        xoaBuffer();
    }
    xoaBuffer();
    
    printf("Thang giao dich (1-12): ");
    while (scanf("%d", &thang) != 1 || thang < 1 || thang > 12) {
        printf("Loi: Thang phai trong khoang 1-12. Nhap lai: ");
        xoaBuffer();
    }
    xoaBuffer();
    
    printf("Nam giao dich (>= 1900): ");
    while (scanf("%d", &nam) != 1 || nam < 1900) {
        printf("Loi: Nam phai >= 1900. Nhap lai: ");
        xoaBuffer();
    }
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
    for (int i = 0; i < so_luong_giao_dich_thu; i++) {
        int ngay_gd = mang_thu[i].nam * 10000 + mang_thu[i].thang * 100 + mang_thu[i].ngay;
        if (ngay_gd == ngay_tim) {
            ds[dem].loai    = 0;
            ds[dem].vi_tri  = i;
            dem++;
        }
        if (ngay_gd > ngay_tim) break; // mang da sap xep -> khoi som
    }
 
    // Duyet mang_chi
    for (int i = 0; i < so_luong_giao_dich_chi; i++) {
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
    int             *so_luong  = (loai_xoa == 0) ? &so_luong_giao_dich_thu : &so_luong_giao_dich_chi;
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
