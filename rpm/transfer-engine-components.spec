Name: declarative-transferengine
Version: 0.0.16
Release: 0
Summary: Sailfish declarative bindings for Nemo Transfer Engine.
Group: System/Libraries
License: TBD
URL: TBD
Source0: %{name}-%{version}.tar.gz
BuildRequires: pkgconfig(QtDeclarative)
BuildRequires: pkgconfig(QtDBus)
BuildRequires: pkgconfig(nemotransferengine)

%description -n declarative-transferengine
%{summary}.

%files -n declarative-transferengine
%defattr(-,root,root,-)
%{_libdir}/qt4/imports/Sailfish/TransferEngine/*
%{_datadir}/translations/sailfish_transferengine_eng_en.qm

%prep
%setup -q -n %{name}-%{version}

%build

%qmake

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake_install
rm -f %{buildroot}/usr/bin/transfer-demo %{buildroot}/usr/lib/debug/usr/bin/transfer-demo.debug %{buildroot}/usr/share/applications/transfer-demo.desktop %{buildroot}/usr/share/dbus-1/services/com.jolla.transferdemoapp.service %{buildroot}/usr/share/jolla-settings/entries/transferui.json %{buildroot}/usr/share/jolla-settings/pages/transferui/TransferCover.qml %{buildroot}/usr/share/jolla-settings/pages/transferui/mainpage.qml %{buildroot}/usr/share/nemo-transferengine/nemo-transfer-engine.conf %{buildroot}/usr/share/translations/source/sailfish_transferengine.ts



