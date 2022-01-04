Summary: ContextKit QML bindings
Name: nemo-qml-plugin-contextkit-qt5
Version: 2.0
Release: 2
License: LGPLv2+
URL: https://github.com/sailfishos/nemo-qml-plugin-contextkit
Source0: %{name}-%{version}.tar.bz2

Provides: contextkit-declarative-qt5
Obsoletes: contextkit-declarative-qt5
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Gui)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: ssu-sysinfo-devel
Requires: nemo-qml-plugin-dbus-qt5
Requires: libmce-qt5-declarative
Requires: kf5bluezqt-declarative
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
%qmake5_install

%files
%defattr(-,root,root,-)
%license LICENSE LICENSE.LGPL
%{_libdir}/qt5/qml/org/freedesktop/contextkit
%{_datadir}/contextkit/providers
