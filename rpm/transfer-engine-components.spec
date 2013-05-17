Name: declarative-transferengine-qt5
Version: 0.0.16
Release: 0
Summary: Sailfish declarative bindings for Nemo Transfer Engine.
Group: System/Libraries
License: TBD
URL: TBD
Source0: %{name}-%{version}.tar.gz
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(nemotransferengine-qt5)
BuildRequires: qt5-qttools-linguist

%description -n declarative-transferengine-qt5
%{summary}.

%files -n declarative-transferengine-qt5
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/Sailfish/TransferEngine/*
%{_datadir}/translations/sailfish_transferengine_eng_en.qm

%prep
%setup -q -n %{name}-%{version}

%build

%qmake5

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake5_install
rm -f %{buildroot}/usr/bin/transfer-demo %{buildroot}/usr/lib/debug/usr/bin/transfer-demo.debug %{buildroot}/usr/share/applications/transfer-demo.desktop %{buildroot}/usr/share/dbus-1/services/com.jolla.transferdemoapp.service %{buildroot}/usr/share/jolla-settings/entries/transferui.json %{buildroot}/usr/share/jolla-settings/pages/transferui/TransferCover.qml %{buildroot}/usr/share/jolla-settings/pages/transferui/mainpage.qml %{buildroot}/usr/share/nemo-transferengine/nemo-transfer-engine.conf %{buildroot}/usr/share/translations/source/sailfish_transferengine.ts
