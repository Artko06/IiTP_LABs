#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double W, H, w, h, x1, x2, y1, y2, ddown, dup, dl, dr, d0;
	cout << "������� ������� W � H �������������: ";
	cin >> W >> H;
	cout << "���������� x � y ������ ������� ���� 1-�� ����� ������������: ";
	cin >> x1 >> y1;
	cout << "���������� x � y ������� �������� ���� 1-�� ����� ������������: ";
	cin >> x2 >> y2;
	cout << "������� 2-�� ����� w � h �������������: ";
	cin >> w >> h;
	d0 = LLONG_MAX;
	ddown = LLONG_MAX;
	dup = LLONG_MAX;
	dl = LLONG_MAX;
	dr = LLONG_MAX;
	if (W <= 0 || H <= 0 || w <= 0 || h <= 0 || y1 >= y2 || x1 >= x2 || x1 > W || x2 > W || y1 > H || y2 > H ) {
		cout << "���������� ������� �������� ������";
	}
	else if (y2 - y1 > H || x2 - x1 > W || w > W || h > H) {
		cout << "� ������� �� ������ ��� �����";
	}
	else if (y2 - y1 + h > H && x2 - x1 + w > W) {
		cout << "� ������� �� ������ ��� �����";
	}
	else if (y2 > 0 || y1 > 0) {
		 if (y2 - y1 + h <= H) {
			if (y1 >= h || H - y2 >= h) {
				d0 = 0;
			}
			else {
				dup = h - y1;
				ddown = h - (H - y2);
			}
		}
		if (x2 - x1 + w <= W) {
			if (x1 > 0 || x2 > 0) {
				if (x1 >= w || W - x2 >= w) {
					d0 = 0;
				}
				else {
					dr = w - x1;
					dl = w - (W - x2);
				}
			}
			else if (x1 < 0 || x2 < 0) {
			    if (-x2 >= w || W + x1 >= w) {
					d0 = 0;
				}
				else {
					dr = w + x2;
					dl = w - (W + x1);
				}
			}
		}
	}
	else if (y2 < 0 || y1 < 0) {
	     if (y2 - y1 + h <= H) {
			if (H + y1 >= h || -y2 >= h) {
				d0 = 0;
			}
			else {
				dup = h - (H + y1);
				ddown = h + y2;
			}
		 }
		if (x2 - x1 + w <= W) {
			if (x1 > 0 || x2 > 0) {
				if (W - x2 >= w || x1 >= w) {
					d0 = 0;
				}
				else {
					dl = w - (W - x2);
					dr = w - x1;
				}
			}
			else if (x1 < 0 || x2 < 0) {
				if (W + x1 >= w || -x2 >= w) {
					d0 = 0;
				}
				else {
					dr = w - (W + x1);
					dl = w + x2;
				}
			}
		}
	}
	if (d0 != dl || d0 != dr || d0 != dup || d0 != ddown || dup != ddown || dl != dr || dup != dl || ddown != dl || dup != dr || ddown != dr) {
		if (d0 <= dr && d0 <= dl && d0 <= dup && d0 <= ddown) {
			cout << "���� ���������� �������� ��: " << d0 << endl;
		}
		else if (dr <= d0 && dr <= dl && dr <= dup && dr <= ddown) {
			cout << "���� ���������� �������� ��: " << dr << endl;
		}
		else if (dl <= dr && dl <= d0 && dl <= dup && dl <= ddown) {
			cout << "���� ���������� �������� ��: " << dl << endl;
		}
		else if (dup <= dr && dup <= dl && dup <= d0 && dup <= ddown) {
			cout << "���� ���������� �������� ��: " << dup << endl;
		}
		else if (ddown <= dr && ddown <= dl && ddown <= d0 && ddown <= dup) {
			cout << "���� ���������� �������� ��: " << ddown << endl;
		}
	}
return 0;
}