/****************************************************************************
** Meta object code from reading C++ file 'renderwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../VSIM_Oblig_3/renderwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'renderwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RenderWindow_t {
    const uint offsetsAndSize[6];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_RenderWindow_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_RenderWindow_t qt_meta_stringdata_RenderWindow = {
    {
QT_MOC_LITERAL(0, 12), // "RenderWindow"
QT_MOC_LITERAL(13, 6), // "render"
QT_MOC_LITERAL(20, 0) // ""

    },
    "RenderWindow\0render\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RenderWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   20,    2, 0x08,    1 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void RenderWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RenderWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->render(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject RenderWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWindow::staticMetaObject>(),
    qt_meta_stringdata_RenderWindow.offsetsAndSize,
    qt_meta_data_RenderWindow,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_RenderWindow_t
, QtPrivate::TypeAndForceComplete<RenderWindow, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *RenderWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RenderWindow.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions_4_1_Core"))
        return static_cast< QOpenGLFunctions_4_1_Core*>(this);
    return QWindow::qt_metacast(_clname);
}

int RenderWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
