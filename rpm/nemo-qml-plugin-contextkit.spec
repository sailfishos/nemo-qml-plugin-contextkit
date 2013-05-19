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
BuildRequires: pkgconfig(contextkit-statefs-qt4)
BuildRequires: pkgconfig(contextsubscriber-1.0) >= 0.5.25
BuildRequires: cmake >= 2.8

%description
%{summary}

%prep
%setup -q -n %{name}-%{version}

%build
%cmake -DUSEQT=4
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%cmake -DUSEQT=4
make install DESTDIR=%{buildroot}

%files
%defattr(-,root,root,-)
%{_libdir}/qt4/imports/org/freedesktop/contextkit/libcontextkit.so
%{_libdir}/qt4/imports/org/freedesktop/contextkit/qmldir
