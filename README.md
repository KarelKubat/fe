# fe

<!-- toc -->
- [User Guide](#user-guide)
  - [Why use fe and not a different utility?](#why-use-fe-and-not-a-different-utility)
  - [Encryption and decryption keys](#encryption-and-decryption-keys)
  - [Don't loose your key](#dont-loose-your-key)
  - [How to encrypt existing files](#how-to-encrypt-existing-files)
- [Installation](#installation)
  - [Optional: override the magic seed](#optional-override-the-magic-seed)
  - [Installing the binary and shared library](#installing-the-binary-and-shared-library)
  - [Mac OSX](#mac-osx)
  - [Sanity checking](#sanity-checking)
    - [Transcrypting a plain text file](#transcrypting-a-plain-text-file)
    - [Transcrypting on the fly](#transcrypting-on-the-fly)
    - [Testing a transcrypting environment](#testing-a-transcrypting-environment)
- [Technical Details](#technical-details)
  - [How it works: shared object preloading](#how-it-works-shared-object-preloading)
  - [Fe's runtime context](#fes-runtime-context)
  - [Transcryption](#transcryption)
  - [What fe cannot do](#what-fe-cannot-do)
  - [Porting](#porting)
  - [API: Embedding fe in your own programs](#api-embedding-fe-in-your-own-programs)
    - [Including fe.h](#including-feh)
    - [The API](#the-api)
    - [Linkage](#linkage)
<!-- /toc -->

## User Guide

Fe is an encryption utility aimed at local files (on your filesystem).

But wait, there's more. Fe is a "somewhat smart" utility. It can prepare an encryption layer that other processes use. For those processes, accessed files are perceived as plaintext, while in fact they are guarded by fe and reside on disk only in an encrypted form.

What good is that? There is a number of use cases:

*  Say you store your passwords, credit card data etc. in a file `mysecretfile.txt` on disk.  You want the file to be encrypted, so that no one (not even "root") can see. So every time you want to update that file, you type:

   ```shell
   fe emacs mysecretfile.txt
   ```

   (Don't worry. You can also use `vi`. The part `emacs mysecretfile.txt` above is just a command that fe runs for you.)

   Fe is smart enough to guess that the commandline argument should be a target for encryption and decryption.

*  Let's say that `mysecretfile.txt` contains a line with `amazon.com` and with the password that you use on that site. If you want to look up this password, you type:

   ```shell
   fe grep amazon mysecretfile.txt
   ```

   Here, fe will assume that only `mysecretfile.txt` is an encryption target, since it's a file. If you need to be sure to restrict the target to only `mysecretfile.txt`, then you can always use flag `-t`:

   ```shell
   fe -t mysecretfile grep amazon mysecretfile.txt
   ```

*  You want to start an entire new shell on top of fe's encryption layer, so that the encrypted file `$HOME/etc/mysecretfile.txt` appears unencrypted to all stacked processes, while in fact it's in encrypted form on disk. You type:

   ```shell
   fe -t ~/etc/mysecretfile.txt bash
   ```

   Next, inside that bash, the file appears plain-text. E.g., when you type

   ```shell
   cat ~/etc/mysecretfile.txt
   emacs ~/etc/mysecretfile.txt
   ```

   then `cat` or `emacs` will just see a plaintext file and the file on disk will never change its encrypted state.

### Why use fe and not a different utility?

There are many encryption utilities around. I think that fe has its particular strengths:

*  Full disk encryption utilities such as *truecrypt* "unlock" the disk once the system boots. If an attacker gains entry into a running system, they can access plaintext files. Also, user "root" can see it all.

*  File-based utilities such as *pgp* work with two files: one encrypted, one plaintext. If you store sensitive information in a pgp-encrypted file, then your workflow is probably: decrypt/edit/encrypt; and therefore, the plaintext version appears on disk once the utility is asked to decrypt it. The same  also goes for e.g. encrypted zip files. Again, others can see it if they time it right.

*  The case is even stronger when you consider the underlying disk.  Once a plaintext file has been on the disk (even though it's removed now), it can possibly be reconstructed. This is even more true for SSD's; the internal processor of an SSD will try to postpone writing over previously used sectors so that there's an even usage spread.  So, on an SSD, the contents of a deleted plaintext file might be in more than one location.

### Encryption and decryption keys

Fe uses symmetric encryption; which means that the same key is used for both encryption and decryption. Actually, fe doesn't even know if it's encrypting or decrypting, it's just "transcrypting" bytes.

When fe needs to transcrypt data, then it needs a key. A key can be given to fe in any of three ways:

1. Having fe prompt for the key. This option is the safest.

1. Using the command line switch `-k`, as in:

   ```shell
   fe -k mysecret  ....
   ```

   This option is unsafe and should be used sparingly, because a simple process listing using "ps" will reveal the commandline and hence the secret key.

1. Using the environment variable `FE_KEY`, as in:

   ```shell
   export FE_KEY=mysecret
   fe  ....
   ```

   This option is unsafe and should be used sparingly, because the secret remains in the environment and can be seen. For example, if fe spawns a shell, then a simple `printenv` will reveal the key.  Furthermore, on Linux, user "root" can access the environments of running programs via `/proc`.

### Don't loose your key

Fe doesn't store key information anywhere. And it can't detect whether the right key is used when decrypting (it just transcrypts bytes, remember). Therefore, don't loose your key. There is no way to get back plaintext information once it's been encrypted and the key is lost.

While that looks like a flaw, consider this. Given the fact that there is no information whether a transcryption is using the right key or not, there must be countless keys that produce some meaningful output.  For example, if a brute-forced key decrypts a file to say "hello", is that the right key? No-one knows; there's also a key to decrypt the same bytes into "world". Brute-forcing has just become a bit harder.  (Nevertheless, make sure that you use "good" keys when using fe.)

### How to encrypt existing files

If you have plaintext files and want to encrypt them for usage with fe, then there are basically two options:

1. Use fe's flag `-f`, as in:

   ```shell
   fe -f plaintext.txt              # make plaintext.txt encrypted
   mv plaintext.txt encrypted.txt   # rename to encrypted.txt
   ```

   In this case, the file is transcrypted "in-place" and you get a nice progress meter showing how far fe is. Most stop-signals, like pressing ^C, are ignored (else you might wind up with a half-transcrypted file).

1. Have a standard utility like `cp` do it for you, while fe is "underneath" and provides a transcryption layer.

   ```shell
   fe -t encrypted.txt  cp plaintext.txt encrypted.txt
   rm plaintext.txt
   ```

   In this case, fe is instructed only to transcrypt bytes for the file `encrypted.txt`. Hence, the `cp` command will read `plaintext.txt` as-is, but when writing `encrypted.txt`, the encryption magic will happen.

## Installation

### Optional: override the magic seed

The source file `lib/feseedserialize.c` contains a setting for the magic seed string, aptly named `MAGIC`. If you wish, you can override it. The magic string is compiled into the fe binary and controls how different processes are aware of fe's settings (these are communicated between processes using either shared memory, or an environment variable).

### Installing the binary and shared library

Type `make install`. You might want to do this as user root (e.g., via `sudo`), because the default installation target directories are `/usr/local/lib` and `/usr/local/bin`.

On Linux, ensure that `/usr/local/lib` (or your chosen library directory) is stated in `/etc/ld.so.conf`. Then type (as user "root"):

```shell
ldconfig
```

This will update the table of the dynamic linker, so that `/usr/local/lib/libfe.so` will be found at next invocations of fe.

If you don't want to install to `/usr/local/{bin,dir}` then you can set `BINDIR` and `LIBDIR` accordingly before typing `make install`:

```shell
export BINDIR=/where/ever/bin
export LIBDIR=/where/ever/lib
make install
```

In that case, for Linux, you will have to add `/where/ever/lib` to your list of library directories in `/etc/ld.so.conf`.

### Mac OSX

On a Mac, ensure that the installation directory for the shared library (default: `/usr/local/lib`) is reflected in the environment variable `DYLD_LIBRARY_PATH`. Typically, put into your shell startup script:

```shell
export DYLD_LIBRARY_PATH="/usr/local/lib:$DYLD_LIBRARY_PATH"
```

Refresh the setting using:

```shell
sudo update_dyld_shared_cache
```

### Sanity checking

Type `make test`. This will run a number of (simple) checks to prove the sanity of the installed software. Alternatively, try one of the below for a "manual run".

#### Transcrypting a plain text file

Create a plaintext file, e.g.:

```shell
echo 'Hello World' > /tmp/hello.txt
```

Encrypt it:

```shell
fe -f /tmp/hello.txt          # fe will prompt for a user secret
od -c /tmp/hello.txt          # will just show gibberish
```

Test reading it back:

```shell
fe cat /tmp/hello.txt         # enter the same secret as above,
                              # will show plain text
```

#### Transcrypting on the fly

This step won't ever create a plaintext file.

```shell
fe vi /tmp/secret.txt         # fe will prompt for a user secret
                              # then in vi enter some plain text
                              # (use emacs, or pico if you want)
od -c /tmp/secret.txt         # will just show gibberish
fe cat /tmp/secret.txt        # enter the same secret above, shows plain text
```

#### Testing a transcrypting environment

This step starts an entire shell with a transcrypt-target.

```shell
fe -t /tmp/another.txt bash   # starts a fresh bash with named target
vi /tmp/another.txt           # put some plain text in there

In another terminal run:

```shell
od -c /tmp/another.txt        # will just show gibberish
```

In the transcrypting shell:

```shell
cat /tmp/another.txt          # shows the plain text
exit                          # stops the bash process on top of fe
```

## Technical Details

### How it works: shared object preloading

Fe's magic is located in a shared library `/usr/local/lib/libfe.so` (or `.dylib` on MacOSX). This library defines a number of libc's routines:

- `open()`
- `read()` and `write()`
- `close()`
- and similar calls, such as `readv()`, `writev()` and `pread()`, `pwrite(`), `fread()`, `fwrite()`

Fe makes sure that this library is inserted underneath spawned programs, so that their libc calls are caught in the library and so that the file contents may be transcrypted if necessary. This works by setting the environment variable `LD_PRELOAD` (Linux) or `DYLD_INSERT_LIBRARIES` (MacOSX). This setting can be seen; if you run

```shell
fe printenv
```

### Fe's runtime context

Each preloaded libc function (`open()`, `read()` etc.) must be able to determine what it should do:

- Is a given file a transcryption target or not
- If yes, using what key should the data be transcrypted

How to find this information is kept in an environment variable `FE_CTX`. Each process that is spawned by fe invokes functions in fe's shared object. The shared object functions inspect the context. The user secret therefore also has to appear in the context, but it's there in an encrypted form (using the built-in magic, see above).

The context is actually either a serialized string where all necessary info is kept, or an integer that points to a shared memory block where this info is stored. This is controlled by fe's flag `-e`.

### Transcryption

All data that passes through fe's own `read()` or `write()` (or similar functions) is eligible for transcryption. This depends on whether the associated file is a target.

Data transcryption is a simple XOR operation with a stream of pseudorandom bytes. The generation of the pseudorandom stream must of course be repeatable and depends on the user-supplied secret passphrase and on offset in the file, meaning that each n'th byte in a file must be XOR-ed with the same pseudorandom value in order to encrypt or decrypt it.

The generation of pseudorandom bytes occurs in blocks of 1024 bits at a time using [Skein hashing](https://en.wikipedia.org/wiki/Skein_(hash_function)). The input for the hash function is the user secret and the file offset rounded down to the next multiple of 128 bytes (1024 bits). That way a new hash value has to be computed only each 128 bytes, which makes transcryption of blocks of data much faster.

### What fe cannot do

Fe can't intercept all calls that access physical files. The most important ones are implemented in fe's shared object, but notably `fprintf()` and `fscanf()` are not. (Such calls don't expose a buffer length, given their format string expansions. Fe can't determine file offsets, and hence, how to apply transcription).

Therefore, programs that access files using such functions are handled in either of two ways:

- Either, fe ignores `fscanf()` and such functions, and the information will occur in plaintext on disk;
- Or, fe aborts when a program uses such a function, in order to avoid plaintext information being committed to disk.

The behavior is controlled using flag `-i`.

### Porting

This source tree is suitable for builds on Linux and on Darwin. If you want to port fe to a different Unix flavor, then proceed as follows:

1. Edit the top level `Makefile` and make sure that `USYS` gets set appropriately given the output of your standard `uname` command.
1. Edit `main/fe.c` and make sure that the new value of `USYS` is interpreted to set the necessary environment variables for dynamic library injection.
1. Send me the diffs and your contribution will be in the next release. Or better, prepare a pull request and let me know.

If you are unclear as to the what and why, don't hesitate to contact me. I may not have access to your flavor of Unix, but I can probably point out where things go wrong when they do.

### API: Embedding fe in your own programs

In order to embed fe's logic into your C programs, you will need to include `fe.h` in your source, and you'll need to link against the shared library (`libfe.so` or `libfe.dylib`). On some systems you'll also need to include libdl.a, the dynamic loader interface.

#### Including fe.h

Make sure that you copy fe.h from the source archive to your "include" directory, e.g., to `/usr/local/include`. When compiling, add the appropriate flag -I, as in:

```
cc -c -I /usr/local/include myprog.c
```

#### The API

The API is fairly straight-forward:

- You obtain a "context" for fe's functions, using `fe_setup()`
- You add file targets to the context using `fe_target_add()`

The first function `fe_setup()` takes 4 parameters:

- A user-supplied transcryption secret
- Verbosity: an integer. When non-0, fe will generate debugging messages
- Verbose messages destination: this is either `dst_syslog` or `dst_stderr`, depending on where you want to see the debug messages.
- Should non-supported functions such as `fscanf()` be ignored or should fe abort? This is a boolean flag, non-zero to ignore, 0 to abort.

The function returns a pointer to a context, which your program needs to store.

The second function `fe_target_add()` takes 2 parameters:

- A pointer to the context, obtained using `fe_setup()`
- A string, which is the name of a file to treat as transcryption target.

Example:

```C
    #include <fe.h>                     // header inclusion

    FeCtx *ctx;                         // the context ptr

    ctx = fe_setup("user secret",       // transcryption key
                   1,                   // verbosity: ON
                   dst_syslog,          // verbose messages to syslog
                   0);                  // abort when unsupported
                                        // functions are seen

    fe_target_add(ctx, "/tmp/myfile");  // add a target
    fe_target_add(ctx, "~/secret.txt"); // add another target
```

After this, the standard functions can be used to open, read, write and close files. Assuming they are marked as targets, fe will transcrypt buffers before committing to disk, or it will transcrypt buffers that are read from disk before returning to the caller.

#### Linkage

Make sure to add `-L` (for library location) and `-l` (for library name) during the linkage phase, as in:

```shell
cc -o myprog myprog.o -L/usr/local/lib -lfe
```

On some systems, notably Linux, you will also need `-ldl`:

```shell
cc -o myprog myprog.o -L/usr/local/lib -lfe -ldl
```




