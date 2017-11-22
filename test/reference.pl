use strict;

sub run($) {
    my $cmd = shift;
    print(">> $cmd\n");
    system($cmd) and die("reference.pl: $cmd failed\n");
}

# Get major version of fe
open(my $if, "fe -V |") or die("can't run fe to get version id");
my $ver = <$if>;
chomp($ver);
$ver =~ s{\..*}{};

# Check that there is a reference file
if (! -f "reference/lorem-$ver.txt") {
    die <<"EOF";

Reference file lorem-$ver.txt is missing. To create it, execute:

  cp test/reference/lorem.txt test/reference/lorem-$ver.txt
  fe -k lorem -f test/reference/lorem-$ver.txt

EOF
}

# We have a reference file. Let's see if this version still encrypts correctly.

print("\n1 Transcrypting reference/lorem.txt into /tmp/$$ for fe version $ver ",
      "using immediate file transcription and a commandline key\n");
run("cp reference/lorem.txt /tmp/$$-clikey");
run("fe -k lorem -f /tmp/$$-clikey");
run("diff /tmp/$$-clikey reference/lorem-$ver.txt");

print("\n2 Transcrypting reference/lorem.txt into /tmp/$$ for fe version $ver ",
      "using immediate file transcription and an environment key\n");
$ENV{FE_KEY} = 'lorem';
run("cp reference/lorem.txt /tmp/$$-envkey");
run("FE_KEY=lorem fe -f /tmp/$$-envkey");
run("diff /tmp/$$-envkey reference/lorem-$ver.txt");

# Get the uname of this system. The following will not work on Darwin because
# of SIP (or you might've disabled it, but that cannot be assumed).
open($if, "uname |");
if (<$if> !~ /Darwin/) {
    # Remove file from above step and run via external cp command
    run("rm -f /tmp/$$*");
    print("\n3 Transcrypting reference/lorem.txt into /tmp/$$ for fe version ",
          "$ver using external command cp\n");
    run("fe -t /tmp/$$ -k lorem cp reference/lorem.txt /tmp/$$");
    print("Comparing /tmp/$$ with pre-existing reference/lorem-$ver.txt\n");
    run("diff /tmp/$$ reference/lorem-$ver.txt");
}

# Check that decryption works
print("\n4 Decrypting /tmp/$$ using an environment key\n");
run("fe -f /tmp/$$");
print("Comparing plaintext /tmp/$$ with reference/lorem.txt\n");
run("diff /tmp/$$ reference/lorem.txt");

print("\n5 Version reference tests: OK\n");

