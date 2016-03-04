# QuexPipe
Application for tokenising text content with multiple and switchable Quex lexers serially like pipelining them.

There are two components: The precompiled Quex definitions which are so-called plugins (shared objects) and a main application which loads the lexer plugins, reads the text and performs the analysis.

In this version only POSIX systems are supported, and it handles only UTF-8 input. (You can use `iconv` for transcoding.)

(To create lexer-plugins: See below.)

### Dependencies:
The following packages are installed on all Linux distributions:
* libc6
* libstdc++6
* libgcc1

### Install:
* On Debian-based systems (or using dpkg): Download the latest package from [here](http://pi.itk.ppke.hu/~morta/quexpipe/) and install it. Take care of the corresponding architecture (x86/amd64)!
* Otherwise download the [master archive](archive/master.zip), unconpress it and use the Makefile:
  ```bash
  $ cd QuexPipe-master/src
  $ make
  $ sudo make install
  ```
	You can also use `sudo make uninstall` to uninstall the application.

### Usage:
You must have at least one precompiled lexer plugin. Then you can load them at start-up-time, and they will analyse the text from stdin. If you specify more lexers, they will be working in order-sensitive manner.
* Reading from file:
```
$ quexpipe lexer1.so lexer2.so lexerN.so < input.txt > output.txt
```
* Reading from stream:
```
$ cat input.txt | quexpipe lexer1.so lexer2.so > output.txt
```
* Transcoding file to UTF-8
```
$ iconv -f ISO-8859-2 -t UTF-8 input.txt | quexpipe lexer.so
```
# Dev-tool

It contains some core implementations and a lot of helper functions to make the lexer-plugin development much easier.

### Dependencies:
You must have Quex int the path and the `$QUEX_PATH` variable correclty set.
### Install:
* On Debian-based systems (or using dpkg): Download the latest quexpipe-dev-tool package from [here](http://pi.itk.ppke.hu/~morta/quexpipe/) and install it.
* Otherwise download the [master archive](archive/master.zip), unconpress it and use the Makefile:
```bash
$ cd QuexPipe-master/dev-tool
$ sudo make install
```
If you want to uninstall it, just run `sudo make uninstall`.
### Usage
#####  Definition
You can use the following headers in the Quex grammar definitions at the header part:
* `#include <quexpipe/charfuncs.h>`
* `#include <quexpipe/xml.h>`

They contain very useful type-insensitive function templates for appending or concatenating cstrings, number-to-text converting and creating xml tags.

For more information about Quex definitions read the documentation.

#####  Compiling
Instead of generating sources from the definition, writing your own implementation, looking for the corresponding buffer handling, and compiling the code to shared object which fits in the application API, you can use the qx-to-plugin compiler. You should just know about the token policy and the optimal buffer size. (See the Quex documentation.)

It is very simple:

```
$ ls
mylexer.qx
$ quexpipe-plugin-builder mylexer.qx -p queue
...
$ ls
mylexer.qx  mylexer.so
```

For more information, see: `quexpipe-plugin-build --help`
