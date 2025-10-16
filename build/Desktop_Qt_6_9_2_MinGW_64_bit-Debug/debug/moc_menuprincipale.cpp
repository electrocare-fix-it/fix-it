/****************************************************************************
** Meta object code from reading C++ file 'menuprincipale.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../menuprincipale.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'menuprincipale.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
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
struct qt_meta_tag_ZN14menuprincipaleE_t {};
} // unnamed namespace

template <> constexpr inline auto menuprincipale::qt_create_metaobjectdata<qt_meta_tag_ZN14menuprincipaleE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "menuprincipale",
        "homeClicked",
        "",
        "openObjects",
<<<<<<< HEAD
        "openClients",
        "on_homeButton_clicked",
        "on_gestionObjectsButton_clicked",
        "on_gestionClientsButton_clicked"
=======
        "on_homeButton_clicked",
        "on_gestionObjectsButton_clicked"
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'homeClicked'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'openObjects'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
<<<<<<< HEAD
        // Signal 'openClients'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_homeButton_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_gestionObjectsButton_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_gestionClientsButton_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
=======
        // Slot 'on_homeButton_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_gestionObjectsButton_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<menuprincipale, qt_meta_tag_ZN14menuprincipaleE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject menuprincipale::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14menuprincipaleE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14menuprincipaleE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14menuprincipaleE_t>.metaTypes,
    nullptr
} };

void menuprincipale::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<menuprincipale *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->homeClicked(); break;
        case 1: _t->openObjects(); break;
<<<<<<< HEAD
        case 2: _t->openClients(); break;
        case 3: _t->on_homeButton_clicked(); break;
        case 4: _t->on_gestionObjectsButton_clicked(); break;
        case 5: _t->on_gestionClientsButton_clicked(); break;
=======
        case 2: _t->on_homeButton_clicked(); break;
        case 3: _t->on_gestionObjectsButton_clicked(); break;
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (menuprincipale::*)()>(_a, &menuprincipale::homeClicked, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (menuprincipale::*)()>(_a, &menuprincipale::openObjects, 1))
            return;
<<<<<<< HEAD
        if (QtMocHelpers::indexOfMethod<void (menuprincipale::*)()>(_a, &menuprincipale::openClients, 2))
            return;
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
    }
}

const QMetaObject *menuprincipale::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *menuprincipale::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14menuprincipaleE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int menuprincipale::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
<<<<<<< HEAD
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
=======
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
    }
    return _id;
}

// SIGNAL 0
void menuprincipale::homeClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void menuprincipale::openObjects()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
<<<<<<< HEAD

// SIGNAL 2
void menuprincipale::openClients()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
QT_WARNING_POP
