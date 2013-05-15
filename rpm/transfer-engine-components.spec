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
%qmake_install

