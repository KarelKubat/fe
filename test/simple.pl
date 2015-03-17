use strict;

# Create simple file
printf("=== Creating: /tmp/simple.txt\n");
open(my $fh, ">/tmp/simple.txt") or die;
print $fh ("Hello World!");
close($fh);
print("File /tmp/simple.txt: created\n");

# Encrypt
printf("=== Encrypting: /tmp/simple.txt\n");
system("fe -k aaa -f /tmp/simple.txt") and die;
print("File /tmp/simple.txt: encrypted\n");

# Read back
printf("=== Decrypting and reading back: /tmp/simple.txt\n");
open(my $if, "fe -k aaa cat /tmp/simple.txt |") or die;
my $line = <$if>;
close($if);
die("Bad decryption: got [$line]\n") if ($line ne "Hello World!");
print("Simple readback: succeeded\n");

# Read back using wrong key
open(my $if, "fe -k bbb cat /tmp/simple.txt |") or die;
my $line = <$if>;
close($if);
die("Decryption with bad key succeeded: got [$line]\n")
  if ($line eq "Hello World!");
print("Wrong key check: succeeded\n");
