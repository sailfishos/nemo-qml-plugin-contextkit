Summary: ContextKit QML bindings
Name: nemo-qml-plugin-contextkit
Version: 1.0
Release: 2
License: LGPLv2
Group: Applications/System
URL: https://github.com/nemomobile/nemo-qml-plugin-contextkit
Source0: %{name}-%{version}.tar.bz2
BuildRequires: pkgconfig(QtCore)
BuildRequires: pkgconfig(QtDeclarative)
BuildRequires: pkgconfig(contextsubscriber-1.0)
BuildRequires: cmake
Requires: contextkit

%description
%{summary}

%define qml_dir %{_libdir}/qt4/imports
%define package_dir %{qml_dir}/org/freedesktop/contextkit

%prep
%setup -q -n %{name}-%{version}

%build
%cmake .
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%files
%defattr(-,root,root,-)
%{package_dir}/libcontextkit.so
%{package_dir}/qmldir
