Summary: ContextKit QML bindings
Name: nemo-qml-plugin-contextkit-qt5
Version: 2.0
Release: 2
License: LGPLv2
URL: https://git.sailfishos.org/mer-core/nemo-qml-plugin-contextkit
Source0: %{name}-%{version}.tar.bz2

Obsoletes: contextkit-declarative-qt5
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: ssu-sysinfo-devel
Requires: nemo-qml-plugin-dbus-qt5
Requires: libmce-qt5-declarative
Requires: kf5bluezqt-bluez5-declarative
Requires: libqofono-qt5-declarative
Requires: connman-qt5-declarative
Requires: qt5-qtdeclarative-import-sensors

%description
%{summary}

%prep
%setup -q -n %{name}-%{version}

%build
%qmake5
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

%files
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/org/freedesktop/contextkit
%{_datadir}/contextkit/providers
