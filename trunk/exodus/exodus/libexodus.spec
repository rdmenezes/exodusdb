%define baseversion 9.6

Summary: Exodus C++ library
Name: exodus
Version: 9.6.0
Release: 1
Source0: %{name}-%{version}.tar.gz
License: MIT http://www.opensource.org/licenses/mit-license.php
Group: Development/Libraries
BuildRoot: /var/tmp/%{name}-%{version}-%{release}-root
%description
The Exodus library enables Pick/Multivalue database style programming
with only C++ and Postgres.
%prep
%setup -q
%build
%configure
%{__make}
%install
if [ "$RPM_BUILD_ROOT" != "/var/tmp/%{name}-%{version}-%{release}-root" ]
then
  echo
  echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  echo @                                                                    @
  echo @  RPM_BUILD_ROOT is not what I expected.  Please clean it yourself. @
  echo @                                                                    @
  echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  echo
else
  echo Cleaning RPM_BUILD_ROOT: "$RPM_BUILD_ROOT"
  rm -rf "$RPM_BUILD_ROOT"
fi
%{__make} install DESTDIR="$RPM_BUILD_ROOT"


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%clean
if [ "$RPM_BUILD_ROOT" != "/var/tmp/%{name}-%{version}-%{release}-root" ]
then
 echo
 echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 echo @                                                                    @
 echo @  RPM_BUILD_ROOT is not what I expected.  Please clean it yourself. @
 echo @                                                                    @
 echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 echo
else
 echo Cleaning RPM_BUILD_ROOT: "$RPM_BUILD_ROOT"
 rm -rf "$RPM_BUILD_ROOT"
fi

%files
%defattr(-,root,root)
/usr/include/%{name}-%{baseversion}/%{name}/*.h
/usr/lib/lib%{name}-%{baseversion}*
/usr/lib/pkgconfig/%{name}-%{baseversion}.pc

#%doc /usr/local/info/exodus.info
#%doc %attr(0444,root,root) /usr/local/man/man1/exodus.1
#%doc COPYING AUTHORS README NEWS