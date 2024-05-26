#include <iostream>
#include <string>
#include <vector>
#include <iomanip> // for setting table format
#include <algorithm> // for sorting
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

#define max 40

struct BarangAntik {
    string nama;
    int tahun;
    float harga;
};

struct Transaksi {
    string username; // Menyimpan nama pengguna yang melakukan transaksi
    string nama;
    int tahun;
    float harga;
    string status; // Status "Dibeli" atau "Dihapus"
};

struct User {
    string username;
    string password;
    bool isAdmin;
};

BarangAntik *daftarBarang = new BarangAntik[max];
Transaksi *daftarTransaksi = new Transaksi[max * 2]; // Buffer transaksi lebih besar
vector<User> users;
string loop;
int pos = 0;
int posTransaksi = 0;
int loggedInUserIndex = -1;

void registerUser();
void userMenu();
void adminMenu();
void loginUser();
void selectionSortByName();
int binarySearchByPrice(float targetPrice);

int getMenuChoice() {
    int choice;
    system("cls");
    cout << "=====================" << endl;
    cout << "  TOKO BARANG ANTIK  " << endl;
    cout << "=====================" << endl;
    cout << "1. Login\n2. Register\n3. Keluar" << endl;
    cout << "Pilihan : ";
    cin >> choice;

    while (cin.fail() || choice < 1 || choice > 3) {
        cout << "Pilihan tidak valid. Masukkan angka dari 1 hingga 3: ";
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> choice;
    }

    return choice;
}

void displayBarang(BarangAntik *daftarBarang, int pos) {
    system("cls");
    if (pos != 0) {
        cout << left << setw(5) << "No" << setw(25) << "Nama Barang" << setw(10) << "Tahun" << setw(15) << "Harga" << endl;
        cout << "=========================================================" << endl;
        for (int a = 0; a < pos; a++) {
            cout << left << setw(5) << a + 1 << setw(25) << daftarBarang[a].nama << setw(10) << daftarBarang[a].tahun << setw(15) << daftarBarang[a].harga << endl;
        }
    } else {
        cout << "=== Data Kosong ===" << endl;
    }
}


void displayTransaksi(Transaksi *daftarTransaksi, int posTransaksi) {
    system("cls");
    if (posTransaksi != 0) {
        cout << left << setw(5) << "No" << setw(15) << "Nama Pengguna" << setw(25) << "Nama Barang" << setw(10) << "Tahun" << setw(15) << "Harga" << setw(10) << "Status" << endl;
        cout << "========================================================================================" << endl;
        for (int a = 0; a < posTransaksi; a++) {
            cout << left << setw(5) << a + 1 << setw(15) << daftarTransaksi[a].username << setw(25) << daftarTransaksi[a].nama << setw(10) << daftarTransaksi[a].tahun << setw(15) << daftarTransaksi[a].harga << setw(10) << daftarTransaksi[a].status << endl;
        }
    } else {
        cout << "=== Tidak Ada Transaksi ===" << endl;
    }
}


void viewUserTransaksi(Transaksi *daftarTransaksi, int posTransaksi, string currentUser) {
    system("cls");
    bool hasTransaksi = false;
    for (int a = 0; a < posTransaksi; a++) {
        if (daftarTransaksi[a].username == currentUser) {
            if (!hasTransaksi) {
                cout << left << setw(5) << "No" << setw(25) << "Nama Barang" << setw(10) << "Tahun" << setw(15) << "Harga" << setw(10) << "Status" << endl;
                cout << "======================================================================" << endl;
                hasTransaksi = true;
            }
            cout << left << setw(5) << a + 1 << setw(25) << daftarTransaksi[a].nama << setw(10) << daftarTransaksi[a].tahun << setw(15) << daftarTransaksi[a].harga << setw(10) << daftarTransaksi[a].status << endl;
        }
    }
    if (!hasTransaksi) {
        cout << "=== Tidak Ada Transaksi ===" << endl;
    }
    cout << "kembali (y) : ";
    cin >> loop;
}

void add(BarangAntik *daftarBarang, int &pos) {
    do {
        displayBarang(daftarBarang, pos);
        if (pos < max) {
            cin.ignore();
            cout << "Tambah Data : " << endl;
            cout << "Nama Barang: ";
            getline(cin, daftarBarang[pos].nama);

            while (true) {
                cout << "Tahun: ";
                cin >> daftarBarang[pos].tahun;
                if (cin.fail() || daftarBarang[pos].tahun <= 0) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Tahun tidak valid. Masukkan angka yang benar." << endl;
                } else break;
            }

            while (true) {
                cout << "Harga: ";
                cin >> daftarBarang[pos].harga;
                if (cin.fail() || daftarBarang[pos].harga <= 0) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Harga tidak valid. Masukkan angka yang benar." << endl;
                } else break;
            }

            pos++;
            displayBarang(daftarBarang, pos);
            cout << "ulangi (y/t) : ";
            cin >> loop;
        } else {
            cout << "=== Memori Penuh ===" << endl;
            cout << "isi t untuk kembali : ";
            cin >> loop;
        }
    } while (loop == "y");
}

void edit(BarangAntik *daftarBarang, int pos) {
    int y;
    do {
        displayBarang(daftarBarang, pos);
        cout << "ubah data ke : ";
        cin >> y;

        while (cin.fail() || y < 1 || y > pos) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Nomor data tidak valid. Masukkan nomor yang ada: ";
            cin >> y;
        }

        cin.ignore();
        cout << "ubah menjadi : " << endl;
        cout << "Nama Barang: ";
        getline(cin, daftarBarang[y - 1].nama);

        while (true) {
            cout << "Tahun: ";
            cin >> daftarBarang[y - 1].tahun;
            if (cin.fail() || daftarBarang[y - 1].tahun <= 0) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Tahun tidak valid. Masukkan angka yang benar." << endl;
            } else break;
        }

        while (true) {
            cout << "Harga: ";
            cin >> daftarBarang[y - 1].harga;
            if (cin.fail() || daftarBarang[y - 1].harga <= 0) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Harga tidak valid. Masukkan angka yang benar." << endl;
            } else break;
        }

        displayBarang(daftarBarang, pos);
        cout << "ubah lagi (y/t) : ";
        cin >> loop;
    } while (loop == "y");
}

void recursiveDelete(BarangAntik *daftarBarang, int pos, int x) {
    if (x < pos) {
        daftarBarang[x - 1] = daftarBarang[x];
        recursiveDelete(daftarBarang, pos, x + 1);
    }
}

void del(BarangAntik *daftarBarang, int &pos) {
    int x;
    do {
        displayBarang(daftarBarang, pos);
        if (pos > 0) {
            cout << "hapus data ke : ";
            cin >> x;

            while (cin.fail() || x < 1 || x > pos) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Nomor data tidak valid. Masukkan nomor yang ada: ";
                cin >> x;
            }

            recursiveDelete(daftarBarang, pos, x);
            pos--;
            displayBarang(daftarBarang, pos);
            cout << "hapus lagi (y/t) : ";
            cin >> loop;
        } else {
            cout << "Data kosong. isikan t untuk kembali : ";
            cin >> loop;
        }
    } while (loop == "y");
}

void adminMenu() {
    int pil;
    do {
        system("cls");
        cout << "=====================" << endl;
        cout << "  ADMIN MENU  " << endl;
        cout << "=====================" << endl;
        cout << "1. Tambah\n2. Ubah\n3. Hapus\n4. Tampilkan Barang\n5. Tampilkan Transaksi\n6. Logout" << endl;
        cout << "Pilihan : ";
        cin >> pil;

        while (cin.fail() || pil < 1 || pil > 6) {
            cout << "Pilihan tidak valid. Masukkan angka dari 1 hingga 6: ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> pil;
        }

        switch (pil) {
            case 1:
                add(daftarBarang, pos);
                break;
            case 2:
                edit(daftarBarang, pos);
                break;
            case 3:
                del(daftarBarang, pos);
                break;
            case 4:
                displayBarang(daftarBarang, pos);
                cout << "kembali (y) : ";
                cin >> loop;
                break;
            case 5:
                displayTransaksi(daftarTransaksi, posTransaksi);
                cout << "kembali (y) : ";
                cin >> loop;
                break;
            case 6:
                cout << "Logout" << endl;
                break;
        }
    } while (pil != 6);
}

void viewBarang() {
    displayBarang(daftarBarang, pos);
    cout << "kembali (y) : ";
    cin >> loop;
}

void buyBarang() {
    int pilihan;
    displayBarang(daftarBarang, pos);
    if (pos > 0) {
        cout << "Pilih nomor barang yang ingin dibeli: ";
        cin >> pilihan;

        while (cin.fail() || pilihan < 1 || pilihan > pos) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Nomor barang tidak valid. Masukkan nomor yang ada: ";
            cin >> pilihan;
        }

        // Catat transaksi
        daftarTransaksi[posTransaksi] = {users[loggedInUserIndex].username, daftarBarang[pilihan - 1].nama, daftarBarang[pilihan - 1].tahun, daftarBarang[pilihan - 1].harga, "Dibeli"};
        posTransaksi++;

        // Hapus barang dari daftarBarang setelah dibeli
        recursiveDelete(daftarBarang, pos, pilihan);
        pos--;
    } else {
        cout << "Tidak ada barang yang tersedia untuk dibeli." << endl;
    }
    cout << "kembali (y) : ";
    cin >> loop;
}

void selectionSortByName() {
    for (int i = 0; i < pos - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < pos; j++) {
            if (daftarBarang[j].nama < daftarBarang[minIndex].nama) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(daftarBarang[i], daftarBarang[minIndex]);
        }
    }
    cout << "Data barang telah diurutkan berdasarkan nama (ascending)." << endl;
}

int binarySearchByPrice(float targetPrice) {
    int left = 0, right = pos - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (daftarBarang[mid].harga == targetPrice) {
            return mid; // Found
        }
        if (daftarBarang[mid].harga < targetPrice) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; // Not found
}

void userMenu() {
    int pil;
    do {
        system("cls");
        cout << "=====================" << endl;
        cout << "  USER MENU  " << endl;
        cout << "=====================" << endl;
        cout << "1. Lihat Barang\n2. Beli Barang\n3. Urutkan Barang berdasarkan Nama\n4. Cari Barang berdasarkan Harga\n5. Tampilkan Transaksi\n6. Logout" << endl;
        cout << "Pilihan : ";
        cin >> pil;

        while (cin.fail() || pil < 1 || pil > 6) {
            cout << "Pilihan tidak valid. Masukkan angka dari 1 hingga 6: ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> pil;
        }

        switch (pil) {
            case 1:
                viewBarang();
                break;
            case 2:
                buyBarang();
                break;
            case 3:
                selectionSortByName();
                displayBarang(daftarBarang, pos);
                cout << "kembali (y) : ";
                cin >> loop;
                break;
            case 4:
                float targetPrice;
                cout << "Masukkan harga yang dicari: ";
                cin >> targetPrice;
                while (cin.fail() || targetPrice <= 0) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Harga tidak valid. Masukkan angka yang benar: ";
                    cin >> targetPrice;
                }
                int resultIndex;
                selectionSortByName(); // Urutkan barang berdasarkan nama sebelum mencari
                resultIndex = binarySearchByPrice(targetPrice);
                system("cls");
                if (resultIndex != -1) {
                    cout << "Barang ditemukan:" << endl;
                    cout << "Nama: " << daftarBarang[resultIndex].nama << endl;
                    cout << "Tahun: " << daftarBarang[resultIndex].tahun << endl;
                    cout << "Harga: " << daftarBarang[resultIndex].harga << endl;
                } else {
                    cout << "Tidak ada barang yang ditemukan dengan harga tersebut." << endl;
                }
                cout << "kembali (y) : ";
                cin >> loop;
                break;
            case 5:
                viewUserTransaksi(daftarTransaksi, posTransaksi, users[loggedInUserIndex].username);
                break;
            case 6:
                cout << "Logout" << endl;
                break;
        }
    } while (pil != 6);
}

#ifdef _WIN32
string getPassword() {
    string password;
    char ch;
    ch = _getch();
    while (ch != 13) { // character 13 is enter
        if (ch == 8 && !password.empty()) { // handle backspace
            password.pop_back();
            cout << "\b \b";
        } else if (ch != 8) {
            password.push_back(ch);
            cout << '*';
        }
        ch = _getch();
    }
    cout << endl;
    return password;
}
#else
string getPassword() {
    string password;
    char ch;
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (ch == 127 && !password.empty()) { // handle backspace
            password.pop_back();
            cout << "\b \b";
        } else if (ch != 127) {
            password.push_back(ch);
            cout << '*';
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << endl;
    return password;
}
#endif

void registerUser() {
    User newUser;
    cout << "Masukkan username: ";
    cin >> newUser.username;
    for (const User &user : users) {
        if (user.username == newUser.username) {
            cout << "Username sudah ada. Silakan coba lagi." << endl;
            return;
        }
    }
    cout << "Masukkan password: ";
    newUser.password = getPassword();
    newUser.isAdmin = false;
    users.push_back(newUser);
    cout << "Registrasi berhasil! Silakan login." << endl;
}

void loginUser() {
    string username, password;
    cout << "username: ";
    cin >> username;
    cout << "password: ";
    password = getPassword();

    for (int i = 0; i < users.size(); i++) {
        if (users[i].username == username && users[i].password == password) {
            loggedInUserIndex = i;
            if (users[i].isAdmin) {
                cout << "Login berhasil sebagai Admin" << endl;
                adminMenu();
            } else {
                cout << "Login berhasil sebagai User" << endl;
                userMenu();
            }
            return;
        }
    }
    cout << "Username atau password salah. Silakan coba lagi." << endl;
}

int main() {
    // Default Admin
    User admin = {"Admin", "Admin", true};
    users.push_back(admin);

    daftarBarang[pos++] = {"Wayang Kulit", 1850, 500000};
    daftarBarang[pos++] = {"Topeng Kayu", 1900, 950000};
    daftarBarang[pos++] = {"Keris", 1727, 985000};
    daftarBarang[pos++] = {"Patung Buddha Perunggu", 1378, 880000};
    daftarBarang[pos++] = {"Guci Tembikar", 1653, 900000};
    daftarBarang[pos++] = {"Naskah Kuno", 1853, 930000};
    daftarBarang[pos++] = {"Uang Kuno", 1932, 500000};
    daftarBarang[pos++] = {"Perhiasan Emas", 1940, 970000};
    daftarBarang[pos++] = {"Batik Kuno", 1878, 900000};
    daftarBarang[pos++] = {"Gamelan", 1810, 5000000};
    daftarBarang[pos++] = {"Kursi Antik", 1768, 5000000};
    daftarBarang[pos++] = {"Meja Antik", 1688, 790000};
    daftarBarang[pos++] = {"Lemari Antik", 1857, 940000};
    daftarBarang[pos++] = {"Cermin Antik", 1790, 900000};
    daftarBarang[pos++] = {"Lampu Antik", 1870, 750000};
    daftarBarang[pos++] = {"Jam Antik", 1742, 960000};
    daftarBarang[pos++] = {"Boneka Antik", 1880, 500000};
    daftarBarang[pos++] = {"Mainan Antik", 1805, 700000};
    daftarBarang[pos++] = {"Buku Antik", 1560, 900000};
    daftarBarang[pos++] = {"Peta Antik", 1650, 500000};
    daftarBarang[pos++] = {"Topeng Antik", 1970, 2000000};
    daftarBarang[pos++] = {"Naskah Daun Lontar Antik", 1770, 993000};
    daftarBarang[pos++] = {"Perhiasan Perak Antik", 1808, 940000};
    daftarBarang[pos++] = {"Sarung Pedang Antik", 1600, 750000};
    daftarBarang[pos++] = {"Jam Saku Antik", 1748, 680000};
    daftarBarang[pos++] = {"Mesin Penjahit Antik", 1864, 972000};
    daftarBarang[pos++] = {"Tombak antik", 1793, 5000000};
    daftarBarang[pos++] = {"Angklung antik", 1876, 989000};
    daftarBarang[pos++] = {"Kain Tenun antik", 1847, 830000};
    daftarBarang[pos++] = {"Kaca Venesia Antik", 1803, 885000};
    daftarBarang[pos++] = {"Piring Keramik Antik", 1500, 760000};
    daftarBarang[pos++] = {"Pistol Antik", 1836, 994000};
    daftarBarang[pos++] = {"Uang Kertas Antik", 1358, 847000};
    daftarBarang[pos++] = {"Patung Totem Antik", 1864, 863000};

    int choice;
    do {
        choice = getMenuChoice();
        switch (choice) {
            case 1:
                loginUser();
                break;
            case 2:
                registerUser();
                break;
            case 3:
                cout << "Keluar" << endl;
                break;
        }
    } while (choice != 3);

    cout << "Program telah selesai" << endl;
    return 0;
}
