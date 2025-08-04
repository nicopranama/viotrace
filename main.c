#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pelanggaran {
  char jenis[51];
  int poin;
  char date[11];
  struct pelanggaran *next;
};

// struct node buat node di AVL
struct node {
  // ini data mahasiswa
  char nama[51];
  char NIM[11];
  int poinPelanggaran;
  struct pelanggaran *head;

  // ini properti AVL
  int height;
  struct node *left;
  struct node *right;
};

struct node *root = NULL;  // root untuk mahasiswa aktif
struct node *root2 = NULL; // root untuk mahasiswa tidak aktif
const int batasPoin = 100;

int abs(int x) {
  if (x < 0)
    return x * -1;

  return x;
}

void capitalizeName(char *name) {
  int len = strlen(name);
  int i = 0;
  int capitalizeNext = 1;

  while (i < len) {
    if (isspace(name[i])) {
      capitalizeNext = 1;
    } else {
      if (capitalizeNext && isalpha(name[i])) {
        name[i] = toupper(name[i]);
        capitalizeNext = 0;
      } else {
        name[i] = tolower(name[i]);
      }
    }
    i++;
  }
}

void capitalizeSentence(char *sentence) {
  int len = strlen(sentence);
  int i = 0;

  while (i < len && !isalpha(sentence[i])) {
    i++;
  }
  if (i < len && isalpha(sentence[i])) {
    sentence[i] = toupper(sentence[i]);
    i++;
  }

  for (; i < len; i++) {
    sentence[i] = tolower(sentence[i]);
  }
}

int validateNIM(const char *NIM) {
  int i;
  int digit = 0;

  for (i = 0; i < strlen(NIM); i++) {
    if (NIM[i] >= 48 && NIM[i] <= 57) {
      digit++;
    }
  }

  if (strlen(NIM) < 0 || strlen(NIM) > 10 || digit != 10)
    return 0;

  return 1;
}

int validateName(const char *name) {
  if (strlen(name) == 0) {
    return 0;
  }

  for (int i = 0; i < strlen(name); i++) {
    if (isdigit(name[i])) {
      return 0;
    }
  }

  if (strlen(name) < 2) {
    return 0;
  }

  return 1;
}

int validatePelanggaran(const char *jenis) {
  if (strlen(jenis) == 0) {
    return 0;
  }

  for (int i = 0; i < strlen(jenis); i++) {
    if (isdigit(jenis[i])) {
      return 0;
    }
  }

  if (strlen(jenis) < 2) {
    return 0;
  }

  return 1;
}

int validateDate(const char *date) {
  if (strlen(date) != 10)
    return 0;

  if (date[2] != '-' || date[5] != '-')
    return 0;

  for (int i = 0; i < 10; i++) {
    if (i == 2 || i == 5)
      continue;
    if (!isdigit(date[i]))
      return 0;
  }

  int day = atoi(date);
  int month = atoi(date + 3);
  int year = atoi(date + 6);

  if (day < 1 || day > 31)
    return 0;

  if (month < 1 || month > 12)
    return 0;

  if (month == 2) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
      if (day > 29)
        return 0;
    } else {
      if (day > 28)
        return 0;
    }
  }

  if (month == 4 || month == 6 || month == 9 || month == 11) {
    if (day > 30)
      return 0;
  }

  if (year < 2000 || year > 2024)
    return 0;

  return 1;
}

int height(struct node *n) {
  if (n == NULL)
    return 0;

  return n->height;
}

int max(int a, int b) {
  return (a > b) ? a : b; // membandingkan nilai terbesar
}

struct node *newNode(const char *NIM, const char *nama, int poinPelanggaran) {
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  strcpy(temp->NIM, NIM);
  strcpy(temp->nama, nama);
  temp->poinPelanggaran = poinPelanggaran;
  temp->head = NULL;

  temp->left = NULL;
  temp->right = NULL;
  temp->height = 1;

  return temp;
}

struct node *rightRotate(struct node *y) {
  struct node *x = y->left;
  struct node *t2 = x->right;

  x->right = y;
  y->left = t2;

  y->height = max(height(y->left), height(y->right) + 1);
  x->height = max(height(x->left), height(x->right) + 1);

  return x;
}

struct node *leftRotate(struct node *x) {
  struct node *y = x->right;
  struct node *t2 = y->left;

  y->left = x;
  x->right = t2;

  x->height = max(height(x->left), height(x->right) + 1);
  y->height = max(height(y->left), height(y->right) + 1);

  return y;
}

int getBalance(struct node *n) {
  if (n == NULL)
    return 0;

  return height(n->left) - height(n->right);
}

struct node *minVal(struct node *node) {
  struct node *curr = node;

  while (curr->left != NULL)
    curr = curr->left;

  return curr;
}

// untuk insert data, sementara key menggunakan NIM
struct node *insertNode(struct node *node, char *key, char *nama,
                        int poinPelanggaran) {

  if (node == NULL)
    return newNode(key, nama, poinPelanggaran);

  if (strcmp(key, node->NIM) < 0)
    node->left = insertNode(node->left, key, nama, poinPelanggaran);

  else if (strcmp(key, node->NIM) > 0)
    node->right = insertNode(node->right, key, nama, poinPelanggaran);

  else
    return node;

  node->height = max(height(node->left), height(node->right)) + 1;
  int balance = getBalance(node);

  if (balance > 1 && key < node->left->NIM)
    return rightRotate(node);

  if (balance < -1 && key > node->right->NIM)
    return leftRotate(node);

  if (balance > 1 && key > node->left->NIM) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && key < node->right->NIM) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

// fungsi searching berdasarkan NIM
struct node *searchNIM(struct node *root, const char *key) {
  if (root == NULL || !strcmp(root->NIM, key))
    return root;

  if (strcmp(key, root->NIM) > 0)
    return searchNIM(root->right, key);

  else
    return searchNIM(root->left, key);
}

// fungsi searching berdasarkan nama
struct node *searchName(struct node *root, const char *key) {
  if (root == NULL || !strcmp(root->nama, key))
    return root;

  if (strcmp(key, root->nama) > 0)
    return searchName(root->right, key);

  else
    return searchName(root->left, key);
}

// fungsi delete, bisa dipake buat hapus manual
// bisa buat hapus mahasiswa yang poin pelanggarannya lebih dari nilai tertentu
struct node *deleteNode(struct node *root, const char *key) {
  if (root == NULL)
    return root;

  if (strcmp(key, root->NIM) < 0)
    root->left = deleteNode(root->left, key);

  else if (strcmp(key, root->NIM) > 0)
    root->right = deleteNode(root->right, key);

  else {
    if (root->left == NULL || root->right == NULL) {
      struct node *temp = root->left ? root->left : root->right;

      if (temp == NULL) {
        temp = root;
        root = NULL;
      }

      else
        *root = *temp;

      free(temp);
    }

    else {
      struct node *temp = minVal(root->right);

      strcpy(root->NIM, temp->NIM);
      strcpy(root->nama, temp->nama);
      root->poinPelanggaran = temp->poinPelanggaran;

      root->right = deleteNode(root->right, temp->NIM);
    }
  }

  if (root == NULL)
    return root;

  root->height = 1 + max(height(root->left), height(root->right));
  int balance = getBalance(root);

  if (balance > 1 && getBalance(root->left) >= 0)
    return rightRotate(root);

  if (balance < -1 && getBalance(root->right) <= 0)
    return leftRotate(root);

  if (balance > 1 && getBalance(root->left) < 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  if (balance < -1 && getBalance(root->right) > 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

void insertMenu() {
  char key[51], nama[51];

  // Validate NIM
  do {
    getchar();
    printf("Masukkan NIM mahasiswa: ");
    scanf("%50[^\n]", key);
    if (!validateNIM(key)) {
      printf("NIM tidak valid! NIM harus berisi 10 digit.\n");
    } else if (searchNIM(root, key) != NULL || searchNIM(root2, key) != NULL) {
      printf("NIM sudah terdaftar!\n");
    }
  } while (!validateNIM(key) || searchNIM(root, key) != NULL ||
           searchNIM(root2, key) != NULL);

  nama[0] = '\0';

  do {
    getchar();
    printf("Masukkan nama mahasiswa: ");
    scanf("%50[^\n]*c", nama);
    if (!validateName(nama)) {
      printf("Nama tidak valid! Nama harus diisi, tidak boleh mengandung angka "
             "dan harus lebih           dari satu huruf.\n");
    }
  } while (!validateName(nama));

  capitalizeName(nama);

  root = insertNode(root, key, nama, 0);
  printf("Mahasiswa berhasil terdaftar!\n");
  return;
}

int ubahDatekeInt(const char *date) {
  char temp[9];
  sprintf(temp, "%c%c%c%c%c%c%c%c", date[6], date[7], date[8], date[9], date[3],
          date[4], date[0], date[1]);
  return atoi(temp);
}

int compareDates(const char *date1, const char *date2) {
  int date1Int = ubahDatekeInt(date1);
  int date2Int = ubahDatekeInt(date2);
  return date1Int - date2Int;
}

void sortPelanggaranDate(struct pelanggaran **head) {
  struct pelanggaran *sorted = NULL;
  struct pelanggaran *current = *head;

  while (current != NULL) {
    struct pelanggaran *next = current->next;
    if (sorted == NULL || compareDates(current->date, sorted->date) <= 0) {
      current->next = sorted;
      sorted = current;
    } else {
      struct pelanggaran *temp = sorted;
      while (temp->next != NULL &&
             compareDates(current->date, temp->next->date) > 0) {
        temp = temp->next;
      }
      current->next = temp->next;
      temp->next = current;
    }
    current = next;
  }

  *head = sorted;
}

void printPelanggaran(struct pelanggaran *head) {
  if (head == NULL) {
    printf("Tidak ada pelanggaran\n");
    return;
  }

  sortPelanggaranDate(&head);

  struct pelanggaran *temp = head;
  printf("Daftar Pelanggaran:\n");
  while (temp != NULL) {
    printf("- %-30s: %4d poin | %s\n", temp->jenis, temp->poin, temp->date);
    temp = temp->next;
  }
}

// search bisa otomatis mencari berdasarkan NIM atau nama
// dimasukkan ke menu search
void searchMenu() {
  char key[51];

  getchar();
  printf("Masukkan NIM atau nama mahasiswa: ");
  scanf("%50[^\n]", key);
  capitalizeName(key);

  struct node *temp = searchNIM(root, key);
  if (temp != NULL) {
    printf("NIM: %s\n", temp->NIM);
    printf("Nama: %s\n", temp->nama);
    printf("Poin pelanggaran: %d\n", temp->poinPelanggaran);
    printPelanggaran(temp->head);
    return;
  }

  temp = searchName(root, key);
  if (temp != NULL) {
    printf("NIM: %s\n", temp->NIM);
    printf("Nama: %s\n", temp->nama);
    printf("Poin pelanggaran: %d\n", temp->poinPelanggaran);
    printPelanggaran(temp->head);
    return;
  }

  printf("Mahasiswa tidak ditemukan atau tidak aktif!\n");
  return;
}

void deleteMenu() {
  char key[51];
  char option;

  getchar();
  printf("Masukkan nama atau NIM mahasiswa: ");
  scanf("%50[^\n]", key);
  capitalizeName(key);

  struct node *temp = searchNIM(root, key);
  if (temp != NULL) {
    printf("NIM: %s\n", temp->NIM);
    printf("Nama: %s\n", temp->nama);
    printf("Poin pelanggaran: %d\n", temp->poinPelanggaran);
    printPelanggaran(temp->head);

    do {
      getchar();
      printf("Hapus data? (y/n): ");
      scanf("%c", &option);
    } while (option != 'y' && option != 'n');

    if (option == 'y') {
      root2 = insertNode(root2, temp->NIM, temp->nama, temp->poinPelanggaran);
      root = deleteNode(root, temp->NIM);
      printf("Data berhasil dihapus!\n");
      return;
    }

    else {
      printf("Data tidak jadi dihapus!\n");
      return;
    }
  }

  temp = searchName(root, key);
  if (temp != NULL) {
    printf("NIM: %s\n", temp->NIM);
    printf("Nama: %s\n", temp->nama);
    printf("Poin pelanggaran: %d\n", temp->poinPelanggaran);
    printPelanggaran(temp->head);

    do {
      printf("Hapus data? (y/n): ");
      scanf("%c", &option);
    } while (option != 'y' && option != 'n');

    if (option == 'y') {
      root = deleteNode(root, temp->NIM);
      printf("Data berhasil dihapus!\n");
      return;
    }

    else {
      printf("Data tidak jadi dihapus!\n");
      return;
    }
  }

  printf("Mahasiswa tidak ditemukan!\n");
  return;
}

void tambahPelanggaran(struct node *mahasiswa, const char *jenis, int poin,
                       const char *date) {
  struct pelanggaran *pelanggaranBaru =
      (struct pelanggaran *)malloc(sizeof(struct pelanggaran));
  strcpy(pelanggaranBaru->jenis, jenis);
  pelanggaranBaru->poin = poin;
  strcpy(pelanggaranBaru->date, date);
  pelanggaranBaru->next = mahasiswa->head;
  mahasiswa->head = pelanggaranBaru;
  mahasiswa->poinPelanggaran += poin;

  if (mahasiswa->poinPelanggaran >= 100) {
    printf("Poin pelanggaran mahasiswa %s melebihi batas. Mahasiswa di-drop "
           "out.\n",
           mahasiswa->nama);
    root2 = insertNode(root2, mahasiswa->NIM, mahasiswa->nama,
                       mahasiswa->poinPelanggaran);
    root = deleteNode(root, mahasiswa->NIM);
  }
}

void hapusPelanggaran(struct node *mahasiswa, const char *jenis,
                      const char *date) {
  struct pelanggaran *temp = mahasiswa->head;
  struct pelanggaran *prev = NULL;

  while (temp != NULL) {
    if (strcmp(temp->jenis, jenis) == 0 && strcmp(temp->date, date) == 0) {
      if (prev == NULL) {
        mahasiswa->head = temp->next;
      } else
        prev->next = temp->next;
      mahasiswa->poinPelanggaran -= temp->poin;
      free(temp);
      printf("Pelanggaran berhasil dihapus\n");
      return;
    }
    prev = temp;
    temp = temp->next;
  }
  printf("Pelanggaran tidak ditemukan\n");
}

void updateMenu() {
  char key[51];
  char jenis[51];
  char date[11];
  int option;
  int poin;
  int batasPoin;

  printf("1. Tambah pelanggaran\n");
  printf("2. Hapus pelanggaran\n");
  printf("3. Kembali ke menu utama\n");
  printf(">> ");
  scanf(" %d", &option);

  switch (option) {
  case 1:
    getchar();
    printf("Masukkan NIM atau nama mahasiswa: ");
    scanf(" %50[^\n]", key);
    capitalizeName(key);
    struct node *temp = searchNIM(root, key);
    if (temp == NULL)
      temp = searchName(root, key);
    if (temp == NULL) {
      printf("Mahasiswa tidak ditemukan\n");
      return;
    }

    jenis[0] = '\0';

    do {
      getchar();
      printf("Masukkan jenis pelanggaran: ");
      scanf("%50[^\n]*c", jenis);
      if (!validatePelanggaran(jenis)) {
        printf("Pelanggaran tidak valid! Masukkan jenis pelanggaran dengan "
               "benar.\n");
      }
    } while (!validatePelanggaran(jenis));

    capitalizeSentence(jenis);

    do {
      getchar();
      printf("Tanggal pelanggaran (DD-MM-YYYY): ");
      scanf(" %10[^\n]", date);
      if (!validateDate(date)) {
        printf("Tanggal tidak valid! Pastikan formatnya adalah DD-MM-YYYY.\n");
      }
    } while (!validateDate(date));

    printf("Masukkan poin pelanggaran: ");
    scanf(" %d", &poin);

    tambahPelanggaran(temp, jenis, abs(poin), date);
    printf("Pelanggaran berhasil ditambahkan!\n");
    break;

  case 2:
    getchar();
    printf("Masukkan NIM atau nama mahasiswa: ");
    scanf(" %50[^\n]", key);
    capitalizeName(key);
    temp = searchNIM(root, key);
    if (temp == NULL)
      temp = searchName(root, key);
    if (temp == NULL) {
      printf("Mahasiswa tidak ditemukan\n");
      return;
    }

    getchar();
    printf("Masukkan jenis pelanggaran yang akan dihapus: ");
    scanf(" %50[^\n]", jenis);
    capitalizeSentence(jenis);

    do {
      getchar();
      printf("Tanggal pelanggaran (DD-MM-YYYY): ");
      scanf("%10[^\n]", date);
      if (!validateDate(date)) {
        printf("Tanggal tidak valid! Pastikan formatnya adalah DD-MM-YYYY.\n");
      }
    } while (!validateDate(date));

    hapusPelanggaran(temp, jenis, date);
    break;

  case 3:
    system("cls");
    return;

  default:
    printf("Menu tidak ditemukan!\n");
    break;
  }
}

// fungsi print data
void printData(struct node *root) {
  if (root != NULL) {
    printf("| %-50s | %-12s | %-20d | \n", root->nama, root->NIM,
           root->poinPelanggaran);
    printf("+------------------------------------------------------------------"
           "------------------------+\n");
    printData(root->left);
    printData(root->right);
  }
}

// menu print data
void printMenu(struct node *root) {
  if (root == NULL) {
    printf("Belum ada mahasiswa terdaftar!\n");
    return;
  }

  printf("+--------------------------------------------------------------------"
         "----------------------+\n");
  printf("| %-50s | %-12s | %-20s | \n", "Nama", "NIM", "Poin Pelanggaran");
  printf("+--------------------------------------------------------------------"
         "----------------------+\n");
  printData(root);
  printf("\n");
  return;
}

int main(void) {

  int option;
  do {
    printf("\n");
    printf("============================\n");
    printf("Sistem Manajemen Pelanggaran\n");
    printf("============================\n");
    printf("\n");
    printf("1. Tambah mahasiswa baru\n");
    printf("2. Hapus mahasiswa\n");
    printf("3. Cari mahasiswa\n");
    printf("4. Update pelanggaran mahasiswa\n");
    printf("5. Tampilkan data mahasiswa\n");
    printf("6. Keluar\n");
    printf(">> ");
    scanf(" %d", &option);

    switch (option) {
    case 1:
      insertMenu();
      system("pause");
      system("cls");
      break;

    case 2:
      deleteMenu();
      system("pause");
      system("cls");
      break;

    case 3:
      searchMenu();
      system("pause");
      system("cls");
      break;

    case 4:
      updateMenu();
      system("pause");
      system("cls");
      break;

    case 5:
      printf("1. Mahasiswa Aktif\n");
      printf("2. Mahasiswa Tidak Aktif\n");
      printf(">> ");
      int choice;
      scanf("%d", &choice);

      if (choice == 1) {
        printf("Mahasiswa Aktif\n");
        printMenu(root);
      }

      else if (choice == 2) {
        printf("Mahasiswa Tidak Aktif\n");
        printMenu(root2);
      }

      else
        printf("Menu tidak ditemukan\n");

      system("pause");
      system("cls");
      break;

    case 6:
      exit(1);
      break;

    default:
      printf("Menu tidak ditemukan!\n");
      system("pause");
      system("cls");
      break;
    }

  } while (option != 6);

  return 0;
}
