#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "giao_dich.h"
#include "khai_bao.h"
#include "file_io.h"
#include "menu.h"
#include "ngan_sach.h"
#include "bao_cao.h"
#include "danh_muc.h"
#include "menu.h"

extern struct GiaoDich* mang_thu;
extern struct GiaoDich* mang_chi;
extern struct NganSach* mang_ngan_sach;
extern int so_luong_giao_dich_thu;
extern int so_luong_giao_dich_chi;
extern int so_luong_ngan_sach;

void giaiPhongBoNho();

int main() {
    napDuLieu();
	dieuHuong();
	luuDuLieu();

	return 0;
}

void giaiPhongBoNho() {
	giaiPhongNganSach();
	giaiPhongGiaoDich();
}