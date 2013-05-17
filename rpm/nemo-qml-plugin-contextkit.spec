Summary: ContextKit QML bindings
Name: nemo-qml-plugin-contextkit
Version: 1.1
Release: 2
License: LGPLv2
Group: Applications/System
URL: https://github.com/nemomobile/nemo-qml-plugin-contextkit
Source0: %{name}-%{version}.tar.bz2
BuildRequires: pkgconfig(QtCore)
BuildRequires: pkgconfig(QtDeclarative)
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(contextkit-statefs-qt4)
BuildRequires: pkgconfig(contextkit-statefs)
BuildRequires: cmake >= 2.8

%description
%{summary}

%package -n nemo-qml-plugin-contextkit-qt5
Summary: Provider to expose contextkit providers properties
Group: System Environment/Libraries
%description -n nemo-qml-plugin-contextkit-qt5
Provider exposes all contextkit providers properties

%define qml_dir %{_libdir}/qt4/imports
%define package_dir %{qml_dir}/org/freedesktop/contextkit

%prep
%setup -q -n %{name}-%{version}

%build
%cmake -DUSEQT=4
make %{?jobs:-j%jobs}
%cmake -DUSEQT=5
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%cmake -DUSEQT=4
make install DESTDIR=%{buildroot}
%cmake -DUSEQT=5
make install DESTDIR=%{buildroot}

%files
%defattr(-,root,root,-)
%{_libdir}/qt4/imports/org/freedesktop/contextkit/libcontextkit.so
%{_libdir}/qt4/imports/org/freedesktop/contextkit/qmldir

%files -n nemo-qml-plugin-contextkit-qt5
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/org/freedesktop/contextkit/libcontextkit.so
%{_libdir}/qt5/qml/org/freedesktop/contextkit/qmldir
