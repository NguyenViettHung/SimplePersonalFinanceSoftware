#ifndef GIAO_DICH_H
#define GIAO_DICH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "khai_bao.h"

int soSanhNgay (struct GiaoDich a, struct GiaoDich b);
void hoanVi (struct GiaoDich* a, struct GiaoDich* b);
void insertionSortGiaoDich (struct GiaoDich* mang, int soLuong);
int timKiemGiaoDich (struct GiaoDich mang[], int soLuong, int ngay_bd, int thang_bd, int nam_bd, int ngay_kt, int thang_kt, int nam_kt, int ma_dm_loc, int mangKetQua[]);
void inKetQuaTimKiem (struct GiaoDich mang[], int mangKetQua[], int soKetQua);
void tinhLaiSoDu(int so_tien_ns_hien_tai);
void capNhatSoDu (int delta);
void tinhTongThuChi (int loai_gd_loc, int ngay_loc, int thang_loc, int nam_loc, int ma_dm_loc, int *tongThu, int *tongChi);
void docFileGiaoDich();
void ghiFileGiaoDich();
void giaiPhongGiaoDich();

#endif
