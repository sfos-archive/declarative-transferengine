Name: declarative-transferengine
Version: 0.0.3
Release: 0
Summary: Sailfish declarative bindings for Nemo Transfer Engine.
Group: System/Libraries
License: TBD
URL: TBD
Source0: %{name}-%{version}.tar.gz
BuildRequires: pkgconfig(QtDeclarative)
BuildRequires: pkgconfig(QtDBus)
BuildRequires: pkgconfig(accounts-qt)
BuildRequires: pkgconfig(nemotransferengine)

Requires: nemo-transferengine 
Requires: sailfishsilica

%description -n declarative-transferengine
%{summary}.

%files -n declarative-transferengine
%defattr(-,root,root,-)
%{_libdir}/qt4/imports/Sailfish/TransferEngine/*
%{_datadir}/translations/sailfish_transferengine_eng_en.qm


%package -n jolla-transferdemo
Summary: Jolla Transfer Demo Application
Group: Applications/Multimedia
Requires: nemo-transferengine 
Requires: declarative-transferengine

%description -n jolla-transferdemo
%{summary}.

%files -n jolla-transferdemo
%defattr(-,root,root,-)
%{_bindir}/transfer-demo
%{_datadir}/applications/transfer-demo.desktop
%{_datadir}/dbus-1/services/com.jolla.transferdemoapp.service


%package -n jolla-settings-transferui
Summary: Jolla Transfer UI extension for the Settings FW
Group: Applications/Multimedia
Requires: declarative-transferengine
Requires: jolla-settings

%description -n jolla-settings-transferui
%{summary}.

%files -n jolla-settings-transferui
%defattr(-,root,root,-)
%{_datadir}/jolla-settings/entries/transferui.json
%{_datadir}/jolla-settings/pages/transferui/mainpage.qml


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

%qmake

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake_install

