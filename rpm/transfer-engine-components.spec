Name: declarative-transferengine-qt5
Version: 0.0.35
Release: 0
Summary: Sailfish declarative bindings for Nemo Transfer Engine.
Group: System/Libraries
License: TBD
URL: TBD
Source0: %{name}-%{version}.tar.gz
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(accounts-qt5)
BuildRequires: pkgconfig(nemotransferengine-qt5)
BuildRequires: qt5-qttools
BuildRequires: qt5-qttools-linguist

Requires: nemo-transferengine-qt5 >= 0.0.23
Requires: sailfishsilica-qt5

%description -n declarative-transferengine-qt5
%{summary}.

%files -n declarative-transferengine-qt5
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/Sailfish/TransferEngine/*
%{_datadir}/translations/sailfish_transferengine_eng_en.qm

%package -n jolla-transferdemo-qt5
Summary: Jolla Transfer Demo Application
Group: Applications/Multimedia
Requires: nemo-transferengine-qt5
Requires: declarative-transferengine-qt5
Obsoletes: jolla-transferdemo <= 0.0.23
Provides: jolla-transferdemo > 0.0.23

%description -n jolla-transferdemo-qt5
%{summary}.

%files -n jolla-transferdemo-qt5
%defattr(-,root,root,-)
%{_bindir}/transfer-demo
%{_datadir}/applications/transfer-demo.desktop
%{_datadir}/dbus-1/services/com.jolla.transferdemoapp.service


%package -n jolla-settings-transferui-qt5
Summary: Jolla Transfer UI extension for the Settings FW
Group: Applications/Multimedia
Requires: declarative-transferengine-qt5
Requires: jolla-settings
Obsoletes: jolla-settings-transferui <= 0.0.23
Provides: jolla-settings-transferui > 0.0.23
Requires: jolla-ambient
Requires: ambient-icons-closed

%description -n jolla-settings-transferui-qt5
%{summary}.

%files -n jolla-settings-transferui-qt5
%defattr(-,root,root,-)
%{_datadir}/jolla-settings/entries/transferui.json
%{_datadir}/jolla-settings/pages/transferui/TransferCover.qml
%{_datadir}/jolla-settings/pages/transferui/mainpage.qml
%{_datadir}/nemo-transferengine/*.conf

%package ts-devel
Summary:   Translation source for Sailfish TransferEngine
License:   TBD
Group:     System/Libraries

%description ts-devel
Translation source for Sailfish TransferEngine

%files ts-devel
%defattr(-,root,root,-)
%{_datadir}/translations/source/sailfish_transferengine.ts

%prep
%setup -q -n %{name}-%{version}

%build

%qmake5

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake5_install

