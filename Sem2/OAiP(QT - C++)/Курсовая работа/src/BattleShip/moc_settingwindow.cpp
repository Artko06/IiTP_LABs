/****************************************************************************
** Meta object code from reading C++ file 'settingwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "settingwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SettingWindow_t {
    QByteArrayData data[11];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SettingWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SettingWindow_t qt_meta_stringdata_SettingWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SettingWindow"
QT_MOC_LITERAL(1, 14, 10), // "menuSignal"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 17), // "personButtonClick"
QT_MOC_LITERAL(4, 44, 14), // "botButtonClick"
QT_MOC_LITERAL(5, 59, 26), // "backButton_from_firstField"
QT_MOC_LITERAL(6, 86, 27), // "backButton_from_statsWindow"
QT_MOC_LITERAL(7, 114, 21), // "on_menuButton_clicked"
QT_MOC_LITERAL(8, 136, 20), // "on_botButton_clicked"
QT_MOC_LITERAL(9, 157, 23), // "on_personButton_clicked"
QT_MOC_LITERAL(10, 181, 22) // "on_statsButton_clicked"

    },
    "SettingWindow\0menuSignal\0\0personButtonClick\0"
    "botButtonClick\0backButton_from_firstField\0"
    "backButton_from_statsWindow\0"
    "on_menuButton_clicked\0on_botButton_clicked\0"
    "on_personButton_clicked\0on_statsButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SettingWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    0,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SettingWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SettingWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->menuSignal(); break;
        case 1: _t->personButtonClick(); break;
        case 2: _t->botButtonClick(); break;
        case 3: _t->backButton_from_firstField(); break;
        case 4: _t->backButton_from_statsWindow(); break;
        case 5: _t->on_menuButton_clicked(); break;
        case 6: _t->on_botButton_clicked(); break;
        case 7: _t->on_personButton_clicked(); break;
        case 8: _t->on_statsButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SettingWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingWindow::menuSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SettingWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingWindow::personButtonClick)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SettingWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingWindow::botButtonClick)) {
                *result = 2;
                return;
            }
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject SettingWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_SettingWindow.data,
    qt_meta_data_SettingWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SettingWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SettingWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SettingWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int SettingWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void SettingWindow::menuSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SettingWindow::personButtonClick()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SettingWindow::botButtonClick()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
