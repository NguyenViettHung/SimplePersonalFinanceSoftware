#ifndef GIAO_DICH_H
#define GIAO_DICH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "khai_bao.h"

#define SUC_CHUA_BAN_DAU  10
#define BUOC_CAP_PHAT      5

void xoaBuffer(void);
int soSanhNgay (struct GiaoDich a, struct GiaoDich b);
void hoanVi (struct GiaoDich* a, struct GiaoDich* b);
void insertionSortGiaoDich (struct GiaoDich* mang, int so_luong);
int timKiemGiaoDich (struct GiaoDich mang[], int so_luong, int ngay_bd, int thang_bd, int nam_bd, int ngay_kt, int thang_kt, int nam_kt, int ma_dm_loc, int mang_ket_qua[]);
void inKetQuaTimKiem (struct GiaoDich mang[], int mang_ket_qua[], int so_ket_qua);
void tinhLaiSoDu(int so_tien_ns_hien_tai);
void capNhatSoDu (int delta);
void tinhtongThuChi (int loai_gd_loc, int ngay_loc, int thang_loc, int nam_loc, int ma_dm_loc, int *tong_thu, int *tong_chi);
struct GiaoDich nhapGiaoDich();
void themGiaoDich (int so_tien_gd, int ngay, int thang, int nam, int loai_gd, const char *ghi_chu, int ma_dm);
void nhapVaThemGiaoDich();
void xoaGiaoDich(int ngay, int thang, int nam);
void nhapVaXoaGiaoDich();
void docFileGiaoDich();
void ghiFileGiaoDich();
void giaiPhongGiaoDich();

#endif
