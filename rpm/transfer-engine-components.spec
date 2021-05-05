Name: declarative-transferengine-qt5
Version: 0.3
Release: 1
Summary: Sailfish declarative bindings for Nemo Transfer Engine.
License: Proprietary
URL: http://sailfishos.org
Source0: %{name}-%{version}.tar.gz
Source1: sailfish-share.privileges
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(accounts-qt5)
BuildRequires: pkgconfig(nemotransferengine-qt5)
BuildRequires: pkgconfig(nemonotifications-qt5)
BuildRequires: pkgconfig(nemodbus)
BuildRequires: qt5-qttools
BuildRequires: qt5-qttools-linguist

Requires: nemo-transferengine-qt5 >= 0.2.2
Requires: sailfishsilica-qt5 >= 1.1.46
Requires: nemo-qml-plugin-filemanager

%description -n declarative-transferengine-qt5
%{summary}.

%files -n declarative-transferengine-qt5
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/Sailfish/TransferEngine/*
%{_datadir}/translations/sailfish_transferengine_eng_en.qm


%package -n sailfishshare-components
Summary: QML application and components for sharing features.
Requires: nemo-qml-plugin-dbus-qt5
BuildRequires: nemo-qml-plugin-dbus-qt5-devel

%description -n sailfishshare-components
%{summary}

%files -n sailfishshare-components
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/Sailfish/Share/*
%{_libexecdir}/sailfish-share
%{_datadir}/sailfish-share
%{_datadir}/applications/sailfish-share.desktop
%{_datadir}/dbus-1/services/org.sailfishos.share.service
%{_datadir}/translations/sailfishshare_eng_en.qm
%{_datadir}/mapplauncherd/privileges.d/*

%package -n sailfishshare-components-ts-devel
Summary: Translation source for sailfishshare-components

%description -n sailfishshare-components-ts-devel
%{summary}

%files -n sailfishshare-components-ts-devel
%defattr(-,root,root,-)
%{_datadir}/translations/source/sailfishshare.ts


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
Requires: sailfish-content-graphics
Requires: ambient-icons-closed

%description -n jolla-settings-transferui-qt5
%{summary}.

%files -n jolla-settings-transferui-qt5
%defattr(-,root,root,-)
%{_datadir}/jolla-settings/entries/transferui.json
%{_datadir}/jolla-settings/pages/transferui/mainpage.qml
%{_datadir}/nemo-transferengine/*.conf

%package ts-devel
Summary:   Translation source for Sailfish TransferEngine
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

make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

mkdir -p %{buildroot}%{_datadir}/mapplauncherd/privileges.d
install -m 644 -p %{SOURCE1} %{buildroot}%{_datadir}/mapplauncherd/privileges.d/
