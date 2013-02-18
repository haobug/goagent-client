goagent-client
==============

A new implementation of GoAgent client with pure C language

To Build:

1. Mac OS X

For Mac OS X:
```
mkdir build
cd build
cmake ..
make
```

For iOS:
First, you need install theos toolset. http://iphonedevwiki.net/index.php/Theos/Getting_Started#On_Mac_OS_X_or_Linux

To Install theos, please excute following commands as root:
```
export THEOS=/opt/theos
svn co http://svn.howett.net/svn/theos/trunk $THEOS
curl -s http://dl.dropbox.com/u/85683265/ldid > $THEOS/bin/ldid; chmod +x $THEOS/bin/ldid
```

Then switch back to normal user to compile our code :
```
export THEOS=/opt/theos
cp ios_makefile makefile
make
```



2. Linux

```
mkdir build
cd build
cmake ..
make
```

3. Windows

```
mkdir build
cd build
cmake ..
```
Open the generated sln file with Visual Studio.
