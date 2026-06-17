// Cấu trúc dữ liệu dùng chung

#ifndef KHAI_BAO_H
#define KHAI_BAO_H

// 1. Cấu trúc dữ liệu cho danh mục
typedef struct DanhMuc {
    int ma_dm;         // mã danh mục
    char ten_dm[50];   // tên danh mục
    int han_muc;       // hạn mức theo phần trăm
    int han_muc_tien;
    int han_muc_tien_goc;
} DanhMuc;

// 2. Cấu trúc dữ liệu cho ngân sách
typedef struct NganSach {
    char ma_ns[15];    // mã ngân sách: "NS" + năm + tháng VD: NS202605
    int so_tien_ns;    // số tiền tương ứng với ngân sách
    int thang;         // tháng ứng với ngân sách
    int nam;           // năm ứng với ngân sách
} NganSach;

// 3. Cấu trúc dữ liệu cho giao dịch
typedef struct GiaoDich {
    char ma_gd[15];    // mã giao dịch: số nguyên tăng dần
    int so_tien_gd;    // số tiền tương ứng với giao dịch
    int ngay;          // ngày giao dịch
    int thang;         // tháng giao dịch
    int nam;           // năm giao dịch
    int loai_gd;       // loại giao dịch: 0 - thu, 1 - chi
    char ghi_chu[100]; // ghi chú
    char ma_ns[15];    // mã ngân sách (FK)
    int ma_dm;         // mã danh mục (FK) 
} GiaoDich;

#endif /* KHAI_BAO_H */
