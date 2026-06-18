#ifndef DANH_MUC_H
#define DANH_MUC_H

#include "khai_bao.h"
// Biến toàn cục (extern để các file khác truy cập)
extern DanhMuc *mang_dm_thu;
extern DanhMuc *mang_dm_chi;
extern int so_luong_dm_thu;
extern int so_luong_dm_chi;

// Chỉ cho phép nhập số nguyên, từ chối chữ/ký tự đặc biệt
int nhapSoNguyen();
int tongHanMuc();
void inTenDanhMucChi();
void inTenDanhMucThu();
void inDanhSachChi();
void themDanhMucChi();
void chiaPhanTramChi();
void xoaDanhMucChi();  // Xóa theo ma_dm
void hoanDoiPhanTramChi();
void tinhTienDanhMucChi(NganSach ns);
void tinhTiLeChi(NganSach ns);
void capNhatHanMucTien(NganSach ns);
void truHanMucVaCanhBao(int ma_dm, int so_tien);

#endif
