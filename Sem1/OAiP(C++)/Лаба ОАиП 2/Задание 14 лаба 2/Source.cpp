#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double W, H, w, h, x1, x2, y1, y2, ddown, dup, dl, dr, d0;
	cout << "Размеры комнаты W и H соответсвенно: ";
	cin >> W >> H;
	cout << "Координаты x и y левого нижнего угла 1-го стола соответсвено: ";
	cin >> x1 >> y1;
	cout << "Координаты x и y правого верхнего угла 1-го стола соответсвено: ";
	cin >> x2 >> y2;
	cout << "Размеры 2-го стола w и h соответсвенно: ";
	cin >> w >> h;
	d0 = LLONG_MAX;
	ddown = LLONG_MAX;
	dup = LLONG_MAX;
	dl = LLONG_MAX;
	dr = LLONG_MAX;
	if (W <= 0 || H <= 0 || w <= 0 || h <= 0 || y1 >= y2 || x1 >= x2 || x1 > W || x2 > W || y1 > H || y2 > H ) {
		cout << "Некоректно введены исходные данные";
	}
	else if (y2 - y1 > H || x2 - x1 > W || w > W || h > H) {
		cout << "В комнату не влезет два стола";
	}
	else if (y2 - y1 + h > H && x2 - x1 + w > W) {
		cout << "В комнату не влезет два стола";
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
			cout << "Стол необхидимо сдвинуть на: " << d0 << endl;
		}
		else if (dr <= d0 && dr <= dl && dr <= dup && dr <= ddown) {
			cout << "Стол необхидимо сдвинуть на: " << dr << endl;
		}
		else if (dl <= dr && dl <= d0 && dl <= dup && dl <= ddown) {
			cout << "Стол необхидимо сдвинуть на: " << dl << endl;
		}
		else if (dup <= dr && dup <= dl && dup <= d0 && dup <= ddown) {
			cout << "Стол необхидимо сдвинуть на: " << dup << endl;
		}
		else if (ddown <= dr && ddown <= dl && ddown <= d0 && ddown <= dup) {
			cout << "Стол необхидимо сдвинуть на: " << ddown << endl;
		}
	}
return 0;
}