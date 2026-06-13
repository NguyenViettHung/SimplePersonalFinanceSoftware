#ifndef DANH_MUC_H
#define DANH_MUC_H

#include "khai_bao.h"

// ---- Biến toàn cục (extern để các file khác truy cập) ----
extern danhMuc *mangDMThu;
extern danhMuc *mangDMChi;
extern int countDMThu;
extern int countDMChi;

// Chỉ cho phép nhập số nguyên, từ chối chữ/ký tự đặc biệt
int nhapSoNguyen();
int tongHanMuc();
void inDanhSachChi();
void themDanhMucChi();
void chiaPhanTramChi();
void xoaDanhMucChi();  // Xóa theo ma_dm
void hoanDoiPhanTramChi();
void tinhTienDanhMucChi(NganSach ns);

#endif
