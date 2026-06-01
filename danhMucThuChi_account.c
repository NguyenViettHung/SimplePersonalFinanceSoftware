#include <stdio.h>
#include <string.h>
#define MaxAcc 10
#define MaxDanhMuc 10
typedef struct {
    char idDanhMuc[10];
    char tenDanhMuc[35];
    //thu = 0, chi = 1;
    int thuChi;
} danhMuc;

typedef struct {
    char id[7];
    char name[35];
    long long soDu;
    danhMuc dsDanhMuc[MaxDanhMuc];
    int soDanhMuc;
} Account;

// Các biến toàn cục
Account Accs[MaxAcc];
int accountCount = 0;

// Hàm thêm tài khoản (ID được tự động thêm)
void addAccount(const char* name, long long soDuBanDau) {
    if (accountCount >= MaxAcc) {
        printf("Canh bao: Khong the them. Da dat gioi han %d tai khoan\n\n", MaxAcc);
        return;
    }
    
    char AutoId[7];
    sprintf(AutoId, "TK%02d", accountCount + 1);
    
    // Lưu dữ liệu vào mảng struct
    strcpy(Accs[accountCount].id, AutoId);
    strcpy(Accs[accountCount].name, name);
    Accs[accountCount].soDu = soDuBanDau;
    
    accountCount++;
    printf(" Da them thanh cong! Tai khoan [%s] duoc cap ID la: %s\n\n", name, AutoId);
}

void hienThiTK() {
    printf("\n================ DANH SACH TAI KHOAN (%d/%d) ================\n", accountCount, MaxAcc);
    if (accountCount == 0) {
        printf("Chua co tai khoan nao trong he thong.\n");
    } else {
        // Hiển thị tài khoản
        for (int i = 0; i < accountCount; i++) {
            printf("ID: %s | Ten tai khoan: %s | So du: %lld\n", 
                   Accs[i].id, 
                   Accs[i].name, 
                   Accs[i].soDu);
        }
    }
}

void saveData() {
    FILE *file = fopen("accounts.txt", "w");
    if (file == NULL) {
        printf("Loi: Khong the luu file!\n");
        return;
    }
    fprintf(file, "%d\n", accountCount);
    
    // Ghi từng tài khoản
    for (int i = 0; i < accountCount; i++) {
        fprintf(file, "%s|%s|%lld|%d\n", 
                Accs[i].id, 
                Accs[i].name, 
                Accs[i].soDu,
                Accs[i].soDanhMuc);
         // Vòng lặp con: Ghi các Danh mục thuộc tài khoản này
        for (int j = 0; j < Accs[i].soDanhMuc; j++) {
            fprintf(file, "%s|%s|%d\n", 
                    Accs[i].dsDanhMuc[j].idDanhMuc, 
                    Accs[i].dsDanhMuc[j].tenDanhMuc, 
                    Accs[i].dsDanhMuc[j].thuChi);
            }
    }

    fclose(file);
    printf("=> Da luu du lieu an toan vao file accounts.txt!\n");
}

// Đọc dữ liệu từ file text
void loadData() {
    FILE *file = fopen("accounts.txt", "r");
    if (file == NULL) {
        return; 
    }
    
    // Đọc số lượng tài khoản
    fscanf(file, "%d\n", &accountCount);
    
    // Đọc từng tài khoản
    for (int i = 0; i < accountCount; i++) {
        fscanf(file, "%[^|]|%[^|]|%lld|%d\n", 
               Accs[i].id, 
               Accs[i].name, 
               &Accs[i].soDu,
               &Accs[i].soDanhMuc);
        // Vòng lặp con: Đọc các Danh mục của tài khoản này
        for (int j = 0; j < Accs[i].soDanhMuc; j++) {
            fscanf(file, "%[^|]|%[^|]|%d\n", 
                   Accs[i].dsDanhMuc[j].idDanhMuc, 
                   Accs[i].dsDanhMuc[j].tenDanhMuc, 
                   &Accs[i].dsDanhMuc[j].thuChi);
            }
    }
    
    fclose(file);
    printf("=> Da tai %d tai khoan tu file text thanh cong\n\n", accountCount);
}

// Hàm xóa tài khoản dựa vào ID
void xoaTaiKhoan() {
    if (accountCount == 0) {
        printf("\nDanh sach tai khoan hien dang trong.\n\n");
        return;
    }

    char idCanXoa[7];
    printf("Nhap ID tai khoan can xoa (VD: TK01): ");
    scanf(" %s", idCanXoa);

    int viTriXoa = -1; // Biến lưu vị trí tìm thấy (mặc định -1 là chưa thấy)

    // Tìm vị trí có ID trùng khớp
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(Accs[i].id, idCanXoa) == 0) {
            viTriXoa = i;
            break;
        }
    }

    // Xử lý kết quả tìm kiếm
    if (viTriXoa == -1) {
        printf("Loi: Khong tim thay tai khoan mang ID '%s'!\n\n", idCanXoa);
    } else {
        // Dịch chuyển các phần tử phía sau lên trước 1 ô để ghi đè
        for (int i = viTriXoa; i < accountCount - 1; i++) {
            Accs[i] = Accs[i + 1]; 
        }
        
        accountCount--; // Giảm tổng số lượng tài khoản đi 1
        
        printf("=> Da xoa tai khoan '%s' thanh cong\n\n", idCanXoa);
        
        // Gọi hàm lưu lại file để cập nhật
        saveData(); 
    }
}

void themDanhMucChoTaiKhoan() {
    if (accountCount == 0) {
        printf("\nChua co tai khoan nao! Vui long tao tai khoan truoc.\n");
        return;
    }

    char idAcc[7];
    printf("\nNhap ID Tai khoan ban muon them danh muc (VD: TK01): ");
    scanf(" %s", idAcc);

    // Tìm tài khoản
    int viTri = -1;
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(Accs[i].id, idAcc) == 0) {
            viTri = i;
            break;
        }
    }

    if (viTri == -1) {
        printf("Loi: Khong tim thay tai khoan %s\n", idAcc);
        return;
    }
    
    // Kiểm tra sức chứa danh mục
    int dmCount = Accs[viTri].soDanhMuc;
    if (dmCount >= MaxDanhMuc) {
        printf("Tai khoan nay da day danh muc!\n");
        return;
    }

    char tempTenDM[35];
    int tempThuChi;
    int choice;

    // Menu Chọn mẫu / Tự nhập
    printf("\n--- THEM DANH MUC CHO [%s] ---\n", Accs[viTri].name);
    printf("1. Chon tu danh muc MAU (An uong, Luong...)\n");
    printf("2. TU NHAP danh muc moi\n");
    printf("Chon (1-2): ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 2) {
        printf("Loi nhap lieu!\n");
        while (getchar() != '\n');
        return;
    }

    if (choice == 1) {
        // CHỌN MẪU
        int mau;
        printf("\n1. An uong (Chi)\n2. Tien dien nuoc (Chi)\n3. Tien luong (Thu)\n");
        printf("Chon mau (1-3): ");
        scanf("%d", &mau);
        
        if (mau == 1) { strcpy(tempTenDM, "An uong"); tempThuChi = 1; }
        else if (mau == 2) { strcpy(tempTenDM, "Tien dien nuoc"); tempThuChi = 1; }
        else { strcpy(tempTenDM, "Tien luong"); tempThuChi = 0; }
        
    } else {
        // TỰ NHẬP
        printf("Nhap Ten danh muc moi: ");
        scanf(" %[^\n]", tempTenDM);
        printf("Loai danh muc (0 = Thu, 1 = Chi): ");
        scanf("%d", &tempThuChi);
    }

    // Tự động sinh ID danh mục (VD: DM01, DM02)
    sprintf(Accs[viTri].dsDanhMuc[dmCount].idDanhMuc, "DM%02d", dmCount + 1);
    strcpy(Accs[viTri].dsDanhMuc[dmCount].tenDanhMuc, tempTenDM);
    Accs[viTri].dsDanhMuc[dmCount].thuChi = tempThuChi;
    
    Accs[viTri].soDanhMuc++; // Tăng biến đếm danh mục lên 1
    
    printf("=> Da them danh muc [%s] vao tai khoan [%s] thanh cong!\n", tempTenDM, Accs[viTri].name);
    
    saveData(); // Lưu lại vào file ngay lập tức
}

void hienThiDanhMucCuaTaiKhoan() {
    if (accountCount == 0) {
        printf("\nHe thong chua co tai khoan nao! Vui long tao tai khoan truoc.\n");
        return;
    }

    char idAcc[7];
    printf("\n--- XEM DANH MUC CUA TAI KHOAN ---\n");
    printf("Nhap ID Tai khoan can xem (VD: TK01): ");
    scanf(" %s", idAcc);

    // Bước 1: Tìm vị trí của tài khoản trong mảng Accs
    int viTri = -1;
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(Accs[i].id, idAcc) == 0) {
            viTri = i;
            break;
        }
    }

    // Nếu không tìm thấy
    if (viTri == -1) {
        printf("Loi: Khong tim thay tai khoan mang ID '%s'!\n", idAcc);
        return;
    }

    // Kiểm tra xem tài khoản này đã có danh mục nào chưa
    if (Accs[viTri].soDanhMuc == 0) {
        printf("\nTai khoan [%s] hien chua co danh muc nao.\n", Accs[viTri].name);
        return;
    }

    // In ra danh sách danh mục của riêng tài khoản đó
    printf("\n================ DANH MUC CUA [%s] ================\n", Accs[viTri].name);
    printf("%-15s | %-20s | %-20s\n", "ID Danh Muc", "Ten Danh Muc", "Phan Loai");
    
    for (int i = 0; i < Accs[viTri].soDanhMuc; i++) {
        // Kiểm tra biến thuChi: 0 in ra "THU", 1 in ra "CHI"
        char loai[10];
        if (Accs[viTri].dsDanhMuc[i].thuChi == 0) {
            strcpy(loai, "THU (+)");
        } else {
            strcpy(loai, "CHI (-)");
        }
        
        printf("%-15s | %-20s | %-20s\n", 
               Accs[viTri].dsDanhMuc[i].idDanhMuc, 
               Accs[viTri].dsDanhMuc[i].tenDanhMuc, 
               loai);
    }
}
int main() {
    char tempName[50];
    long long tempsoDu;
    int choice;

    loadData();

    // Vòng lặp menu
    do {
        printf("1. Xem danh sach tai khoan\n");
        printf("2. Them tai khoan moi\n");
        printf("3: Xoa tai khoan hien co\n");
        printf("4. Them danh muc vao mot tai khoan hien co\n");
        printf("5. Hien thi danh muc\n");
        printf("0. Thoat\n");
        if(scanf("%d", &choice)!=1 || choice < 0 || choice > 5){
            printf("Lua chon khong hop le\n");
            while (getchar() != '\n');
            choice=-1;
            continue;
        };

        switch(choice) {
            case 1:
                hienThiTK();
                break;
            case 2:
                if (accountCount >= MaxAcc) {
                    printf("\nKho tai khoan da day! Khong the them.\n\n");
                    break;
                }
                
                printf("\n--- NHAP THONG TIN TAI KHOAN MOI ---\n");
                printf("Nhap Ten tai khoan: ");
                scanf(" %[^\n]", tempName);
                
                printf("Nhap So du ban dau: ");
                scanf("%lld", &tempsoDu);
                printf("\n");
                addAccount(tempName, tempsoDu); 
                saveData();
                break;
            case 3:
                xoaTaiKhoan();
                break;
            case 4:
                themDanhMucChoTaiKhoan();
                break;
            case 5:
                hienThiDanhMucCuaTaiKhoan();
                break;
            case 0:
                printf("Dang thoat chuong trinh...\n");
                break;
        }
    } while (choice != 0);

}