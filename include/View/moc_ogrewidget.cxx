/****************************************************************************
** Meta object code from reading C++ file 'QtOgreWidget.h'
**
** Created: Mon Aug 27 14:11:08 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QtOgreWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtOgreWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_sandgis__OverlayWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_sandgis__OverlayWidget[] = {
    "sandgis::OverlayWidget\0"
};

void sandgis::OverlayWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData sandgis::OverlayWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject sandgis::OverlayWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sandgis__OverlayWidget,
      qt_meta_data_sandgis__OverlayWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &sandgis::OverlayWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *sandgis::OverlayWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *sandgis::OverlayWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sandgis__OverlayWidget))
        return static_cast<void*>(const_cast< OverlayWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int sandgis::OverlayWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_sandgis__QtOgreWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      38,   35,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_sandgis__QtOgreWidget[] = {
    "sandgis::QtOgreWidget\0\0timerLoop()\0"
    "id\0contextMenu(int)\0"
};

void sandgis::QtOgreWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtOgreWidget *_t = static_cast<QtOgreWidget *>(_o);
        switch (_id) {
        case 0: _t->timerLoop(); break;
        case 1: _t->contextMenu((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData sandgis::QtOgreWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject sandgis::QtOgreWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sandgis__QtOgreWidget,
      qt_meta_data_sandgis__QtOgreWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &sandgis::QtOgreWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *sandgis::QtOgreWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *sandgis::QtOgreWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sandgis__QtOgreWidget))
        return static_cast<void*>(const_cast< QtOgreWidget*>(this));
    if (!strcmp(_clname, "RenderView"))
        return static_cast< RenderView*>(const_cast< QtOgreWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int sandgis::QtOgreWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
