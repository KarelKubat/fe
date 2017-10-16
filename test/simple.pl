use strict;

my $uname = undef;
sub decrypt_and_cat($$) {
    my ($fname, $key) = @_;
    
    # Get the uname. External cp command won't work on Darwin.
    if (not $uname) {
	open(my $if, "uname |") or die;
	$uname = <$if>;
    }

    # If not on Darwin, use external cp command.
    my $if;
    if ($uname !~ /Darwin/) {
	open($if, "fe -k $key cat $fname |") or die;
    } else {
	system("cp $fname $fname.dec") and die;
	system("fe -k $key -f $fname.dec") and die;
	open($if, "$fname.dec") or die;
    }

    # Return first line of that stream.
    return <$if>;
}

# Create simple file
printf("=== Creating: /tmp/$$.txt\n");
open(my $fh, ">/tmp/$$.txt") or die;
print $fh ("Hello World!");
close($fh);
print("File /tmp/$$.txt: created\n");

# Encrypt
printf("=== Encrypting: /tmp/$$.txt\n");
system("fe -k aaa -f /tmp/$$.txt") and die;
print("File /tmp/$$.txt: encrypted\n");

# Read back
printf("=== Decrypting and reading back: /tmp/$$.txt\n");
my $line = decrypt_and_cat("/tmp/$$.txt", "aaa");
die("Bad decryption: got [$line]\n") if ($line ne "Hello World!");
print("Readback: succeeded\n");

# Read back using wrong key
my $line = decrypt_and_cat("/tmp/$$.txt", "bbb");
die("Decryption with bad key succeeded: got [$line]\n")
  if ($line eq "Hello World!");
print("Wrong key check: succeeded\n");
