/****************************************************************************
** Meta object code from reading C++ file 'elevator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../assignment1/elevator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'elevator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Elevator_t {
    QByteArrayData data[9];
    char stringdata0[77];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Elevator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Elevator_t qt_meta_stringdata_Elevator = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Elevator"
QT_MOC_LITERAL(1, 9, 14), // "startScheduler"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 14), // "setPushedFloor"
QT_MOC_LITERAL(4, 40, 1), // "i"
QT_MOC_LITERAL(5, 42, 13), // "setAskedFloor"
QT_MOC_LITERAL(6, 56, 6), // "status"
QT_MOC_LITERAL(7, 63, 6), // "number"
QT_MOC_LITERAL(8, 70, 6) // "eleRun"

    },
    "Elevator\0startScheduler\0\0setPushedFloor\0"
    "i\0setAskedFloor\0status\0number\0eleRun"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Elevator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    1,   35,    2, 0x08 /* Private */,
       5,    3,   38,    2, 0x08 /* Private */,
       8,    1,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    6,    4,    7,
    QMetaType::Void, QMetaType::Int,    4,

       0        // eod
};

void Elevator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Elevator *_t = static_cast<Elevator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startScheduler(); break;
        case 1: _t->setPushedFloor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setAskedFloor((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->eleRun((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Elevator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Elevator.data,
      qt_meta_data_Elevator,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Elevator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Elevator::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Elevator.stringdata0))
        return static_cast<void*>(const_cast< Elevator*>(this));
    return QObject::qt_metacast(_clname);
}

int Elevator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
