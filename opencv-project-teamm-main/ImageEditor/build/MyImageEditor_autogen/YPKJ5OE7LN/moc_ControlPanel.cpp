/****************************************************************************
** Meta object code from reading C++ file 'ControlPanel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/ui/ControlPanel.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControlPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN12ControlPanelE_t {};
} // unnamed namespace

template <> constexpr inline auto ControlPanel::qt_create_metaobjectdata<qt_meta_tag_ZN12ControlPanelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ControlPanel",
        "applyThreshold",
        "",
        "mode",
        "value",
        "blockSize",
        "C",
        "applyHistEq",
        "clipLimit",
        "tileSize",
        "applyMorphology",
        "op",
        "kernelSize",
        "kernelShape",
        "applyCanny",
        "t1",
        "t2",
        "aperture",
        "applyAffine",
        "applyPerspective",
        "applyStitch",
        "applyCartoon",
        "applyPencilSketch",
        "color",
        "applyCrop"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'applyThreshold'
        QtMocHelpers::SignalData<void(int, int, int, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 4 }, { QMetaType::Int, 5 }, { QMetaType::Int, 6 },
        }}),
        // Signal 'applyHistEq'
        QtMocHelpers::SignalData<void(int, double, int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Double, 8 }, { QMetaType::Int, 9 },
        }}),
        // Signal 'applyMorphology'
        QtMocHelpers::SignalData<void(int, int, int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Int, 12 }, { QMetaType::Int, 13 },
        }}),
        // Signal 'applyCanny'
        QtMocHelpers::SignalData<void(double, double, int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 15 }, { QMetaType::Double, 16 }, { QMetaType::Int, 17 },
        }}),
        // Signal 'applyAffine'
        QtMocHelpers::SignalData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'applyPerspective'
        QtMocHelpers::SignalData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'applyStitch'
        QtMocHelpers::SignalData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'applyCartoon'
        QtMocHelpers::SignalData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'applyPencilSketch'
        QtMocHelpers::SignalData<void(bool)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 23 },
        }}),
        // Signal 'applyCrop'
        QtMocHelpers::SignalData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ControlPanel, qt_meta_tag_ZN12ControlPanelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ControlPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ControlPanelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ControlPanelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12ControlPanelE_t>.metaTypes,
    nullptr
} };

void ControlPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ControlPanel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->applyThreshold((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 1: _t->applyHistEq((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 2: _t->applyMorphology((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 3: _t->applyCanny((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 4: _t->applyAffine(); break;
        case 5: _t->applyPerspective(); break;
        case 6: _t->applyStitch(); break;
        case 7: _t->applyCartoon(); break;
        case 8: _t->applyPencilSketch((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 9: _t->applyCrop(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)(int , int , int , int )>(_a, &ControlPanel::applyThreshold, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)(int , double , int )>(_a, &ControlPanel::applyHistEq, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)(int , int , int )>(_a, &ControlPanel::applyMorphology, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)(double , double , int )>(_a, &ControlPanel::applyCanny, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)()>(_a, &ControlPanel::applyAffine, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)()>(_a, &ControlPanel::applyPerspective, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)()>(_a, &ControlPanel::applyStitch, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)()>(_a, &ControlPanel::applyCartoon, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)(bool )>(_a, &ControlPanel::applyPencilSketch, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControlPanel::*)()>(_a, &ControlPanel::applyCrop, 9))
            return;
    }
}

const QMetaObject *ControlPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ControlPanelE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ControlPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ControlPanel::applyThreshold(int _t1, int _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 1
void ControlPanel::applyHistEq(int _t1, double _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3);
}

// SIGNAL 2
void ControlPanel::applyMorphology(int _t1, int _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3);
}

// SIGNAL 3
void ControlPanel::applyCanny(double _t1, double _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3);
}

// SIGNAL 4
void ControlPanel::applyAffine()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ControlPanel::applyPerspective()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ControlPanel::applyStitch()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void ControlPanel::applyCartoon()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void ControlPanel::applyPencilSketch(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void ControlPanel::applyCrop()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}
QT_WARNING_POP
