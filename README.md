# fe

Fe is an encryption utility aimed at files on your filesystem.

But wait, there's more. Fe is a "somewhat smart" utility. It can prepare an
encryption layer that other processes use. For those processes, accessed files
are perceived as plaintext, while in fact they are guarded by fe and reside on
disk only in an encrypted form.

What good is that? There is a number of use cases:

*  You store your passwords, credit card data etc. in a file on disk.  You want
   the file to be encrypted, so that no one (not even "root") can see. So every
   time you want to update that file, you type:

   ```shell
   fe emacs mysecretfile.txt
   ```

   (Don't worry. You can also use vi. The "emacs mysecretfile.txt" part above is
   just a command that fe runs for you.)

   You could also use flag `-t` (for `--target`), as follows:

   ```shell
   fe -t mysecretfile.txt emacs mysecretfile.txt
   ```

   But fe is smart enough to guess that the commandline argument should be a
   target for encryption and decryption.

   If you want to look up your password for say amazon.com, you type:

   ```shell
   fe grep amazon mysecretfile.txt
   ```

   Here, fe will assume that "amazon" and "mysecretfile.txt" are encryption
   targets, but since "amazon" probably isn't a file, that's fine. If you need
   to be sure to restrict the target to only "mysecretfile.txt", then you can
   always use flag `-t`:

   ```shell
   fe -t mysecretfile  grep amazon mysecretfile.txt
   ```

*  You want to start an entire new shell on top of fe's encryption layer, so
   that the encrypted file $HOME/etc/mysecretfile.txt appears unencrypted to all
   stacked processes, while in fact it's in encrypted form on disk. You type:

   ```shell
   fe -t ~/etc/mysecretfile.txt  bash
   ```

   Next, inside that bash, the file appears plain-text. E.g., when you type

   ```shell
   cat ~/etc/mysecretfile.txt
   emacs ~/etc/mysecretfile.txt
   ```

   then `cat` or `emacs` will just see a plaintext file and the file on disk
   will never change its encrypted state.

## Why use fe and not a different utility?

There are many encryption utilities around. I think that fe has its
particular strengths:

*  Full disk encryption utilities such as *truecrypt* "unlock" the disk once the
   system boots. If an attacker gains entry into a running system, they can
   access plaintext files. Also, user "root" can see it all.

*  File-based utilities such as *pgp* work with two files: one encrypted, one
   plaintext. If you store sensitive information in a pgp-encrypted file, then
   your workflow is probably: decrypt/edit/encrypt; and therefore, the plaintext
   version appears on disk once the utility is asked to decrypt it. The same
   also goes for e.g. encrypted zip files. Again, others can see it if they time
   it right.

*  The case is even stronger when you consider the underlying disk.  Once a
   plaintext file has been on the disk (even though it's removed now), it can
   possibly be reconstructed. This is even more true for SSD's; the internal
   processor of an SSD will try to postpone writing over previously used sectors
   so that there's an even usage spread.  So, on an SSD, the contents of a
   deleted plaintext file might be in more than one location.

## Encryption and decryption keys

Fe uses symmetric encryption; which means that the same key is used for both
encryption and decryption. Actually, fe doesn't even know if it's encrypting or
decrypting, it's just "transcrypting" bytes.

When fe needs to transcrypt data, then it needs a key. A key can be given to fe
in any of three ways:

1. Using the command line switch -k, as in:
 
   ```shell
   fe -k mysecret  ....
   ```
   
   This option is unsafe and should be used sparingly, because a simple process
   listing using "ps" will reveal the commandline and hence the secret key.

1. Using the environment variable `FE_KEY`, as in:

   ```shell
   export FE_KEY=mysecret
   fe  ....
   ```
   
   This option is unsafe and should be used sparingly, because the secret
   remains in the environment and can be seen. For example, if fe spawns a
   shell, then a simple "printenv" will reveal the key.  Furthermore, on Linux,
   user "root" can access the environments of running programs via `/proc`.

1.  Using none of the above. In that case fe will prompt twice for the key.
    This option is the safest.

## Don't loose your key

Fe doesn't store key information anywhere. And it can't detect whether the right
key is used when decrypting (it just transcrypts bytes, remember). Therefore,
don't loose your key. There is no way to get back plaintext information once
it's been encrypted and the key is lost.

While that looks like a flaw, consider this. Given the fact that there is no
information whether a transcryption is using the right key or not, there must be
countless keys that produce some meaningful output.  For example, if a
brute-forced key decrypts a file to say "hello", is that the right key? No-one
knows; there's also a key to decrypt the same bytes into "world". Brute-forcing
has just become a bit harder.  (Nevertheless, make sure that you use "good" keys
when using fe.)

## How to encrypt existing files

If you have plaintext files and want to encrypt them for usage with fe, then
there are basically two options:

1. Use fe's flag `-f`, as in:

   ```shell
   fe -f plaintext.txt
   mv plaintext.txt encrypted.txt
   ```
   
   In this case, the file is transcrypted "in-place" and you get a nice progress
   meter showing how far fe is. Most stop-signals, like pressing ^C, are ignored
   (else you might wind up with a half-transcrypted file).

1. Have a standard utility like `cp` do it for you, while fe is "underneath"
   and provides a transcryption layer.
   
   ```shell
   fe -t encrypted.txt  cp plaintext.txt encrypted.txt
   rm plaintext.txt
   ```
