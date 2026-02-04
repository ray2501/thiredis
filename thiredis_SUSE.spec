%{!?directory:%define directory /usr}

%define buildroot %{_tmppath}/%{name}

Name:          thiredis
Summary:       Tcl wrapper for Redis client hiredis
Version:       0.9
Release:       0
License:       BSD-3-Clause
Group:         Development/Libraries/Tcl
Source:        %{name}-%{version}.tar.gz
URL:           https://github.com/ray2501/thiredis
BuildRequires: autoconf
BuildRequires: gcc
BuildRequires: make
BuildRequires: tcl-devel >= 8.4
Requires:      tcl >= 8.4
BuildRoot:     %{buildroot}

%description
It is Tcl wrapper for Redis client hiredis.
It is using Synchronous API and only for test.

%prep
%setup -q -n %{name}-%{version}

%build
./configure \
	--prefix=%{directory} \
	--exec-prefix=%{directory} \
	--libdir=%{directory}/%{_lib}
make 

%install
make DESTDIR=%{buildroot} pkglibdir=%{tcl_archdir}/%{name}%{version} install

%clean
rm -rf %buildroot

%files
%defattr(-,root,root)
%{tcl_archdir}
