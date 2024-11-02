#include <iostream>
using namespace std;
int main() {
    setlocale(LC_ALL, "RU");
    char c = '+';
    while (c == '+') {
    int n, m;
    char p = 48;
    cout << "�� ���� ������� ���� ��� ���� � C��� �������� n*m ��������, ��� ������ '.' �������� ������ �����, � ������ '*' - �����. ��������� ��������� ��� ���� �������, ��� � ������������ ����\n";
    cout << "����� � ������ ���� �������������: ";
    while (true) {
        if (!(cin >> m)) { // ���� ����������� �����
            cin.clear(); //������� ���� ������ ��� cin (����� ������� �������� �����-������ �������� ���������)
            while (cin.get() != '\n'); // ������ �� ������ ���� ������
            cout << "�������� ���� \n";
            continue;
        }
        if (cin.peek() != '\n') { //����������� �� ������� ����� � �������� ���, ����� ����� ��������� ������
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������� ������� �� ������
            cout << "�������� ���� \n";
            continue;
        }
        if (m < 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�������� ���� \n";
            continue;
        }
        break;
    }
    while (true) {
        if (!(cin >> n)) { // ���� ����������� �����
            cin.clear(); //������� ���� ������ ��� cin (����� ������� �������� �����-������ �������� ���������)
            while (cin.get() != '\n'); // ������ �� ������ ���� ������
            cout << "�������� ���� \n";
            continue;
        }
        if (cin.peek() != '\n') { //����������� �� ������� ����� � �������� ���, ����� ����� ��������� ������
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������� ������� �� ������
            cout << "�������� ���� \n";
            continue;
        }
        if (n < 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�������� ���� \n";
            continue;
        }
        break;
    }
    char** a = NULL;
    a = (char**)malloc((n + 2) * sizeof(char*));
    for (int i = 0; i < n + 2; i++) {
        a[i] = (char*)malloc((m + 2) * sizeof(char));
    }
    for (int q = 0; q < n + 2; q++) {
        for (int i = 0; i < m + 2;) {
            if (q == 0 || q == n + 1 || i == 0 || i == m + 1) {
                a[q][i] = '.';
                i++;
                continue;
            }
            cout << i << " ������ ���� " << q << " c������: ";
            cin >> a[q][i];
            if (a[q][i] == '*' || a[q][i] == '.') { i++; }
        }
    }
    for (int q = 1; q < n + 1; q++) {
        for (int i = 1; i < m + 1; i++) {
            if (a[q][i] == '.') {
                for (int w = q - 1; w < q + 2; w++) {
                    for (int e = i - 1; e < i + 2; e++) {
                        if (a[w][e] == '*') { p++; }
                    }
                }
                if (p != 48) { a[q][i] = p; }
                p = 48;
            }
        }
    }
    cout << "���� ��� ���� � �����:\n";
    for (int q = 1; q < n + 1; q++) {
        for (int i = 1; i < m + 1; i++) {
            cout << a[q][i] << "\t ";
        }
        cout << "\n\n";
    }
    for (int i = 0; i < n + 2; i++) {
        free(a[i]);
        a[i] = NULL;
    }
    free(a);
    a = NULL;
    cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
    cin >> c;
}
    return 0;
}